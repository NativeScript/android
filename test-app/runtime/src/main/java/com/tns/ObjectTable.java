package com.tns;

import java.lang.ref.WeakReference;

/**
 * A runtime's registry of the Java objects its JS wrappers refer to, by object id. An entry
 * holds its object strongly while the JS wrapper is reachable and through a WeakReference
 * after the wrapper was collected (until the object is collected too).
 *
 * Any thread may read and findOrRegister: every access synchronizes on the table. Neither
 * table allocates per entry.
 *
 * A monitor rather than java.util.concurrent locks: those live in the boot classpath but
 * outside the boot image, so on device they run interpreted, and an app's profile can't
 * compile them. Uncontended monitor enter/exit is compiled into every caller.
 */
final class ObjectTable {
    static final int NO_ID = -1;

    // Set in findOrRegister's result when that call registered the object.
    static final long FRESH = 1L << 32;

    interface IdSource {
        int newId();
    }

    private static Object deref(Object value) {
        return value instanceof WeakRef ? ((WeakRef) value).get() : value;
    }

    /*
     * id -> entry, as a radix tree indexed by id: chunks of leaves of slots. Ids are unsigned
     * 32-bit values carried in an int (NO_ID is the one excluded value); they come from a
     * monotonic counter and are never reused, so a leaf or chunk that empties is dropped
     * unless it is the newest; an id registered after its leaf was dropped re-creates it.
     * Leaves and chunks are small, so a long-lived object keeps at most one of each alive.
     */
    private static final int LEAF_BITS = 6;
    private static final int LEAF_SIZE = 1 << LEAF_BITS;
    private static final int LEAF_MASK = LEAF_SIZE - 1;
    private static final int CHUNK_BITS = 6;
    private static final int CHUNK_MASK = (1 << CHUNK_BITS) - 1;
    private static final int CHUNK_SHIFT = LEAF_BITS + CHUNK_BITS;

    private static final class Leaf {
        // The object, or a WeakRef to it; null for no entry.
        final Object[] values = new Object[LEAF_SIZE];
        // The object's identity hash, to find its identity index entry on removal.
        final int[] hashes = new int[LEAF_SIZE];
        int live;
    }

    private static final class Chunk {
        final Leaf[] leaves = new Leaf[1 << CHUNK_BITS];
        int liveLeaves;
    }

    private Chunk[] chunks = new Chunk[16];
    // id >>> LEAF_BITS of the highest id registered.
    private int newestLeaf = 0;
    private int idCount = 0;

    // identity hash -> id + 1, open addressing; the id's entry decides which object it holds.
    // identityIndexHashes[i] is the hash of identityIndex[i]'s object, so a probe only looks
    // the id up when the hashes match.
    private int[] identityIndex = new int[1024];
    private int[] identityIndexHashes = new int[1024];
    private int identityCount = 0;

    /** Registers obj under id, held strongly, replacing whatever id held before. */
    void putStrong(int id, Object obj) {
        synchronized (this) {
            putStrongLocked(id, obj);
        }
    }

    private void putStrongLocked(int id, Object obj) {
        putStrongLocked(id, obj, System.identityHashCode(obj));
    }

    private void putStrongLocked(int id, Object obj, int hash) {
        Leaf existingLeaf = leafFor(id);
        if (existingLeaf != null) {
            Object value = existingLeaf.values[id & LEAF_MASK];
            if (value != null) {
                if (deref(value) == obj) {
                    existingLeaf.values[id & LEAF_MASK] = obj;
                    return;
                }
                removeLocked(id);
            }
        }

        insertEntry(id, obj, hash);
        idCount++;

        if ((identityCount + 1) * 2 > identityIndex.length) {
            growIdentity();
        }
        insertIdentity(hash, id);
        identityCount++;
    }

    /**
     * The id obj is registered under, registering it strongly under a new id from ids when
     * it has none; FRESH is set in the result when this call registered it. The lookup and
     * the insert happen under one lock, so concurrent calls agree on one id.
     */
    long findOrRegister(Object obj, IdSource ids) {
        synchronized (this) {
            int hash = System.identityHashCode(obj);
            int id = findIdLocked(obj, hash);
            if (id != NO_ID) {
                return id & 0xFFFFFFFFL;
            }
            id = ids.newId();
            putStrongLocked(id, obj, hash);
            return (id & 0xFFFFFFFFL) | FRESH;
        }
    }

    /**
     * The id obj is registered under, or NO_ID. From a thread without the isolate lock, the
     * object may have been released since, which the caller must check under the lock.
     */
    int findId(Object obj) {
        synchronized (this) {
            return findIdLocked(obj);
        }
    }

    private int findIdLocked(Object obj) {
        return findIdLocked(obj, System.identityHashCode(obj));
    }

    private int findIdLocked(Object obj, int hash) {
        int mask = identityIndex.length - 1;
        int i = mix(hash) & mask;
        while (true) {
            int entry = identityIndex[i];
            if (entry == 0) {
                return NO_ID;
            }
            int id = entry - 1;
            if (identityIndexHashes[i] == hash && getLocked(id) == obj) {
                return id;
            }
            i = (i + 1) & mask;
        }
    }

    /** The object registered under id; null if there is none or it was collected. */
    Object get(int id) {
        synchronized (this) {
            return getLocked(id);
        }
    }

    private Object getLocked(int id) {
        Object value = rawLocked(id);
        return value == null ? null : deref(value);
    }

    boolean contains(int id) {
        synchronized (this) {
            return rawLocked(id) != null;
        }
    }

    /**
     * The JS wrapper for id was collected: hold the object weakly from now on. Returns whether
     * the object is still alive; an entry whose object was already collected is removed.
     */
    boolean weakenAndCheckAlive(int id) {
        synchronized (this) {
            return weakenAndCheckAliveLocked(id);
        }
    }

    /**
     * weakenAndCheckAlive for ids[0..count), under one lock; overwrites the id of every
     * object no longer alive with NO_ID.
     */
    void weakenAndCheckAlive(int[] ids, int count) {
        if (count == 0) {
            return;
        }
        synchronized (this) {
            weakenAndCheckAliveAllLocked(ids, count);
        }
    }

    private void weakenAndCheckAliveAllLocked(int[] ids, int count) {
        for (int i = 0; i < count; i++) {
            if (!weakenAndCheckAliveLocked(ids[i])) {
                ids[i] = NO_ID;
            }
        }
    }

    private boolean weakenAndCheckAliveLocked(int id) {
        Leaf leaf = leafFor(id);
        Object value = leaf == null ? null : leaf.values[id & LEAF_MASK];
        if (value == null) {
            return false;
        }
        if (value instanceof WeakRef) {
            if (((WeakRef) value).get() != null) {
                return true;
            }
            removeLocked(id);
            return false;
        }
        leaf.values[id & LEAF_MASK] = new WeakRef(value);
        return true;
    }

    void remove(int id) {
        synchronized (this) {
            removeLocked(id);
        }
    }

    private void removeLocked(int id) {
        Leaf leaf = leafFor(id);
        int i = id & LEAF_MASK;
        if (leaf == null || leaf.values[i] == null) {
            return;
        }
        int hash = leaf.hashes[i];
        leaf.values[i] = null;
        leaf.hashes[i] = 0;
        leaf.live--;
        int leafNumber = id >>> LEAF_BITS;
        if (leafNumber != newestLeaf) {
            releaseIfEmpty(leafNumber);
        }
        idCount--;
        removeIdentity(hash, id);
        identityCount--;
    }

    int size() {
        return idCount;
    }

    // Distinguishes the table's own weak entries from a registered object that happens to be
    // a WeakReference.
    private static final class WeakRef extends WeakReference<Object> {
        WeakRef(Object referent) {
            super(referent);
        }
    }

    private Leaf leafFor(int id) {
        if (id == NO_ID) {
            return null;
        }
        int c = id >>> CHUNK_SHIFT;
        Chunk chunk = c < chunks.length ? chunks[c] : null;
        return chunk == null ? null : chunk.leaves[(id >>> LEAF_BITS) & CHUNK_MASK];
    }

    // The entry for id as stored (the object or a WeakRef to it), or null.
    private Object rawLocked(int id) {
        Leaf leaf = leafFor(id);
        return leaf == null ? null : leaf.values[id & LEAF_MASK];
    }

    // Adds an entry for an id that has none.
    private void insertEntry(int id, Object value, int hash) {
        int c = id >>> CHUNK_SHIFT;
        if (c >= chunks.length) {
            int length = chunks.length;
            while (length <= c) {
                length *= 2;
            }
            Chunk[] grown = new Chunk[length];
            System.arraycopy(chunks, 0, grown, 0, chunks.length);
            chunks = grown;
        }
        Chunk chunk = chunks[c];
        if (chunk == null) {
            chunk = new Chunk();
            chunks[c] = chunk;
        }
        int l = (id >>> LEAF_BITS) & CHUNK_MASK;
        Leaf leaf = chunk.leaves[l];
        if (leaf == null) {
            leaf = new Leaf();
            chunk.leaves[l] = leaf;
            chunk.liveLeaves++;
        }
        int i = id & LEAF_MASK;
        leaf.values[i] = value;
        leaf.hashes[i] = hash;
        leaf.live++;
        int leafNumber = id >>> LEAF_BITS;
        if (leafNumber > newestLeaf) {
            int previous = newestLeaf;
            newestLeaf = leafNumber;
            // Kept while it was the newest; nothing else drops it once it is empty.
            releaseIfEmpty(previous);
        }
    }

    // Drops leaf leafNumber if it is empty, then its chunk if that empties and is not the
    // newest.
    private void releaseIfEmpty(int leafNumber) {
        int c = leafNumber >>> CHUNK_BITS;
        Chunk chunk = c < chunks.length ? chunks[c] : null;
        if (chunk == null) {
            return;
        }
        int l = leafNumber & CHUNK_MASK;
        Leaf leaf = chunk.leaves[l];
        if (leaf == null || leaf.live != 0) {
            return;
        }
        chunk.leaves[l] = null;
        if (--chunk.liveLeaves == 0 && c != (newestLeaf >>> CHUNK_BITS)) {
            chunks[c] = null;
        }
    }

    private void insertIdentity(int hash, int id) {
        insertIdentity(identityIndex, identityIndexHashes, hash, id);
    }

    private static void insertIdentity(int[] index, int[] hashes, int hash, int id) {
        int mask = index.length - 1;
        int i = mix(hash) & mask;
        while (index[i] != 0) {
            i = (i + 1) & mask;
        }
        hashes[i] = hash;
        index[i] = id + 1;
    }

    private void removeIdentity(int hash, int id) {
        int mask = identityIndex.length - 1;
        int i = mix(hash) & mask;
        while (identityIndex[i] != id + 1) {
            i = (i + 1) & mask;
        }
        int hole = i;
        int j = i;
        while (true) {
            j = (j + 1) & mask;
            int entry = identityIndex[j];
            if (entry == 0) {
                break;
            }
            int home = mix(identityIndexHashes[j]) & mask;
            if (!isBetween(hole, home, j)) {
                identityIndex[hole] = entry;
                identityIndexHashes[hole] = identityIndexHashes[j];
                hole = j;
            }
        }
        identityIndex[hole] = 0;
        identityIndexHashes[hole] = 0;
    }

    // Whether home lies cyclically in (hole, j], i.e. the entry at j must stay put.
    private static boolean isBetween(int hole, int home, int j) {
        return hole <= j ? (hole < home && home <= j) : (hole < home || home <= j);
    }

    private void growIdentity() {
        int[] oldIndex = identityIndex;
        int[] oldHashes = identityIndexHashes;
        int[] index = new int[oldIndex.length * 2];
        int[] hashes = new int[index.length];
        for (int i = 0; i < oldIndex.length; i++) {
            if (oldIndex[i] != 0) {
                insertIdentity(index, hashes, oldHashes[i], oldIndex[i] - 1);
            }
        }
        identityIndexHashes = hashes;
        identityIndex = index;
    }

    private static int mix(int h) {
        h ^= (h >>> 16);
        h *= 0x85ebca6b;
        h ^= (h >>> 13);
        return h;
    }
}
