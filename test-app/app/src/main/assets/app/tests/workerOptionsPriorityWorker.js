// Entry for testWorkerOptions: reports the nice value the runtime gave this
// worker's thread, which is the only observable effect of the
// `android.priority` option.
postMessage({ priority: android.os.Process.getThreadPriority(android.os.Process.myTid()) });
