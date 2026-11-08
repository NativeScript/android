package com.tns;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.IdentityHashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

public class ObjectTableTest {
    @Test
    public void registersLooksUpAndRemoves() {
        ObjectTable table = new ObjectTable();
        Object a = new Object();
        Object b = new Object();
        table.putStrong(7, a);
        table.putStrong(9, b);

        assertEquals(7, table.findId(a));
        assertEquals(9, table.findId(b));
        assertSame(a, table.get(7));
        assertEquals(ObjectTable.NO_ID, table.findId(new Object()));

        table.remove(7);
        assertEquals(ObjectTable.NO_ID, table.findId(a));
        assertNull(table.get(7));
        assertEquals(9, table.findId(b));
        assertEquals(1, table.size());
    }

    @Test
    public void weakEntriesStayFindableWhileTheObjectLives() {
        ObjectTable table = new ObjectTable();
        Object a = new Object();
        table.putStrong(1, a);

        assertTrue(table.weakenAndCheckAlive(1));
        assertSame(a, table.get(1));
        assertEquals(1, table.findId(a));
        assertTrue(table.weakenAndCheckAlive(1));
        assertFalse(table.weakenAndCheckAlive(2));
    }

    @Test
    public void registeringAnIdAgainReplacesItsEntry() {
        ObjectTable table = new ObjectTable();
        Object a = new Object();
        Object b = new Object();
        table.putStrong(5, a);
        table.weakenAndCheckAlive(5);
        table.putStrong(5, a);
        assertEquals(1, table.size());
        assertSame(a, table.get(5));

        table.putStrong(5, b);
        assertEquals(1, table.size());
        assertSame(b, table.get(5));
        assertEquals(5, table.findId(b));
        assertEquals(ObjectTable.NO_ID, table.findId(a));
    }

    @Test
    public void aRegisteredWeakReferenceIsNotMistakenForAWeakEntry() {
        ObjectTable table = new ObjectTable();
        java.lang.ref.WeakReference<Object> ref = new java.lang.ref.WeakReference<>(new Object());
        table.putStrong(3, ref);

        assertSame(ref, table.get(3));
        assertEquals(3, table.findId(ref));
    }

    @Test
    public void matchesReferenceMapsUnderRandomOperations() {
        Random random = new Random(42);
        ObjectTable table = new ObjectTable();
        Map<Integer, Object> byId = new HashMap<>();
        Map<Object, Integer> byObject = new IdentityHashMap<>();
        List<Integer> live = new ArrayList<>();
        int nextId = 0;

        for (int step = 0; step < 200_000; step++) {
            int op = random.nextInt(10);
            if (op < 5 || live.isEmpty()) {
                Object obj = new Object();
                int id = nextId++;
                table.putStrong(id, obj);
                byId.put(id, obj);
                byObject.put(obj, id);
                live.add(id);
            } else if (op < 8) {
                int id = live.get(random.nextInt(live.size()));
                Object obj = byId.get(id);
                assertEquals(id, table.findId(obj));
                assertSame(obj, table.get(id));
            } else {
                int index = random.nextInt(live.size());
                int id = live.get(index);
                live.set(index, live.get(live.size() - 1));
                live.remove(live.size() - 1);
                Object obj = byId.remove(id);
                byObject.remove(obj);
                table.remove(id);
                assertEquals(ObjectTable.NO_ID, table.findId(obj));
                assertNull(table.get(id));
            }
        }

        assertEquals(byId.size(), table.size());
        for (Map.Entry<Object, Integer> entry : byObject.entrySet()) {
            assertEquals((int) entry.getValue(), table.findId(entry.getKey()));
        }
    }
}
