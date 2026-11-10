#ifndef OBJECTIDINDEX_H_
#define OBJECTIDINDEX_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace tns {

/*
 * Object id -> entry, as a radix tree indexed by id: chunks of leaves of slots. Ids come
 * from a per-runtime monotonic counter and are never reused, so a leaf or chunk that
 * empties is freed unless it is the newest; an id linked after its leaf was freed just
 * re-creates it. Leaves and chunks are small, so a long-lived entry keeps at most one of
 * each alive. Not thread-safe: used under the isolate lock.
 */
template <typename T>
class ObjectIdIndex {
 public:
  T* Find(int id) const {
    if (id < 0) {
      return nullptr;
    }
    size_t chunkIndex = static_cast<size_t>(id) >> kChunkShift;
    if (chunkIndex >= m_chunks.size()) {
      return nullptr;
    }
    const Chunk* chunk = m_chunks[chunkIndex].get();
    if (chunk == nullptr) {
      return nullptr;
    }
    const Leaf* leaf = chunk->leaves[LeafIndex(id)].get();
    return leaf == nullptr ? nullptr : leaf->slots[id & kLeafMask];
  }

  // Keeps an existing entry for id, like std::unordered_map::emplace.
  void Insert(int id, T* value) {
    if (id < 0) {
      return;
    }
    size_t chunkIndex = static_cast<size_t>(id) >> kChunkShift;
    if (chunkIndex >= m_chunks.size()) {
      m_chunks.resize(chunkIndex + 1);
    }
    auto& chunk = m_chunks[chunkIndex];
    if (chunk == nullptr) {
      chunk = std::make_unique<Chunk>();
    }
    auto& leaf = chunk->leaves[LeafIndex(id)];
    if (leaf == nullptr) {
      leaf = std::make_unique<Leaf>();
      chunk->liveLeaves++;
    }
    T*& slot = leaf->slots[id & kLeafMask];
    size_t leafNumber = static_cast<size_t>(id) >> kLeafBits;
    if (leafNumber > m_newestLeaf) {
      size_t previous = m_newestLeaf;
      m_newestLeaf = leafNumber;
      // Kept while it was the newest; nothing else frees it once it is empty.
      ReleaseIfEmpty(previous);
    }
    if (slot == nullptr) {
      slot = value;
      leaf->live++;
    }
  }

  void Erase(int id) {
    if (id < 0) {
      return;
    }
    size_t chunkIndex = static_cast<size_t>(id) >> kChunkShift;
    if (chunkIndex >= m_chunks.size() || m_chunks[chunkIndex] == nullptr) {
      return;
    }
    auto& chunk = m_chunks[chunkIndex];
    auto& leaf = chunk->leaves[LeafIndex(id)];
    if (leaf == nullptr) {
      return;
    }
    T*& slot = leaf->slots[id & kLeafMask];
    if (slot == nullptr) {
      return;
    }
    slot = nullptr;
    leaf->live--;
    size_t leafNumber = static_cast<size_t>(id) >> kLeafBits;
    if (leafNumber != m_newestLeaf) {
      ReleaseIfEmpty(leafNumber);
    }
  }

  // Empties the index, then calls f on every entry it held.
  template <typename F>
  void DrainAll(F&& f) {
    auto chunks = std::move(m_chunks);
    m_chunks.clear();
    m_newestLeaf = 0;
    for (auto& chunk : chunks) {
      if (chunk == nullptr) {
        continue;
      }
      for (auto& leaf : chunk->leaves) {
        if (leaf == nullptr) {
          continue;
        }
        for (T* entry : leaf->slots) {
          if (entry != nullptr) {
            f(entry);
          }
        }
      }
    }
  }

 private:
  static constexpr int kLeafBits = 6;
  static constexpr int kLeafMask = (1 << kLeafBits) - 1;
  static constexpr int kChunkLeafBits = 6;
  static constexpr int kChunkShift = kLeafBits + kChunkLeafBits;

  struct Leaf {
    std::array<T*, 1 << kLeafBits> slots{};
    uint32_t live = 0;
  };

  struct Chunk {
    std::array<std::unique_ptr<Leaf>, 1 << kChunkLeafBits> leaves;
    uint32_t liveLeaves = 0;
  };

  // Frees leaf leafNumber if it is empty, then its chunk if that empties and is not the
  // newest.
  void ReleaseIfEmpty(size_t leafNumber) {
    size_t chunkIndex = leafNumber >> kChunkLeafBits;
    if (chunkIndex >= m_chunks.size() || m_chunks[chunkIndex] == nullptr) {
      return;
    }
    auto& chunk = m_chunks[chunkIndex];
    auto& leaf = chunk->leaves[leafNumber & ((1 << kChunkLeafBits) - 1)];
    if (leaf == nullptr || leaf->live != 0) {
      return;
    }
    leaf.reset();
    if (--chunk->liveLeaves == 0 && chunkIndex != (m_newestLeaf >> kChunkLeafBits)) {
      chunk.reset();
    }
  }

  static size_t LeafIndex(int id) {
    return (static_cast<size_t>(id) >> kLeafBits) & ((1 << kChunkLeafBits) - 1);
  }

  std::vector<std::unique_ptr<Chunk>> m_chunks;
  size_t m_newestLeaf = 0;
};

}  // namespace tns

#endif /* OBJECTIDINDEX_H_ */
