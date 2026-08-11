"use strict";
// High Resolution Time + User Timing Level 3 + Performance Timeline: the
// `performance` global and its interface objects. The native side contributes
// exactly two things through the binding bag — `now()` (double ms since this
// isolate's time origin, monotonic) and `timeOrigin` (wall-clock ms since the
// Unix epoch, sampled when the isolate's time origin was taken) — everything
// else is portable JS, kept in sync with the iOS runtime's copy of this file,
// which runs against the same bag.
//
// Deliberate deviations from the specs:
// - Observer callbacks are delivered from a microtask rather than a queued
//   task. Delivery is still asynchronous relative to mark()/measure(), but it
//   precedes timer callbacks scheduled in the same turn.
// - Failures the specs express as DOMException (SyntaxError,
//   InvalidModificationError) are Error instances with `name` patched —
//   DOMException does not exist in this runtime.
const { now, timeOrigin } = binding;
const {
  ArrayPrototypeIndexOf,
  ArrayPrototypePush,
  ArrayPrototypeSlice,
  ArrayPrototypeSort,
  ArrayPrototypeSplice,
  Error,
  FunctionPrototypeCall,
  Number,
  NumberIsFinite,
  ObjectDefineProperty,
  ObjectFreeze,
  ObjectGetOwnPropertyDescriptor,
  String,
  SymbolIterator,
  SymbolToStringTag,
  TypeError,
} = primordials;
var g = globalThis;
// Init order (Runtime::PrepareV8Runtime) guarantees events.js and
// error-events.js ran first; captured before user code can replace them.
const EventTarget = g.EventTarget;
const enqueueMicrotask = g.queueMicrotask;
const reportException = g.reportError;
// mark/measure `detail` is structured-cloned per spec, so an entry holds a
// snapshot and an uncloneable detail throws DataCloneError. The identity
// fallback keeps this file portable to a runtime that ships the Performance
// API before structuredClone — there, detail degrades to by-reference; the
// user-timing buffers are unbounded either way, so entries retain their
// detail until clearMarks()/clearMeasures().
const cloneDetail =
  typeof g.structuredClone === "function"
    ? g.structuredClone
    : function (value) {
        return value;
      };

// Construction token: interfaces whose constructors the spec marks as not
// user-invocable accept instances only from factories inside this module.
const kInternal = {};

function illegalConstructor() {
  return new TypeError("Illegal constructor");
}

// SyntaxError / InvalidModificationError stand-in (see header).
function domException(message, name) {
  const e = new Error(message);
  e.name = name;
  return e;
}

// WebIDL sequence<DOMString> conversion: only an object with a callable
// @@iterator converts (so a Set works and a string primitive does not);
// anything else is a TypeError, never a silent no-op.
function convertStringSequence(value, context) {
  if (
    value === null ||
    (typeof value !== "object" && typeof value !== "function") ||
    typeof value[SymbolIterator] !== "function"
  ) {
    throw new TypeError(context + " is not iterable");
  }
  const result = [];
  for (const item of value) {
    ArrayPrototypePush(result, String(item));
  }
  return result;
}

class PerformanceEntry {
  #name;
  #entryType;
  #startTime;
  #duration;
  constructor(token, name, entryType, startTime, duration) {
    if (token !== kInternal) {
      throw illegalConstructor();
    }
    this.#name = name;
    this.#entryType = entryType;
    this.#startTime = startTime;
    this.#duration = duration;
  }
  get name() {
    return this.#name;
  }
  get entryType() {
    return this.#entryType;
  }
  get startTime() {
    return this.#startTime;
  }
  get duration() {
    return this.#duration;
  }
  toJSON() {
    return {
      name: this.#name,
      entryType: this.#entryType,
      startTime: this.#startTime,
      duration: this.#duration,
    };
  }
}

class PerformanceMark extends PerformanceEntry {
  #detail;
  constructor(markName, markOptions) {
    if (arguments.length < 1) {
      throw new TypeError("PerformanceMark: 1 argument required, but only 0 present");
    }
    const name = String(markName);
    let startTime;
    let detail = null;
    if (markOptions !== undefined && markOptions !== null) {
      if (typeof markOptions !== "object" && typeof markOptions !== "function") {
        throw new TypeError("PerformanceMark: options must be an object");
      }
      if (markOptions.startTime !== undefined) {
        startTime = Number(markOptions.startTime);
        if (!NumberIsFinite(startTime)) {
          throw new TypeError("PerformanceMark: startTime must be a finite number");
        }
        if (startTime < 0) {
          throw new TypeError("PerformanceMark: startTime cannot be negative");
        }
      }
      if (markOptions.detail !== undefined && markOptions.detail !== null) {
        detail = cloneDetail(markOptions.detail);
      }
    }
    super(kInternal, name, "mark", startTime === undefined ? now() : startTime, 0);
    this.#detail = detail;
  }
  get detail() {
    return this.#detail;
  }
  toJSON() {
    const json = super.toJSON();
    json.detail = this.#detail;
    return json;
  }
}

class PerformanceMeasure extends PerformanceEntry {
  #detail;
  constructor(token, name, startTime, duration, detail) {
    if (token !== kInternal) {
      throw illegalConstructor();
    }
    super(token, name, "measure", startTime, duration);
    this.#detail = detail;
  }
  get detail() {
    return this.#detail;
  }
  toJSON() {
    const json = super.toJSON();
    json.detail = this.#detail;
    return json;
  }
}

// ---- Performance timeline (per-isolate; this module runs once per isolate).

const entries = []; // marks + measures, insertion order

// Spec ordering for every query result: chronological by startTime; V8's
// stable sort preserves insertion order for ties. A copy is always returned —
// a measure may be inserted with a startTime that precedes buffered marks.
function chronological(list) {
  const copy = ArrayPrototypeSlice(list);
  ArrayPrototypeSort(copy, function (a, b) {
    return a.startTime - b.startTime;
  });
  return copy;
}

function queryEntries(name, type) {
  const result = [];
  for (let i = 0; i < entries.length; i++) {
    const e = entries[i];
    if (type !== undefined && e.entryType !== type) {
      continue;
    }
    if (name !== undefined && e.name !== name) {
      continue;
    }
    ArrayPrototypePush(result, e);
  }
  return chronological(result);
}

function clearEntries(type, name) {
  for (let i = entries.length - 1; i >= 0; i--) {
    const e = entries[i];
    if (e.entryType !== type) {
      continue;
    }
    if (name !== undefined && e.name !== name) {
      continue;
    }
    ArrayPrototypeSplice(entries, i, 1);
  }
}

function bufferEntry(entry) {
  ArrayPrototypePush(entries, entry);
  notifyObservers(entry);
}

// User Timing "convert a mark to a timestamp": numbers are timestamps
// (negative is a TypeError), everything else names the most recent mark.
function convertMarkToTimestamp(value) {
  if (typeof value === "number") {
    if (!NumberIsFinite(value)) {
      throw new TypeError("Given timestamp must be a finite number");
    }
    if (value < 0) {
      throw new TypeError("Given timestamp cannot be negative");
    }
    return value;
  }
  const name = String(value);
  for (let i = entries.length - 1; i >= 0; i--) {
    const e = entries[i];
    if (e.entryType === "mark" && e.name === name) {
      return e.startTime;
    }
  }
  throw domException("The mark '" + name + "' does not exist", "SyntaxError");
}

// ---- Performance observers.

const SUPPORTED_ENTRY_TYPES = ObjectFreeze(["mark", "measure"]);

// Registration order is the spec's delivery order. Each element is an
// observer's internal record ({ observer, callback, types, queue, mode }),
// reachable only from its PerformanceObserver instance and this list.
const observers = [];
let flushScheduled = false;

function scheduleFlush() {
  if (flushScheduled) {
    return;
  }
  flushScheduled = true;
  enqueueMicrotask(flushObservers);
}

function flushObservers() {
  flushScheduled = false;
  const snapshot = ArrayPrototypeSlice(observers);
  for (let i = 0; i < snapshot.length; i++) {
    const record = snapshot[i];
    if (record.queue.length === 0) {
      continue;
    }
    const taken = chronological(record.queue);
    record.queue = [];
    const list = new PerformanceObserverEntryList(kInternal, taken);
    try {
      FunctionPrototypeCall(record.callback, record.observer, list, record.observer);
    } catch (e) {
      // Spec: report the exception; one throwing observer must not starve
      // the observers behind it.
      reportException(e);
    }
  }
}

function notifyObservers(entry) {
  const type = entry.entryType;
  let queued = false;
  for (let i = 0; i < observers.length; i++) {
    const record = observers[i];
    if (ArrayPrototypeIndexOf(record.types, type) === -1) {
      continue;
    }
    ArrayPrototypePush(record.queue, entry);
    queued = true;
  }
  if (queued) {
    scheduleFlush();
  }
}

class PerformanceObserverEntryList {
  #entries;
  constructor(token, list) {
    if (token !== kInternal) {
      throw illegalConstructor();
    }
    this.#entries = list;
  }
  getEntries() {
    return ArrayPrototypeSlice(this.#entries);
  }
  getEntriesByType(type) {
    if (arguments.length < 1) {
      throw new TypeError("getEntriesByType: 1 argument required, but only 0 present");
    }
    return filterList(this.#entries, undefined, String(type));
  }
  getEntriesByName(name, type) {
    if (arguments.length < 1) {
      throw new TypeError("getEntriesByName: 1 argument required, but only 0 present");
    }
    return filterList(this.#entries, String(name), type === undefined ? undefined : String(type));
  }
}

function filterList(list, name, type) {
  const result = [];
  for (let i = 0; i < list.length; i++) {
    const e = list[i];
    if (type !== undefined && e.entryType !== type) {
      continue;
    }
    if (name !== undefined && e.name !== name) {
      continue;
    }
    ArrayPrototypePush(result, e);
  }
  return result;
}

class PerformanceObserver {
  #record;
  constructor(callback) {
    if (typeof callback !== "function") {
      throw new TypeError("PerformanceObserver: the callback is not a function");
    }
    this.#record = {
      observer: this,
      callback: callback,
      types: [],
      queue: [],
      // Locked to "multiple" (entryTypes) or "single" (type) by the first
      // observe() call, permanently — the spec forbids switching forms.
      mode: null,
    };
  }
  observe(options) {
    const record = this.#record;
    if (options === null || typeof options !== "object") {
      throw new TypeError("observe: options must be an object");
    }
    const hasEntryTypes = options.entryTypes !== undefined;
    const hasType = options.type !== undefined;
    if (!hasEntryTypes && !hasType) {
      throw new TypeError("observe: an observe() call must include either entryTypes or type");
    }
    if (hasEntryTypes && (hasType || options.buffered !== undefined)) {
      throw new TypeError("observe: entryTypes cannot be combined with type or buffered");
    }
    const mode = hasEntryTypes ? "multiple" : "single";
    if (record.mode !== null && record.mode !== mode) {
      throw domException(
        "observe: this observer already used the " +
          (record.mode === "multiple" ? "entryTypes" : "type") +
          " form and cannot switch",
        "InvalidModificationError"
      );
    }
    if (hasEntryTypes) {
      const requested = convertStringSequence(options.entryTypes, "entryTypes");
      const supported = [];
      for (let i = 0; i < requested.length; i++) {
        const t = requested[i];
        if (
          ArrayPrototypeIndexOf(SUPPORTED_ENTRY_TYPES, t) !== -1 &&
          ArrayPrototypeIndexOf(supported, t) === -1
        ) {
          ArrayPrototypePush(supported, t);
        }
      }
      // Spec: no supported types → abort without throwing (and without
      // registering).
      if (supported.length === 0) {
        return;
      }
      record.mode = mode;
      record.types = supported; // the entryTypes form replaces the set
    } else {
      const t = String(options.type);
      if (ArrayPrototypeIndexOf(SUPPORTED_ENTRY_TYPES, t) === -1) {
        return;
      }
      record.mode = mode;
      if (ArrayPrototypeIndexOf(record.types, t) === -1) {
        ArrayPrototypePush(record.types, t); // the type form accumulates
      }
      if (options.buffered) {
        for (let i = 0; i < entries.length; i++) {
          if (entries[i].entryType === t) {
            ArrayPrototypePush(record.queue, entries[i]);
          }
        }
        if (record.queue.length !== 0) {
          scheduleFlush();
        }
      }
    }
    if (ArrayPrototypeIndexOf(observers, record) === -1) {
      ArrayPrototypePush(observers, record);
    }
  }
  disconnect() {
    const record = this.#record;
    const idx = ArrayPrototypeIndexOf(observers, record);
    if (idx !== -1) {
      ArrayPrototypeSplice(observers, idx, 1);
    }
    record.queue = []; // pending records are dropped; takeRecords() first to keep them
  }
  takeRecords() {
    const record = this.#record;
    const taken = chronological(record.queue);
    record.queue = [];
    return taken;
  }
  static get supportedEntryTypes() {
    return SUPPORTED_ENTRY_TYPES;
  }
}

class Performance extends EventTarget {
  constructor(token) {
    if (token !== kInternal) {
      throw illegalConstructor();
    }
    super();
    // The EventTarget base installs `_listeners` as an own enumerable field;
    // keep it out of Object.keys(performance)/JSON.stringify(performance).
    ObjectDefineProperty(this, "_listeners", {
      value: this._listeners,
      writable: true,
      enumerable: false,
      configurable: true,
    });
  }
  get timeOrigin() {
    return timeOrigin;
  }
  now() {
    return now();
  }
  toJSON() {
    return { timeOrigin: timeOrigin };
  }
  mark(markName, markOptions) {
    if (arguments.length < 1) {
      throw new TypeError("mark: 1 argument required, but only 0 present");
    }
    const entry = new PerformanceMark(markName, markOptions);
    bufferEntry(entry);
    return entry;
  }
  measure(measureName, startOrMeasureOptions, endMark) {
    if (arguments.length < 1) {
      throw new TypeError("measure: 1 argument required, but only 0 present");
    }
    const name = String(measureName);
    const isOptionsObject =
      startOrMeasureOptions !== null && typeof startOrMeasureOptions === "object";
    let startTime;
    let endTime;
    let detail = null;
    if (
      isOptionsObject &&
      (startOrMeasureOptions.start !== undefined ||
        startOrMeasureOptions.end !== undefined ||
        startOrMeasureOptions.duration !== undefined ||
        startOrMeasureOptions.detail !== undefined)
    ) {
      const o = startOrMeasureOptions;
      if (endMark !== undefined) {
        throw new TypeError("measure: endMark cannot be combined with a measure options object");
      }
      if (o.start === undefined && o.end === undefined) {
        throw new TypeError("measure: the options object must specify start and/or end");
      }
      if (o.start !== undefined && o.end !== undefined && o.duration !== undefined) {
        throw new TypeError("measure: cannot specify start, end and duration together");
      }
      let duration;
      if (o.duration !== undefined) {
        duration = Number(o.duration);
        if (!NumberIsFinite(duration)) {
          throw new TypeError("measure: duration must be a finite number");
        }
      }
      if (o.end !== undefined) {
        endTime = convertMarkToTimestamp(o.end);
      } else if (o.start !== undefined && duration !== undefined) {
        endTime = convertMarkToTimestamp(o.start) + duration;
      } else {
        endTime = now();
      }
      if (o.start !== undefined) {
        startTime = convertMarkToTimestamp(o.start);
      } else if (duration !== undefined) {
        startTime = endTime - duration;
      } else {
        startTime = 0;
      }
      if (o.detail !== undefined && o.detail !== null) {
        detail = cloneDetail(o.detail);
      }
    } else {
      endTime = endMark !== undefined ? convertMarkToTimestamp(endMark) : now();
      // A members-free options object means "no start given", not a mark name.
      startTime =
        startOrMeasureOptions !== undefined && !isOptionsObject
          ? convertMarkToTimestamp(startOrMeasureOptions)
          : 0;
    }
    const entry = new PerformanceMeasure(kInternal, name, startTime, endTime - startTime, detail);
    bufferEntry(entry);
    return entry;
  }
  clearMarks(markName) {
    clearEntries("mark", markName === undefined ? undefined : String(markName));
  }
  clearMeasures(measureName) {
    clearEntries("measure", measureName === undefined ? undefined : String(measureName));
  }
  getEntries() {
    return chronological(entries);
  }
  getEntriesByType(type) {
    if (arguments.length < 1) {
      throw new TypeError("getEntriesByType: 1 argument required, but only 0 present");
    }
    return queryEntries(undefined, String(type));
  }
  getEntriesByName(name, type) {
    if (arguments.length < 1) {
      throw new TypeError("getEntriesByName: 1 argument required, but only 0 present");
    }
    return queryEntries(String(name), type === undefined ? undefined : String(type));
  }
}

// WebIDL shape: interface members are enumerable prototype properties and the
// class string is a configurable, non-writable Symbol.toStringTag; class
// syntax alone yields non-enumerable members.
function finishInterface(ctor, tag, members) {
  const proto = ctor.prototype;
  ObjectDefineProperty(proto, SymbolToStringTag, {
    value: tag,
    writable: false,
    enumerable: false,
    configurable: true,
  });
  for (let i = 0; i < members.length; i++) {
    const desc = ObjectGetOwnPropertyDescriptor(proto, members[i]);
    desc.enumerable = true;
    ObjectDefineProperty(proto, members[i], desc);
  }
}
finishInterface(PerformanceEntry, "PerformanceEntry", [
  "name", "entryType", "startTime", "duration", "toJSON",
]);
finishInterface(PerformanceMark, "PerformanceMark", ["detail", "toJSON"]);
finishInterface(PerformanceMeasure, "PerformanceMeasure", ["detail", "toJSON"]);
finishInterface(PerformanceObserverEntryList, "PerformanceObserverEntryList", [
  "getEntries", "getEntriesByType", "getEntriesByName",
]);
finishInterface(PerformanceObserver, "PerformanceObserver", [
  "observe", "disconnect", "takeRecords",
]);
finishInterface(Performance, "Performance", [
  "timeOrigin", "now", "toJSON", "mark", "measure",
  "clearMarks", "clearMeasures",
  "getEntries", "getEntriesByType", "getEntriesByName",
]);

g.Performance = Performance;
g.PerformanceEntry = PerformanceEntry;
g.PerformanceMark = PerformanceMark;
g.PerformanceMeasure = PerformanceMeasure;
g.PerformanceObserver = PerformanceObserver;
g.PerformanceObserverEntryList = PerformanceObserverEntryList;
g.performance = new Performance(kInternal);
