#include "LeakProbe.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ArgConverter.h"
#include "JEnv.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "Runtime.h"
#include "WrapperLifetime.h"
#include "v8-profiler.h"

using namespace v8;

namespace tns {

namespace {

constexpr uint32_t kNone = UINT32_MAX;

// --- The Java heap, from an HPROF dump --------------------------------------

// Big-endian reader over the mapped dump; any read past the end clears ok.
struct Reader {
  const uint8_t* p;
  const uint8_t* end;
  uint32_t idSize = 4;
  bool ok = true;

  bool Has(size_t n) {
    if (!ok || static_cast<size_t>(end - p) < n) {
      ok = false;
      return false;
    }
    return true;
  }
  uint64_t Read(size_t n) {
    if (!Has(n)) {
      return 0;
    }
    uint64_t value = 0;
    for (size_t i = 0; i < n; i++) {
      value = (value << 8) | p[i];
    }
    p += n;
    return value;
  }
  uint8_t U1() { return static_cast<uint8_t>(Read(1)); }
  uint16_t U2() { return static_cast<uint16_t>(Read(2)); }
  uint32_t U4() { return static_cast<uint32_t>(Read(4)); }
  uint64_t Id() { return Read(idSize); }
  void Skip(size_t n) {
    if (Has(n)) {
      p += n;
    }
  }
};

size_t TypeSize(uint8_t type, uint32_t idSize) {
  switch (type) {
    case 2:  // object
      return idSize;
    case 4:  // boolean
    case 8:  // byte
      return 1;
    case 5:  // char
    case 9:  // short
      return 2;
    case 6:   // float
    case 10:  // int
      return 4;
    case 7:   // double
    case 11:  // long
      return 8;
    default:
      return 0;
  }
}

struct JavaClass {
  uint64_t nameId = 0;
  uint64_t super = 0;
  // Instance fields this class declares: name string id, type.
  std::vector<std::pair<uint64_t, uint8_t>> fields;
  // Object-typed static fields: name string id, value.
  std::vector<std::pair<uint64_t, uint64_t>> statics;
};

struct JavaObject {
  uint64_t id;
  uint64_t classId;  // the instance's class, the array's class, or the class itself
  const uint8_t* data;
  uint32_t length;   // instance: bytes; object array: element count
  uint8_t kind;      // 0 instance, 1 object array, 2 class
};

struct JavaHeap {
  uint32_t idSize = 4;
  std::unordered_map<uint64_t, std::string> strings;
  std::unordered_map<uint64_t, uint64_t> classNameIds;  // class id -> name string id
  std::unordered_map<uint64_t, JavaClass> classes;
  std::vector<JavaObject> objects;
  std::unordered_map<uint64_t, uint32_t> indexOf;
  std::vector<uint64_t> roots;

  // Edges by object index, built after the parse.
  std::vector<uint32_t> edgeStart;
  std::vector<uint32_t> edgeTo;
  std::vector<uint32_t> edgeLabel;
  std::vector<std::string> labels;
  std::unordered_map<std::string, uint32_t> labelIndex;

  const std::string& String(uint64_t id) const {
    static const std::string empty;
    auto it = strings.find(id);
    return it != strings.end() ? it->second : empty;
  }

  std::string ClassName(uint64_t classId) const {
    auto it = classNameIds.find(classId);
    return it != classNameIds.end() ? String(it->second) : std::string("<class>");
  }

  uint32_t Label(const std::string& label) {
    auto it = labelIndex.find(label);
    if (it != labelIndex.end()) {
      return it->second;
    }
    labels.push_back(label);
    return labelIndex.emplace(label, static_cast<uint32_t>(labels.size() - 1)).first->second;
  }

  uint32_t IndexOf(uint64_t id) const {
    auto it = indexOf.find(id);
    return it != indexOf.end() ? it->second : kNone;
  }
};

bool ParseRecords(Reader& in, JavaHeap& heap, std::string& error) {
  while (in.ok && in.p < in.end) {
    uint8_t tag = in.U1();
    in.U4();  // time
    uint32_t length = in.U4();
    if (!in.Has(length)) {
      break;
    }
    const uint8_t* recordEnd = in.p + length;
    if (tag == 0x01) {  // STRING
      uint64_t id = in.Id();
      if (!in.ok || in.p > recordEnd) {
        error = "malformed HPROF string record";
        return false;
      }
      heap.strings.emplace(id, std::string(reinterpret_cast<const char*>(in.p),
                                           recordEnd - in.p));
    } else if (tag == 0x02) {  // LOAD CLASS
      in.U4();
      uint64_t classId = in.Id();
      in.U4();
      heap.classNameIds[classId] = in.Id();
    } else if (tag == 0x0C || tag == 0x1C) {  // HEAP DUMP (SEGMENT)
      Reader sub{in.p, recordEnd, heap.idSize};
      while (sub.ok && sub.p < sub.end) {
        uint8_t subtag = sub.U1();
        switch (subtag) {
          case 0xFF:  // ROOT UNKNOWN
          case 0x05:  // STICKY CLASS
          case 0x07:  // MONITOR USED
          case 0x89:  // INTERNED STRING
          case 0x8A:  // FINALIZING
          case 0x8B:  // DEBUGGER
          case 0x8C:  // REFERENCE CLEANUP
          case 0x8D:  // VM INTERNAL
            heap.roots.push_back(sub.Id());
            break;
          case 0x90:  // UNREACHABLE: not a root
            sub.Id();
            break;
          case 0x01:  // JNI GLOBAL
            heap.roots.push_back(sub.Id());
            sub.Id();
            break;
          case 0x02:  // JNI LOCAL
          case 0x03:  // JAVA FRAME
          case 0x08:  // THREAD OBJECT
          case 0x8E:  // JNI MONITOR
            heap.roots.push_back(sub.Id());
            sub.U4();
            sub.U4();
            break;
          case 0x04:  // NATIVE STACK
          case 0x06:  // THREAD BLOCK
            heap.roots.push_back(sub.Id());
            sub.U4();
            break;
          case 0xFE:  // HEAP DUMP INFO
            sub.U4();
            sub.Id();
            break;
          case 0x20: {  // CLASS DUMP
            uint64_t classId = sub.Id();
            sub.U4();
            JavaClass& cls = heap.classes[classId];
            cls.super = sub.Id();
            sub.Skip(5 * heap.idSize);  // loader, signers, protection domain, 2 reserved
            sub.U4();                   // instance size
            uint16_t constants = sub.U2();
            for (uint16_t i = 0; i < constants && sub.ok; i++) {
              sub.U2();
              sub.Skip(TypeSize(sub.U1(), heap.idSize));
            }
            uint16_t statics = sub.U2();
            for (uint16_t i = 0; i < statics && sub.ok; i++) {
              uint64_t name = sub.Id();
              uint8_t type = sub.U1();
              if (type == 2) {
                uint64_t value = sub.Id();
                if (value != 0) {
                  cls.statics.emplace_back(name, value);
                }
              } else {
                sub.Skip(TypeSize(type, heap.idSize));
              }
            }
            uint16_t fields = sub.U2();
            for (uint16_t i = 0; i < fields && sub.ok; i++) {
              uint64_t name = sub.Id();
              cls.fields.emplace_back(name, sub.U1());
            }
            heap.objects.push_back({classId, classId, nullptr, 0, 2});
            break;
          }
          case 0x21: {  // INSTANCE DUMP
            uint64_t id = sub.Id();
            sub.U4();
            uint64_t classId = sub.Id();
            uint32_t bytes = sub.U4();
            const uint8_t* data = sub.p;
            sub.Skip(bytes);
            heap.objects.push_back({id, classId, data, bytes, 0});
            break;
          }
          case 0x22: {  // OBJECT ARRAY DUMP
            uint64_t id = sub.Id();
            sub.U4();
            uint32_t count = sub.U4();
            uint64_t classId = sub.Id();
            const uint8_t* data = sub.p;
            sub.Skip(static_cast<size_t>(count) * heap.idSize);
            heap.objects.push_back({id, classId, data, count, 1});
            break;
          }
          case 0x23: {  // PRIMITIVE ARRAY DUMP
            sub.Id();
            sub.U4();
            uint32_t count = sub.U4();
            sub.Skip(static_cast<size_t>(count) * TypeSize(sub.U1(), heap.idSize));
            break;
          }
          case 0xC3: {  // PRIMITIVE ARRAY NODATA
            sub.Id();
            sub.U4();
            sub.U4();
            sub.U1();
            break;
          }
          default:
            error = "unknown HPROF heap dump subrecord 0x" + std::to_string(subtag);
            return false;
        }
      }
      if (!sub.ok) {
        error = "truncated HPROF heap dump segment";
        return false;
      }
    }
    in.p = recordEnd;
  }
  return in.ok;
}

// The array LeakProbe.pinned holds, by class name.
uint64_t FindPinnedArray(const JavaHeap& heap) {
  for (const auto& [classId, cls] : heap.classes) {
    if (heap.ClassName(classId) != "com.tns.LeakProbe") {
      continue;
    }
    for (const auto& [name, value] : cls.statics) {
      if (heap.String(name) == "pinned") {
        return value;
      }
    }
  }
  return 0;
}

// References the analysis must not follow: what only the runtime or the probe
// holds, and what Java holds weakly.
bool SkippedField(const std::string& className, const std::string& field) {
  return (field == "referent" && className == "java.lang.ref.Reference") ||
         (field == "zombie" && className == "java.lang.ref.FinalizerReference") ||
         (field == "values" && className == "com.tns.ObjectTable$Leaf");
}

void BuildEdges(JavaHeap& heap, uint64_t pinnedArray) {
  heap.indexOf.reserve(heap.objects.size());
  for (uint32_t i = 0; i < heap.objects.size(); i++) {
    heap.indexOf.emplace(heap.objects[i].id, i);
  }
  uint32_t elementLabel = heap.Label("[]");
  heap.edgeStart.assign(heap.objects.size() + 1, 0);
  std::unordered_map<uint64_t, std::string> classNames;
  auto nameOf = [&](uint64_t classId) -> const std::string& {
    auto it = classNames.find(classId);
    if (it == classNames.end()) {
      it = classNames.emplace(classId, heap.ClassName(classId)).first;
    }
    return it->second;
  };
  for (uint32_t i = 0; i < heap.objects.size(); i++) {
    heap.edgeStart[i] = static_cast<uint32_t>(heap.edgeTo.size());
    const JavaObject& object = heap.objects[i];
    auto add = [&](uint64_t target, uint32_t label) {
      uint32_t to = heap.IndexOf(target);
      if (to != kNone) {
        heap.edgeTo.push_back(to);
        heap.edgeLabel.push_back(label);
      }
    };
    if (object.kind == 1) {
      if (object.id == pinnedArray) {
        continue;
      }
      Reader elements{object.data, object.data + static_cast<size_t>(object.length) * heap.idSize,
                      heap.idSize};
      for (uint32_t e = 0; e < object.length; e++) {
        uint64_t target = elements.Id();
        if (target != 0) {
          add(target, elementLabel);
        }
      }
    } else if (object.kind == 2) {
      auto it = heap.classes.find(object.id);
      if (it == heap.classes.end()) {
        continue;
      }
      const std::string& className = nameOf(object.id);
      for (const auto& [name, value] : it->second.statics) {
        const std::string& field = heap.String(name);
        if (className == "com.tns.LeakProbe" && field == "pinned") {
          continue;
        }
        add(value, heap.Label(className + "." + field));
      }
    } else {
      Reader fields{object.data, object.data + object.length, heap.idSize};
      for (uint64_t classId = object.classId; classId != 0 && fields.ok;) {
        auto it = heap.classes.find(classId);
        if (it == heap.classes.end()) {
          break;
        }
        const std::string& className = nameOf(classId);
        for (const auto& [name, type] : it->second.fields) {
          if (type != 2) {
            fields.Skip(TypeSize(type, heap.idSize));
            continue;
          }
          uint64_t target = fields.Id();
          if (target == 0) {
            continue;
          }
          const std::string& field = heap.String(name);
          if (!SkippedField(className, field)) {
            add(target, heap.Label(className + "." + field));
          }
        }
        classId = it->second.super;
      }
    }
  }
  heap.edgeStart[heap.objects.size()] = static_cast<uint32_t>(heap.edgeTo.size());
}

// Maps an HPROF dump read-only for the parse; the parsed objects point into it.
struct MappedFile {
  void* data = MAP_FAILED;
  size_t size = 0;
  ~MappedFile() {
    if (data != MAP_FAILED) {
      munmap(data, size);
    }
  }
  bool Open(const std::string& path) {
    int fd = open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
      return false;
    }
    struct stat st;
    if (fstat(fd, &st) == 0 && st.st_size > 0) {
      size = static_cast<size_t>(st.st_size);
      data = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
    }
    close(fd);
    return data != MAP_FAILED;
  }
};

bool ParseHprof(const MappedFile& file, JavaHeap& heap, std::string& error) {
  auto begin = static_cast<const uint8_t*>(file.data);
  Reader in{begin, begin + file.size};
  const uint8_t* nul = static_cast<const uint8_t*>(memchr(begin, 0, std::min<size_t>(file.size, 64)));
  if (nul == nullptr) {
    error = "not an HPROF file";
    return false;
  }
  in.p = nul + 1;
  heap.idSize = in.U4();
  in.idSize = heap.idSize;
  in.Read(8);  // timestamp
  if (!in.ok || (heap.idSize != 4 && heap.idSize != 8)) {
    error = "unsupported HPROF header";
    return false;
  }
  return ParseRecords(in, heap, error);
}

// --- The joined graph -------------------------------------------------------

std::string ToUtf8(Isolate* isolate, Local<Value> value) {
  if (value.IsEmpty()) {
    return std::string();
  }
  if (value->IsString()) {
    return ArgConverter::ConvertToString(value.As<v8::String>());
  }
  if (value->IsNumber()) {
    return "[" + std::to_string(static_cast<int64_t>(value.As<Number>()->Value())) + "]";
  }
  return std::string();
}

// V8 gives a WeakMap value two edges with the same name, one from its key and
// one from the map's table ("<n> / part of key (...) -> value (...) pair in
// WeakMap (table @<id>)"). Only the key keeps the value alive, so the table's
// is not followed.
constexpr char kEphemeronEdgePrefix[] = "/ part of key (";
constexpr char kEphemeronTableMarker[] = "(table @";

bool IsEphemeronEdge(const HeapGraphEdge* edge, std::string* name) {
  if (edge->GetType() != HeapGraphEdge::kInternal) {
    return false;
  }
  Local<Value> value = edge->GetName();
  if (value.IsEmpty() || !value->IsString()) {
    return false;
  }
  *name = ArgConverter::ConvertToString(value.As<v8::String>());
  return name->find(kEphemeronEdgePrefix) != std::string::npos;
}

bool IsEphemeronTableEdge(const HeapGraphNode* from, const HeapGraphEdge* edge) {
  std::string name;
  if (!IsEphemeronEdge(edge, &name)) {
    return false;
  }
  size_t marker = name.rfind(kEphemeronTableMarker);
  if (marker == std::string::npos) {
    return false;
  }
  return strtoull(name.c_str() + marker + sizeof(kEphemeronTableMarker) - 1, nullptr, 10) ==
         from->GetId();
}

// One followed edge: a JS edge, a Java field or element (label index), or a
// cross-heap edge of the runtime (neither).
struct EdgeRef {
  const HeapGraphEdge* js = nullptr;
  uint32_t javaLabel = kNone;
};

struct Graph {
  Isolate* isolate;
  const HeapSnapshot* snapshot;
  JavaHeap& java;
  uint32_t jsCount = 0;
  std::unordered_map<const HeapGraphNode*, uint32_t> jsIndex;
  std::vector<const HeapGraphNode*> jsNodes;
  // JS root edges to these snapshot nodes are the runtime's handles to canary
  // records, not real roots.
  std::unordered_set<uint32_t> runtimeHandleTargets;
  // Cross-heap edges.
  std::unordered_map<uint32_t, std::vector<uint32_t>> wrapperToJava;
  std::unordered_map<uint32_t, std::vector<uint32_t>> javaToRecord;

  uint32_t JavaNode(uint32_t index) const { return jsCount + index; }
  bool IsJava(uint32_t node) const { return node >= jsCount; }

  // Calls visit(target, labelText) for each followed edge of node.
  template <typename F>
  void ForEachEdge(uint32_t node, bool fromRoots, F&& visit) const {
    if (!IsJava(node)) {
      const HeapGraphNode* js = jsNodes[node];
      bool synthetic = js->GetType() == HeapGraphNode::kSynthetic;
      for (int i = 0; i < js->GetChildrenCount(); i++) {
        const HeapGraphEdge* edge = js->GetChild(i);
        if (edge->GetType() == HeapGraphEdge::kWeak || edge->GetType() == HeapGraphEdge::kShortcut) {
          continue;
        }
        auto it = jsIndex.find(edge->GetToNode());
        if (it == jsIndex.end()) {
          continue;
        }
        if (fromRoots && synthetic && runtimeHandleTargets.count(it->second) != 0) {
          continue;
        }
        if (IsEphemeronTableEdge(js, edge)) {
          continue;
        }
        visit(it->second, EdgeRef{edge, kNone});
      }
      auto cross = wrapperToJava.find(node);
      if (cross != wrapperToJava.end()) {
        for (uint32_t target : cross->second) {
          visit(target, EdgeRef{});
        }
      }
      return;
    }
    uint32_t index = node - jsCount;
    for (uint32_t e = java.edgeStart[index]; e < java.edgeStart[index + 1]; e++) {
      visit(JavaNode(java.edgeTo[e]), EdgeRef{nullptr, java.edgeLabel[e]});
    }
    auto cross = javaToRecord.find(node);
    if (cross != javaToRecord.end()) {
      for (uint32_t target : cross->second) {
        visit(target, EdgeRef{});
      }
    }
  }

  std::string NodeLabel(uint32_t node) const {
    if (IsJava(node)) {
      const JavaObject& object = java.objects[node - jsCount];
      return object.kind == 2 ? "class " + java.ClassName(object.id) : java.ClassName(object.classId);
    }
    std::string name = ToUtf8(isolate, jsNodes[node]->GetName());
    // V8 names some nodes with their snapshot id ("system / Context / scope @123").
    size_t at = name.rfind(" @");
    if (at != std::string::npos &&
        name.find_first_not_of("0123456789", at + 2) == std::string::npos) {
      name.resize(at);
    }
    return "JS " + (name.empty() ? std::string("(anonymous)") : name);
  }

  std::string EdgeLabel(uint32_t from, const EdgeRef& edge) const {
    if (edge.js != nullptr) {
      std::string name;
      if (IsEphemeronEdge(edge.js, &name)) {
        return "WeakMap value";
      }
      return ToUtf8(isolate, edge.js->GetName());
    }
    if (edge.javaLabel != kNone) {
      return java.labels[edge.javaLabel];
    }
    return IsJava(from) ? "ns:record" : "ns:wrapper";
  }
};

std::vector<uint8_t> Reach(const Graph& graph, uint32_t jsRoot,
                           const std::vector<uint32_t>& javaRoots) {
  std::vector<uint8_t> reached(graph.jsCount + graph.java.objects.size(), 0);
  std::deque<uint32_t> queue;
  auto push = [&](uint32_t node) {
    if (!reached[node]) {
      reached[node] = 1;
      queue.push_back(node);
    }
  };
  push(jsRoot);
  for (uint32_t root : javaRoots) {
    push(root);
  }
  while (!queue.empty()) {
    uint32_t node = queue.front();
    queue.pop_front();
    graph.ForEachEdge(node, true, [&](uint32_t target, const EdgeRef&) { push(target); });
  }
  return reached;
}

// A shortest path from `from` to `to` through unreached nodes, closed by the
// runtime's edge from `to` (a wrapper) back to `from` (its Java object), as
// alternating node and edge labels; empty when there is none within the limit.
std::vector<uint32_t> FindCycle(const Graph& graph, const std::vector<uint8_t>& reached,
                                uint32_t from, uint32_t to, std::vector<std::string>* path,
                                std::vector<std::string>* nodeLabels) {
  constexpr size_t kVisitLimit = 2'000'000;
  std::unordered_map<uint32_t, std::pair<uint32_t, EdgeRef>> parent;
  std::deque<uint32_t> queue;
  queue.push_back(from);
  parent.emplace(from, std::make_pair(kNone, EdgeRef{}));
  bool found = false;
  while (!queue.empty() && !found && parent.size() < kVisitLimit) {
    uint32_t node = queue.front();
    queue.pop_front();
    graph.ForEachEdge(node, false, [&](uint32_t target, const EdgeRef& edge) {
      if (found || reached[target] || !parent.emplace(target, std::make_pair(node, edge)).second) {
        return;
      }
      if (target == to) {
        found = true;
        return;
      }
      queue.push_back(target);
    });
  }
  if (!found) {
    return {};
  }
  std::vector<std::pair<uint32_t, EdgeRef>> steps;  // node, edge out of it
  steps.emplace_back(to, EdgeRef{});
  for (uint32_t node = to; node != from;) {
    auto [previous, edge] = parent.at(node);
    steps.emplace_back(previous, edge);
    node = previous;
  }
  std::reverse(steps.begin(), steps.end());
  std::vector<uint32_t> nodes;
  for (const auto& [node, edge] : steps) {
    nodes.push_back(node);
    nodeLabels->push_back(graph.NodeLabel(node));
    path->push_back(graph.NodeLabel(node));
    path->push_back("--" + graph.EdgeLabel(node, edge) + "-->");
  }
  path->push_back(graph.NodeLabel(from));
  return nodes;
}

// The cycle's node labels from its lexicographically smallest rotation, so the
// same shape reads the same whichever member the search started from.
std::string Signature(const std::vector<std::string>& labels) {
  auto rotation = [&](size_t start) {
    std::string text;
    for (size_t i = 0; i < labels.size(); i++) {
      text += labels[(start + i) % labels.size()];
      text += " -> ";
    }
    return text;
  };
  std::string bestText = rotation(0);
  for (size_t i = 1; i < labels.size(); i++) {
    std::string text = rotation(i);
    if (text < bestText) {
      bestText = std::move(text);
    }
  }
  return bestText + "(back)";
}

void Set(Isolate* isolate, Local<Context> context, Local<Object> target, const char* name,
         Local<Value> value) {
  (void) target->Set(context, ArgConverter::ConvertToV8String(isolate, name), value);
}

Local<Array> ToArray(Isolate* isolate, Local<Context> context,
                     const std::vector<std::string>& items) {
  Local<Array> array = Array::New(isolate, static_cast<int>(items.size()));
  for (uint32_t i = 0; i < items.size(); i++) {
    (void) array->Set(context, i, ArgConverter::ConvertToV8String(isolate, items[i]));
  }
  return array;
}

}  // namespace

MaybeLocal<Object> LeakProbe::FindLeakCycles(Isolate* isolate) {
  auto started = std::chrono::steady_clock::now();
  EscapableHandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
  ObjectManager* objectManager = Runtime::GetObjectManager(isolate);
  auto fail = [&](const std::string& message) -> MaybeLocal<Object> {
    isolate->ThrowException(Exception::Error(ArgConverter::ConvertToV8String(
        isolate, "findLeakCycles: " + message)));
    return MaybeLocal<Object>();
  };

  // Resurrection breaks these cycles: nothing to look for.
  if (GetWrapperLifetimeMode() != WrapperLifetimeMode::kCanary) {
    Local<Object> report = Object::New(isolate);
    Set(isolate, context, report, "mode",
        ArgConverter::ConvertToV8String(isolate, WrapperLifetimeModeName(GetWrapperLifetimeMode())));
    Set(isolate, context, report, "cycles", Array::New(isolate));
    Set(isolate, context, report, "leaked", Number::New(isolate, 0));
    Set(isolate, context, report, "leakedOutsideCycles", Number::New(isolate, 0));
    return scope.Escape(report);
  }

  // The snapshot first, before this call holds a handle to any wrapper or state:
  // a handle would root it.
  objectManager->FlushFinalizedBatchIfPending();
  HeapProfiler* profiler = isolate->GetHeapProfiler();
  // A snapshot leaves V8 tracking object moves, at a cost on every scavenge,
  // until its object ids are cleared. Cleared afterwards unless something else
  // (DevTools) already holds snapshots and so relies on the ids.
  bool clearIds = profiler->GetSnapshotCount() == 0;
  const HeapSnapshot* snapshot = profiler->TakeHeapSnapshot();
  if (snapshot == nullptr) {
    return fail("could not take a heap snapshot");
  }
  struct DeleteSnapshot {
    HeapProfiler* profiler;
    const HeapSnapshot* snapshot;
    bool clearIds;
    ~DeleteSnapshot() {
      const_cast<HeapSnapshot*>(snapshot)->Delete();
      if (clearIds) {
        profiler->ClearObjectIds();
      }
    }
  } deleteSnapshot{profiler, snapshot, clearIds};

  std::vector<ObjectManager::LeakProbeEntry> entries = objectManager->GetLeakProbeEntries();

  // The Java heap, with the entries' objects pinned in order.
  JEnv env;
  JNIEnv* raw = env;
  JniLocalRef ids(raw->NewIntArray(static_cast<jsize>(entries.size())));
  if (ids.IsNull()) {
    raw->ExceptionClear();
    return fail("out of memory");
  }
  {
    std::vector<jint> values;
    values.reserve(entries.size());
    for (const auto& entry : entries) {
      values.push_back(entry.id);
    }
    raw->SetIntArrayRegion((jintArray) ids, 0, static_cast<jsize>(values.size()), values.data());
  }
  static jmethodID dumpMethod = nullptr;
  if (dumpMethod == nullptr) {
    JniLocalRef runtimeClass(raw->GetObjectClass(objectManager->GetJavaRuntimeObject()));
    dumpMethod = raw->GetMethodID((jclass) runtimeClass, "dumpHeapForLeakProbe",
                                  "([I)Ljava/lang/String;");
  }
  JniLocalRef pathRef(raw->CallObjectMethod(objectManager->GetJavaRuntimeObject(), dumpMethod,
                                            (jintArray) ids));
  if (raw->ExceptionCheck()) {
    raw->ExceptionClear();
    return fail("the Java heap dump failed");
  }
  if (pathRef.IsNull()) {
    return fail("the Java heap dump failed; see the log");
  }
  std::string path = ArgConverter::jstringToString((jstring) pathRef);
  struct RemoveFile {
    std::string path;
    ~RemoveFile() { unlink(path.c_str()); }
  } removeFile{path};

  MappedFile file;
  if (!file.Open(path)) {
    return fail("could not read the Java heap dump");
  }
  JavaHeap java;
  std::string error;
  if (!ParseHprof(file, java, error)) {
    return fail(error);
  }
  uint64_t pinnedArray = FindPinnedArray(java);
  BuildEdges(java, pinnedArray);

  Graph graph{isolate, snapshot, java};
  graph.jsCount = static_cast<uint32_t>(snapshot->GetNodesCount());
  graph.jsNodes.reserve(graph.jsCount);
  graph.jsIndex.reserve(graph.jsCount);
  for (uint32_t i = 0; i < graph.jsCount; i++) {
    const HeapGraphNode* node = snapshot->GetNode(static_cast<int>(i));
    graph.jsNodes.push_back(node);
    graph.jsIndex.emplace(node, i);
  }
  auto jsNodeOf = [&](Local<Value> value) -> uint32_t {
    if (value.IsEmpty() || !value->IsObject()) {
      return kNone;
    }
    const HeapGraphNode* node = snapshot->GetNodeById(profiler->GetObjectId(value));
    auto it = graph.jsIndex.find(node);
    return it != graph.jsIndex.end() ? it->second : kNone;
  };

  // The pinned array's elements, in the entries' order.
  std::vector<uint32_t> javaOf(entries.size(), kNone);
  uint32_t pinnedIndex = java.IndexOf(pinnedArray);
  if (pinnedIndex == kNone) {
    return fail("the heap dump has no LeakProbe.pinned array");
  }
  {
    const JavaObject& pinned = java.objects[pinnedIndex];
    Reader elements{pinned.data, pinned.data + static_cast<size_t>(pinned.length) * java.idSize,
                    java.idSize};
    for (uint32_t i = 0; i < pinned.length && i < entries.size(); i++) {
      uint64_t id = elements.Id();
      javaOf[i] = id != 0 ? java.IndexOf(id) : kNone;
    }
  }

  for (size_t i = 0; i < entries.size(); i++) {
    const auto& entry = entries[i];
    if (javaOf[i] == kNone) {
      continue;
    }
    uint32_t javaNode = graph.JavaNode(javaOf[i]);
    if (!entry.wrapper.IsEmpty() && entry.wrapperHoldsJava) {
      uint32_t wrapperNode = jsNodeOf(entry.wrapper);
      if (wrapperNode != kNone) {
        graph.wrapperToJava[wrapperNode].push_back(javaNode);
      }
    }
    for (Local<Value> value : entry.recordValues) {
      uint32_t node = jsNodeOf(value);
      if (node != kNone) {
        graph.javaToRecord[javaNode].push_back(node);
        graph.runtimeHandleTargets.insert(node);
      }
    }
  }

  std::vector<uint32_t> javaRoots;
  javaRoots.reserve(java.roots.size());
  for (uint64_t root : java.roots) {
    uint32_t index = java.IndexOf(root);
    if (index != kNone) {
      javaRoots.push_back(graph.JavaNode(index));
    }
  }
  auto rootIt = graph.jsIndex.find(snapshot->GetRoot());
  if (rootIt == graph.jsIndex.end()) {
    return fail("the heap snapshot has no root");
  }
  std::vector<uint8_t> reached = Reach(graph, rootIt->second, javaRoots);

  // The runtime keeps a Java object only through a live wrapper; one nothing
  // else reaches would be collected by a single collector. A canary record's
  // state alone keeps nothing: once its canary died, Java holds the object
  // weakly and collects it normally. Resurrection breaks these cycles, so only
  // canary mode reports them.
  std::unordered_map<uint32_t, size_t> entryOfJavaNode;
  std::vector<size_t> leaked;
  std::unordered_set<size_t> leakedSet;
  std::vector<uint32_t> wrapperOf(entries.size(), kNone);
  bool canaryMode = GetWrapperLifetimeMode() == WrapperLifetimeMode::kCanary;
  for (size_t i = 0; i < entries.size(); i++) {
    if (javaOf[i] == kNone) {
      continue;
    }
    uint32_t javaNode = graph.JavaNode(javaOf[i]);
    entryOfJavaNode.emplace(javaNode, i);
    if (!entries[i].wrapper.IsEmpty() && entries[i].wrapperHoldsJava) {
      wrapperOf[i] = jsNodeOf(entries[i].wrapper);
    }
    if (canaryMode && !reached[javaNode] && wrapperOf[i] != kNone) {
      leaked.push_back(i);
      leakedSet.insert(i);
    }
  }

  // Each search can visit most of both heaps; past this many, leaked objects are
  // counted but not explained.
  constexpr uint32_t kMaxCycleSearches = 256;
  uint32_t searches = 0;
  bool truncated = false;
  Local<Array> cycles = Array::New(isolate);
  std::unordered_set<size_t> covered;
  uint32_t cycleCount = 0;
  for (size_t i : leaked) {
    if (covered.count(i) != 0) {
      continue;
    }
    if (searches++ == kMaxCycleSearches) {
      truncated = true;
      break;
    }
    std::vector<std::string> pathLabels;
    std::vector<std::string> nodeLabels;
    std::vector<uint32_t> nodes = FindCycle(graph, reached, graph.JavaNode(javaOf[i]),
                                            wrapperOf[i], &pathLabels, &nodeLabels);
    if (nodes.empty()) {
      continue;
    }
    std::vector<std::string> members;
    for (uint32_t node : nodes) {
      auto it = entryOfJavaNode.find(node);
      if (it != entryOfJavaNode.end() && leakedSet.count(it->second) != 0) {
        covered.insert(it->second);
        members.push_back(entries[it->second].className);
      }
    }
    std::string signature = Signature(nodeLabels);
    Local<Object> cycle = Object::New(isolate);
    Set(isolate, context, cycle, "signature", ArgConverter::ConvertToV8String(isolate, signature));
    Set(isolate, context, cycle, "path", ToArray(isolate, context, pathLabels));
    Set(isolate, context, cycle, "members", ToArray(isolate, context, members));
    (void) cycles->Set(context, cycleCount++, cycle);
  }

  auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started);
  Local<Object> report = Object::New(isolate);
  Set(isolate, context, report, "mode",
      ArgConverter::ConvertToV8String(isolate, WrapperLifetimeModeName(GetWrapperLifetimeMode())));
  Set(isolate, context, report, "cycles", cycles);
  Set(isolate, context, report, "leaked", Number::New(isolate, static_cast<double>(leaked.size())));
  Set(isolate, context, report, "leakedOutsideCycles",
      Number::New(isolate, static_cast<double>(leaked.size() - covered.size())));
  Set(isolate, context, report, "truncated", v8::Boolean::New(isolate, truncated));
  Set(isolate, context, report, "probed", Number::New(isolate, static_cast<double>(entries.size())));
  Set(isolate, context, report, "jsNodes", Number::New(isolate, graph.jsCount));
  Set(isolate, context, report, "javaObjects",
      Number::New(isolate, static_cast<double>(java.objects.size())));
  Set(isolate, context, report, "dumpBytes", Number::New(isolate, static_cast<double>(file.size)));
  Set(isolate, context, report, "durationMs", Number::New(isolate, elapsed.count()));
  return scope.Escape(report);
}

}  // namespace tns
