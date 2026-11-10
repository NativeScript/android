package com.tns;

import java.lang.ref.WeakReference;

/**
 * A runtime's registry of the Java objects its JS wrappers refer to, by object id. An entry
 * holds its object strongly while the JS wrapper is reachable and through a WeakReference
 * after the wrapper was collected (until the object is collected too).
 *
 * Where StampedLock exists (API 24+) any thread may read and findOrRegister: writers are
 * serialized by its write lock and exact reads validate against it. Without it, every
 * mutation and every exact read must come from the thread that holds the runtime's
 * isolate lock. Neither table allocates per entry.
 */
final class ObjectTable {
    static final int NO_ID = -1;

    // Set in findOrRegister's result when that call registered the object.
    static final long FRESH = 1L << 32;

    interface IdSource {
        int newId();
    }

    // Null where StampedLock is unavailable (API < 24): findIdOptimistic then always misses.
    private final Stamps stamps;

    ObjectTable() {
        this(false);
    }

    ObjectTable(boolean optimisticReads) {
        stamps = optimisticReads ? new Stamps() : null;
    }

    /**
     * Writers are already serialized by the isolate lock; the write stamp only lets an
     * optimistic reader detect that a write overlapped it. Kept in its own class so that
     * runtimes where StampedLock is missing never load it.
     */
    private static final class Stamps {
        private final java.util.concurrent.locks.StampedLock lock = new java.util.concurrent.locks.StampedLock();

        long beginWrite() {
            return lock.writeLock();
        }

        void endWrite(long stamp) {
            lock.unlockWrite(stamp);
        }

        long beginRead() {
            return lock.tryOptimisticRead();
        }

        long readLock() {
            return lock.readLock();
        }

        void unlockRead(long stamp) {
            lock.unlockRead(stamp);
        }

        boolean validate(long stamp) {
            return lock.validate(stamp);
        }
    }

    /**
     * The id obj is registered under, from any thread, without the isolate lock. A miss
     * (NO_ID) is not authoritative: it is also returned when a write overlapped the read.
     * A hit was correct at the time of the read; the object may have been released since,
     * which the caller must check under the isolate lock.
     */
    int findIdOptimistic(Object obj) {
        if (stamps == null) {
            return NO_ID;
        }
        long stamp = stamps.beginRead();
        if (stamp == 0) {
            return NO_ID;
        }
        int id;
        try {
            id = findIdUnlocked(obj);
        } catch (RuntimeException e) {
            // A torn view of a resize can index past an array; the stamp check below fails then.
            id = NO_ID;
        }
        return stamps.validate(stamp) ? id : NO_ID;
    }

    private int findIdUnlocked(Object obj) {
        int[] index = identityIndex;
        int[] hashes = identityIndexHashes;
        int mask = index.length - 1;
        int hash = System.identityHashCode(obj);
        int i = mix(hash) & mask;
        for (int probes = 0; probes <= mask; probes++) {
            int entry = index[i];
            if (entry == 0) {
                return NO_ID;
            }
            if (hashes[i] == hash && getUnlocked(entry - 1) == obj) {
                return entry - 1;
            }
            i = (i + 1) & mask;
        }
        return NO_ID;
    }

    // get() without the lock. Safe on a torn view: every array it reads is indexed within
    // its own bounds, and a stale one only makes the caller's stamp check fail.
    private Object getUnlocked(int id) {
        if (id < 0) {
            return null;
        }
        Chunk[] cs = chunks;
        int c = id >>> CHUNK_SHIFT;
        if (c >= cs.length) {
            return null;
        }
        Chunk chunk = cs[c];
        if (chunk == null) {
            return null;
        }
        Leaf leaf = chunk.leaves[(id >>> LEAF_BITS) & CHUNK_MASK];
        if (leaf == null) {
            return null;
        }
        Object value = leaf.values[id & LEAF_MASK];
        return value == null ? null : deref(value);
    }

    private static Object deref(Object value) {
        return value instanceof WeakRef ? ((WeakRef) value).get() : value;
    }

    /*
     * id -> entry, as a radix tree indexed by id: chunks of leaves of slots. Ids come from a
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
        if (stamps == null) {
            putStrongLocked(id, obj);
            return;
        }
        long stamp = stamps.beginWrite();
        try {
            putStrongLocked(id, obj);
        } finally {
            stamps.endWrite(stamp);
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

    boolean allowsConcurrentRegistration() {
        return stamps != null;
    }

    /**
     * The id obj is registered under, registering it strongly under a new id from ids when
     * it has none; FRESH is set in the result when this call registered it. The lookup and
     * the insert happen under one write lock, so concurrent calls agree on one id. Only
     * when allowsConcurrentRegistration().
     */
    long findOrRegister(Object obj, IdSource ids) {
        long stamp = stamps.beginWrite();
        try {
            int hash = System.identityHashCode(obj);
            int id = findIdLocked(obj, hash);
            if (id != NO_ID) {
                return id;
            }
            id = ids.newId();
            putStrongLocked(id, obj, hash);
            return id | FRESH;
        } finally {
            stamps.endWrite(stamp);
        }
    }

    /** The id obj is registered under, or NO_ID. */
    int findId(Object obj) {
        if (stamps == null) {
            return findIdLocked(obj);
        }
        long stamp = stamps.beginRead();
        if (stamp != 0) {
            try {
                int id = findIdUnlocked(obj);
                if (stamps.validate(stamp)) {
                    return id;
                }
            } catch (RuntimeException e) {
                // A torn view of a resize; retried under the read lock.
            }
        }
        stamp = stamps.readLock();
        try {
            return findIdLocked(obj);
        } finally {
            stamps.unlockRead(stamp);
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
        if (stamps == null) {
            return getLocked(id);
        }
        long stamp = stamps.beginRead();
        if (stamp != 0) {
            try {
                Object value = getUnlocked(id);
                if (stamps.validate(stamp)) {
                    return value;
                }
            } catch (RuntimeException e) {
                // A torn view of a resize; retried under the read lock.
            }
        }
        stamp = stamps.readLock();
        try {
            return getLocked(id);
        } finally {
            stamps.unlockRead(stamp);
        }
    }

    private Object getLocked(int id) {
        Object value = rawLocked(id);
        return value == null ? null : deref(value);
    }

    boolean contains(int id) {
        if (stamps == null) {
            return rawLocked(id) != null;
        }
        long stamp = stamps.readLock();
        try {
            return rawLocked(id) != null;
        } finally {
            stamps.unlockRead(stamp);
        }
    }

    /**
     * The JS wrapper for id was collected: hold the object weakly from now on. Returns whether
     * the object is still alive; an entry whose object was already collected is removed.
     */
    boolean weakenAndCheckAlive(int id) {
        if (stamps == null) {
            return weakenAndCheckAliveLocked(id);
        }
        long stamp = stamps.beginWrite();
        try {
            return weakenAndCheckAliveLocked(id);
        } finally {
            stamps.endWrite(stamp);
        }
    }

    /**
     * weakenAndCheckAlive for ids[0..count), under one write lock; overwrites the id of
     * every object no longer alive with NO_ID.
     */
    void weakenAndCheckAlive(int[] ids, int count) {
        if (count == 0) {
            return;
        }
        long stamp = stamps == null ? 0 : stamps.beginWrite();
        try {
            for (int i = 0; i < count; i++) {
                if (!weakenAndCheckAliveLocked(ids[i])) {
                    ids[i] = NO_ID;
                }
            }
        } finally {
            if (stamps != null) {
                stamps.endWrite(stamp);
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
        if (stamps == null) {
            removeLocked(id);
            return;
        }
        long stamp = stamps.beginWrite();
        try {
            removeLocked(id);
        } finally {
            stamps.endWrite(stamp);
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
        if (id < 0) {
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
