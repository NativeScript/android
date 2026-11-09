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
        int mask = index.length - 1;
        int i = mix(System.identityHashCode(obj)) & mask;
        for (int probes = 0; probes <= mask; probes++) {
            int entry = index[i];
            if (entry == 0) {
                return NO_ID;
            }
            if (getUnlocked(entry - 1) == obj) {
                return entry - 1;
            }
            i = (i + 1) & mask;
        }
        return NO_ID;
    }

    // get() with every probe bounded: a torn view need not contain the empty slot that
    // ends a probe.
    private Object getUnlocked(int id) {
        int[] keys = idKeys;
        Object[] vals = values;
        int mask = keys.length - 1;
        int i = mix(id) & mask;
        int key = id + 1;
        for (int probes = 0; probes <= mask; probes++) {
            int k = keys[i];
            if (k == key) {
                Object value = vals[i];
                return value instanceof WeakRef ? ((WeakRef) value).get() : value;
            }
            if (k == 0) {
                return null;
            }
            i = (i + 1) & mask;
        }
        return null;
    }

    // id -> entry, open addressing on the id. Keys are id + 1 so 0 marks an empty slot.
    private int[] idKeys = new int[1024];
    private Object[] values = new Object[1024];
    private int[] identityHashes = new int[1024];
    private int idCount = 0;

    // identity hash -> id + 1, open addressing; values[] decides which object an id holds.
    private int[] identityIndex = new int[1024];
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
        int existing = findIdSlot(id);
        if (existing >= 0) {
            Object value = values[existing];
            if ((value instanceof WeakRef ? ((WeakRef) value).get() : value) == obj) {
                values[existing] = obj;
                return;
            }
            removeLocked(id);
        }

        int hash = System.identityHashCode(obj);
        if ((idCount + 1) * 2 > idKeys.length) {
            growIds();
        }
        insertId(id, obj, hash);
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
            int id = findIdLocked(obj);
            if (id != NO_ID) {
                return id;
            }
            id = ids.newId();
            putStrongLocked(id, obj);
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
        int mask = identityIndex.length - 1;
        int i = mix(System.identityHashCode(obj)) & mask;
        while (true) {
            int entry = identityIndex[i];
            if (entry == 0) {
                return NO_ID;
            }
            int id = entry - 1;
            if (getLocked(id) == obj) {
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
        int slot = findIdSlot(id);
        if (slot < 0) {
            return null;
        }
        Object value = values[slot];
        return value instanceof WeakRef ? ((WeakRef) value).get() : value;
    }

    boolean contains(int id) {
        if (stamps == null) {
            return findIdSlot(id) >= 0;
        }
        long stamp = stamps.readLock();
        try {
            return findIdSlot(id) >= 0;
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
        int slot = findIdSlot(id);
        if (slot < 0) {
            return false;
        }
        Object value = values[slot];
        if (value instanceof WeakRef) {
            if (((WeakRef) value).get() != null) {
                return true;
            }
            removeLocked(id);
            return false;
        }
        values[slot] = new WeakRef(value);
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
        int slot = findIdSlot(id);
        if (slot < 0) {
            return;
        }
        int hash = identityHashes[slot];
        removeIdSlot(slot);
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

    private int findIdSlot(int id) {
        int mask = idKeys.length - 1;
        int i = mix(id) & mask;
        int key = id + 1;
        while (true) {
            int k = idKeys[i];
            if (k == key) {
                return i;
            }
            if (k == 0) {
                return -1;
            }
            i = (i + 1) & mask;
        }
    }

    private void insertId(int id, Object value, int hash) {
        int mask = idKeys.length - 1;
        int i = mix(id) & mask;
        while (idKeys[i] != 0) {
            i = (i + 1) & mask;
        }
        idKeys[i] = id + 1;
        values[i] = value;
        identityHashes[i] = hash;
    }

    // Backward-shift deletion keeps linear probing tombstone-free.
    private void removeIdSlot(int slot) {
        int mask = idKeys.length - 1;
        int hole = slot;
        int j = slot;
        while (true) {
            j = (j + 1) & mask;
            int key = idKeys[j];
            if (key == 0) {
                break;
            }
            int home = mix(key - 1) & mask;
            if (!isBetween(hole, home, j)) {
                idKeys[hole] = key;
                values[hole] = values[j];
                identityHashes[hole] = identityHashes[j];
                hole = j;
            }
        }
        idKeys[hole] = 0;
        values[hole] = null;
        identityHashes[hole] = 0;
    }

    private void insertIdentity(int hash, int id) {
        int mask = identityIndex.length - 1;
        int i = mix(hash) & mask;
        while (identityIndex[i] != 0) {
            i = (i + 1) & mask;
        }
        identityIndex[i] = id + 1;
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
            int home = mix(identityHashes[findIdSlot(entry - 1)]) & mask;
            if (!isBetween(hole, home, j)) {
                identityIndex[hole] = entry;
                hole = j;
            }
        }
        identityIndex[hole] = 0;
    }

    // Whether home lies cyclically in (hole, j], i.e. the entry at j must stay put.
    private static boolean isBetween(int hole, int home, int j) {
        return hole <= j ? (hole < home && home <= j) : (hole < home || home <= j);
    }

    private void growIds() {
        int[] oldKeys = idKeys;
        Object[] oldValues = values;
        int[] oldHashes = identityHashes;
        int capacity = oldKeys.length * 2;
        idKeys = new int[capacity];
        values = new Object[capacity];
        identityHashes = new int[capacity];
        for (int i = 0; i < oldKeys.length; i++) {
            if (oldKeys[i] != 0) {
                insertId(oldKeys[i] - 1, oldValues[i], oldHashes[i]);
            }
        }
    }

    private void growIdentity() {
        int[] old = identityIndex;
        identityIndex = new int[old.length * 2];
        for (int entry : old) {
            if (entry != 0) {
                insertIdentity(identityHashes[findIdSlot(entry - 1)], entry - 1);
            }
        }
    }

    private static int mix(int h) {
        h ^= (h >>> 16);
        h *= 0x85ebca6b;
        h ^= (h >>> 13);
        return h;
    }
}
