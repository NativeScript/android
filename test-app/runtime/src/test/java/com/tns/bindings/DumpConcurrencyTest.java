package com.tns.bindings;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;

import org.junit.Test;
import org.ow2.asmdex.ApplicationWriter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.concurrent.Callable;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;

/**
 * Every runtime (main and each worker) owns a Dump and can generate on its own
 * thread, so concurrent generations must produce exactly the bytes a lone one does.
 */
public class DumpConcurrencyTest {

    private static final Class<?>[] SUBJECTS = {
            Runnable.class,
            Callable.class,
            java.util.Comparator.class,
            java.util.Iterator.class,
            java.io.Closeable.class,
            CharSequence.class,
            java.util.function.BiFunction.class,
            Object.class,
            java.util.AbstractList.class,
            java.io.InputStream.class,
            java.util.TimerTask.class,
            java.io.Writer.class,
            java.util.AbstractMap.class,
    };

    private static final int THREADS = 8;
    private static final int ROUNDS = 150;

    private static byte[] generate(Dump dump, Class<?> subject) {
        ApplicationWriter aw = new ApplicationWriter();
        aw.visit();
        ClassDescriptor descriptor = new ClassInfo(subject);
        dump.generateProxy(aw, "DumpConcurrencyTest", descriptor, null, new HashSet<ClassDescriptor>(), null);
        aw.visitEnd();
        return aw.toByteArray();
    }

    private static byte[][] generateAll(Dump dump) {
        byte[][] result = new byte[SUBJECTS.length][];
        for (int i = 0; i < SUBJECTS.length; i++) {
            result[i] = generate(dump, SUBJECTS[i]);
        }
        return result;
    }

    @Test
    public void sequentialGenerationIsByteStable() {
        byte[][] first = generateAll(new Dump());
        byte[][] second = generateAll(new Dump());
        for (int i = 0; i < SUBJECTS.length; i++) {
            assertArrayEquals(SUBJECTS[i].getName(), first[i], second[i]);
        }
    }

    @Test
    public void concurrentGenerationMatchesSequentialGeneration() throws Exception {
        final byte[][] expected = generateAll(new Dump());

        ExecutorService pool = Executors.newFixedThreadPool(THREADS);
        try {
            final CountDownLatch start = new CountDownLatch(1);
            List<Future<List<String>>> results = new ArrayList<Future<List<String>>>();
            for (int t = 0; t < THREADS; t++) {
                final long seed = t;
                results.add(pool.submit(new Callable<List<String>>() {
                    @Override
                    public List<String> call() throws Exception {
                        Dump dump = new Dump();
                        Random random = new Random(seed);
                        List<Integer> order = new ArrayList<Integer>();
                        for (int i = 0; i < SUBJECTS.length; i++) {
                            order.add(i);
                        }
                        List<String> mismatches = new ArrayList<String>();
                        start.await();
                        for (int round = 0; round < ROUNDS; round++) {
                            Collections.shuffle(order, random);
                            for (int i : order) {
                                if (!Arrays.equals(expected[i], generate(dump, SUBJECTS[i]))) {
                                    mismatches.add(SUBJECTS[i].getName());
                                }
                            }
                        }
                        return mismatches;
                    }
                }));
            }
            start.countDown();

            List<String> mismatches = new ArrayList<String>();
            for (Future<List<String>> result : results) {
                mismatches.addAll(result.get(5, TimeUnit.MINUTES));
            }
            assertEquals("proxies generated concurrently differ from sequential ones: " + mismatches, 0, mismatches.size());
        } finally {
            pool.shutdownNow();
        }
    }
}
