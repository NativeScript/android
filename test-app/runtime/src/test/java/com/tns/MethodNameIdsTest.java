package com.tns;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;

public class MethodNameIdsTest {
    @Test
    public void givesEqualNamesOneIdAndDistinctNamesDistinctIds() {
        int a = MethodNameIds.idOf("testName_a");
        int b = MethodNameIds.idOf("testName_b");

        assertNotEquals(a, b);
        assertEquals(a, MethodNameIds.idOf("testName_a"));
        assertEquals(a, MethodNameIds.idOf(new String("testName_a")));
        assertEquals(-1, MethodNameIds.idOf(null));
    }

    @Test
    public void keepsIdsAcrossGrowth() {
        List<Integer> ids = new ArrayList<>();
        for (int i = 0; i < 2000; i++) {
            ids.add(MethodNameIds.idOf("testGrowth_" + i));
        }
        Set<Integer> distinct = new HashSet<>(ids);
        assertEquals(2000, distinct.size());
        for (int i = 0; i < 2000; i++) {
            assertEquals((int) ids.get(i), MethodNameIds.idOf("testGrowth_" + i));
        }
    }

    @Test
    public void agreesAcrossThreadsRegisteringTheSameNames() throws Exception {
        final int threads = 8;
        final int names = 500;
        final ConcurrentHashMap<String, Integer> seen = new ConcurrentHashMap<>();
        final List<Throwable> failures = new ArrayList<>();
        final CountDownLatch start = new CountDownLatch(1);
        List<Thread> workers = new ArrayList<>();
        for (int t = 0; t < threads; t++) {
            Thread worker = new Thread(() -> {
                try {
                    start.await();
                    for (int i = 0; i < names; i++) {
                        String name = "testRace_" + i;
                        int id = MethodNameIds.idOf(name);
                        Integer previous = seen.putIfAbsent(name, id);
                        if (previous != null && previous != id) {
                            throw new AssertionError(name + " got " + id + " and " + previous);
                        }
                    }
                } catch (Throwable e) {
                    synchronized (failures) {
                        failures.add(e);
                    }
                }
            });
            workers.add(worker);
            worker.start();
        }
        start.countDown();
        for (Thread worker : workers) {
            worker.join();
        }
        assertTrue(failures.toString(), failures.isEmpty());
        assertEquals(names, new HashSet<>(seen.values()).size());
    }
}
