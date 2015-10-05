package com.tns;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.lang.ref.WeakReference;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;

import org.json.JSONObject;

import android.util.SparseArray;

import com.tns.bindings.ProxyGenerator;

public class Platform
{
	private static native void initNativeScript(String filesPath, boolean verboseLoggingEnabled, String packageName, String jsOptions);

	private static native void runModule(String filePath) throws NativeScriptException;
	
	private static native Object runScript(String filePath) throws NativeScriptException;

	private static native Object callJSMethodNative(int javaObjectID, String methodName, int retType, boolean isConstructor, Object... packagedArgs) throws NativeScriptException;

	private static native void createJSInstanceNative(Object javaObject, int javaObjectID, String canonicalName);

	private static native int generateNewObjectId();

	private static native void enableVerboseLoggingNative();

	private static native void disableVerboseLoggingNative();
	
	private static native void adjustAmountOfExternalAllocatedMemoryNative(long changeInBytes);
	
	private static native void passUncaughtExceptionToJsNative(Throwable ex, String stackTrace);
	
	private static boolean initialized;
	
	private static final HashMap<String, Class<?>> classCache = new HashMap<String, Class<?>>();
	
	private static final HashSet<ClassLoader> classLoaderCache = new HashSet<ClassLoader>();

	private static final SparseArray<Object> strongInstances = new SparseArray<Object>();
	
	private static final SparseArray<WeakReference<Object>> weakInstances = new SparseArray<WeakReference<Object>>();
	
	private static final com.tns.internal.HashMap<Object, Integer> strongJavaObjectToID = new com.tns.internal.HashMap<Object, Integer>();
	
	private static final com.tns.internal.WeakHashMap<Object, Integer> weakJavaObjectToID = new com.tns.internal.WeakHashMap<Object, Integer>();
	
	private static final Runtime runtime = Runtime.getRuntime();
	private static Class<?> errorActivityClass;
	
	private final static Object keyNotFoundObject = new Object();
	private static int currentObjectId = -1;
	
	private static ExtractPolicy extractPolicy;

	private static long lastUsedMemory = 0;
	
	private static ArrayList<Constructor<?>> ctorCache = new ArrayList<Constructor<?>>();
	
	private static Logger logger;
	
	private static ThreadScheduler threadScheduler;

	private static JsDebugger jsDebugger;
	
	private static DexFactory dexFactory;
	
	private final static Comparator<Method> methodComparator = new Comparator<Method>()
	{
		public int compare(Method lhs, Method rhs)
		{
			return lhs.getName().compareTo(rhs.getName());
		}
	};

	public static Class<?> getErrorActivityClass()
	{
		return errorActivityClass;
	}
	
	public static void setErrorActivityClass(Class<?> clazz)
	{
		errorActivityClass = clazz;
	}
	
	public static void init(ThreadScheduler threadScheduler, Logger logger, String appName, File runtimeLibPath, File rootDir, File appDir, File debuggerSetupDir, ClassLoader classLoader, File dexDir, String dexThumb) throws RuntimeException
	{
		if (initialized)
		{
			throw new RuntimeException("NativeScriptApplication already initialized");
		}
		
		Platform.threadScheduler = threadScheduler;
		
		Platform.logger = logger;
		
		loadLibrary(runtimeLibPath, "NativeScript");
		
		Platform.dexFactory = new DexFactory(logger, classLoader, dexDir, dexThumb);

		try
		{
			Module.init(logger, rootDir, appDir);
		}
		catch (IOException ex)
		{
			throw new RuntimeException("Fail to initialize Require class", ex);
		}
		String jsOptions = readJsOptions(appDir);
		Platform.initNativeScript(Module.getApplicationFilesPath(), logger.isEnabled(), appName, jsOptions);
		
		if (debuggerSetupDir != null)
		{
			jsDebugger = new JsDebugger(logger, threadScheduler, debuggerSetupDir);
											//also runs javaServerThread with resolved port
			int debuggerPort = jsDebugger.getDebuggerPortFromEnvironment();
			if (logger.isEnabled()) logger.write("port=" + debuggerPort);
		}
		
		//
		if (logger.isEnabled())
		{
			Date d = new Date();
			int pid = android.os.Process.myPid();
			File f = new File("/proc/" + pid);
			Date lastModDate = new Date(f.lastModified());
			logger.write("init time=" + (d.getTime() - lastModDate.getTime()));
		}
		//

		initialized = true;
	}
	
	static void loadLibrary(File runtimeLibPath, String libName)
	{
		if (runtimeLibPath == null)
		{
			System.loadLibrary(libName);
		}
		else if (runtimeLibPath.isDirectory())
		{
			String arch = System.getProperty("os.arch");
			String lcArch = arch.toLowerCase();
			String archDir;
			if (lcArch.startsWith("arm"))
			{
				archDir = "arm";
			}
			else if (lcArch.startsWith("i686"))
			{
				archDir = "x86";
			}
			else
			{
				// TODO: add arm64, x64
				throw new RuntimeException("Unsupported arch=" + arch);
			}
			System.load(new File(runtimeLibPath, "lib/" + archDir + "/" + libName +  ".so").getAbsolutePath());
		}
		else
		{
			System.load(runtimeLibPath.getAbsolutePath());
		}
	}
	
	@RuntimeCallable
	public static void enableVerboseLogging()
	{
		logger.setEnabled(true);
		ProxyGenerator.IsLogEnabled = true;
		enableVerboseLoggingNative();
	}

	@RuntimeCallable
	public static void disableVerboseLogging()
	{
		logger.setEnabled(false);
		ProxyGenerator.IsLogEnabled = false;
		disableVerboseLoggingNative();
	}

	@RuntimeCallable
	static void appFail(Throwable ex, String message)
	{
		// TODO: allow app to handle fail message report here. For example
		// integrate google app reports
		// Log.d(DEFAULT_LOG_TAG, message);
		// System.exit(-1);
		
		if (ex == null) {
			ex = new NativeScriptException(message);
		}
		
		// TODO: We need to know whether to fail the app fast here - e.g. System.exit or just raise an exception
		try {
			UncheckedThrow.Throw(ex);
		} catch (NativeScriptException e) {
			if(e == ex){
				// exception is not caught by user code, go to the JavaScript side to raise the __uncaughtError handler
				// TODO: We may want to allow JS code to mark the exception as Handled
				
				//don't throw exception to v8 if v8 is not initialized yet
				if(!initialized) {
					// TODO:
					//passUncaughtExceptionToJsNative(e, ErrorReport.getErrorMessage(e));	
				}
			}
			
			// re-throw the exception to crash the current thread
			UncheckedThrow.Throw(e);
		}
	}
	
	public static void runModule(File jsFile) throws NativeScriptException
	{
		if (jsFile.exists() && jsFile.isFile())
		{
			String filePath = jsFile.getAbsolutePath();
			runModule(filePath);
		}
	}
	
	public static Object runScript(File jsFile) throws NativeScriptException
	{
		Object result = null;
		
		if (jsFile.exists() && jsFile.isFile())
		{
			String filePath = jsFile.getAbsolutePath();
			result = runScript(filePath);
		}
		
		return result;
	}
	
	@RuntimeCallable
	private static Class<?> resolveClass(String fullClassName, String[] methodOverrides) throws ClassNotFoundException, IOException{
		Class<?> javaClass = ClassResolver.resolveClass(fullClassName, dexFactory, methodOverrides);
		
		return javaClass;
	}

	@RuntimeCallable
	private static int cacheConstructor(Class<?> clazz, Object[] args) throws ClassNotFoundException, IOException
	{
		Constructor<?> ctor = MethodResolver.resolveConstructor(clazz, args);

		//TODO: Lubo: Not thread safe already.
		//TODO: Lubo: Does not check for existing items
		int ctorId = ctorCache.size();
		
		ctorCache.add(ctor);
		
		return ctorId;
	}

	@RuntimeCallable
	private static Object createInstance(Object[] args, int objectId, int constructorId) throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException, IllegalArgumentException, InvocationTargetException, IOException
	{
		Constructor<?> ctor = ctorCache.get(constructorId);
		boolean success = MethodResolver.convertConstructorArgs(ctor, args);

		if (!success)
		{
			StringBuilder builder = new StringBuilder();
			builder.append(constructorId + "(");
			if (args != null)
			{
				for (Object arg : args)
				{
					if (arg != null)
					{
						builder.append(arg.toString() + ", ");
					}
					else
					{
						builder.append("null, ");
					}
				}
			}
			builder.append(")");

			throw new InstantiationException("MethodResolver didn't resolve any constructor with the specified arguments " + builder.toString());
		}

		Object instance;
		try
		{
			Platform.currentObjectId = objectId;
			instance = ctor.newInstance(args);

			makeInstanceStrong(instance, objectId);
		}
		finally
		{
			Platform.currentObjectId = -1;
		}
		
		adjustAmountOfExternalAllocatedMemory();
		
		return instance;
	}
	
	@RuntimeCallable
	private static long getChangeInBytesOfUsedMemory()
	{
		long usedMemory = runtime.totalMemory() - runtime.freeMemory();
		long changeInBytes = usedMemory - lastUsedMemory;
		lastUsedMemory = usedMemory;
		return changeInBytes;
	}
	
	private static void adjustAmountOfExternalAllocatedMemory()
	{
		long changeInBytes = getChangeInBytesOfUsedMemory();
		adjustAmountOfExternalAllocatedMemoryNative(changeInBytes);
	}

	// TODO: recomplie the bindings to support the args method and remove this
	// one since it is not needed after bindings recompile.
	public static void initInstance(Object instance)
	{
		int objectId = Platform.currentObjectId;

		if (objectId != -1)
		{
			makeInstanceStrong(instance, objectId);
		}
		else
		{
			createJSInstance(instance);
		}
	}

	private static void createJSInstance(Object instance)
	{
		int javaObjectID = generateNewObjectId();

		makeInstanceStrong(instance, javaObjectID);

		String className = instance.getClass().getName();
				
		createJSInstanceNative(instance, javaObjectID, className);

		if (logger.isEnabled()) logger.write("JSInstance for " + instance.getClass().toString() + " created with overrides");
	}
	
	@RuntimeCallable
	private static String[] getTypeMetadata(String className, int index) throws ClassNotFoundException
	{
		Class<?> clazz = classCache.get(className);
		
		if (clazz == null)
		{
			for (ClassLoader classLoader: classLoaderCache)
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
					if (logger.isEnabled()) logger.write(">>loader=" + classLoader.toString() + " " + e1.getMessage());
				}
			}
			if (clazz == null)
			{
				clazz = Class.forName(className);
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
		for (int i=0; i<pname.length(); i++)
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
		
		for (int i=index; i<endIdx; i++)
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
		
		for (Method m: methods)
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
		for (Field f: fields)
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
	private static void makeInstanceStrong(Object instance, int objectId)
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

		if (logger.isEnabled()) logger.write("MakeInstanceStrong (" + key + ", " + instance.getClass().toString() + ")");
	}

	private static void makeInstanceWeak(int javaObjectID, boolean keepAsWeak)
	{
		if (logger.isEnabled()) logger.write("makeInstanceWeak instance " + javaObjectID + " keepAsWeak=" + keepAsWeak);
		Object instance = strongInstances.get(javaObjectID);
		
		if (keepAsWeak) {
			weakJavaObjectToID.put(instance, Integer.valueOf(javaObjectID));
			weakInstances.put(javaObjectID, new WeakReference<Object>(instance));
		}
		
		strongInstances.delete(javaObjectID);
		strongJavaObjectToID.remove(instance);
	}
	
	@RuntimeCallable
	private static void makeInstanceWeak(ByteBuffer buff, int length, boolean keepAsWeak)
	{
		buff.position(0);
		for (int i=0; i<length; i++)
		{
			int javaObjectId = buff.getInt();
			makeInstanceWeak(javaObjectId, keepAsWeak);
		}
	}
	
	@RuntimeCallable
	private static void checkWeakObjectAreAlive(ByteBuffer input, ByteBuffer output, int length)
	{
		input.position(0);
		output.position(0);
		for (int i=0; i<length; i++)
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
				isReleased = 1;
			}
			
			output.putInt(isReleased);
		}
	}
	
	@RuntimeCallable
	private static Object getJavaObjectByID(int javaObjectID) throws Exception
	{
		if (logger.isEnabled()) logger.write("Platform.getJavaObjectByID:" + javaObjectID);

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

	private static Integer getJavaObjectID(Object obj)
	{
		Integer id = strongJavaObjectToID.get(obj);
		if (id == null)
		{
			id = weakJavaObjectToID.get(obj);
		}

		return id;
	}

	@RuntimeCallable
	public static int getOrCreateJavaObjectID(Object obj)
	{
		Integer result = getJavaObjectID(obj);

		if (result == null)
		{
			int objectId = generateNewObjectId();
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
	
	public static Object callJSMethod(Object javaObject, String methodName, boolean isConstructor,  Object... args) throws NativeScriptException
	{
		return callJSMethod(javaObject, methodName, void.class, isConstructor, 0, args);
	}

	public static Object callJSMethod(Object javaObject, String methodName, Class<?> retType, boolean isConstructor, long delay, Object... args) throws NativeScriptException
	{
		Integer javaObjectID = getJavaObjectID(javaObject);
		if (javaObjectID == null)
		{
			if (logger.isEnabled()) logger.write("Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getName() : "null"));
			appFail(null, "Application failed");
		}

		if (logger.isEnabled()) logger.write("Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getName() : "null"));

		Object result = dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, delay, retType, args);

		return result;
	}

	// Packages args in format (typeID, value, null) except for objects where it
	// is
	// (typeid, javaObjectID, canonicalName)
	// if javaObject has no javaObjecID meaning javascript object does not
	// exists for this object we assign one.
	static Object[] packageArgs(Object... args)
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

	@RuntimeCallable
	private static String resolveMethodOverload(String className, String methodName, Object[] args) throws Exception
	{
		if (logger.isEnabled()) logger.write("resolveMethodOverload: Resolving method " + methodName + " on class " + className);
		String res = MethodResolver.resolveMethodOverload(classCache, className, methodName, args);
		if (logger.isEnabled()) logger.write("resolveMethodOverload: method found :" + res);
		if (res == null)
		{
			throw new Exception("Failed resolving method " + methodName + " on class " + className);
		}

		return res;
	}
	
	private static boolean isJavaThrowable(Object obj)
	{
		boolean isJavaThrowable = false;

		if (obj != null)
		{
			Class<?> c = obj.getClass();

			isJavaThrowable = Throwable.class.isAssignableFrom(c);
		}

		return isJavaThrowable;
	}
	
	private static Object[] extendConstructorArgs(String methodName, boolean isConstructor, Object[] args)
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

	private static Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, Class<?> retType, final Object[] args) throws NativeScriptException
	{
		return dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, 0, retType, args);
	}

	private static Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, long delay, Class<?> retType, final Object[] args) throws NativeScriptException
	{
		final int returnType = TypeIDs.GetObjectTypeId(retType);
		Object ret = null;
		
		boolean isWorkThread = threadScheduler.getThread().equals(Thread.currentThread());
		
		final Object[] tmpArgs = extendConstructorArgs(methodName, isConstructor, args);

		if (isWorkThread)
		{
			Object[] packagedArgs = packageArgs(tmpArgs);
			ret = callJSMethodNative(javaObjectID, methodName, returnType, isConstructor, packagedArgs);
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
							arr[0] = callJSMethodNative(javaObjectID, methodName, returnType, isCtor, packagedArgs);
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
				catch (InterruptedException e) {}
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
	private static Class<?> findClass(String className) throws ClassNotFoundException
    {
    	Class<?> clazz = dexFactory.findClass(className);
		return clazz;
    }
    
	public static void purgeAllProxies()
	{
		if (dexFactory == null)
		{
			return;
		}
		
		dexFactory.purgeAllProxies();
	}
	
	private static String readJsOptions(File appDir)
	{
		String options = "--expose_gc";
		
		File packageInfo = new File (appDir, "/app/package.json");
		
		if (packageInfo.exists())
		{
			JSONObject object;
			try
			{
				object = FileSystem.readJSONFile(packageInfo);
				if (object != null)
				{
					String opt = object.getString("jsoptions");
					if (opt != null)
					{
						options = opt;
					}
				}
			}
			catch (Exception e)
			{
				if (logger.isEnabled()) e.printStackTrace();
			}
		}
		
		return options;
	}
}