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

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.util.SparseArray;

import com.tns.bindings.ProxyGenerator;

public class Runtime
{
	private native void initNativeScript(int runtimeId, String filesPath, boolean verboseLoggingEnabled, String packageName, Object[] v8Options, JsDebugger jsDebugger);

	private native void runModule(int runtimeId, String filePath) throws NativeScriptException;

	private native Object runScript(int runtimeId, String filePath) throws NativeScriptException;

	private native Object callJSMethodNative(int runtimeId, int javaObjectID, String methodName, int retType, boolean isConstructor, Object... packagedArgs) throws NativeScriptException;

	private native void createJSInstanceNative(int runtimeId, Object javaObject, int javaObjectID, String canonicalName);

	private native int generateNewObjectId(int runtimeId);

	private native void adjustAmountOfExternalAllocatedMemoryNative(int runtimeId, long changeInBytes);

	private native void passUncaughtExceptionToJsNative(int runtimeId, Throwable ex, String stackTrace);
	
	private native void clearStartupData(int runtimeId);

	// Used to determine the bitness of the current process (32 vs 64)
	public static native int getPointerSize();
	
	void passUncaughtExceptionToJs(Throwable ex, String stackTrace)
	{
		passUncaughtExceptionToJsNative(getRuntimeId(), ex, stackTrace);
	}

	private boolean initialized;

	private final static HashMap<String, Class<?>> classCache = new HashMap<String, Class<?>>();

	private final static HashSet<ClassLoader> classLoaderCache = new HashSet<ClassLoader>();

	private final static String FAILED_CTOR_RESOLUTION_MSG = "Check the number and type of arguments.\n" +
			"Primitive types need to be manually wrapped in their respective Object wrappers.\n" +
			"If you are creating an instance of an inner class, make sure to always provide reference to the outer `this` as the first argument.";

	private final SparseArray<Object> strongInstances = new SparseArray<Object>();

	private final SparseArray<WeakReference<Object>> weakInstances = new SparseArray<WeakReference<Object>>();

	private final NativeScriptHashMap<Object, Integer> strongJavaObjectToID = new NativeScriptHashMap<Object, Integer>();

	private final NativeScriptWeakHashMap<Object, Integer> weakJavaObjectToID = new NativeScriptWeakHashMap<Object, Integer>();
	
	private final Map<Class<?>, JavaScriptImplementation> loadedJavaScriptExtends = new HashMap<Class<?>, JavaScriptImplementation>();

	private final java.lang.Runtime dalvikRuntime = java.lang.Runtime.getRuntime();

	private final Object keyNotFoundObject = new Object();
	private int currentObjectId = -1;

	private ExtractPolicy extractPolicy;

	private long lastUsedMemory = 0;

	private ArrayList<Constructor<?>> ctorCache = new ArrayList<Constructor<?>>();

	private Logger logger;

	private ThreadScheduler threadScheduler;

	private JsDebugger jsDebugger;

	private DexFactory dexFactory;
	
	private final ClassResolver classResolver;

	private final static Comparator<Method> methodComparator = new Comparator<Method>()
	{
		public int compare(Method lhs, Method rhs)
		{
			return lhs.getName().compareTo(rhs.getName());
		}
	};
	
	private final StaticConfiguration config;
	private static StaticConfiguration staticConfiguration;

	private final DynamicConfiguration dynamicConfig;
	
	private final int runtimeId;

	/*
		Used to map to Handler, for messaging between Main and the other Workers
	 */
	private final int workerId;

	/*
		Used by all Worker threads to communicate with the Main thread
	 */
	private Handler mainThreadHandler;

	private static int nextRuntimeId = 0;
	private final static ThreadLocal<Runtime> currentRuntime = new ThreadLocal<Runtime>();
	private final static Map<Integer, Runtime> runtimeCache = new HashMap<Integer, Runtime>();

	/*
		Holds reference to all Worker Threads' handlers
		Note: Should only be used on the main thread
	 */
	private Map<Integer, Handler> workerIdToHandler = new HashMap<>();
	
	public Runtime(StaticConfiguration config, DynamicConfiguration dynamicConfiguration)
	{
		synchronized(Runtime.currentRuntime)
		{
			Runtime existingRuntime = currentRuntime.get();
			if (existingRuntime != null)
			{
				throw new NativeScriptException("There is an existing runtime on this thread with id=" + existingRuntime.getRuntimeId());
			}

			this.runtimeId = nextRuntimeId++;
			this.config = config;
			this.dynamicConfig = dynamicConfiguration;
			this.threadScheduler = dynamicConfiguration.getHandler();
			this.workerId = dynamicConfiguration.getWorkerId();
			this.mainThreadHandler = dynamicConfiguration.getMainThreadhandler();
			classResolver = new ClassResolver(this);
			currentRuntime.set(this);
			runtimeCache.put(this.runtimeId, this);
		}
	}
	
	public int getRuntimeId()
	{
		return this.runtimeId;
	}
	
	public static Runtime getCurrentRuntime()
	{
		Runtime runtime = currentRuntime.get();
				
		return runtime;
	}
	
	private static Runtime getObjectRuntime(Object object)
	{
		Runtime runtime = null;
		
		for (Runtime r: runtimeCache.values())
		{
			if (r.getJavaObjectID(object) != null)
			{
				runtime = r;
				break;
			}
		}
				
		return runtime;
	}

	public DynamicConfiguration getDynamicConfig() {
		return dynamicConfig;
	}

	public static boolean isInitialized()
	{
		Runtime runtime = Runtime.getCurrentRuntime();
		
		return (runtime != null) ? runtime.isInitializedImpl() : false;
	}

	public int getWorkerId() {
		return workerId;
	}

	private static class WorkerThreadHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			//todo: plamen5kov: implement worker handle message
			/*
				Handle messages coming from the Main thread
			 */
			if(msg.arg1 == MessageType.ToWorkerThread) {

			}
		}
	}

	private static class WorkerThread extends HandlerThread {

		private Integer workerId;
		private ThreadScheduler mainThreadScheduler;

		public WorkerThread(String name) {
			super(name);
		}

		public WorkerThread(String name, Integer workerId, ThreadScheduler mainThreadScheduler) {
			super("W: " + name);
			this.workerId = workerId;
		}

		@Override
		public void run() {
			super.run();
			WorkThreadScheduler workThreadScheduler = new WorkThreadScheduler(new WorkerThreadHandler());

			DynamicConfiguration dynamicConfiguration = new DynamicConfiguration(this.workerId, workThreadScheduler, mainThreadScheduler);
			Runtime runtime = initRuntime(dynamicConfiguration);

			/*
				Send a message to the Main Thread to `shake hands`,
				Main Thread will cache the Worker Handler for later use
			 */
			Message msg = Message.obtain();
			msg.arg1 = MessageType.Handshake;
			msg.arg2 = runtime.runtimeId;

			runtime.mainThreadHandler.sendMessage(msg);
		}
	}

	private static class MainThreadHandler extends Handler {
		public MainThreadHandler(Looper looper) {
			super(looper);
		}

		@Override
		public void handleMessage(Message msg) {
			//todo: plamen5kov: implement main handle message
			/*
				Handle messages coming from a Worker thread
			 */
			if(msg.arg1 == MessageType.ToMainThread) {

			}
			/*
				Handle a 'Handshake' message sent from a new Worker,
				so that the Main may cache it and send messages to it later
			 */
			else if(msg.arg1 == MessageType.Handshake) {
				int senderRuntimeId = msg.arg2;
				Runtime workerRuntime = runtimeCache.get(senderRuntimeId);
				Runtime mainRuntime = Runtime.getCurrentRuntime();

				/*
					Main thread now has a reference to the Worker's handler,
					so messaging between the two threads can begin
				 */
				mainRuntime.workerIdToHandler.put(workerRuntime.getWorkerId(), workerRuntime.threadScheduler.getHandler());

				mainRuntime.logger.write("A new Worker thread shook hands with the main thread!");
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
	public static void initWorker (String jsFileName, int id) {
		Runtime runtime = Runtime.getCurrentRuntime();
		ThreadScheduler mainThreadScheduler = runtime.getDynamicConfig().getHandler();

		HandlerThread worker = new WorkerThread(jsFileName, id, mainThreadScheduler);
		worker.start();
	}

	/*
		This method deals with initializing the runtime with given configuration
		Does it both for workers and for the main thread
	 */
	private static Runtime initRuntime(DynamicConfiguration dynamicConfiguration) {
		Runtime runtime = new Runtime(staticConfiguration, dynamicConfiguration);
		runtime.init();

		return runtime;
	}

	private boolean isInitializedImpl()
	{
		return initialized;
	}
	
	public void init()
	{
		init(config.logger, config.debugger, config.appName, config.runtimeLibPath, config.rootDir, config.appDir, config.classLoader, config.dexDir, config.dexThumb, config.v8Config);
	}

	private void init(Logger logger, Debugger debugger, String appName, File runtimeLibPath, File rootDir, File appDir, ClassLoader classLoader, File dexDir, String dexThumb, Object[] v8Config) throws RuntimeException
	{
		if (initialized)
		{
			throw new RuntimeException("NativeScriptApplication already initialized");
		}

		this.logger = logger;

		this.dexFactory = new DexFactory(logger, classLoader, dexDir, dexThumb);

		if (logger.isEnabled())
		{
			logger.write("Initializing NativeScript JAVA");
		}

		try
		{
			Module.init(logger, rootDir, appDir);
		}
		catch (IOException ex)
		{
			throw new RuntimeException("Fail to initialize Require class", ex);
		}

		if (debugger != null)
		{
			jsDebugger = new JsDebugger(debugger, threadScheduler);
		}
		
		initNativeScript(getRuntimeId(), Module.getApplicationFilesPath(), logger.isEnabled(), appName, v8Config, jsDebugger);

		if (jsDebugger != null)
		{
			jsDebugger.start();
		}
		
		clearStartupData(getRuntimeId()); // It's safe to delete the data after the V8 debugger is initialized
		
		if (logger.isEnabled())
		{
			Date d = new Date();
			int pid = android.os.Process.myPid();
			File f = new File("/proc/" + pid);
			Date lastModDate = new Date(f.lastModified());
			logger.write("init time=" + (d.getTime() - lastModDate.getTime()));
		}

		initialized = true;
	}

	@RuntimeCallable
	public void enableVerboseLogging()
	{
		logger.setEnabled(true);
		ProxyGenerator.IsLogEnabled = true;
	}

	@RuntimeCallable
	public void disableVerboseLogging()
	{
		logger.setEnabled(false);
		ProxyGenerator.IsLogEnabled = false;
	}

	public void run() throws NativeScriptException
	{
		String mainModule = Module.bootstrapApp();
		runModule(new File(mainModule));
	}

	public void runModule(File jsFile) throws NativeScriptException
	{
		String filePath = jsFile.getPath();
		runModule(getRuntimeId(), filePath);
	}

	public Object runScript(File jsFile) throws NativeScriptException
	{
		Object result = null;

		if (jsFile.exists() && jsFile.isFile())
		{
			final String filePath = jsFile.getAbsolutePath();

			boolean isWorkThread = threadScheduler.getThread().equals(Thread.currentThread());

			if (isWorkThread)
			{
				result = runScript(getRuntimeId(), filePath);
			}
			else
			{
				final Object[] arr = new Object[2];

				Runnable r = new Runnable()
				{
					@Override
					public void run()
					{
						synchronized (this)
						{
							try
							{
								arr[0] = runScript(getRuntimeId(), filePath);
							}
							finally
							{
								this.notify();
								arr[1] = Boolean.TRUE;
							}
						}
					}
				};

				boolean success = threadScheduler.post(r);

				if (success)
				{
					synchronized (r)
					{
						try
						{
							if (arr[1] == null)
							{
								r.wait();
							}
						}
						catch (InterruptedException e)
						{
							result = e;
						}
					}
				}
			}
		}

		return result;
	}

	@RuntimeCallable
	private Class<?> resolveClass(String fullClassName, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) throws ClassNotFoundException, IOException
	{
		Class<?> javaClass = classResolver.resolveClass(fullClassName, dexFactory, methodOverrides, implementedInterfaces, isInterface);

		return javaClass;
	}

	@RuntimeCallable
	private long getChangeInBytesOfUsedMemory()
	{
		long usedMemory = dalvikRuntime.totalMemory() - dalvikRuntime.freeMemory();
		long changeInBytes = usedMemory - lastUsedMemory;
		lastUsedMemory = usedMemory;
		return changeInBytes;
	}

	private void adjustAmountOfExternalAllocatedMemory()
	{
		long changeInBytes = getChangeInBytesOfUsedMemory();
		adjustAmountOfExternalAllocatedMemoryNative(getRuntimeId(), changeInBytes);
	}

	public static void initInstance(Object instance)
	{
		Runtime runtime = Runtime.getCurrentRuntime();
		
		int objectId = runtime.currentObjectId;

		if (objectId != -1)
		{
			runtime.makeInstanceStrong(instance, objectId);
		}
		else
		{
			runtime.createJSInstance(instance);
		}
	}

	private void createJSInstance(Object instance)
	{
		int javaObjectID = generateNewObjectId(getRuntimeId());

		makeInstanceStrong(instance, javaObjectID);
		
		Class<?> clazz = instance.getClass();
		
		if (!loadedJavaScriptExtends.containsKey(clazz))
		{
			JavaScriptImplementation jsImpl = clazz.getAnnotation(JavaScriptImplementation.class);
			if (jsImpl != null)
			{
				File jsFile = new File(jsImpl.javaScriptFile());
				runModule(jsFile);
			}
			loadedJavaScriptExtends.put(clazz, jsImpl);
		}
  		  
		String className = clazz.getName();

		createJSInstanceNative(getRuntimeId(), instance, javaObjectID, className);

		if (logger.isEnabled())
			logger.write("JSInstance for " + instance.getClass().toString() + " created with overrides");
	}

	@RuntimeCallable
	private static String[] getTypeMetadata(String className, int index) throws ClassNotFoundException
	{
		Class<?> clazz = classCache.get(className);

		if (clazz == null)
		{
			for (ClassLoader classLoader : classLoaderCache)
			{
				try
				{
					clazz = classLoader.loadClass(className);
					if (clazz != null)
					{
						classCache.put(className, clazz);
						break;
					}
				}
				catch (Exception e1)
				{
					e1.printStackTrace();
				}
			}
			if (clazz == null)
			{
				clazz = Class.forName(className);
				classCache.put(className, clazz);
			}
		}

		String[] result = getTypeMetadata(clazz, index);

		return result;
	}

	private static String[] getTypeMetadata(Class<?> clazz, int index)
	{
		Class<?> mostOuterClass = clazz.getEnclosingClass();
		ArrayList<Class<?>> outerClasses = new ArrayList<Class<?>>();
		while (mostOuterClass != null)
		{
			outerClasses.add(0, mostOuterClass);
			Class<?> nextOuterClass = mostOuterClass.getEnclosingClass();
			if (nextOuterClass == null)
				break;
			mostOuterClass = nextOuterClass;
		}

		Package p = (mostOuterClass != null)
				? mostOuterClass.getPackage()
				: clazz.getPackage();

		int packageCount = 1;
		String pname = p.getName();
		for (int i = 0; i < pname.length(); i++)
		{
			if (pname.charAt(i) == '.')
				++packageCount;
		}

		String name = clazz.getName();
		String[] parts = name.split("[\\.\\$]");

		int endIdx = parts.length;
		int len = endIdx - index;
		String[] result = new String[len];

		int endOuterTypeIdx = packageCount + outerClasses.size();

		for (int i = index; i < endIdx; i++)
		{
			if (i < packageCount)
			{
				result[i - index] = "P";
			}
			else
			{
				if (i < endOuterTypeIdx)
				{
					result[i - index] = getTypeMetadata(outerClasses.get(i - packageCount));
				}
				else
				{
					result[i - index] = getTypeMetadata(clazz);
				}
			}
		}

		return result;
	}

	private static String getTypeMetadata(Class<?> clazz)
	{
		StringBuilder sb = new StringBuilder();

		if (clazz.isInterface())
		{
			sb.append("I ");
		}
		else
		{
			sb.append("C ");
		}

		if (Modifier.isStatic(clazz.getModifiers()))
		{
			sb.append("S\n");
		}
		else
		{
			sb.append("I\n");
		}

		Class<?> baseClass = clazz.getSuperclass();
		sb.append("B " + ((baseClass != null) ? baseClass.getName() : "").replace('.', '/') + "\n");

		Method[] methods = clazz.getDeclaredMethods();
		Arrays.sort(methods, methodComparator);

		for (Method m : methods)
		{
			int modifiers = m.getModifiers();
			if (!Modifier.isStatic(modifiers) && (Modifier.isPublic(modifiers) || Modifier.isProtected(modifiers)))
			{
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
		for (Field f : fields)
		{
			int modifiers = f.getModifiers();
			if (!Modifier.isStatic(modifiers) && (Modifier.isPublic(modifiers) || Modifier.isProtected(modifiers)))
			{
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
	private void makeInstanceStrong(Object instance, int objectId)
	{
		if (instance == null)
		{
			throw new IllegalArgumentException("instance cannot be null");
		}

		int key = objectId;
		strongInstances.put(key, instance);
		strongJavaObjectToID.put(instance, key);

		Class<?> clazz = instance.getClass();
		String className = clazz.getName();
		if (!classCache.containsKey(className))
		{
			classCache.put(className, clazz);
			ClassLoader clazzloader = clazz.getClassLoader();
			if (!classLoaderCache.contains(clazzloader))
			{
				classLoaderCache.add(clazzloader);
			}
		}

		if (logger != null && logger.isEnabled())
		{
			logger.write("MakeInstanceStrong (" + key + ", " + instance.getClass().toString() + ")");
		}
	}

	private void makeInstanceWeak(int javaObjectID, boolean keepAsWeak)
	{
		if (logger.isEnabled())
			logger.write("makeInstanceWeak instance " + javaObjectID + " keepAsWeak=" + keepAsWeak);
		Object instance = strongInstances.get(javaObjectID);

		if (keepAsWeak)
		{
			weakJavaObjectToID.put(instance, Integer.valueOf(javaObjectID));
			weakInstances.put(javaObjectID, new WeakReference<Object>(instance));
		}

		strongInstances.delete(javaObjectID);
		strongJavaObjectToID.remove(instance);
	}

	@RuntimeCallable
	private void makeInstanceWeak(ByteBuffer buff, int length, boolean keepAsWeak)
	{
		buff.position(0);
		for (int i = 0; i < length; i++)
		{
			int javaObjectId = buff.getInt();
			makeInstanceWeak(javaObjectId, keepAsWeak);
		}
	}

	@RuntimeCallable
	private void checkWeakObjectAreAlive(ByteBuffer input, ByteBuffer output, int length)
	{
		input.position(0);
		output.position(0);
		for (int i = 0; i < length; i++)
		{
			int javaObjectId = input.getInt();

			WeakReference<Object> weakRef = weakInstances.get(javaObjectId);

			int isReleased;

			if (weakRef != null)
			{
				Object instance = weakRef.get();

				if (instance == null)
				{
					isReleased = 1;
					weakInstances.delete(javaObjectId);
				}
				else
				{
					isReleased = 0;
				}
			}
			else
			{
				isReleased = (strongInstances.get(javaObjectId) == null) ? 1 : 0;
			}

			output.putInt(isReleased);
		}
	}

	@RuntimeCallable
	private Object getJavaObjectByID(int javaObjectID) throws Exception
	{
		if (logger.isEnabled())
			logger.write("Platform.getJavaObjectByID:" + javaObjectID);

		Object instance = strongInstances.get(javaObjectID, keyNotFoundObject);

		if (instance == keyNotFoundObject)
		{
			WeakReference<Object> wr = weakInstances.get(javaObjectID);
			if (wr == null)
			{
				throw new NativeScriptException("No weak reference found. Attempt to use cleared object reference id=" + javaObjectID);
			}

			instance = wr.get();
			if (instance == null)
			{
				throw new NativeScriptException("Attempt to use cleared object reference id=" + javaObjectID);
			}
		}

		// Log.d(DEFAULT_LOG_TAG,
		// "Platform.getJavaObjectByID found strong object with id:" +
		// javaObjectID);
		return instance;
	}

	private Integer getJavaObjectID(Object obj)
	{
		Integer id = strongJavaObjectToID.get(obj);
		if (id == null)
		{
			id = weakJavaObjectToID.get(obj);
		}

		return id;
	}

	@RuntimeCallable
	private int getOrCreateJavaObjectID(Object obj)
	{
		Integer result = getJavaObjectID(obj);

		if (result == null)
		{
			int objectId = generateNewObjectId(getRuntimeId());
			makeInstanceStrong(obj, objectId);

			result = objectId;
		}

		return result;
	}

	// sends args in pairs (typeID, value, null) except for objects where its
	// (typeid, javaObjectID, javaJNIClassPath)
	public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, Object... args) throws NativeScriptException
	{
		return callJSMethod(javaObject, methodName, retType, false /* isConstructor */, args);
	}

	public static Object callJSMethodWithDelay(Object javaObject, String methodName, Class<?> retType, long delay, Object... args) throws NativeScriptException
	{
		return callJSMethod(javaObject, methodName, retType, false /* isConstructor */, delay, args);
	}

	public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, Object... args) throws NativeScriptException
	{
		Object ret = callJSMethod(javaObject, methodName, retType, isConstructor, 0, args);

		return ret;
	}

	public static Object callJSMethod(Object javaObject, String methodName, boolean isConstructor, Object... args) throws NativeScriptException
	{
		return callJSMethod(javaObject, methodName, void.class, isConstructor, 0, args);
	}

	public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, long delay, Object... args) throws NativeScriptException
	{
		Runtime runtime = Runtime.getCurrentRuntime();
		
		if (runtime == null)
		{
			runtime = getObjectRuntime(javaObject);
		}
		
		return runtime.callJSMethodImpl(javaObject, methodName, retType, isConstructor, delay, args);
	}
	
	private Object callJSMethodImpl(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, long delay, Object... args) throws NativeScriptException
	{
		Integer javaObjectID = getJavaObjectID(javaObject);
		if (javaObjectID == null)
		{
			throw new NativeScriptException("Cannot find object id for instance=" + ((javaObject == null) ? "null" : javaObject));
		}

		if (logger.isEnabled())
			logger.write("Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getName() : "null"));

		Object result = dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, delay, retType, args);

		return result;
	}

	// Packages args in format (typeID, value, null) except for objects where it
	// is
	// (typeid, javaObjectID, canonicalName)
	// if javaObject has no javaObjecID meaning javascript object does not
	// exists for this object we assign one.
	private Object[] packageArgs(Object... args)
	{
		int len = (args != null) ? (args.length * 3) : 0;
		Object[] packagedArgs = new Object[len];

		if (len > 0)
		{
			int jsArgsIndex = 0;

			for (int i = 0; i < args.length; i++)
			{
				Object value = args[i];
				int typeId = TypeIDs.GetObjectTypeId(value);
				String javaClassPath = null;

				if (typeId == TypeIDs.JsObject)
				{
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
		if (clazz == null)
		{
			clazz = Class.forName(className);
			if (clazz != null) {
				classCache.put(className, clazz);
			}
		}
		
		return clazz;
	}

	@RuntimeCallable
	private String resolveConstructorSignature(Class<?> clazz, Object[] args) throws Exception
	{
		// Pete: cache stuff here, or in the cpp part
		
		if (logger.isEnabled())
			logger.write("resolveConstructorSignature: Resolving constructor for class " + clazz.getName());
		
		String res = MethodResolver.resolveConstructorSignature(clazz, args);

		if (res == null)
		{
			throw new Exception("Failed resolving constructor for class \'" + clazz.getName() + "\' with " + (args != null ? args.length : 0) + " parameters. " + FAILED_CTOR_RESOLUTION_MSG);
		}

		return res;
	}
	
	@RuntimeCallable
	private String resolveMethodOverload(String className, String methodName, Object[] args) throws Exception
	{
		if (logger.isEnabled())
			logger.write("resolveMethodOverload: Resolving method " + methodName + " on class " + className);
		
		Class<?> clazz = getClassForName(className);
		
		String res = MethodResolver.resolveMethodOverload(clazz, methodName, args);
		if (logger.isEnabled())
			logger.write("resolveMethodOverload: method found :" + res);
		if (res == null)
		{
			throw new Exception("Failed resolving method " + methodName + " on class " + className);
		}

		return res;
	}

	private Object[] extendConstructorArgs(String methodName, boolean isConstructor, Object[] args)
	{
		Object[] arr = null;

		if (methodName.equals("init"))
		{
			if (args == null)
			{
				arr = new Object[]
				{ isConstructor };
			}
			else
			{
				arr = new Object[args.length + 1];
				System.arraycopy(args, 0, arr, 0, args.length);
				arr[arr.length - 1] = isConstructor;
			}
		}
		else
		{
			arr = args;
		}

		return arr;
	}

	private Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, Class<?> retType, final Object[] args) throws NativeScriptException
	{
		return dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, 0, retType, args);
	}

	private Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, long delay, Class<?> retType, final Object[] args) throws NativeScriptException
	{
		final int returnType = TypeIDs.GetObjectTypeId(retType);
		Object ret = null;

		boolean isWorkThread = threadScheduler.getThread().equals(Thread.currentThread());

		final Object[] tmpArgs = extendConstructorArgs(methodName, isConstructor, args);

		if (isWorkThread)
		{
			Object[] packagedArgs = packageArgs(tmpArgs);
			ret = callJSMethodNative(getRuntimeId(), javaObjectID, methodName, returnType, isConstructor, packagedArgs);
		}
		else
		{
			final Object[] arr = new Object[2];

			final boolean isCtor = isConstructor;
			Runnable r = new Runnable()
			{
				@Override
				public void run()
				{
					synchronized (this)
					{
						try
						{
							final Object[] packagedArgs = packageArgs(tmpArgs);
							arr[0] = callJSMethodNative(getRuntimeId(), javaObjectID, methodName, returnType, isCtor, packagedArgs);
						}
						finally
						{
							this.notify();
							arr[1] = Boolean.TRUE;
						}
					}
				}
			};

			if (delay > 0)
			{
				try
				{
					Thread.sleep(delay);
				}
				catch (InterruptedException e)
				{
				}
			}

			boolean success = threadScheduler.post(r);

			if (success)
			{
				synchronized (r)
				{
					try
					{
						if (arr[1] == null)
						{
							r.wait();
						}
					}
					catch (InterruptedException e)
					{
						ret = e;
					}
				}
			}

			ret = arr[0];
		}

		return ret;
	}

	@RuntimeCallable
	private static Class<?> getCachedClass(String className)
	{
		Class<?> clazz = classCache.get(className);
		return clazz;
	}

	@RuntimeCallable
	private Class<?> findClass(String className) throws ClassNotFoundException
	{
		Class<?> clazz = dexFactory.findClass(className);
		return clazz;
	}

	private void purgeAllProxies()
	{
		if (dexFactory == null)
		{
			return;
		}

		dexFactory.purgeAllProxies();
	}
	
	@RuntimeCallable
	private static Object createArrayHelper(String arrayClassName, int size) throws ClassNotFoundException
	{
		Class<?> clazz = getClassForName(arrayClassName);
		
		Object arr = Array.newInstance(clazz, size);
		
		return arr;
	}

	@RuntimeCallable
	private static boolean useGlobalRefs()
	{
		int JELLY_BEAN = 16;
		boolean useGlobalRefs = android.os.Build.VERSION.SDK_INT >= JELLY_BEAN; 
		return useGlobalRefs;
	}
}
