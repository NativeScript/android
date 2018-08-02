package com.tns;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.lang.ref.WeakReference;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.util.Log;
import android.util.SparseArray;

import com.tns.bindings.ProxyGenerator;

public class Runtime {
    private native void initNativeScript(int runtimeId, String filesPath, String nativeLibDir, boolean verboseLoggingEnabled, boolean isDebuggable, String packageName,
                                         Object[] v8Options, String callingDir, int maxLogcatObjectSize, boolean forceLog);

    private native void runModule(int runtimeId, String filePath) throws NativeScriptException;

    private native void runWorker(int runtimeId, String filePath) throws NativeScriptException;

    private native Object runScript(int runtimeId, String filePath) throws NativeScriptException;

    private native Object callJSMethodNative(int runtimeId, int javaObjectID, String methodName, int retType, boolean isConstructor, Object... packagedArgs) throws NativeScriptException;

    private native void createJSInstanceNative(int runtimeId, Object javaObject, int javaObjectID, String canonicalName);

    private native int generateNewObjectId(int runtimeId);

    private native boolean notifyGc(int runtimeId);
    private native void lock(int runtimeId);
    private native void unlock(int runtimeId);

    private native void passUncaughtExceptionToJsNative(int runtimeId, Throwable ex, String stackTrace);

    private native void clearStartupData(int runtimeId);

    public static native int getPointerSize();

    public static native void SetManualInstrumentationMode(String mode);

    private static native void WorkerGlobalOnMessageCallback(int runtimeId, String message);

    private static native void WorkerObjectOnMessageCallback(int runtimeId, int workerId, String message);

    private static native void TerminateWorkerCallback(int runtimeId);

    private static native void ClearWorkerPersistent(int runtimeId, int workerId);

    private static native void CallWorkerObjectOnErrorHandleMain(int runtimeId, int workerId, String message, String stackTrace, String filename, int lineno, String threadName) throws NativeScriptException;

    private static native void ResetDateTimeConfigurationCache(int runtimeId);

    void passUncaughtExceptionToJs(Throwable ex, String stackTrace) {
        passUncaughtExceptionToJsNative(getRuntimeId(), ex, stackTrace);
    }

    private boolean initialized;

    private final static HashMap<String, Class<?>> classCache = new HashMap<String, Class<?>>();

    private final static HashSet<ClassLoader> classLoaderCache = new HashSet<ClassLoader>();

    private final static String FAILED_CTOR_RESOLUTION_MSG = "Check the number and type of arguments.\n" +
            "Primitive types need to be manually wrapped in their respective Object wrappers.\n" +
            "If you are creating an instance of an inner class, make sure to always provide reference to the outer `this` as the first argument.";

    private final HashMap<Integer, Object> strongInstances = new HashMap<>();

    private final HashMap<Integer, WeakReference<Object>> weakInstances = new HashMap<>();

    private final NativeScriptHashMap<Object, Integer> strongJavaObjectToID = new NativeScriptHashMap<Object, Integer>();

    private final NativeScriptWeakHashMap<Object, Integer> weakJavaObjectToID = new NativeScriptWeakHashMap<Object, Integer>();

    private final Map<Class<?>, JavaScriptImplementation> loadedJavaScriptExtends = new HashMap<Class<?>, JavaScriptImplementation>();

    private final java.lang.Runtime dalvikRuntime = java.lang.Runtime.getRuntime();

    private final Object keyNotFoundObject = new Object();
    private int currentObjectId = -1;

    private ExtractPolicy extractPolicy;

    private ArrayList<Constructor<?>> ctorCache = new ArrayList<Constructor<?>>();

    private Logger logger;

    private boolean isLiveSyncStarted;
    public boolean getIsLiveSyncStarted() { return this.isLiveSyncStarted; }
    public void setIsLiveSyncStarted(boolean value) { this.isLiveSyncStarted = value; }

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

    private boolean isTerminating;

    /*
        Used to map to Handler, for messaging between Main and the other Workers
     */
    private final int workerId;

    /*
        Used by all Worker threads to communicate with the Main thread
     */
    private Handler mainThreadHandler;

    private static AtomicInteger nextRuntimeId = new AtomicInteger(0);
    private final static ThreadLocal<Runtime> currentRuntime = new ThreadLocal<Runtime>();
    private final static Map<Integer, Runtime> runtimeCache = new ConcurrentHashMap<>();
    public static Map<Integer, ConcurrentLinkedQueue<Message>> pendingWorkerMessages = new ConcurrentHashMap<>();

    /*
        Holds reference to all Worker Threads' handlers
        Note: Should only be used on the main thread
     */
    private Map<Integer, Handler> workerIdToHandler = new HashMap<>();

    public Runtime(StaticConfiguration config, DynamicConfiguration dynamicConfiguration) {
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
                if (dynamicConfiguration.mainThreadScheduler != null) {
                    this.mainThreadHandler = dynamicConfiguration.mainThreadScheduler.getHandler();
                }

                classResolver = new ClassResolver(this);
                currentRuntime.set(this);

                runtimeCache.put(this.runtimeId, this);

                gcListener = GcListener.getInstance(config.appConfig.getGcThrottleTime(), config.appConfig.getMemoryCheckInterval(), config.appConfig.getFreeMemoryRatio());
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

        return runtime;
    }

    private static Runtime getObjectRuntime(Object object) {
        Runtime runtime = null;

        for (Runtime r : runtimeCache.values()) {
            if (r.getJavaObjectID(object) != null) {
                runtime = r;
                break;
            }
        }

        return runtime;
    }

    public DynamicConfiguration getDynamicConfig() {
        return dynamicConfig;
    }

    @RuntimeCallable
    public int getMarkingModeOrdinal() {
        if (staticConfiguration != null && staticConfiguration.appConfig != null) {
            return staticConfiguration.appConfig.getMarkingMode().ordinal();
        } else {
            return ((MarkingMode)AppConfig.KnownKeys.MarkingMode.getDefaultValue()).ordinal();
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

    private static class WorkerThreadHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            Runtime currentRuntime = Runtime.getCurrentRuntime();

            if (currentRuntime.isTerminating) {
                if (currentRuntime.logger.isEnabled()) {
                    currentRuntime.logger.write("Worker(id=" + currentRuntime.workerId + ") is terminating, it will not process the message.");
                }

                return;
            }

            /*
                Handle messages coming from the Main thread
             */
            if (msg.arg1 == MessageType.MainToWorker) {
                /*
                    Calls the Worker script's onmessage implementation with arg -> msg.obj.toString()
                 */
                WorkerGlobalOnMessageCallback(currentRuntime.runtimeId, msg.obj.toString());
            } else if (msg.arg1 == MessageType.TerminateThread) {
                currentRuntime.isTerminating = true;
                GcListener.unsubscribe(currentRuntime);

                runtimeCache.remove(currentRuntime.runtimeId);

                TerminateWorkerCallback(currentRuntime.runtimeId);

                if (currentRuntime.logger.isEnabled()) {
                    currentRuntime.logger.write("Worker(id=" + currentRuntime.workerId + ", name=\"" + Thread.currentThread().getName() + "\") has terminated execution. Don't make further function calls to it.");
                }

                this.getLooper().quit();
            } else if (msg.arg1 == MessageType.TerminateAndCloseThread) {
                Message msgToMain = Message.obtain();
                msgToMain.arg1 = MessageType.CloseWorker;
                msgToMain.arg2 = currentRuntime.workerId;

                currentRuntime.mainThreadHandler.sendMessage(msgToMain);

                currentRuntime.isTerminating = true;
                GcListener.unsubscribe(currentRuntime);

                runtimeCache.remove(currentRuntime.runtimeId);

                TerminateWorkerCallback(currentRuntime.runtimeId);

                if (currentRuntime.logger.isEnabled()) {
                    currentRuntime.logger.write("Worker(id=" + currentRuntime.workerId + ", name=\"" + Thread.currentThread().getName() + "\") has terminated execution. Don't make further function calls to it.");
                }

                this.getLooper().quit();
            }
        }
    }

    private static class WorkerThread extends HandlerThread {

        private Integer workerId;
        private ThreadScheduler mainThreadScheduler;
        private String filePath;
        private String callingJsDir;

        public WorkerThread(String name, Integer workerId, ThreadScheduler mainThreadScheduler, String callingJsDir) {
            super("W" + workerId + ": " + name);
            this.filePath = name;
            this.workerId = workerId;
            this.mainThreadScheduler = mainThreadScheduler;
            this.callingJsDir = callingJsDir;
        }

        public void startRuntime() {
            Handler handler = new Handler(this.getLooper());

            handler.post((new Runnable() {
                @Override
                public void run() {
                    Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);

                    WorkThreadScheduler workThreadScheduler = new WorkThreadScheduler(new WorkerThreadHandler());

                    DynamicConfiguration dynamicConfiguration = new DynamicConfiguration(workerId, workThreadScheduler, mainThreadScheduler, callingJsDir);

                    if (staticConfiguration.logger.isEnabled()) {
                        staticConfiguration.logger.write("Worker (id=" + workerId + ")'s Runtime is initializing!");
                    }

                    Runtime runtime = initRuntime(dynamicConfiguration);

                    if (staticConfiguration.logger.isEnabled()) {
                        staticConfiguration.logger.write("Worker (id=" + workerId + ")'s Runtime initialized!");
                    }

                    /*
                    	Send a message to the Main Thread to `shake hands`,
                    	Main Thread will cache the Worker Handler for later use
                     */
                    Message msg = Message.obtain();
                    msg.arg1 = MessageType.Handshake;
                    msg.arg2 = runtime.runtimeId;

                    runtime.mainThreadHandler.sendMessage(msg);
                    runtime.runWorker(runtime.runtimeId, filePath);

                    runtime.processPendingMessages();
                }
            }));
        }
    }

    private void processPendingMessages() {
        Queue<Message> messages = Runtime.pendingWorkerMessages.get(this.getWorkerId());
        if (messages == null) {
            return;
        }

        Handler handler = this.getHandler();
        while (!messages.isEmpty()) {
            handler.sendMessage(messages.poll());
        }
    }

    private static class MainThreadHandler extends Handler {
        public MainThreadHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            /*
            	Handle messages coming from a Worker thread
             */
            if (msg.arg1 == MessageType.WorkerToMain) {
                /*
                    Calls the Worker (with id - workerId) object's onmessage implementation with arg -> msg.obj.toString()
                 */
                WorkerObjectOnMessageCallback(Runtime.getCurrentRuntime().runtimeId, msg.arg2, msg.obj.toString());
            }
            /*
            	Handle a 'Handshake' message sent from a new Worker,
            	so that the Main may cache it and send messages to it later
             */
            else if (msg.arg1 == MessageType.Handshake) {
                int senderRuntimeId = msg.arg2;
                Runtime workerRuntime = runtimeCache.get(senderRuntimeId);
                Runtime mainRuntime = Runtime.getCurrentRuntime();

                // If worker has had its close/terminate called before the threads could shake hands
                if (workerRuntime == null) {
                    if (mainRuntime.logger.isEnabled()) {
                        mainRuntime.logger.write("Main thread couldn't shake hands with worker (runtimeId: " + workerRuntime + ") because it has been terminated!");
                    }

                    return;
                }

                /*
                	Main thread now has a reference to the Worker's handler,
                	so messaging between the two threads can begin
                 */
                mainRuntime.workerIdToHandler.put(workerRuntime.getWorkerId(), workerRuntime.getHandler());

                if (mainRuntime.logger.isEnabled()) {
                    mainRuntime.logger.write("Worker thread (workerId:" + workerRuntime.getWorkerId() + ") shook hands with the main thread!");
                }
            } else if (msg.arg1 == MessageType.CloseWorker) {
                Runtime currentRuntime = Runtime.getCurrentRuntime();

                // remove reference to a Worker thread's handler that is in the process of closing
                currentRuntime.workerIdToHandler.put(msg.arg2, null);

                ClearWorkerPersistent(currentRuntime.runtimeId, msg.arg2);
            }
            /*
               Handle unhandled exceptions/errors coming from the worker thread
            */
            else if (msg.arg1 == MessageType.BubbleUpException) {
                Runtime currentRuntime = Runtime.getCurrentRuntime();

                int workerId = msg.arg2;
                JavaScriptErrorMessage errorMessage = (JavaScriptErrorMessage) msg.obj;

                CallWorkerObjectOnErrorHandleMain(currentRuntime.runtimeId, workerId, errorMessage.getMessage(), errorMessage.getStackTrace(), errorMessage.getFilename(), errorMessage.getLineno(), errorMessage.getThreadName());
            }
        }
    }


    /*
        This method initializes the runtime and should always be called first and through the main thread
        in order to set static configuration that all following workers can use
     */
    public static Runtime initializeRuntimeWithConfiguration(StaticConfiguration config) {
        staticConfiguration = config;
        WorkThreadScheduler mainThreadScheduler = new WorkThreadScheduler(new MainThreadHandler(Looper.myLooper()));
        DynamicConfiguration dynamicConfiguration = new DynamicConfiguration(0, mainThreadScheduler, null);
        Runtime runtime = initRuntime(dynamicConfiguration);

        return runtime;
    }

    /*
        This method should be called via native code only after the static configuration has been initialized.
        It will use the static configuration for all following calls to initialize a new runtime.
     */
    @RuntimeCallable
    public static void initWorker(String jsFileName, String callingJsDir, int id) {
        // This method will always be called from the Main thread
        Runtime runtime = Runtime.getCurrentRuntime();
        ThreadScheduler mainThreadScheduler = runtime.getDynamicConfig().myThreadScheduler;

        WorkerThread worker = new WorkerThread(jsFileName, id, mainThreadScheduler, callingJsDir);
        worker.start();
        worker.startRuntime();
    }

    /*
        This method deals with initializing the runtime with given configuration
        Does it for both workers and for the main thread
     */
    private static Runtime initRuntime(DynamicConfiguration dynamicConfiguration) {
        Runtime runtime = new Runtime(staticConfiguration, dynamicConfiguration);
        runtime.init();

        return runtime;
    }

    private boolean isInitializedImpl() {
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

            this.dexFactory = new DexFactory(logger, classLoader, dexDir, dexThumb);

            if (logger.isEnabled()) {
                logger.write("Initializing NativeScript JAVA");
            }

            try {
                Module.init(logger, rootDir, appDir);
            } catch (IOException ex) {
                throw new RuntimeException("Fail to initialize Require class", ex);
            }

            boolean forceConsoleLog = appConfig.getForceLog() || "timeline".equalsIgnoreCase(appConfig.getProfilingMode());

            initNativeScript(getRuntimeId(), Module.getApplicationFilesPath(), nativeLibDir, logger.isEnabled(), isDebuggable, appName, appConfig.getAsArray(),
                    callingJsDir, appConfig.getMaxLogcatObjectSize(), forceConsoleLog);

            //clearStartupData(getRuntimeId()); // It's safe to delete the data after the V8 debugger is initialized

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
        String filePath = jsFile.getPath();
        runModule(getRuntimeId(), filePath);
    }

    public Object runScript(File jsFile) throws NativeScriptException {
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
                            if (arr[1] == null) {
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

    public void notifyGc() { notifyGc(runtimeId); }
    public void lock() { lock(runtimeId); }
    public void unlock() { unlock(runtimeId); }

    public static void initInstance(Object instance) {
        ManualInstrumentation.Frame frame = ManualInstrumentation.start("Runtime.initInstance");
        try {
            Runtime runtime = Runtime.getCurrentRuntime();

            int objectId = runtime.currentObjectId;

            if (objectId != -1) {
                runtime.makeInstanceStrong(instance, objectId);
            } else {
                runtime.createJSInstance(instance);
            }
        } finally {
            frame.close();
        }
    }

    private void createJSInstance(Object instance) {
        int javaObjectID = generateNewObjectId(getRuntimeId());

        makeInstanceStrong(instance, javaObjectID);

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
        Class<?> clazz = classCache.get(className);

        if (clazz == null) {
            for (ClassLoader classLoader : classLoaderCache) {
                try {
                    clazz = classLoader.loadClass(className);
                    if (clazz != null) {
                        classCache.put(className, clazz);
                        break;
                    }
                } catch (ClassNotFoundException e1) {
                    Log.w("JS", "Dynamically loading class " + className + " was unsuccessful. Will attempt to load class from alternative ClassLoader.");
                }
            }
            if (clazz == null) {
                clazz = Class.forName(className);
                classCache.put(className, clazz);
            }
        }

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
        strongInstances.put(key, instance);
        strongJavaObjectToID.put(instance, key);

        Class<?> clazz = instance.getClass();
        String className = clazz.getName();
        if (!classCache.containsKey(className)) {
            classCache.put(className, clazz);
            ClassLoader clazzloader = clazz.getClassLoader();
            if (!classLoaderCache.contains(clazzloader)) {
                classLoaderCache.add(clazzloader);
            }
        }

        if (logger != null && logger.isEnabled()) {
            logger.write("MakeInstanceStrong (" + key + ", " + instance.getClass().toString() + ")");
        }
    }

    private void makeInstanceWeak(int javaObjectID, boolean keepAsWeak) {
        if (logger.isEnabled()) {
            logger.write("makeInstanceWeak instance " + javaObjectID + " keepAsWeak=" + keepAsWeak);
        }
        Object instance = strongInstances.get(javaObjectID);

        if (keepAsWeak) {
            weakJavaObjectToID.put(instance, Integer.valueOf(javaObjectID));
            weakInstances.put(javaObjectID, new WeakReference<Object>(instance));
        }

        strongInstances.remove(javaObjectID);
        strongJavaObjectToID.remove(instance);
    }

    @RuntimeCallable
    private void makeInstanceWeak(ByteBuffer buff, int length, boolean keepAsWeak) {
        buff.position(0);
        for (int i = 0; i < length; i++) {
            int javaObjectId = buff.getInt();
            makeInstanceWeak(javaObjectId, keepAsWeak);
        }
    }

    @RuntimeCallable
    private boolean makeInstanceWeakAndCheckIfAlive(int javaObjectID) {
        if (logger.isEnabled()) {
            logger.write("makeInstanceWeakAndCheckIfAlive instance " + javaObjectID);
        }
        Object instance = strongInstances.get(javaObjectID);
        if (instance == null) {
            WeakReference<Object> ref = weakInstances.get(javaObjectID);
            if (ref == null) {
                return false;
            } else {
                instance = ref.get();
                if (instance == null) {
                    // The Java was moved from strong to weak, and then the Java instance was collected.
                    weakInstances.remove(javaObjectID);
                    weakJavaObjectToID.remove(javaObjectID);
                    return false;
                } else {
                    return true;
                }
            }
        } else {
            strongInstances.remove(javaObjectID);
            strongJavaObjectToID.remove(instance);

            weakJavaObjectToID.put(instance, javaObjectID);
            weakInstances.put(javaObjectID, new WeakReference<Object>(instance));

            return true;
        }
    }

    @RuntimeCallable
    private void checkWeakObjectAreAlive(ByteBuffer input, ByteBuffer output, int length) {
        input.position(0);
        output.position(0);
        for (int i = 0; i < length; i++) {
            int javaObjectId = input.getInt();

            WeakReference<Object> weakRef = weakInstances.get(javaObjectId);

            int isReleased;

            if (weakRef != null) {
                Object instance = weakRef.get();

                if (instance == null) {
                    isReleased = 1;
                    weakInstances.remove(javaObjectId);
                } else {
                    isReleased = 0;
                }
            } else {
                isReleased = (strongInstances.get(javaObjectId) == null) ? 1 : 0;
            }

            output.putInt(isReleased);
        }
    }

    @RuntimeCallable
    private Object getJavaObjectByID(int javaObjectID) throws Exception {
        if (logger.isEnabled()) {
            logger.write("Platform.getJavaObjectByID:" + javaObjectID);
        }

        Object instance = strongInstances.get(javaObjectID);

        if (instance == null) {
            instance = keyNotFoundObject;
        }

        if (instance == keyNotFoundObject) {
            WeakReference<Object> wr = weakInstances.get(javaObjectID);
            if (wr == null) {
                throw new NativeScriptException("No weak reference found. Attempt to use cleared object reference id=" + javaObjectID);
            }

            instance = wr.get();
            if (instance == null) {
                throw new NativeScriptException("Attempt to use cleared object reference id=" + javaObjectID);
            }
        }

        // Log.d(DEFAULT_LOG_TAG,
        // "Platform.getJavaObjectByID found strong object with id:" +
        // javaObjectID);
        return instance;
    }

    private Integer getJavaObjectID(Object obj) {
        Integer id = strongJavaObjectToID.get(obj);
        if (id == null) {
            id = weakJavaObjectToID.get(obj);
        }

        return id;
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

    // sends args in pairs (typeID, value, null) except for objects where its
    // (typeid, javaObjectID, javaJNIClassPath)
    public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, Object... args) throws NativeScriptException {
        return callJSMethod(javaObject, methodName, retType, false /* isConstructor */, args);
    }

    public static Object callJSMethodWithDelay(Object javaObject, String methodName, Class<?> retType, long delay, Object... args) throws NativeScriptException {
        return callJSMethod(javaObject, methodName, retType, false /* isConstructor */, delay, args);
    }

    public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, Object... args) throws NativeScriptException {
        Object ret = callJSMethod(javaObject, methodName, retType, isConstructor, 0, args);

        return ret;
    }

    public static Object callJSMethod(Object javaObject, String methodName, boolean isConstructor, Object... args) throws NativeScriptException {
        return callJSMethod(javaObject, methodName, void.class, isConstructor, 0, args);
    }

    public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, long delay, Object... args) throws NativeScriptException {
        Runtime runtime = Runtime.getCurrentRuntime();

        if (runtime == null) {
            runtime = getObjectRuntime(javaObject);
        }

        return runtime.callJSMethodImpl(javaObject, methodName, retType, isConstructor, delay, args);
    }

    private Object callJSMethodImpl(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, long delay, Object... args) throws NativeScriptException {
        Integer javaObjectID = getJavaObjectID(javaObject);
        if (javaObjectID == null) {
            throw new NativeScriptException("Cannot find object id for instance=" + ((javaObject == null) ? "null" : javaObject));
        }

        if (logger.isEnabled()) {
            logger.write("Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getName() : "null"));
        }

        Object result = dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, delay, retType, args);

        return result;
    }

    // Packages args in format (typeID, value, null) except for objects where it is
    // (typeid, javaObjectID, canonicalName)
    // if javaObject has no javaObjecID meaning javascript object does not
    // exists for this object we assign one.
    private Object[] packageArgs(Object... args) {
        int len = (args != null) ? (args.length * 3) : 0;
        Object[] packagedArgs = new Object[len];

        if (len > 0) {
            int jsArgsIndex = 0;

            for (int i = 0; i < args.length; i++) {
                Object value = args[i];
                int typeId = TypeIDs.GetObjectTypeId(value);
                String javaClassPath = null;

                if (typeId == TypeIDs.JsObject) {
                    javaClassPath = value.getClass().getName();
                    value = getOrCreateJavaObjectID(value);
                }

                packagedArgs[jsArgsIndex++] = typeId;

                packagedArgs[jsArgsIndex++] = value;

                packagedArgs[jsArgsIndex++] = javaClassPath;
            }
        }

        return packagedArgs;
    }

    static Class<?> getClassForName(String className) throws ClassNotFoundException {
        Class<?> clazz = classCache.get(className);
        if (clazz == null) {
            clazz = Class.forName(className);
            if (clazz != null) {
                classCache.put(className, clazz);
            }
        }

        return clazz;
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

        Class<?> clazz = getClassForName(className);

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

    private Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, Class<?> retType, final Object[] args) throws NativeScriptException {
        return dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, 0, retType, args);
    }

    private Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, long delay, Class<?> retType, final Object[] args) throws NativeScriptException {
        final int returnType = TypeIDs.GetObjectTypeId(retType);
        Object ret = null;

        boolean isWorkThread = threadScheduler.getThread().equals(Thread.currentThread());

        final Object[] tmpArgs = extendConstructorArgs(methodName, isConstructor, args);
        final boolean discardUncaughtJsExceptions = this.config.appConfig.getDiscardUncaughtJsExceptions();

        if (isWorkThread) {
            Object[] packagedArgs = packageArgs(tmpArgs);
            try {
                ret = callJSMethodNative(getRuntimeId(), javaObjectID, methodName, returnType, isConstructor, packagedArgs);
            } catch (NativeScriptException e) {
                if(discardUncaughtJsExceptions) {
                    logger.write("Error on currentThread for callJSMethodNative:", e.getMessage());
                    e.printStackTrace();
                } else {
                    throw e;
                }
            }
        } else {
            final Object[] arr = new Object[2];

            final boolean isCtor = isConstructor;

            Runnable r = new Runnable() {
                @Override
                public void run() {
                    synchronized (this) {
                        try {
                            final Object[] packagedArgs = packageArgs(tmpArgs);
                            arr[0] = callJSMethodNative(getRuntimeId(), javaObjectID, methodName, returnType, isCtor, packagedArgs);
                        } catch (NativeScriptException e) {
                            if(discardUncaughtJsExceptions) {
                                logger.write("Error off currentThread for callJSMethodNative:", e.getMessage());
                                e.printStackTrace();
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
                        if (arr[1] == null) {
                            r.wait();
                        }
                    } catch (InterruptedException e) {
                        ret = e;
                    }
                }
            }

            ret = arr[0];
        }

        return ret;
    }

    @RuntimeCallable
    private static Class<?> getCachedClass(String className) {
        Class<?> clazz = classCache.get(className);
        return clazz;
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

    /*
        ======================================================================
        ======================================================================
                            Workers messaging callbacks
        ======================================================================
        ======================================================================
     */
    @RuntimeCallable
    public static void sendMessageFromMainToWorker(int workerId, String message) {
        Runtime currentRuntime = Runtime.getCurrentRuntime();

        Message msg = Message.obtain();
        msg.obj = message;
        msg.arg1 = MessageType.MainToWorker;

        boolean hasKey = currentRuntime.workerIdToHandler.containsKey(workerId);
        Handler workerHandler = currentRuntime.workerIdToHandler.get(workerId);

        // TODO: Pete: Ensure that we won't end up in an endless loop. Can we get an invalid workerId?
        /*
            If workHandler is null then the new Worker Thread still hasn't completed initializing

            OR

            The workHandler is null because it has been closed; Check if its key is still in the map
         */
        if (workerHandler == null) {
            // Attempt to send a message to a closed worker, throw error or just log a message
            if (hasKey) {
                if (currentRuntime.logger.isEnabled()) {
                    currentRuntime.logger.write("Worker(id=" + msg.arg2 + ") that you are trying to send a message to has been terminated. No message will be sent.");
                }

                return;
            }

            if (currentRuntime.logger.isEnabled()) {
                currentRuntime.logger.write("Worker(id=" + msg.arg2 + ")'s handler still not initialized. Requeueing message for Worker(id=" + msg.arg2 + ")");
            }

            if (pendingWorkerMessages.get(workerId) == null) {
                pendingWorkerMessages.put(workerId, new ConcurrentLinkedQueue<Message>());
            }

            Queue<Message> messages = pendingWorkerMessages.get(workerId);
            messages.add(msg);

            return;
        }

        if (!workerHandler.getLooper().getThread().isAlive()) {
            return;
        }

        workerHandler.sendMessage(msg);
    }

    @RuntimeCallable
    public static void sendMessageFromWorkerToMain(String message) {
        Runtime currentRuntime = Runtime.getCurrentRuntime();

        Message msg = Message.obtain();
        msg.arg1 = MessageType.WorkerToMain;

        /*
            Send the workerId associated with the JavaScript Worker object
         */
        msg.arg2 = currentRuntime.getWorkerId();
        msg.obj = message;

        currentRuntime.mainThreadHandler.sendMessage(msg);
    }

    @RuntimeCallable
    public static void workerObjectTerminate(int workerId) {
        // Thread should always be main here
        Runtime currentRuntime = Runtime.getCurrentRuntime();
        final long ResendDelay = 1000;

        Message msg = Message.obtain();

        boolean hasKey = currentRuntime.workerIdToHandler.containsKey(workerId);
        Handler workerHandler = currentRuntime.workerIdToHandler.get(workerId);

        msg.arg1 = MessageType.TerminateThread;
        msg.arg2 = workerId;

        /*
            If workHandler is null then the new Worker Thread still hasn't completed initializing

            OR

            The workHandler is null because it has been closed; Check if its key is still in the map
         */
        if (workerHandler == null) {
            // Attempt to send a message to a closed worker, throw error or just log a message
            if (hasKey) {
                if (currentRuntime.logger.isEnabled()) {
                    currentRuntime.logger.write("Worker(id=" + msg.arg2 + ") is already terminated. No message will be sent.");
                }

                return;
            } else {
                if (currentRuntime.logger.isEnabled()) {
                    currentRuntime.logger.write("Worker(id=" + msg.arg2 + ")'s handler still not initialized. Requeueing terminate() message for Worker(id=" + msg.arg2 + ")");
                }

                if (pendingWorkerMessages.get(workerId) == null) {
                    pendingWorkerMessages.put(workerId, new ConcurrentLinkedQueue<Message>());
                }

                Queue<Message> messages = pendingWorkerMessages.get(workerId);
                messages.add(msg);
                return;
            }
        }

        // Worker was closed during this 'terminate' call, nothing to do here
        if (!workerHandler.getLooper().getThread().isAlive()) {
            return;
        }

        // 'terminate' message must be executed immediately
        workerHandler.sendMessageAtFrontOfQueue(msg);

        // Set value for workerId key to null
        currentRuntime.workerIdToHandler.put(workerId, null);
    }

    @RuntimeCallable
    public static void workerScopeClose() {
        // Thread should always be a worker
        Runtime currentRuntime = Runtime.getCurrentRuntime();

        Message msgToWorker = Message.obtain();
        msgToWorker.arg1 = MessageType.TerminateAndCloseThread;

        currentRuntime.getHandler().sendMessageAtFrontOfQueue(msgToWorker);
    }

    @RuntimeCallable
    public static void passUncaughtExceptionFromWorkerToMain(String message, String filename, String stackTrace, int lineno) {
        // Thread should always be a worker
        Runtime currentRuntime = Runtime.getCurrentRuntime();

        Message msg = Message.obtain();
        msg.arg1 = MessageType.BubbleUpException;
        msg.arg2 = currentRuntime.workerId;

        String threadName = currentRuntime.getHandler().getLooper().getThread().getName();
        JavaScriptErrorMessage error = new JavaScriptErrorMessage(message, stackTrace, filename, lineno, threadName);

        msg.obj = error;

        // TODO: Pete: Should we treat the message with higher priority?
        currentRuntime.mainThreadHandler.sendMessage(msg);
    }

    public void clearStartupData() {
        clearStartupData(getRuntimeId());
    }
}
