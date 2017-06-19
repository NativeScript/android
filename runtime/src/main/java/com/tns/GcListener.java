package com.tns;

import java.lang.ref.PhantomReference;
import java.lang.ref.ReferenceQueue;
import java.util.WeakHashMap;

public class GcListener {
    private final ReferenceQueue<Object> q;
    private final Thread gcThread;

    private final Thread monitorThread;

    private final int throttleTime;
    private final int monitorInterval;
    private final double freeMemoryRatio;
    private final WeakHashMap<Runtime, Object> subscribers;
    private boolean firstStart = true;

    private static volatile GcListener instance;

    private class GcMonitor implements Runnable {
        private long lastUpdateTime = 0;
        private final long throttleTime;

        public GcMonitor(int throttleTime) {
            this.throttleTime = throttleTime * 1000000;
        }

        public void run() {
            while (true) {
                try {
                    PhantomReference<Object> ref = createRef();
                    PhantomReference<Objects> o = (PhantomReference<Objects>)GcListener.this.q.remove();
                    long currentUpdateTime = System.nanoTime();
                    if ((currentUpdateTime - lastUpdateTime) > throttleTime) {
                        GcListener.this.notifyGc();
                        lastUpdateTime = currentUpdateTime;
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        private PhantomReference<Object> createRef() {
            PhantomReference<Object> ref = new PhantomReference<Object>(new Object(), GcListener.this.q);
            return ref;
        }
    }

    private class MemoryMonitor implements Runnable {
        private final int timeInterval;
        private final double freeMemoryRatio;
        private final java.lang.Runtime runtime;

        public MemoryMonitor(int timeInterval, double freeMemoryRatio) {
            this.timeInterval = timeInterval;
            this.freeMemoryRatio = freeMemoryRatio;
            this.runtime = java.lang.Runtime.getRuntime();
        }

        public void run() {
            while (true) {
                try {
                    long freeMemory = MemoryMonitor.this.runtime.freeMemory();
                    long totalMemory = MemoryMonitor.this.runtime.totalMemory();
                    long maxMemory = MemoryMonitor.this.runtime.maxMemory();
                    double ratio = ((double)(maxMemory - totalMemory + freeMemory)) / ((double)maxMemory);
                    if (ratio < freeMemoryRatio) {
                        GcListener.this.notifyGc();
                    }
                    Thread.sleep(timeInterval);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private GcListener(int throttleTime, int monitorInterval, double freeMemoryRatio) {
        this.throttleTime = throttleTime;
        this.monitorInterval = monitorInterval;
        this.freeMemoryRatio = freeMemoryRatio;
        this.subscribers = new WeakHashMap<Runtime, Object>();

        if (throttleTime > 0) {
            q = new ReferenceQueue<Object>();
            gcThread = new Thread(new GcMonitor(throttleTime));
            gcThread.setName("NativeScript GC thread");
            gcThread.setDaemon(true);
        } else {
            q = null;
            gcThread = null;
        }
        if (monitorInterval > 0) {
            monitorThread = new Thread(new MemoryMonitor(monitorInterval, freeMemoryRatio));
            monitorThread.setName("NativeScript monitor thread");
            monitorThread.setDaemon(true);
        } else {
            monitorThread = null;
        }
    }

    /**
     * @param throttleTime throttle time in milliseconds
     * @param monitorInterval time in milliseconds
     */
    public static GcListener getInstance(int throttleTime, int monitorInterval, double freeMemoryRatio) {
        if (instance == null) {
            synchronized (GcListener.class) {
                if (instance == null) {
                    instance = new GcListener(throttleTime, monitorInterval, freeMemoryRatio);
                }
            }
        }
        return instance;
    }

    public static void subscribe(Runtime runtime) {
        synchronized (instance) {
            if (instance.firstStart) {
                instance.start();
                instance.firstStart = false;
            }
            instance.subscribers.put(runtime, null);
        }
    }

    public static void unsubscribe(Runtime runtime) {
        synchronized (instance) {
            instance.subscribers.remove(runtime);
        }
    }

    private void start() {
        if (gcThread != null) {
            gcThread.start();
        }
        if (monitorThread != null) {
            monitorThread.start();
        }
    }

    private void notifyGc() {
        synchronized (instance) {
            ManualInstrumentation.Frame frame = ManualInstrumentation.start("GcListener.notifyGc");
            try {
                for (Runtime runtime : subscribers.keySet()) {
                    runtime.notifyGc();
                }
            } finally {
                frame.close();
            }
        }
    }
}
