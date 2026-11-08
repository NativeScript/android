package com.tns;

import java.lang.ref.WeakReference;

/**
 * A runtime's registry of the Java objects its JS wrappers refer to, by object id. An entry
 * holds its object strongly while the JS wrapper is reachable and through a WeakReference
 * after the wrapper was collected (until the object is collected too).
 *
 * Not synchronized: every access must come from the thread that holds the runtime's isolate
 * lock. Neither table allocates per entry.
 */
final class ObjectTable {
    static final int NO_ID = -1;

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
        int existing = findIdSlot(id);
        if (existing >= 0) {
            Object value = values[existing];
            if ((value instanceof WeakRef ? ((WeakRef) value).get() : value) == obj) {
                values[existing] = obj;
                return;
            }
            remove(id);
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

    /** The id obj is registered under, or NO_ID. */
    int findId(Object obj) {
        int mask = identityIndex.length - 1;
        int i = mix(System.identityHashCode(obj)) & mask;
        while (true) {
            int entry = identityIndex[i];
            if (entry == 0) {
                return NO_ID;
            }
            int id = entry - 1;
            if (get(id) == obj) {
                return id;
            }
            i = (i + 1) & mask;
        }
    }

    /** The object registered under id; null if there is none or it was collected. */
    Object get(int id) {
        int slot = findIdSlot(id);
        if (slot < 0) {
            return null;
        }
        Object value = values[slot];
        return value instanceof WeakRef ? ((WeakRef) value).get() : value;
    }

    boolean contains(int id) {
        return findIdSlot(id) >= 0;
    }

    /**
     * The JS wrapper for id was collected: hold the object weakly from now on. Returns whether
     * the object is still alive; an entry whose object was already collected is removed.
     */
    boolean weakenAndCheckAlive(int id) {
        int slot = findIdSlot(id);
        if (slot < 0) {
            return false;
        }
        Object value = values[slot];
        if (value instanceof WeakRef) {
            if (((WeakRef) value).get() != null) {
                return true;
            }
            remove(id);
            return false;
        }
        values[slot] = new WeakRef(value);
        return true;
    }

    void remove(int id) {
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
