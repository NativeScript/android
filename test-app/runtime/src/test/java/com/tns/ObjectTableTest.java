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
    public void concurrentReadsFindStableEntriesWhileAnotherThreadWrites() throws Exception {
        final ObjectTable table = new ObjectTable();
        final int stableCount = 64;
        final Object[] stable = new Object[stableCount];
        for (int i = 0; i < stableCount; i++) {
            stable[i] = new Object();
            table.putStrong(i, stable[i]);
        }

        final java.util.concurrent.atomic.AtomicBoolean done = new java.util.concurrent.atomic.AtomicBoolean();
        final java.util.concurrent.atomic.AtomicReference<String> failure = new java.util.concurrent.atomic.AtomicReference<>();
        final java.util.concurrent.atomic.AtomicLong hits = new java.util.concurrent.atomic.AtomicLong();

        Thread writer = new Thread(new Runnable() {
            @Override
            public void run() {
                Random random = new Random(7);
                List<Integer> churn = new ArrayList<>();
                int nextId = stableCount;
                while (!done.get()) {
                    // Grow well past the initial capacity so readers overlap resizes and
                    // backward-shift deletions, then shrink again.
                    if (churn.size() < 20_000 && random.nextInt(3) != 0) {
                        int id = nextId++;
                        table.putStrong(id, new Object());
                        churn.add(id);
                    } else if (!churn.isEmpty()) {
                        int id = churn.remove(churn.size() - 1);
                        if (random.nextBoolean()) {
                            table.weakenAndCheckAlive(id);
                        }
                        table.remove(id);
                    }
                }
            }
        });

        Thread[] readers = new Thread[3];
        for (int r = 0; r < readers.length; r++) {
            final int seed = r;
            readers[r] = new Thread(new Runnable() {
                @Override
                public void run() {
                    Random random = new Random(seed);
                    while (!done.get()) {
                        int expected = random.nextInt(stableCount);
                        int id = table.findId(stable[expected]);
                        if (id == expected) {
                            hits.incrementAndGet();
                        } else {
                            failure.compareAndSet(null, "expected " + expected + ", got " + id);
                        }
                    }
                }
            });
        }

        writer.start();
        for (Thread reader : readers) {
            reader.start();
        }
        Thread.sleep(2000);
        done.set(true);
        writer.join();
        for (Thread reader : readers) {
            reader.join();
        }

        assertNull(failure.get(), failure.get());
        assertTrue("readers should have run", hits.get() > 1000);
    }

    @Test
    public void concurrentRegistrationsAgreeOnOneIdPerObject() throws Exception {
        final ObjectTable table = new ObjectTable();
        final java.util.concurrent.atomic.AtomicInteger nextId = new java.util.concurrent.atomic.AtomicInteger();
        final ObjectTable.IdSource ids = new ObjectTable.IdSource() {
            @Override
            public int newId() {
                return nextId.getAndIncrement();
            }
        };
        final int objectCount = 5000;
        final Object[] objects = new Object[objectCount];
        for (int i = 0; i < objectCount; i++) {
            objects[i] = new Object();
        }
        final int threadCount = 6;
        final int[][] seen = new int[threadCount][objectCount];
        final int[] freshCounts = new int[threadCount];
        final java.util.concurrent.CountDownLatch go = new java.util.concurrent.CountDownLatch(1);
        Thread[] threads = new Thread[threadCount];
        for (int t = 0; t < threadCount; t++) {
            final int index = t;
            threads[t] = new Thread(new Runnable() {
                @Override
                public void run() {
                    Random random = new Random(index);
                    try {
                        go.await();
                    } catch (InterruptedException e) {
                        return;
                    }
                    for (int n = 0; n < objectCount; n++) {
                        int i = (n * 7 + index * 131 + random.nextInt(3)) % objectCount;
                        long found = table.findOrRegister(objects[i], ids);
                        if ((found & ObjectTable.FRESH) != 0) {
                            freshCounts[index]++;
                        }
                        seen[index][i] = (int) found + 1;
                    }
                }
            });
            threads[t].start();
        }
        go.countDown();
        for (Thread thread : threads) {
            thread.join();
        }

        int fresh = 0;
        for (int count : freshCounts) {
            fresh += count;
        }
        assertEquals(table.size(), fresh);
        for (int i = 0; i < objectCount; i++) {
            int id = table.findId(objects[i]);
            for (int t = 0; t < threadCount; t++) {
                if (seen[t][i] != 0) {
                    assertEquals(id, seen[t][i] - 1);
                }
            }
        }
    }

    @Test
    public void exactReadsStayExactWhileAnotherThreadRegisters() throws Exception {
        final ObjectTable table = new ObjectTable();
        final java.util.concurrent.atomic.AtomicInteger nextId = new java.util.concurrent.atomic.AtomicInteger(1000);
        final ObjectTable.IdSource ids = new ObjectTable.IdSource() {
            @Override
            public int newId() {
                return nextId.getAndIncrement();
            }
        };
        final int stableCount = 64;
        final Object[] stable = new Object[stableCount];
        for (int i = 0; i < stableCount; i++) {
            stable[i] = new Object();
            table.putStrong(i, stable[i]);
        }
        final java.util.concurrent.atomic.AtomicBoolean done = new java.util.concurrent.atomic.AtomicBoolean();
        final java.util.concurrent.atomic.AtomicReference<String> failure = new java.util.concurrent.atomic.AtomicReference<>();

        Thread writer = new Thread(new Runnable() {
            @Override
            public void run() {
                List<Integer> churn = new ArrayList<>();
                Random random = new Random(3);
                while (!done.get()) {
                    // Grow well past the initial capacity, then shrink, so reads overlap resizes.
                    if (churn.size() < 20_000 && random.nextInt(3) != 0) {
                        churn.add((int) table.findOrRegister(new Object(), ids));
                    } else if (!churn.isEmpty()) {
                        table.remove(churn.remove(churn.size() - 1));
                    }
                }
            }
        });
        Thread reader = new Thread(new Runnable() {
            @Override
            public void run() {
                Random random = new Random(5);
                while (!done.get()) {
                    int i = random.nextInt(stableCount);
                    int id = table.findId(stable[i]);
                    if (id != i) {
                        failure.compareAndSet(null, "findId: expected " + i + ", got " + id);
                    }
                    if (table.get(i) != stable[i]) {
                        failure.compareAndSet(null, "get(" + i + ") returned another object");
                    }
                    if (!table.contains(i)) {
                        failure.compareAndSet(null, "contains(" + i + ") was false");
                    }
                }
            }
        });
        writer.start();
        reader.start();
        Thread.sleep(2000);
        done.set(true);
        writer.join();
        reader.join();

        assertNull(failure.get(), failure.get());
    }

    @Test
    public void keepsSurvivorsOfReleasedRangesAndOldIdsRegisteredLate() {
        ObjectTable table = new ObjectTable();
        Object[] objects = new Object[3000];
        for (int id = 0; id < objects.length; id++) {
            objects[id] = new Object();
            table.putStrong(id, objects[id]);
        }
        for (int id = 0; id < 2048; id++) {
            if (id != 5 && id != 1500) {
                table.remove(id);
            }
        }

        assertTrue(table.weakenAndCheckAlive(5));
        assertSame(objects[5], table.get(5));
        assertEquals(5, table.findId(objects[5]));
        assertEquals(1500, table.findId(objects[1500]));

        Object late = new Object();
        table.putStrong(700, late);
        assertSame(late, table.get(700));
        assertEquals(700, table.findId(late));

        table.remove(5);
        assertNull(table.get(5));
        assertFalse(table.contains(5));
        assertEquals(ObjectTable.NO_ID, table.findId(objects[5]));
        assertEquals(954, table.size());
    }

    @Test
    public void idsPastIntMaxAreUnsigned() {
        ObjectTable table = new ObjectTable();
        Object a = new Object();
        Object b = new Object();
        table.putStrong(Integer.MIN_VALUE + 3, a);

        assertSame(a, table.get(Integer.MIN_VALUE + 3));
        assertEquals(Integer.MIN_VALUE + 3, table.findId(a));
        assertTrue(table.weakenAndCheckAlive(Integer.MIN_VALUE + 3));

        long found = table.findOrRegister(b, new ObjectTable.IdSource() {
            @Override
            public int newId() {
                return -7;
            }
        });
        assertTrue((found & ObjectTable.FRESH) != 0);
        assertEquals(-7, (int) found);
        assertEquals(-7, table.findId(b));
        assertEquals(-7L & 0xFFFFFFFFL, table.findOrRegister(b, null));

        table.remove(-7);
        assertNull(table.get(-7));
        assertEquals(ObjectTable.NO_ID, table.findId(b));
        assertEquals(1, table.size());
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
