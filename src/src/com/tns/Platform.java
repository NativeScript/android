package com.tns;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
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

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.util.SparseArray;

import com.tns.bindings.ProxyGenerator;
import com.tns.internal.ExtractPolicy;
import com.tns.internal.Plugin;

public class Platform
{
	private static native void initNativeScript(String filesPath, int appJavaObjectId, boolean verboseLoggingEnabled, String packageName, String jsOptions);

	private static native void runNativeScript(String appModuleName);

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
	
	private static final NativeScriptHashMap<Object, Integer> strongJavaObjectToID = new NativeScriptHashMap<Object, Integer>();
	
	private static final NativeScriptWeakHashMap<Object, Integer> weakJavaObjectToID = new NativeScriptWeakHashMap<Object, Integer>();
	
	private static final Runtime runtime = Runtime.getRuntime();
	private static Class<?> errorActivityClass;
	
	private final static Object keyNotFoundObject = new Object();
	public final static String ApplicationAssetsPath = "app/";
	private static int currentObjectId = -1;
	
	private static ExtractPolicy extractPolicy;

	private static Thread mainThread;

	private static long lastUsedMemory = 0;
	
	private static ArrayList<Constructor<?>> ctorCache = new ArrayList<Constructor<?>>();

	private static JsDebugger jsDebugger;
	
	public static boolean IsLogEnabled = BuildConfig.DEBUG;
	
	private static DexFactory dexFactory;
	public final static String DEFAULT_LOG_TAG = "TNS.Java";
	
	public static Class<?> getErrorActivityClass(){
		return errorActivityClass;
	}
	
	public static void setErrorActivityClass(Class<?> clazz){
		errorActivityClass = clazz;
	}
	
	public static int init(String appName, File runtimeLibPath, File rootDir, File appDir, File debuggerSetupDir, ClassLoader classLoader, File dexDir, String dexThumb) throws RuntimeException
	{
		return init(null, appName, runtimeLibPath, rootDir, appDir, debuggerSetupDir, classLoader, dexDir, dexThumb);
	}
	
	public static int init(Object application, String appName, File runtimeLibPath, File rootDir, File appDir, File debuggerSetupDir, ClassLoader classLoader, File dexDir, String dexThumb) throws RuntimeException
	{
		if (initialized)
		{
			throw new RuntimeException("NativeScriptApplication already initialized");
		}
		initialized = true;
		
		loadLibrary(runtimeLibPath, "NativeScript");
		
		mainThread = Thread.currentThread();

		Platform.dexFactory = new DexFactory(classLoader, dexDir, dexThumb);

		int appJavaObjectId = -1;
		if (application != null)
		{
			if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Initializing NativeScript JAVA");
			appJavaObjectId = generateNewObjectId();
			makeInstanceStrong(application, appJavaObjectId);
			if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Initialized app instance id:" + appJavaObjectId);
		}

		try
		{
			Require.init(rootDir, appDir);
		}
		catch (IOException ex)
		{
			throw new RuntimeException("Fail to initialize Require class", ex);
		}
		String jsOptions = readJsOptions(appDir);
		Platform.initNativeScript(Require.getApplicationFilesPath(), appJavaObjectId, IsLogEnabled, appName, jsOptions);
		
		if (debuggerSetupDir != null)
		{
			jsDebugger = new JsDebugger(debuggerSetupDir);
											//also runs javaServerThread with resolved port
			int debuggerPort = jsDebugger.getDebuggerPortFromEnvironment();
		}
		
		//
		if (IsLogEnabled)
		{
			Date d = new Date();
			int pid = android.os.Process.myPid();
			File f = new File("/proc/" + pid);
			Date lastModDate = new Date(f.lastModified());
			Log.d(DEFAULT_LOG_TAG, "init time=" + (d.getTime() - lastModDate.getTime()));
		}
		//
		
		return appJavaObjectId;
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
	
	public static void enableVerboseLogging()
	{
		IsLogEnabled = true;
		ProxyGenerator.IsLogEnabled = true;
		enableVerboseLoggingNative();
	}

	public static void disableVerboseLogging()
	{
		IsLogEnabled = false;
		ProxyGenerator.IsLogEnabled = false;
		disableVerboseLoggingNative();
	}

	public static void appFail(Throwable ex, String message)
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
				passUncaughtExceptionToJsNative(e, ErrorReport.getErrorMessage(e));
			}
			
			// re-throw the exception to crash the current thread
			UncheckedThrow.Throw(e);
		}
	}
	
	public static void run()
	{
		String bootstrapPath = Require.bootstrapApp();
		runNativeScript(bootstrapPath);
	}
	
	private static Class<?> resolveClass(String fullClassName, String[] methodOverrides) throws ClassNotFoundException, IOException{
		Class<?> javaClass = ClassResolver.resolveClass(fullClassName, dexFactory, methodOverrides);
		
		return javaClass;
	}

	private static int cacheConstructor(Class<?> clazz, Object[] args) throws ClassNotFoundException, IOException
	{
		Constructor<?> ctor = MethodResolver.resolveConstructor(clazz, args);

		//TODO: Lubo: Not thread safe already.
		//TODO: Lubo: Does not check for existing items
		int ctorId = ctorCache.size();
		
		ctorCache.add(ctor);
		
		return ctorId;
	}

	private static Object createInstance(Object[] args, int objectId, int constructorId) throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException, IllegalArgumentException, InvocationTargetException, IOException
	{
		Constructor<?> ctor = ctorCache.get(constructorId);
		boolean success = MethodResolver.convertConstructorArgs(ctor, args);

		if (!success)
		{
			StringBuilder builder = new StringBuilder();
			builder.append(constructorId + "(");
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

		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "JSInstance for " + instance.getClass().toString() + " created with overrides");
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
					if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, ">>loader=" + classLoader.toString() + " " + e1.getMessage());
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
	
	private final static Comparator<Method> methodComparator = new Comparator<Method>()
	{
		public int compare(Method lhs, Method rhs)
		{
			return lhs.getName().compareTo(rhs.getName());
		}
	};

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

		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "MakeInstanceStrong (" + key + ", " + instance.getClass().toString() + ")");
	}

	private static void makeInstanceWeak(int javaObjectID, boolean keepAsWeak)
	{
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "makeInstanceWeak instance " + javaObjectID + " keepAsWeak=" + keepAsWeak);
		Object instance = strongInstances.get(javaObjectID);
		
		if (keepAsWeak) {
			weakJavaObjectToID.put(instance, Integer.valueOf(javaObjectID));
			weakInstances.put(javaObjectID, new WeakReference<Object>(instance));
		}
		
		strongInstances.delete(javaObjectID);
		strongJavaObjectToID.remove(instance);
	}
	
	private static void makeInstanceWeak(ByteBuffer buff, int length, boolean keepAsWeak)
	{
		buff.position(0);
		for (int i=0; i<length; i++)
		{
			int javaObjectId = buff.getInt();
			makeInstanceWeak(javaObjectId, keepAsWeak);
		}
	}
	
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
	
	public static Object getJavaObjectByID(int javaObjectID) throws Exception
	{
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Platform.getJavaObjectByID:" + javaObjectID);

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

	public static int getorCreateJavaObjectID(Object obj)
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
			if (IsLogEnabled) Log.e(DEFAULT_LOG_TAG, "Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getName() : "null"));
			appFail(null, "Application failed");
		}

		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getName() : "null"));

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
					value = getorCreateJavaObjectID(value);
				}

				packagedArgs[jsArgsIndex++] = typeId;

				packagedArgs[jsArgsIndex++] = value;

				packagedArgs[jsArgsIndex++] = javaClassPath;
			}
		}

		return packagedArgs;
	}

	public static String resolveMethodOverload(String className, String methodName, Object[] args) throws Exception
	{
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "resolveMethodOverload: Resolving method " + methodName + " on class " + className);
		String res = MethodResolver.resolveMethodOverload(classCache, className, methodName, args);
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "resolveMethodOverload: method found :" + res);
		if (res == null)
		{
			throw new Exception("Failed resolving method " + methodName + " on class " + className);
		}

		return res;
	}
	
	public static boolean isJavaThrowable(Object obj)
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
		
		boolean isMainThread = mainThread.equals(Thread.currentThread());
		
		final Object[] tmpArgs = extendConstructorArgs(methodName, isConstructor, args);

		if (isMainThread)
		{
			Object[] packagedArgs = packageArgs(tmpArgs);
			ret = callJSMethodNative(javaObjectID, methodName, returnType, isConstructor, packagedArgs);
		}
		else
		{
			Handler mainThreadHandler = new Handler(Looper.getMainLooper());

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

			boolean success = mainThreadHandler.post(r);

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
	
	private static Class<?> getCachedClass(String className)
	{
		Class<?> clazz = classCache.get(className);
		return clazz;
	}

    private static Class<?> findClass(String className) throws ClassNotFoundException
    {
    	Class<?> clazz = dexFactory.findClass(className);
		return clazz;
    }
    
    public static void initLogging(Context context)
    {
    	int flags;
		boolean verboseLogging = false;
		try
		{
			flags = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).applicationInfo.flags;
			verboseLogging = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
		}
		catch (NameNotFoundException e)
		{
			e.printStackTrace();
			return;
		}
		
		if (!verboseLogging)
		{
			return;
		}
		
    	String verboseLoggingProp = readSystemProperty("nativescript.verbose.logging");
    	if (verboseLoggingProp.equals("true") || verboseLoggingProp.equals("TRUE") ||
    		verboseLoggingProp.equals("yes") || verboseLoggingProp.equals("YES") ||
    		verboseLoggingProp.equals("enabled") || verboseLoggingProp.equals("ENABLED"))
    	{
    		Platform.IsLogEnabled = true;
    	}
    }
    
    
	private static String readSystemProperty(String name)
	{
		InputStreamReader in = null;
		BufferedReader reader = null;
		try
		{
			Process proc = Runtime.getRuntime().exec(new String[] { "/system/bin/getprop", name });
			in = new InputStreamReader(proc.getInputStream());
			reader = new BufferedReader(in);
			return reader.readLine();
		}
		catch (IOException e)
		{
			return null;
		}
		finally
		{
			silentClose(in);
			silentClose(reader);
		}
	}

	public static void silentClose(Closeable closeable)
	{
		if (closeable == null)
		{
			return;
		}
		try
		{
			closeable.close();
		}
		catch (IOException ignored)
		{
		}
	}
	
	static boolean runPlugin(Context context)
	{
		boolean success = false;
		String pluginClassName;
		try
		{
			ApplicationInfo ai = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
			pluginClassName = ai.metaData.getString("com.tns.internal.Plugin");
		}
		catch (Exception e)
		{
			pluginClassName = "org.nativescript.livesync.LiveSyncPlugin";
			if (IsLogEnabled) e.printStackTrace();
		}
		try
		{
			Class<?> liveSyncPluginClass = Class.forName(pluginClassName);
			Plugin p = (Plugin)liveSyncPluginClass.newInstance();
			success = p.execute(context);
		}
		catch (Exception e)
		{
			if (IsLogEnabled) e.printStackTrace();
		}
		return success;
	}
	
	public static void purgeAllProxies()
	{
		if (dexFactory == null)
		{
			return;
		}
		
		dexFactory.purgeAllProxies();
	}
	
	public static boolean isDebuggableApp(Context context)
	{
		int flags;
		try
		{
			flags = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).applicationInfo.flags;
		}
		catch (NameNotFoundException e)
		{
			flags = 0;
			e.printStackTrace();
		}

		boolean isDebuggableApp = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
		return isDebuggableApp;
	}
	
	public static String getDexThumb(Context context)
	{
		try
		{
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
			int code = packageInfo.versionCode;
			long updateTime = packageInfo.lastUpdateTime;
			return String.valueOf(updateTime) + "-" + String.valueOf(code);
		}
		catch (PackageManager.NameNotFoundException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Error while getting current proxy thumb");
			e.printStackTrace();
		}

		return null;
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
				if (IsLogEnabled) e.printStackTrace();
			}
		}
		
		return options;
	}
}