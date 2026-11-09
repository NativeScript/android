package com.tns;

import android.os.Handler;
import android.os.Looper;

import com.tns.bindings.ProxyGenerator;
import com.tns.system.classes.caching.impl.ClassCacheImpl;
import com.tns.system.classes.loading.ClassStorageService;
import com.tns.system.classes.loading.impl.ClassStorageServiceImpl;
import com.tns.system.classloaders.impl.ClassLoadersCollectionImpl;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;
import java.util.concurrent.RunnableFuture;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.Collections;
import dalvik.annotation.optimization.CriticalNative;
import dalvik.annotation.optimization.FastNative;

public class Runtime {
    private native void initNativeScript(int runtimeId, String filesPath, String nativeLibDir, boolean verboseLoggingEnabled, boolean isDebuggable, String packageName,
                                         String v8Flags, boolean codeCache, String profilerOutputDir, boolean discardUncaughtJsExceptions, String uncaughtErrorPolicy,
                                         String callingDir, int maxLogcatObjectSize, boolean forceLog);

    private native void runModule(int runtimeId, String filePath) throws NativeScriptException;

    private native Object runScript(int runtimeId, String filePath) throws NativeScriptException;

    private native Object callJSMethodNative(int runtimeId, Object javaObject, int javaObjectID, String methodName, int retType, boolean isConstructor, int[] argTypes, long[] argValues, Object[] argRefs, long[] primitiveResult) throws NativeScriptException;

    private native void createJSInstanceNative(int runtimeId, Object javaObject, int javaObjectID, String canonicalName);

    // Dynamic JNI lookup for @CriticalNative / @FastNative is unimplemented on Android 8-10
    // and buggy on Android 11; it works reliably only on Android 12+ (API 31).
    // For 8-11 the annotated methods must be bound via RegisterNatives in JNI_OnLoad.
    // The 26-30 path below uses the registered annotated variants; everything older
    // falls back to standard JNI auto-bind.
    private static final boolean SUPPORTS_OPTIMIZED_NATIVE = android.os.Build.VERSION.SDK_INT >= 26;

    @CriticalNative
    private static native int generateNewObjectIdCritical(int runtimeId);
    private static native int generateNewObjectIdLegacy(int runtimeId);

    private static int generateNewObjectId(int runtimeId) {
        return SUPPORTS_OPTIMIZED_NATIVE
                ? generateNewObjectIdCritical(runtimeId)
                : generateNewObjectIdLegacy(runtimeId);
    }

    @FastNative
    private native boolean notifyGcFast(int runtimeId);
    private native boolean notifyGcLegacy(int runtimeId);

    private boolean notifyGc(int runtimeId) {
        return SUPPORTS_OPTIMIZED_NATIVE ? notifyGcFast(runtimeId) : notifyGcLegacy(runtimeId);
    }

    private native void lock(int runtimeId);

    private native void unlock(int runtimeId);

    private native boolean passExceptionToJsNative(int runtimeId, Throwable ex, String message, String fullStackTrace, String jsStackTrace, boolean isDiscarded);

    @CriticalNative
    private static native int getCurrentRuntimeIdCritical();
    private static native int getCurrentRuntimeIdLegacy();

    public static int getCurrentRuntimeId() {
        return SUPPORTS_OPTIMIZED_NATIVE ? getCurrentRuntimeIdCritical() : getCurrentRuntimeIdLegacy();
    }

    @CriticalNative
    private static native int getPointerSizeCritical();
    private static native int getPointerSizeLegacy();

    public static int getPointerSize() {
        return SUPPORTS_OPTIMIZED_NATIVE ? getPointerSizeCritical() : getPointerSizeLegacy();
    }

    @FastNative
    private static native void setManualInstrumentationModeFast(String mode);
    private static native void setManualInstrumentationModeLegacy(String mode);

    public static void SetManualInstrumentationMode(String mode) {
        if (SUPPORTS_OPTIMIZED_NATIVE) {
            setManualInstrumentationModeFast(mode);
        } else {
            setManualInstrumentationModeLegacy(mode);
        }
    }

    private static native void ResetDateTimeConfigurationCache(int runtimeId);

    /**
     * Reports an uncaught exception to JS (WHATWG `error` event, then the
     * `__onUncaughtError` hook). Returns true when an `error` event listener
     * called preventDefault(), i.e. the exception is fully handled and the
     * caller should not crash the process.
     */
    boolean passUncaughtExceptionToJs(Throwable ex, String message, String fullStackTrace, String jsStackTrace) {
        return passExceptionToJsNative(getRuntimeId(), ex, message, fullStackTrace, jsStackTrace, false);
    }

    void passDiscardedExceptionToJs(Throwable ex, String prefix) {
        //String message = prefix + ex.getMessage();
        // we'd better not prefix the error with something like - Error on "main" thread for reportSupressedException
        // as it doesn't seem very useful for the users
        passExceptionToJsNative(getRuntimeId(), ex, ex.getMessage(), Runtime.getStackTraceErrorMessage(ex), Runtime.getJSStackTrace(ex), true);
    }

    /**
     * For callers with no binding instance at hand; the calling thread's runtime
     * is only a guess at where the exception belongs.
     */
    public static void passSuppressedExceptionToJs(Throwable ex, String methodName) {
        passSuppressedExceptionToJs(null, ex, methodName);
    }

    public static void passSuppressedExceptionToJs(Object instance, Throwable ex, String methodName) {
        int owningRuntimeId = getOwningRuntimeId(instance);

        // A known owner that has terminated gets nothing: any other runtime
        // receiving the report would surface an error that is not its own.
        com.tns.Runtime runtime = (owningRuntimeId != NO_OWNING_RUNTIME)
                ? runtimeCache.get(owningRuntimeId)
                : com.tns.Runtime.getCurrentRuntime();

        if (runtime != null) {
            runtime.postDiscardedExceptionToJs(ex);
        } else {
            android.util.Log.w("Warning", "NativeScript discarding suppressed exception from \"" + methodName
                    + "\": no runtime is left to report it to", ex);
        }
    }

    /*
     * Reports on the owning runtime's thread without waiting: the caller has
     * already swallowed the exception and has no result to collect. A runtime
     * whose looper is gone simply drops the report - throwing out of an
     * error-reporting path would replace a suppressed exception with a live one.
     */
    private void postDiscardedExceptionToJs(final Throwable ex) {
        if (config.appConfig.getEnableMultithreadedJavascript() || threadScheduler.getThread().equals(Thread.currentThread())) {
            passDiscardedExceptionToJs(ex, "");
            return;
        }

        threadScheduler.post(new Runnable() {
            @Override
            public void run() {
                passDiscardedExceptionToJs(ex, "");
            }
        });
    }

    private boolean initialized;

    private final static String FAILED_CTOR_RESOLUTION_MSG = "Check the number and type of arguments.\n" +
            "Primitive types need to be manually wrapped in their respective Object wrappers.\n" +
            "If you are creating an instance of an inner class, make sure to always provide reference to the outer `this` as the first argument.";

    // Mutated and read exactly only by the thread holding this runtime's isolate lock (the
    // native side resolves and registers objects after taking it); other threads may only
    // findIdOptimistic, whose result the native side re-checks.
    private final ObjectTable objectTable;

    private Map<Class<?>, JavaScriptImplementation> loadedJavaScriptExtends = new HashMap<Class<?>, JavaScriptImplementation>();

    private final java.lang.Runtime dalvikRuntime = java.lang.Runtime.getRuntime();

    private final Object keyNotFoundObject = new Object();
    private int currentObjectId = -1;

    private ExtractPolicy extractPolicy;

    private ArrayList<Constructor<?>> ctorCache = new ArrayList<Constructor<?>>();

    private Logger logger;

    private boolean isLiveSyncStarted;

    public boolean getIsLiveSyncStarted() {
        return this.isLiveSyncStarted;
    }

    public void setIsLiveSyncStarted(boolean value) {
        this.isLiveSyncStarted = value;
    }

    public Logger getLogger() {
        return this.logger;
    }

    private ThreadScheduler threadScheduler;

    private DexFactory dexFactory;

    private final ClassResolver classResolver;

    private final GcListener gcListener;

    private final static Comparator<Method> methodComparator = new Comparator<Method>() {
        public int compare(Method lhs, Method rhs) {
            return lhs.getName().compareTo(rhs.getName());
        }
    };

    private final StaticConfiguration config;
    private static StaticConfiguration staticConfiguration;

    private final DynamicConfiguration dynamicConfig;

    private final int runtimeId;

    /*
        The worker's id (0 for the main thread's runtime)
     */
    private final int workerId;

    private static AtomicInteger nextRuntimeId = new AtomicInteger(0);
    private final static ThreadLocal<Runtime> currentRuntime = new ThreadLocal<Runtime>();
    /*
     * The main (workerId 0) runtime. Fallback reporting target for uncaught
     * exceptions on threads that have no runtime of their own (see
     * NativeScriptUncaughtExceptionHandler): entering the main isolate from an
     * arbitrary crashing thread is safe - the JNI layer takes the v8::Locker.
     */
    private static volatile Runtime mainRuntime;
    private final static Map<Integer, Runtime> runtimeCache = new ConcurrentHashMap<>();
    public static boolean nativeLibraryLoaded;

    private static final ClassStorageService classStorageService = new ClassStorageServiceImpl(ClassCacheImpl.INSTANCE, ClassLoadersCollectionImpl.INSTANCE);

    // A dense index per runtime class, shared by every runtime, so native code can cache a
    // class's name and global ref by index instead of asking Java for them per object. 0 is
    // never assigned; native reads it as "no index".
    private static final ConcurrentHashMap<Class<?>, Integer> classIndices = new ConcurrentHashMap<>();
    private static int nextClassIndex = 1;

    static int classIndexOf(Class<?> clazz) {
        Integer index = classIndices.get(clazz);
        return index != null ? index : registerClass(clazz);
    }

    private static int registerClass(Class<?> clazz) {
        synchronized (classIndices) {
            Integer index = classIndices.get(clazz);
            if (index == null) {
                classStorageService.storeClass(clazz.getName(), clazz);
                index = nextClassIndex++;
                classIndices.put(clazz, index);
            }
            return index;
        }
    }

    // An object id in the low half, the object's class index in the high half.
    static long packIdAndClass(int objectId, Object obj) {
        return ((long) classIndexOf(obj.getClass()) << 32) | (objectId & 0xFFFFFFFFL);
    }

    public Runtime(ClassResolver classResolver, GcListener gcListener, StaticConfiguration config, DynamicConfiguration dynamicConfig, int runtimeId, int workerId, ObjectTable objectTable) {
        this.classResolver = classResolver;
        this.gcListener = gcListener;
        this.config = config;
        this.dynamicConfig = dynamicConfig;
        this.runtimeId = runtimeId;
        this.workerId = workerId;
        this.objectTable = objectTable;
    }

    public Runtime(StaticConfiguration config, DynamicConfiguration dynamicConfiguration) {
        this.objectTable = new ObjectTable(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.N);
        synchronized (Runtime.currentRuntime) {
            ManualInstrumentation.Frame frame = ManualInstrumentation.start("new Runtime");
            try {
                Runtime existingRuntime = currentRuntime.get();
                if (existingRuntime != null) {
                    throw new NativeScriptException("There is an existing runtime on this thread with id=" + existingRuntime.getRuntimeId());
                }

                this.runtimeId = nextRuntimeId.getAndIncrement();
                this.config = config;
                this.dynamicConfig = dynamicConfiguration;
                this.threadScheduler = dynamicConfiguration.myThreadScheduler;
                this.workerId = dynamicConfiguration.workerId;
                // if multithreadedJS, make all maps concurrent or synchronized:
                if (config.appConfig.getEnableMultithreadedJavascript()) {
                    // TODO: can't use a ConcurrentHashMap for loadedJavaScriptExtends because it loads null objects, which aren't supported
                    // either leave it like this or create a separate set for null caches
                    this.loadedJavaScriptExtends = Collections.synchronizedMap(new HashMap<>());
                }

                classResolver = new ClassResolver(classStorageService);
                gcListener = GcListener.getInstance(config.appConfig.getGcThrottleTime(), config.appConfig.getMemoryCheckInterval(), config.appConfig.getFreeMemoryRatio());
                // capture static configuration to allow native lookups when currentRuntime is unavailable
                staticConfiguration = config;

                // publish the instance only after everything that can throw has
                // completed, so a failed construction is never reachable through
                // the thread-local or the cache
                currentRuntime.set(this);
                runtimeCache.put(this.runtimeId, this);
            } finally {
                frame.close();
            }
        }
    }

    public int getRuntimeId() {
        return this.runtimeId;
    }

    public static Runtime getCurrentRuntime() {
        Runtime runtime = currentRuntime.get();

        if (runtime == null && nativeLibraryLoaded) {
            // Attempt to retrieve the runtime id from the currently
            // entered V8 isolate
            int runtimeId = getCurrentRuntimeId();
            runtime = runtimeCache.get(runtimeId);
        }

        return runtime;
    }

    public static boolean isDebuggable() {
        Runtime runtime = com.tns.Runtime.getCurrentRuntime();
        if (runtime != null) {
            return runtime.config.isDebuggable;
        } else {
            return false;
        }
    }

    // Security config
    
    /**
     * checks security.allowRemoteModules from nativescript.config
     */
    public static boolean isRemoteModulesAllowed() {
        // Debug mode always allows remote modules for development convenience
        if (isDebuggable()) {
            return true;
        }
        
        // Production: check security config
        Runtime runtime = com.tns.Runtime.getCurrentRuntime();
        if (runtime != null && runtime.config != null && runtime.config.appConfig != null) {
            return runtime.config.appConfig.getAllowRemoteModules();
        }
        if (staticConfiguration != null && staticConfiguration.appConfig != null) {
            return staticConfiguration.appConfig.getAllowRemoteModules();
        }
        return false;
    }
    
    /**
     * checks against security.remoteModuleAllowlist.
     */
    public static boolean isRemoteUrlAllowed(String url) {
        // Debug mode always allows all URLs
        if (isDebuggable()) {
            return true;
        }
        
        // Production: first check if remote modules are allowed at all
        if (!isRemoteModulesAllowed()) {
            return false;
        }
        
        // Get the allowlist
        String[] allowlist = getRemoteModuleAllowlist();
        
        // If no allowlist is configured, allow all URLs (user explicitly enabled remote modules)
        if (allowlist == null || allowlist.length == 0) {
            return true;
        }
        
        // Check if URL matches any allowlist prefix at a URL-component boundary
        // (exact match, entry ends in '/', or next char is '/', '?', or '#').
        // This refuses lookalike-host and lookalike-port bypasses.
        for (String prefix : allowlist) {
            if (url != null && prefix != null && remoteUrlMatchesAllowlistEntry(url, prefix)) {
                return true;
            }
        }
        
        return false;
    }

    private static boolean remoteUrlMatchesAllowlistEntry(String url, String entry) {
        if (entry.isEmpty() || url.length() < entry.length()) {
            return false;
        }
        if (!url.startsWith(entry)) {
            return false;
        }
        if (url.length() == entry.length()) {
            return true;
        }
        if (entry.charAt(entry.length() - 1) == '/') {
            return true;
        }
        char next = url.charAt(entry.length());
        return next == '/' || next == '?' || next == '#';
    }
    
    /**
     * Test/JNI helper: boot-time security.allowRemoteModules (debug always true).
     */
    public static boolean getSecurityAllowRemoteModules() {
        return isRemoteModulesAllowed();
    }

    /**
     * Test/JNI helper: boot-time security.remoteModuleAllowlist.
     */
    public static String[] getSecurityRemoteModuleAllowlist() {
        return getRemoteModuleAllowlist();
    }
    
    /**
     * Returns the remote module allowlist as a String array for JNI.
     */
    public static String[] getRemoteModuleAllowlist() {
        Runtime runtime = com.tns.Runtime.getCurrentRuntime();
        if (runtime != null && runtime.config != null && runtime.config.appConfig != null) {
            return runtime.config.appConfig.getRemoteModuleAllowlistArray();
        }
        if (staticConfiguration != null && staticConfiguration.appConfig != null) {
            return staticConfiguration.appConfig.getRemoteModuleAllowlistArray();
        }
        return new String[0];
    }

    private static final int NO_OWNING_RUNTIME = -1;

    /*
     * Records which runtime a binding instance belongs to. Called once, from
     * initInstance, by the runtime that registers the instance; later
     * registrations in other runtimes (getOrCreateJavaObjectID when the instance
     * crosses into another isolate) must not overwrite it, because only the
     * creating runtime holds the JS implementation behind the generated overrides.
     *
     * The tag is the runtime id plus one, so that the field's zero default means
     * "no owner yet": runtime id 0 is the main runtime's.
     */
    private static void recordOwningRuntime(Object instance, int runtimeId) {
        if (instance instanceof NativeScriptRuntimeBound) {
            NativeScriptRuntimeBound bound = (NativeScriptRuntimeBound) instance;
            if (bound.getRuntimeTag__ns() == 0) {
                bound.setRuntimeTag__ns(runtimeId + 1);
            }
        }
    }

    /*
     * The id of the runtime that created the instance, or NO_OWNING_RUNTIME when
     * nothing has recorded one yet. A returned id may name a runtime that has
     * since been torn down; callers decide how to report that.
     */
    private static int getOwningRuntimeId(Object javaObject) {
        if (!(javaObject instanceof NativeScriptRuntimeBound)) {
            return NO_OWNING_RUNTIME;
        }

        int tag = ((NativeScriptRuntimeBound) javaObject).getRuntimeTag__ns();
        return (tag == 0) ? NO_OWNING_RUNTIME : tag - 1;
    }

    /*
        Removes the error message lines to leave just the stack trace
     */
    private static String getStackTraceOnly(String content) {
        String[] lines = content.split("\n");
        while (lines.length > 0 && !lines[0].trim().startsWith("at")) {
            lines = Arrays.copyOfRange(lines, 1, lines.length);
        }
        String result = "";
        for (String line : lines) {
            result += line + "\n";
        }
        return result;
    }

    public static String getJSStackTrace(Throwable ex) {
        Throwable cause = ex;
        while(cause != null) {
            if(cause instanceof NativeScriptException) {
                return ((NativeScriptException) cause).getIncomingStackTrace();
            }
            cause = cause.getCause();
        }
        return null;
    }

    public static String getStackTraceErrorMessage(Throwable ex) {
        String content;
        java.io.PrintStream ps = null;

        try {
            java.io.ByteArrayOutputStream baos = new java.io.ByteArrayOutputStream();
            ps = new java.io.PrintStream(baos);
            ex.printStackTrace(ps);

            try {
                content = baos.toString("UTF-8");
                String jsStackTrace = Runtime.getJSStackTrace(ex);
                if (jsStackTrace != null) {
                    content = getStackTraceOnly(content);
                    content = jsStackTrace + content;
                }
            } catch (java.io.UnsupportedEncodingException e) {
                content = e.getMessage();
            }
        } finally {
            if (ps != null) {
                ps.close();
            }
        }

        return content;
    }

    public DynamicConfiguration getDynamicConfig() {
        return dynamicConfig;
    }

    @RuntimeCallable
    public static boolean getLineBreakpointsEnabled() {
        if (staticConfiguration != null && staticConfiguration.appConfig != null) {
            return staticConfiguration.appConfig.getLineBreakpointsEnabled();
        } else {
            return ((boolean) AppConfig.KnownKeys.EnableLineBreakpoins.getDefaultValue());
        }
    }

    public static boolean isInitialized() {
        Runtime runtime = Runtime.getCurrentRuntime();
        return (runtime != null) ? runtime.isInitializedImpl() : false;
    }

    public int getWorkerId() {
        return workerId;
    }

    public Handler getHandler() {
        return this.threadScheduler.getHandler();
    }

    public void ResetDateTimeConfigurationCache() {
        Runtime runtime = getCurrentRuntime();
        if (runtime != null) {
            ResetDateTimeConfigurationCache(runtime.getRuntimeId());
        }
    }

    // Only native code may call this, from the thread holding the isolate lock.
    @RuntimeCallable
    void releaseNativeCounterpart(int nativeObjectId) {
        objectTable.remove(nativeObjectId);
    }

    /*
        This method initializes the runtime and should always be called first and through the main thread
        in order to set static configuration that all following workers can use
     */
    public static Runtime initializeRuntimeWithConfiguration(StaticConfiguration config) {
        staticConfiguration = config;
        WorkThreadScheduler mainThreadScheduler = new WorkThreadScheduler(new Handler(Looper.myLooper()));
        DynamicConfiguration dynamicConfiguration = new DynamicConfiguration(0, mainThreadScheduler, null);
        Runtime runtime = initRuntime(dynamicConfiguration);
        return runtime;
    }

    /*
        Called via JNI from a native-spawned worker thread (WorkerWrapper), after the
        thread has attached to the JVM and before the worker script runs.
        Prepares the thread's Java Looper - this must happen before initRuntime so
        that the native runtime (timers, worker inbox, message loop) binds its fds
        to the looper that runWorkerLoop later pumps - and creates the worker's
        Runtime with a Looper-backed scheduler so cross-thread Java->JS calls
        (dispatchCallJSMethodNative, runScript) keep working.
        Should only be called after the static configuration has been initialized.
     */
    @RuntimeCallable
    public static int initWorkerRuntime(int workerId, String callingJsDir) {
        Looper.prepare();

        WorkThreadScheduler workThreadScheduler = new WorkThreadScheduler(new Handler(Looper.myLooper()));
        DynamicConfiguration dynamicConfiguration = new DynamicConfiguration(workerId, workThreadScheduler, null, callingJsDir);

        if (staticConfiguration.logger.isEnabled()) {
            staticConfiguration.logger.write("Worker (id=" + workerId + ")'s Runtime is initializing!");
        }

        Runtime runtime = initRuntime(dynamicConfiguration);

        if (staticConfiguration.logger.isEnabled()) {
            staticConfiguration.logger.write("Worker (id=" + workerId + ")'s Runtime initialized!");
        }

        return runtime.getRuntimeId();
    }

    /*
        Called via JNI from the native worker thread. Blocks pumping the worker's
        looper (Java Handler messages and the native fds registered on it) until
        the looper is quit by WorkerWrapper on terminate()/close().
     */
    @RuntimeCallable
    public static void runWorkerLoop() {
        Looper.loop();
    }

    /*
        Called via JNI from the native worker thread during shutdown, before the
        worker's isolate is disposed.
     */
    @RuntimeCallable
    public static void detachWorkerRuntime(int runtimeId) {
        Runtime runtime = runtimeCache.remove(runtimeId);
        if (runtime != null) {
            GcListener.unsubscribe(runtime);

            if (runtime.logger != null && runtime.logger.isEnabled()) {
                runtime.logger.write("Worker(id=" + runtime.workerId + ", name=\"" + Thread.currentThread().getName() + "\") has terminated execution. Don't make further function calls to it.");
            }
        }
        currentRuntime.remove();
    }

    /*
        This method deals with initializing the runtime with given configuration
        Does it for both workers and for the main thread
     */
    private static Runtime initRuntime(DynamicConfiguration dynamicConfiguration) {
        Runtime runtime = new Runtime(staticConfiguration, dynamicConfiguration);
        try {
            runtime.init();
            runtime.runScript(new File(staticConfiguration.appDir, "internal/ts_helpers.js"));
        } catch (Throwable t) {
            // the constructor already registered the instance - roll back so a
            // failed bootstrap doesn't leave a stale, half-initialized runtime
            // reachable through the caches
            runtimeCache.remove(runtime.getRuntimeId());
            currentRuntime.remove();
            GcListener.unsubscribe(runtime);
            throw t;
        }

        if (runtime.workerId == 0) {
            mainRuntime = runtime;
        }

        return runtime;
    }

    /**
     * The main runtime, or null before initialization. Used as the reporting
     * fallback for uncaught exceptions on threads with no runtime of their own.
     */
    public static Runtime getMainRuntime() {
        return mainRuntime;
    }

    boolean isInitializedImpl() {
        return initialized;
    }

    public void init() {
        init(config.logger, config.appName, config.nativeLibDir, config.rootDir, config.appDir, config.classLoader, config.dexDir, config.dexThumb, config.appConfig, dynamicConfig.callingJsDir, config.isDebuggable);
    }

    private void init(Logger logger, String appName, String nativeLibDir, File rootDir, File appDir, ClassLoader classLoader, File dexDir, String dexThumb, AppConfig appConfig, String callingJsDir, boolean isDebuggable) throws RuntimeException {
        if (initialized) {
            throw new RuntimeException("NativeScriptApplication already initialized");
        }

        ManualInstrumentation.Frame frame = ManualInstrumentation.start("Runtime.init");
        try {
            this.logger = logger;

            boolean isMainThread = this.workerId == 0;
            this.dexFactory = new DexFactory(logger, classLoader, dexDir, dexThumb, classStorageService, isMainThread);

            if (logger.isEnabled()) {
                logger.write("Initializing NativeScript JAVA");
            }

            try {
                Module.init(logger, rootDir, appDir);
            } catch (IOException ex) {
                throw new RuntimeException("Fail to initialize Require class", ex);
            }

            boolean forceConsoleLog = appConfig.getForceLog() || "timeline".equalsIgnoreCase(appConfig.getProfilingMode());

            initNativeScript(getRuntimeId(), Module.getApplicationFilesPath(), nativeLibDir, logger.isEnabled(), isDebuggable, appName,
                    appConfig.getV8Flags(), appConfig.getCodeCache(), appConfig.getProfilerOutputDir(),
                    appConfig.getDiscardUncaughtJsExceptions(), appConfig.getUncaughtErrorPolicy(),
                    callingJsDir, appConfig.getMaxLogcatObjectSize(), forceConsoleLog);

            if (logger.isEnabled()) {
                Date d = new Date();
                int pid = android.os.Process.myPid();
                File f = new File("/proc/" + pid);
                Date lastModDate = new Date(f.lastModified());
                logger.write("init time=" + (d.getTime() - lastModDate.getTime()));
            }

            GcListener.subscribe(this);

            initialized = true;
        } finally {
            frame.close();
        }
    }

    @RuntimeCallable
    public void enableVerboseLogging() {

        logger.setEnabled(true);
        ProxyGenerator.IsLogEnabled = true;
    }


    @RuntimeCallable
    public void disableVerboseLogging() {
        logger.setEnabled(false);
        ProxyGenerator.IsLogEnabled = false;
    }

    public void run() throws NativeScriptException {
        ManualInstrumentation.Frame frame = ManualInstrumentation.start("Runtime.run");
        try {
            String mainModule = Module.bootstrapApp();
            runModule(new File(mainModule));
        } finally {
            frame.close();
        }
    }

    public void runModule(File jsFile) throws NativeScriptException {
        runModule(getRuntimeId(), Module.resolveEntryPath(jsFile.getPath()));
    }

    public Object runScript(File jsFile) throws NativeScriptException {
        return this.runScript(jsFile, true);
    }

    public Object runScript(File jsFile, final boolean waitForResultOnMainThread) throws NativeScriptException {
        Object result = null;

        if (jsFile.exists() && jsFile.isFile()) {
            final String filePath = jsFile.getAbsolutePath();

            boolean isWorkThread = threadScheduler.getThread().equals(Thread.currentThread());

            if (isWorkThread) {
                result = runScript(getRuntimeId(), filePath);
            } else {
                final Object[] arr = new Object[2];

                Runnable r = new Runnable() {
                    @Override
                    public void run() {
                        synchronized (this) {
                            try {
                                arr[0] = runScript(getRuntimeId(), filePath);
                            } finally {
                                this.notify();
                                arr[1] = Boolean.TRUE;
                            }
                        }
                    }
                };

                boolean success = threadScheduler.post(r);

                if (success) {
                    synchronized (r) {
                        try {
                            if (arr[1] == null && waitForResultOnMainThread) {
                                r.wait();
                            }
                        } catch (InterruptedException e) {
                            result = e;
                        }
                    }
                }
            }
        }

        return result;
    }

    @RuntimeCallable
    private Class<?> resolveClass(String baseClassName, String fullClassName, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) throws ClassNotFoundException, IOException {
        Class<?> javaClass = classResolver.resolveClass(baseClassName, fullClassName, dexFactory, methodOverrides, implementedInterfaces, isInterface);

        return javaClass;
    }

    @RuntimeCallable
    private long getUsedMemory() {
        long usedMemory = dalvikRuntime.totalMemory() - dalvikRuntime.freeMemory();
        return usedMemory;
    }

    public void notifyGc() {
        notifyGc(runtimeId);
    }

    public void lock() {
        lock(runtimeId);
    }

    public void unlock() {
        unlock(runtimeId);
    }

    public static void initInstanceFromPossibleNonMainThread(final Object instance) {
        if (isNotOnMainThread()) {
            Runnable runnable = new Runnable() {
                @Override
                public void run() {
                    initInstance(instance);
                }
            };

            RunnableFuture<Void> task = new FutureTask<>(runnable, null);
            getMainThreadHandler().post(task);

            try {
                task.get(); // this will block until Runnable completes
            } catch (InterruptedException | ExecutionException e) {
                throw new RuntimeException(e);
            }

        } else {
            initInstance(instance);
        }
    }

    private static Handler getMainThreadHandler() {
        return new Handler(Looper.getMainLooper());
    }

    private static boolean isNotOnMainThread() {
        return Looper.myLooper() != Looper.getMainLooper();
    }

    public static void initInstance(Object instance) {
        ManualInstrumentation.Frame frame = ManualInstrumentation.start("Runtime.initInstance");
        try {
            Runtime runtime = Runtime.getCurrentRuntime();

            if (runtime == null) {
                throw new NativeScriptException("Cannot initialize an instance of " + instance.getClass().getName()
                        + ": no NativeScript runtime is bound to thread \"" + Thread.currentThread().getName()
                        + "\". Construct it on the runtime's own thread, or from JS.");
            }

            int objectId = runtime.currentObjectId;

            if (objectId != -1) {
                runtime.makeInstanceStrong(instance, objectId);
            } else {
                runtime.createJSInstance(instance);
            }

            recordOwningRuntime(instance, runtime.getRuntimeId());
        } finally {
            frame.close();
        }
    }

    private void createJSInstance(Object instance) {
        // Registered by createJSInstanceNative, which holds the isolate lock.
        int javaObjectID = generateNewObjectId(getRuntimeId());

        Class<?> clazz = instance.getClass();

        if (!loadedJavaScriptExtends.containsKey(clazz)) {
            JavaScriptImplementation jsImpl = clazz.getAnnotation(JavaScriptImplementation.class);
            if (jsImpl != null) {
                File jsFile = new File(jsImpl.javaScriptFile());
                runModule(jsFile);
            } else {
                logger.write("Couldn't find JavaScriptImplementation annotation for class " + clazz.toString());
            }
            loadedJavaScriptExtends.put(clazz, jsImpl);
        }

        String className = clazz.getName();

        createJSInstanceNative(getRuntimeId(), instance, javaObjectID, className);

        if (logger.isEnabled()) {
            logger.write("JSInstance for " + instance.getClass().toString() + " created with overrides");
        }
    }

    @RuntimeCallable
    private static String[] getTypeMetadata(String className, int index) throws ClassNotFoundException {
        Class<?> clazz = classStorageService.retrieveClass(className);

        String[] result = getTypeMetadata(clazz, index);

        return result;
    }

    private static String[] getTypeMetadata(Class<?> clazz, int index) {
        Class<?> mostOuterClass = clazz.getEnclosingClass();
        ArrayList<Class<?>> outerClasses = new ArrayList<Class<?>>();
        while (mostOuterClass != null) {
            outerClasses.add(0, mostOuterClass);
            Class<?> nextOuterClass = mostOuterClass.getEnclosingClass();
            if (nextOuterClass == null) {
                break;
            }
            mostOuterClass = nextOuterClass;
        }

        Package p = (mostOuterClass != null)
                ? mostOuterClass.getPackage()
                : clazz.getPackage();
        int packageCount = p != null ? 1 : 0;

        String pname = p != null ? p.getName() : "";

        for (int i = 0; i < pname.length(); i++) {
            if (pname.charAt(i) == '.') {
                ++packageCount;
            }
        }

        String name = clazz.getName();
        String[] parts = name.split("[\\.\\$]");

        int endIdx = parts.length;
        int len = endIdx - index;
        String[] result = new String[len];

        int endOuterTypeIdx = packageCount + outerClasses.size();

        for (int i = index; i < endIdx; i++) {
            if (i < packageCount) {
                result[i - index] = "P";
            } else {
                if (i < endOuterTypeIdx) {
                    result[i - index] = getTypeMetadata(outerClasses.get(i - packageCount));
                } else {
                    result[i - index] = getTypeMetadata(clazz);
                }
            }
        }

        return result;
    }

    private static String getTypeMetadata(Class<?> clazz) {
        StringBuilder sb = new StringBuilder();

        if (clazz.isInterface()) {
            sb.append("I ");
        } else {
            sb.append("C ");
        }

        if (Modifier.isStatic(clazz.getModifiers())) {
            sb.append("S\n");
        } else {
            sb.append("I\n");
        }

        Class<?> baseClass = clazz.getSuperclass();
        sb.append("B " + ((baseClass != null) ? baseClass.getName() : "").replace('.', '/') + "\n");

        Method[] methods = clazz.getDeclaredMethods();
        Arrays.sort(methods, methodComparator);

        for (Method m : methods) {
            int modifiers = m.getModifiers();
            if (!Modifier.isStatic(modifiers) && (Modifier.isPublic(modifiers) || Modifier.isProtected(modifiers))) {
                sb.append("M ");
                sb.append(m.getName());
                Class<?>[] params = m.getParameterTypes();
                String sig = MethodResolver.getMethodSignature(m.getReturnType(), params);
                sb.append(" ");
                sb.append(sig);
                int paramCount = params.length;
                sb.append(" ");
                sb.append(paramCount);
                sb.append("\n");
            }
        }

        Field[] fields = clazz.getDeclaredFields();
        for (Field f : fields) {
            int modifiers = f.getModifiers();
            if (!Modifier.isStatic(modifiers) && (Modifier.isPublic(modifiers) || Modifier.isProtected(modifiers))) {
                sb.append("F ");
                sb.append(f.getName());
                sb.append(" ");
                String sig = MethodResolver.getTypeSignature(f.getType());
                sb.append(sig);
                sb.append(" 0\n");
            }
        }

        String ret = sb.toString();

        return ret;
    }

    @RuntimeCallable
    private void makeInstanceStrong(Object instance, int objectId) {
        if (instance == null) {
            throw new IllegalArgumentException("instance cannot be null");
        }

        int key = objectId;
        objectTable.putStrong(key, instance);

        // Stores the class by name on its first registration.
        classIndexOf(instance.getClass());

        if (logger != null && logger.isEnabled()) {
            logger.write("MakeInstanceStrong (" + key + ", " + instance.getClass().toString() + ")");
        }
    }

    /*
     * One call per finalizer batch: the JS wrappers of these ids were collected. weaken holds
     * first collections (held strongly until now), check later ones. Each entry whose Java
     * object is already gone is overwritten with -1 so its wrapper can be released too.
     */
    @RuntimeCallable
    private void processFinalizedBatch(int[] weaken, int weakenCount, int[] check, int checkCount) {
        for (int i = 0; i < weakenCount; i++) {
            if (!objectTable.weakenAndCheckAlive(weaken[i])) {
                weaken[i] = -1;
            }
        }
        for (int i = 0; i < checkCount; i++) {
            if (!objectTable.weakenAndCheckAlive(check[i])) {
                check[i] = -1;
            }
        }
        if (logger.isEnabled()) {
            logger.write("processFinalizedBatch weakened=" + weakenCount + " checked=" + checkCount);
        }
    }

    @RuntimeCallable
    private Object getJavaObjectByID(int javaObjectID) throws Exception {
        if (logger.isEnabled()) {
            logger.write("Platform.getJavaObjectByID:" + javaObjectID);
        }

        Object instance = objectTable.get(javaObjectID);
        if (instance == null) {
            throw new NativeScriptException(objectTable.contains(javaObjectID)
                    ? "Attempt to use cleared object reference id=" + javaObjectID
                    : "No weak reference found. Attempt to use cleared object reference id=" + javaObjectID);
        }

        return instance;
    }

    private Integer getJavaObjectID(Object obj) {
        int id = objectTable.findId(obj);
        return id == ObjectTable.NO_ID ? null : id;
    }

    @RuntimeCallable
    private int findJavaObjectID(Object obj) {
        return objectTable.findId(obj);
    }

    // The class index is only needed to create a wrapper, and an object that already has
    // an id almost always has one; native asks for the class itself when it does not.
    @RuntimeCallable
    private long getOrCreateJavaObjectIDAndClass(Object obj) {
        int id = objectTable.findId(obj);
        if (id != ObjectTable.NO_ID) {
            return id & 0xFFFFFFFFL;
        }
        id = generateNewObjectId(getRuntimeId());
        makeInstanceStrong(obj, id);
        return packIdAndClass(id, obj);
    }

    @RuntimeCallable
    private int getOrCreateJavaObjectID(Object obj) {
        Integer result = getJavaObjectID(obj);

        if (result == null) {
            int objectId = generateNewObjectId(getRuntimeId());
            makeInstanceStrong(obj, objectId);

            result = objectId;
        }

        return result;
    }

    public static Object callJSMethodFromPossibleNonMainThread(Object javaObject, String methodName, Class<?> retType, Object... args) throws NativeScriptException {
        return callJSMethodFromPossibleNonMainThread(javaObject, methodName, retType, false /* isConstructor */, args);
    }

    public static Object callJSMethodFromPossibleNonMainThread(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, Object... args) throws NativeScriptException {
        return callJSMethodFromPossibleNonMainThread(javaObject, methodName, retType, isConstructor, 0, args);
    }

    public static Object callJSMethodFromPossibleNonMainThread(Object javaObject, String methodName, boolean isConstructor, Object... args) throws NativeScriptException {
        return callJSMethodFromPossibleNonMainThread(javaObject, methodName, void.class, isConstructor, 0, args);
    }

    public static Object callJSMethodFromPossibleNonMainThread(final Object javaObject, final String methodName, final Class<?> retType, final boolean isConstructor, final long delay, final Object... args) throws NativeScriptException {
        if (isNotOnMainThread()) {
            Callable<Object> callable = new Callable<Object>() {
                @Override
                public Object call() {
                    return callJSMethod(javaObject, methodName, retType, isConstructor, delay, args);
                }
            };

            RunnableFuture<Object> task = new FutureTask<>(callable);
            getMainThreadHandler().post(task);

            try {
                return task.get(); // this will block until Runnable completes
            } catch (InterruptedException | ExecutionException e) {
                throw new RuntimeException(e);
            }

        } else {
            return callJSMethod(javaObject, methodName, retType, isConstructor, delay, args);
        }
    }


    // sends args in pairs (typeID, value, null) except for objects where its
    // (typeid, javaObjectID, javaJNIClassPath)
    public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, Object... args) throws NativeScriptException {
        return callJSMethod(javaObject, methodName, retType, false /* isConstructor */, args);
    }

    public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, Object... args) throws NativeScriptException {
        return callJSMethod(javaObject, methodName, retType, isConstructor, 0, args);
    }

    public static Object callJSMethod(Object javaObject, String methodName, boolean isConstructor, Object... args) throws NativeScriptException {
        return callJSMethod(javaObject, methodName, void.class, isConstructor, 0, args);
    }

    public static Object callJSMethodWithDelay(Object javaObject, String methodName, Class<?> retType, long delay, Object... args) throws NativeScriptException {
        return callJSMethod(javaObject, methodName, retType, false /* isConstructor */, delay, args);
    }

    public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, long delay, Object... args) throws NativeScriptException {
        int owningRuntimeId = getOwningRuntimeId(javaObject);
        Runtime runtime;

        if (owningRuntimeId != NO_OWNING_RUNTIME) {
            runtime = runtimeCache.get(owningRuntimeId);

            if (runtime == null) {
                return discardCall(methodName, retType, describeInstance(javaObject)
                        + " outlived the runtime that created it (id=" + owningRuntimeId + ")");
            }
        } else {
            // No owner recorded yet: the instance is still being constructed and
            // registered (a generated method invoked from the superclass
            // constructor, or re-entered while initInstance runs), so only the
            // constructing thread's runtime can hold it.
            runtime = Runtime.getCurrentRuntime();

            if (runtime == null) {
                throw new NativeScriptException("Cannot find runtime for " + describeInstance(javaObject));
            }
        }

        return runtime.dispatchCallJSMethodNative(javaObject, methodName, isConstructor, delay, retType, args);
    }

    /*
     * Names the class rather than calling toString(): that may be one of the JS
     * overrides, and these messages are built exactly when calling into JS is
     * not possible.
     */
    private static String describeInstance(Object javaObject) {
        return (javaObject == null) ? "null" : "an instance of " + javaObject.getClass().getName();
    }

    /*
     * Arguments cross to native as three parallel arrays so the native side
     * reads every primitive with one bulk region copy instead of unboxing each
     * through JNI. argValues holds primitives as raw bits (float/double via
     * floatToRawIntBits/doubleToRawLongBits) and object IDs; argRefs holds
     * strings and, for objects, the class name a missing JS wrapper needs.
     * Type IDs are those of TypeIDs.GetObjectTypeId(Object).
     */
    private static final class PackagedArgs {
        final int[] types;
        final long[] values;
        final Object[] refs;

        PackagedArgs(int[] types, long[] values, Object[] refs) {
            this.types = types;
            this.values = values;
            this.refs = refs;
        }
    }

    private static final PackagedArgs NO_ARGS = new PackagedArgs(new int[0], new long[0], null);

    // If a javaObject has no javaObjectID, meaning no JavaScript object exists
    // for it yet, it is assigned one.
    private PackagedArgs packageArgs(Object... args) {
        int len = (args != null) ? args.length : 0;
        if (len == 0) {
            return NO_ARGS;
        }

        int[] types = new int[len];
        long[] values = new long[len];
        Object[] refs = null;

        for (int i = 0; i < len; i++) {
            Object value = args[i];
            int typeId;

            if (value == null) {
                typeId = TypeIDs.Null;
            } else if (value instanceof Integer) {
                typeId = TypeIDs.Int;
                values[i] = (Integer) value;
            } else if (value instanceof Boolean) {
                typeId = TypeIDs.Boolean;
                values[i] = ((Boolean) value) ? 1 : 0;
            } else if (value instanceof Character) {
                typeId = TypeIDs.Char;
                values[i] = (Character) value;
            } else if (value instanceof Byte) {
                typeId = TypeIDs.Byte;
                values[i] = (Byte) value;
            } else if (value instanceof Short) {
                typeId = TypeIDs.Short;
                values[i] = (Short) value;
            } else if (value instanceof Long) {
                typeId = TypeIDs.Long;
                values[i] = (Long) value;
            } else if (value instanceof Float) {
                typeId = TypeIDs.Float;
                values[i] = Float.floatToRawIntBits((Float) value);
            } else if (value instanceof Double) {
                typeId = TypeIDs.Double;
                values[i] = Double.doubleToRawLongBits((Double) value);
            } else if (value instanceof String) {
                typeId = TypeIDs.string;
                if (refs == null) {
                    refs = new Object[len];
                }
                refs[i] = value;
            } else {
                typeId = TypeIDs.JsObject;
                if (refs == null) {
                    refs = new Object[len];
                }
                // The id is a hint; resolved or registered natively, under the isolate lock,
                // on a miss.
                refs[i] = value;
                values[i] = packIdAndClass(objectTable.findIdOptimistic(value), value);
            }

            types[i] = typeId;
        }

        return new PackagedArgs(types, values, refs);
    }

    static Class<?> getClassForName(String className) {
        return classStorageService.retrieveClass(className);
    }

    @RuntimeCallable
    private String resolveConstructorSignature(Class<?> clazz, Object[] args) throws Exception {
        // Pete: cache stuff here, or in the cpp part

        if (logger.isEnabled()) {
            logger.write("resolveConstructorSignature: Resolving constructor for class " + clazz.getName());
        }

        String res = MethodResolver.resolveConstructorSignature(clazz, args);

        if (res == null) {
            throw new Exception("Failed resolving constructor for class \'" + clazz.getName() + "\' with " + (args != null ? args.length : 0) + " parameters. " + FAILED_CTOR_RESOLUTION_MSG);
        }

        return res;
    }

    @RuntimeCallable
    private String resolveMethodOverload(String className, String methodName, Object[] args) throws Exception {
        if (logger.isEnabled()) {
            logger.write("resolveMethodOverload: Resolving method " + methodName + " on class " + className);
        }

        Class<?> clazz = classStorageService.retrieveClass(className);


        String res = MethodResolver.resolveMethodOverload(clazz, methodName, args);
        if (logger.isEnabled()) {
            logger.write("resolveMethodOverload: method found :" + res);
        }
        if (res == null) {
            throw new Exception("Failed resolving method " + methodName + " on class " + className);
        }

        return res;
    }

    private Object[] extendConstructorArgs(String methodName, boolean isConstructor, Object[] args) {
        Object[] arr = null;

        if (methodName.equals("init")) {
            if (args == null) {
                arr = new Object[]
                        {isConstructor};
            } else {
                arr = new Object[args.length + 1];
                System.arraycopy(args, 0, arr, 0, args.length);
                arr[arr.length - 1] = isConstructor;
            }
        } else {
            arr = args;
        }

        return arr;
    }

    /*
     * While a cross-thread call waits, the owner may terminate. Looper.quit()
     * discards queued messages, so a call posted just before that never runs and
     * nothing would wake the caller; it re-checks the owner at this interval.
     */
    private static final long QUEUED_CALL_LIVENESS_CHECK_MS = 100;

    private boolean isAcceptingWork() {
        return runtimeCache.get(runtimeId) == this && threadScheduler.getThread().isAlive();
    }

    /*
     * Must run on this runtime's thread unless multithreaded JS is enabled: the
     * object maps are otherwise unsynchronized, and a weak-map lookup is not even
     * read-only (it expunges stale entries).
     */
    private int resolveJavaObjectID(Object javaObject, String methodName) throws NativeScriptException {
        Integer javaObjectID = getJavaObjectID(javaObject);
        if (javaObjectID == null) {
            throw new NativeScriptException("Cannot find object id for " + describeInstance(javaObject));
        }

        if (logger.isEnabled()) {
            logger.write("Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getName() : "null"));
        }

        return javaObjectID;
    }

    private Object dispatchCallJSMethodNative(final Object javaObject, final String methodName, boolean isConstructor, long delay, Class<?> retType, final Object[] args) throws NativeScriptException {
        final int returnType = TypeIDs.GetObjectTypeId(retType);
        Object ret = null;

        boolean isWorkThread = threadScheduler.getThread().equals(Thread.currentThread());

        final Object[] tmpArgs = extendConstructorArgs(methodName, isConstructor, args);
        final boolean discardUncaughtJsExceptions = this.config.appConfig.getDiscardUncaughtJsExceptions();
        boolean enableMultithreadedJavascript = this.config.appConfig.getEnableMultithreadedJavascript();

        if (enableMultithreadedJavascript || isWorkThread) {
            PackagedArgs packagedArgs = packageArgs(tmpArgs);
            long[] primitiveResult = newPrimitiveResult(returnType);
            try {
                // A hint: other threads may be writing the table, so native re-checks it under the
                // isolate lock and resolves the id itself on a miss.
                ret = callJSMethodNative(getRuntimeId(), javaObject, objectTable.findIdOptimistic(javaObject), methodName, returnType, isConstructor, packagedArgs.types, packagedArgs.values, packagedArgs.refs, primitiveResult);
                ret = takePrimitiveResult(ret, returnType, primitiveResult);
            } catch (NativeScriptException e) {
                // With multithreaded JS the owner can be torn down between the
                // routing check and the native call, which then throws
                // "Cannot find runtime"; that is the same lifecycle race the
                // other dead-owner paths drop.
                if (!isAcceptingWork()) {
                    return discardCall(methodName, retType, "runtime id=" + getRuntimeId() + " (workerId=" + workerId
                            + ") terminated during the call");
                }

                if (discardUncaughtJsExceptions && !e.isEscapedFromJs()) {
                    String errorMessage = "Error on \"" + Thread.currentThread().getName() + "\" thread for callJSMethodNative\n";
                    android.util.Log.w("Warning", "NativeScript discarding uncaught JS exception!");
                    passDiscardedExceptionToJs(e, errorMessage);
                } else {
                    throw e;
                }
            }
        } else {
            // [0] result, [1] completion flag, [2] a lookup failure to rethrow on
            // the calling thread rather than on the runtime's looper
            final Object[] arr = new Object[3];

            final boolean isCtor = isConstructor;

            Runnable r = new Runnable() {
                @Override
                public void run() {
                    synchronized (this) {
                        try {
                            final int javaObjectID;
                            try {
                                javaObjectID = resolveJavaObjectID(javaObject, methodName);
                            } catch (NativeScriptException e) {
                                arr[2] = e;
                                return;
                            }
                            final PackagedArgs packagedArgs = packageArgs(tmpArgs);
                            final long[] primitiveResult = newPrimitiveResult(returnType);
                            arr[0] = takePrimitiveResult(callJSMethodNative(getRuntimeId(), javaObject, javaObjectID, methodName, returnType, isCtor, packagedArgs.types, packagedArgs.values, packagedArgs.refs, primitiveResult), returnType, primitiveResult);
                        } catch (NativeScriptException e) {
                            if (discardUncaughtJsExceptions && !e.isEscapedFromJs()) {
                                String errorMessage = "Error on \"" + Thread.currentThread().getName() + "\" thread for callJSMethodNative\n";
                                passDiscardedExceptionToJs(e, errorMessage);
                                android.util.Log.w("Warning", "NativeScript discarding uncaught JS exception!");
                            } else {
                                throw e;
                            }
                        } finally {
                            this.notify();
                            arr[1] = Boolean.TRUE;
                        }
                    }
                }
            };

            if (delay > 0) {
                try {
                    Thread.sleep(delay);
                } catch (InterruptedException e) {
                }
            }

            boolean success = threadScheduler.post(r);

            if (success) {
                synchronized (r) {
                    try {
                        while (arr[1] == null) {
                            r.wait(QUEUED_CALL_LIVENESS_CHECK_MS);

                            if (arr[1] == null && !isAcceptingWork()) {
                                logDiscardedCall(methodName, "runtime id=" + getRuntimeId() + " (workerId=" + workerId
                                        + ") terminated before the call ran");
                                break;
                            }
                        }
                    } catch (InterruptedException e) {
                        ret = e;
                    }
                }
            } else {
                // arr[0] stays null and primitive returns are defaulted below
                logDiscardedCall(methodName, "runtime id=" + getRuntimeId() + " (workerId=" + workerId
                        + ") is no longer accepting work");
            }

            if (arr[2] != null) {
                throw (NativeScriptException) arr[2];
            }

            ret = arr[0];
        }

        // A contained uncaught JS error (or a discarded one) yields null; for
        // primitive return types the generated binding would NPE unboxing it,
        // so substitute the type's default value.
        if (ret == null && retType != null && retType.isPrimitive() && retType != void.class) {
            ret = defaultPrimitiveValue(retType);
        }

        return ret;
    }

    private static void logDiscardedCall(String methodName, String reason) {
        android.util.Log.w("Warning", "NativeScript discarding call to \"" + methodName + "\": " + reason);
    }

    /*
     * A call with nowhere left to run is dropped rather than thrown. These arrive
     * on Android callbacks - lifecycle, listeners, draw - where an exception would
     * take down the process because a worker ended, so the loss is contained to
     * the one call and reported through the log instead. Primitive returns still
     * need a value the generated cast can unbox.
     */
    private static Object discardCall(String methodName, Class<?> retType, String reason) {
        logDiscardedCall(methodName, reason);

        return (retType != null && retType.isPrimitive() && retType != void.class)
                ? defaultPrimitiveValue(retType)
                : null;
    }

    /*
     * For primitive return types the native side hands the converted value back
     * as raw bits: it writes {bits, 1} here and returns null, or leaves the
     * array untouched and returns a boxed value from the generic conversion.
     */
    private static long[] newPrimitiveResult(int returnType) {
        boolean primitive = returnType >= TypeIDs.Boolean && returnType <= TypeIDs.Double;
        return primitive ? new long[2] : null;
    }

    private static Object takePrimitiveResult(Object nativeResult, int returnType, long[] primitiveResult) {
        if (primitiveResult == null || primitiveResult[1] == 0) {
            return nativeResult;
        }

        long bits = primitiveResult[0];
        if (returnType == TypeIDs.Boolean) {
            return bits != 0;
        } else if (returnType == TypeIDs.Char) {
            return (char) bits;
        } else if (returnType == TypeIDs.Byte) {
            return (byte) bits;
        } else if (returnType == TypeIDs.Short) {
            return (short) bits;
        } else if (returnType == TypeIDs.Int) {
            return (int) bits;
        } else if (returnType == TypeIDs.Long) {
            return bits;
        } else if (returnType == TypeIDs.Float) {
            return Float.intBitsToFloat((int) bits);
        } else {
            return Double.longBitsToDouble(bits);
        }
    }

    private static Object defaultPrimitiveValue(Class<?> type) {
        if (type == boolean.class) {
            return Boolean.FALSE;
        } else if (type == byte.class) {
            return (byte) 0;
        } else if (type == char.class) {
            return (char) 0;
        } else if (type == short.class) {
            return (short) 0;
        } else if (type == int.class) {
            return 0;
        } else if (type == long.class) {
            return 0L;
        } else if (type == float.class) {
            return 0f;
        } else if (type == double.class) {
            return 0d;
        }
        return null;
    }

    /*
     * Called by the native runtime under uncaughtErrorPolicy: "throw" for
     * errors with no Java caller to unwind into (unhandled promise
     * rejections): throws from a clean Java frame on this thread's looper so
     * the thread's uncaught-exception handler (and its reporting) runs
     * exactly as for a sync uncaught error.
     */
    @RuntimeCallable
    private static void throwUncaughtJsErrorOnCurrentThread(String message, String stackTrace) {
        final NativeScriptException ex = new NativeScriptException(message, stackTrace, 0);
        JavaScriptStackTrace.applyFrames(ex, stackTrace, null);
        // The failure was already reported (event + hook + log) at the throw
        // decision point - the uncaught-exception handler must not report it
        // a second time.
        ex.markReportedToJs();
        android.os.Looper looper = android.os.Looper.myLooper();
        if (looper == null) {
            throw ex;
        }
        new android.os.Handler(looper).post(new Runnable() {
            @Override
            public void run() {
                throw ex;
            }
        });
    }

    @RuntimeCallable
    private static Class<?> getCachedClass(String className) {
        Class<?> clazz;

        try {
            clazz = classStorageService.retrieveClass(className);
            return clazz;
        } catch (RuntimeException e) {
            return null;
        }
    }

    @RuntimeCallable
    private Class<?> findClass(String className) throws ClassNotFoundException {
        Class<?> clazz = dexFactory.findClass(className);
        return clazz;
    }

    private void purgeAllProxies() {
        if (dexFactory == null) {
            return;
        }

        dexFactory.purgeAllProxies();
    }

    @RuntimeCallable
    private static Object createArrayHelper(String arrayClassName, int size) throws ClassNotFoundException {
        Class<?> clazz = getClassForName(arrayClassName);

        Object arr = Array.newInstance(clazz, size);

        return arr;
    }

    @RuntimeCallable
    private static boolean useGlobalRefs() {
        int JELLY_BEAN = 16;
        boolean useGlobalRefs = android.os.Build.VERSION.SDK_INT >= JELLY_BEAN;
        return useGlobalRefs;
    }

}
