package com.tns;

/**
 * Process-wide dense ids for the method names Java calls into JS with, so native code can
 * keep each name's V8 string instead of copying the characters out of the String on every
 * call. An id names the same string in every runtime.
 *
 * Lookups take no lock. The table is open-addressed on String.hashCode(), which String
 * caches, and entries are immutable, so a reader racing an insert either sees a complete
 * entry or misses and registers under the lock.
 */
final class MethodNameIds {
    private static final class Entry {
        final String name;
        final int id;

        Entry(String name, int id) {
            this.name = name;
            this.id = id;
        }
    }

    private static volatile Entry[] table = new Entry[256];
    private static int count;

    private MethodNameIds() {
    }

    static int idOf(String name) {
        if (name == null) {
            return -1;
        }
        Entry[] t = table;
        int mask = t.length - 1;
        for (int i = slot(name, mask); ; i = (i + 1) & mask) {
            Entry e = t[i];
            if (e == null) {
                return register(name);
            }
            if (e.name == name || e.name.equals(name)) {
                return e.id;
            }
        }
    }

    private static synchronized int register(String name) {
        Entry[] t = table;
        int mask = t.length - 1;
        int i = slot(name, mask);
        for (Entry e; (e = t[i]) != null; i = (i + 1) & mask) {
            if (e.name.equals(name)) {
                return e.id;
            }
        }
        int id = count++;
        if (count * 2 > t.length) {
            t = grow(t);
            mask = t.length - 1;
            for (i = slot(name, mask); t[i] != null; i = (i + 1) & mask) {
            }
        }
        t[i] = new Entry(name, id);
        table = t;
        return id;
    }

    private static Entry[] grow(Entry[] old) {
        Entry[] t = new Entry[old.length * 2];
        int mask = t.length - 1;
        for (Entry e : old) {
            if (e == null) {
                continue;
            }
            int i = slot(e.name, mask);
            while (t[i] != null) {
                i = (i + 1) & mask;
            }
            t[i] = e;
        }
        return t;
    }

    private static int slot(String name, int mask) {
        int h = name.hashCode();
        return (h ^ (h >>> 16)) & mask;
    }
}
