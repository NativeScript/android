package com.tns;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.InputStream;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.lang.Thread.UncaughtExceptionHandler;
import java.lang.ref.WeakReference;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.ZipEntry;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.util.SparseArray;

import com.tns.internal.ExtractPolicy;

public class Platform
{
	private static native void initNativeScript(String filesPath, int appJavaObjectId, boolean verboseLoggingEnabled, String packageName,  int debuggerPort);

	private static native void runNativeScript(String appModuleName, String appContent);

	private static native Object callJSMethodNative(int javaObjectID, String methodName, Object... packagedArgs) throws NativeScriptException;

	private static native String[] createJSInstanceNative(Object javaObject, int javaObjectID, String canonicalName, boolean createActivity, Object[] packagedCreationArgs);

	private static native int generateNewObjectId();

	private static native void enableVerboseLoggingNative();

	private static native void disableVerboseLoggingNative();
	
	private static native void adjustAmountOfExternalAllocatedMemoryNative(long changeInBytes);
	
	private static native void passUncaughtExceptionToJsNative(Throwable ex, String stackTrace);
	
	private static Context NativeScriptContext;
	
	private static final HashMap<String, Class<?>> classCache = new HashMap<String, Class<?>>();

	private static final SparseArray<Object> strongInstances = new SparseArray<Object>();
	
	private static final SparseArray<WeakReference<Object>> weakInstances = new SparseArray<WeakReference<Object>>();
	
	private static final NativeScriptHashMap<Object, Integer> strongJavaObjectToID = new NativeScriptHashMap<Object, Integer>();
	
	private static final NativeScriptWeakHashMap<Object, Integer> weakJavaObjectToID = new NativeScriptWeakHashMap<Object, Integer>();
	
	private static final Runtime runtime = Runtime.getRuntime();
	private static Class<?> errorActivityClass;
	
	private final static Object keyNotFoundObject = new Object();
	public final static String ApplicationAssetsPath = "app/";
	private static Object[] empty = new Object[0];
	private static String[] methodOverrides;
	private static int currentObjectId = -1;
	
	private static ExtractPolicy extractPolicy;

	private static Thread mainThread;

	private static long lastUsedMemory = 0;
	
	private static ArrayList<Constructor<?>> ctorCache = new ArrayList<Constructor<?>>();

	public final static String DefaultApplicationModuleName = "./bootstrap";
	
	private static JsDebugger jsDebugger;
	
	public static boolean IsLogEnabled = BuildConfig.DEBUG;
	
	public final static String DEFAULT_LOG_TAG = "TNS.Java";
	
	public static Class<?> getErrorActivityClass(){
		return errorActivityClass;
	}
	
	public static void setErrorActivityClass(Class<?> clazz){
		errorActivityClass = clazz;
	}
	
	public static int init(Context context) throws Exception
	{
		jsDebugger = new JsDebugger(context);
		mainThread = Thread.currentThread();
		if (NativeScriptContext != null)
		{
			throw new Exception("NativeScriptApplication already initialized");
		}

		Require.init(context);

		NativeScriptContext = context;

		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Initializing NativeScript JAVA");
		int appJavaObjectId = generateNewObjectId();
		makeInstanceStrong(context, appJavaObjectId);
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Initialized app instance id:" + appJavaObjectId);

		AssetExtractor.extractAssets(context, extractPolicy);
		int debuggerPort = jsDebugger.getDebuggerPortFromEnvironment();
		Platform.initNativeScript(Require.getApplicationFilesPath(), appJavaObjectId, IsLogEnabled, context.getPackageName(), debuggerPort);
		
		//
		Date d = new Date();
		int pid = android.os.Process.myPid();
		File f = new File("/proc/" + pid);
		Date lastModDate = new Date(f.lastModified());
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "init time=" + (d.getTime() - lastModDate.getTime()));
		//
		
		return appJavaObjectId;
	}
	
	static void installSecondaryDexes(Context context)
	{
		com.tns.multidex.MultiDex.install(context);
	}
	
	public static void enableVerboseLogging()
	{
		IsLogEnabled = true;
		enableVerboseLoggingNative();
	}

	public static void disableVerboseLogging()
	{
		IsLogEnabled = false;
		disableVerboseLoggingNative();
	}

	static void setDefaultUncaughtExceptionHandler(Thread.UncaughtExceptionHandler handler)
	{
		if (handler == null)
		{
			final UncaughtExceptionHandler h = Thread.getDefaultUncaughtExceptionHandler();
	
			handler = new UncaughtExceptionHandler()
			{
				@Override
				public void uncaughtException(Thread thread, Throwable ex)
				{
					if (IsLogEnabled) Log.e(DEFAULT_LOG_TAG, "Uncaught Exception Message=" + ex.getMessage());
	
					String content;
					PrintStream ps = null;
	
					try
					{
						ByteArrayOutputStream baos = new ByteArrayOutputStream();
						ps = new PrintStream(baos);
						ex.printStackTrace(ps);
	
						try
						{
							content = baos.toString("US-ASCII");
						}
						catch (UnsupportedEncodingException e)
						{
							content = e.getMessage();
						}
						
						passUncaughtExceptionToJsNative(ex, content);
					}
					finally
					{
						if (ps != null)
							ps.close();
					}
	
					if (IsLogEnabled) Log.e(DEFAULT_LOG_TAG, "Uncaught Exception Stack=" + content);
					
					final String errMsg = content;
					
					new Thread() {
						@Override
						public void run()
						{
							Intent intent = ErrorReport.getIntent(NativeScriptContext, errMsg);
							NativeScriptContext.startActivity(intent);
						}
					}.start();
	
	
					if (h != null)
					{
						h.uncaughtException(thread, ex);
					}
				}
			};
		}
		
		Thread.setDefaultUncaughtExceptionHandler(handler);
	}
	
	static void setExtractPolicy(ExtractPolicy policy)
	{
		if (policy == null)
		{
			policy = new ExtractPolicy()
			{
				@Override
				public boolean extract(String appRoot) {
					return true;
				}
				
				@Override
				public boolean shouldSkip(File outputFile, File zipFile, ZipEntry zipEnty)
				{
					boolean shouldSkip  = outputFile.exists() && (zipFile.lastModified() <= outputFile.lastModified()); //TODO: check lastModified when application is installed on sdcard with FAT32 format
					
					return shouldSkip;
				}
			};
		}
		
		extractPolicy = policy;
	}

	public static void run(String appFileName) throws Exception
	{
		String appContent = Require.getAppContent(appFileName);
		runNativeScript(appFileName, appContent);
	}

	private static int cacheConstructor(String className, Object[] args) throws ClassNotFoundException
	{
		Constructor<?> ctor = MethodResolver.resolveConstructor(className, args);
		
		int ctorId = ctorCache.size();
		
		ctorCache.add(ctor);
		
		return ctorId;
	}

	public static Object createInstance(Object[] args, String[] methodOverrides, int objectId, int constructorId) throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException, IllegalArgumentException, InvocationTargetException
	{
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Creating instance for ctorId=" + constructorId);
		
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
			Platform.methodOverrides = methodOverrides;
			instance = ctor.newInstance(args);

			makeInstanceStrong(instance, objectId);
		}
		finally
		{
			Platform.methodOverrides = null;
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
		initInstance(instance, empty);
	}

	public static void initInstance(Object instance, Object... args)
	{
		String[] methodOverrides = Platform.methodOverrides;

		if (methodOverrides == null)
		{
			methodOverrides = createJSInstance(instance, args);
		}

		if (instance instanceof NativeScriptHashCodeProvider)
		{
			NativeScriptHashCodeProvider obj = (NativeScriptHashCodeProvider) instance;
			obj.setNativeScriptOverrides(methodOverrides);
		}

		int objectId = Platform.currentObjectId;

		if (objectId != -1)
		{
			makeInstanceStrong(instance, objectId);

			Platform.currentObjectId = -1;
		}
	}

	private static String[] createJSInstance(Object instance, Object... args)
	{
		int javaObjectID = generateNewObjectId();

		makeInstanceStrong(instance, javaObjectID);

		Object[] packagedArgs = packageArgs(args);

		String[] methodOverrides = createJSInstanceNative(instance, javaObjectID, instance.getClass().getCanonicalName(), instance instanceof Activity, packagedArgs);

		if (IsLogEnabled)
		{
			Log.d(DEFAULT_LOG_TAG, "JSInstance for " + instance.getClass().toString() + " created with overrides");
			for (Object methodOverride : methodOverrides)
			{
				Log.d(DEFAULT_LOG_TAG, methodOverride.toString());
			}
		}

		return methodOverrides;
	}
	
	private static String[] getTypeMetadata(String className, int index) throws ClassNotFoundException
	{
		Class<?> clazz = classCache.get(className);
		
		if (clazz == null)
		{
			clazz = Class.forName(className);
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
	
		Method[] allMethods = clazz.getMethods();
		for (Method m: allMethods)
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
		
		Field[] allFields = clazz.getFields();
		for (Field f: allFields)
		{
			sb.append("F ");
			sb.append(f.getName());
			sb.append(" ");
			String sig = MethodResolver.getTypeSignature(f.getType());
			sb.append(sig);
			sb.append(" 0\n");
		}
		
		String ret = sb.toString();
		
		return ret;
	}

	public static int makeClassInstanceOfTypeStrong(String classPath) throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException, IllegalArgumentException, InvocationTargetException
	{
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Making Class<?> instance of " + classPath + " strong");
		Class<?> clazz = Class.forName(classPath.replace('/', '.'));
		int key = generateNewObjectId();
		makeInstanceStrong(clazz, key);
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Class<?> of " + classPath + " made strong id:" + key);
		return key;
	}
	
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
	public static Object callJSMethod(Object javaObject, String methodName, Object... args) throws NativeScriptException
	{
		return callJSMethod(javaObject, methodName, false /* isConstructor */, args);
	}
	
	public static Object callJSMethodWithDelay(Object javaObject, String methodName, long delay, Object... args) throws NativeScriptException
	{
		return callJSMethod(javaObject, methodName, false /* isConstructor */, delay, args); 
	}
	
	public static Object callJSMethod(Object javaObject, String methodName, boolean isConstructor, Object... args) throws NativeScriptException
	{
		Object ret = callJSMethod(javaObject, methodName, isConstructor, 0, args);
		
		return ret;
	}

	public static Object callJSMethod(Object javaObject, String methodName, boolean isConstructor, long delay, Object... args) throws NativeScriptException
	{
		Integer javaObjectID = getJavaObjectID(javaObject);
		if (javaObjectID == null)
		{
			if (IsLogEnabled) Log.e(DEFAULT_LOG_TAG, "Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getCanonicalName() : "null"));
			APP_FAIL("Application failed");
		}

		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "Platform.CallJSMethod: calling js method " + methodName + " with javaObjectID " + javaObjectID + " type=" + ((javaObject != null) ? javaObject.getClass().getCanonicalName() : "null"));

		Object result = dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, delay, args);

		return result;
	}

	// Packages args in format (typeID, value, null) except for objects where it
	// is
	// (typeid, javaObjectID, canonicalName)
	// if javaObject has no javaObjecID meaning javascript object does not
	// exists for this object we assign one.
	private static Object[] packageArgs(Object... args)
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

	public static void APP_FAIL(String message)
	{
		// TODO: allow app to handle fail message report here. For example
		// integrate google app reports
		// Log.d(DEFAULT_LOG_TAG, message);
		// System.exit(-1);
		
		// throw an exception to enter the Thread.setDefaultUncaughtExceptionHandler
		throw new NativeScriptException(message);
	}

	public static String resolveMethodOverload(String className, String methodName, Object[] args) throws Exception
	{
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "resolveMethodOverload: Resolving method " + methodName + " on class " + className);
		String res = MethodResolver.resolveMethodOverload(className, methodName, args);
		if (IsLogEnabled) Log.d(DEFAULT_LOG_TAG, "resolveMethodOverload: method found :" + res);
		if (res == null)
		{
			throw new Exception("Failed resolving method " + methodName + " on class " + className);
		}

		return res;
	}
	
	private static HashMap<String, String> map = null;
	
	private static String readMetadata(String name) throws Exception
	{
		if (map == null)
		{
			InputStream inputStream = NativeScriptContext.getAssets().open("metadata/metadata.txt", AssetManager.ACCESS_STREAMING);
			String mappings = FileSystem.readAll(inputStream);
			inputStream.close();
			
			String[] lines = mappings.split(System.getProperty("line.separator"));
			
			HashMap<String, String> tmpMap = new HashMap<String, String>();
			
			for (String s: lines)
			{
				String[] parts = s.split(" ");
				tmpMap.put(parts[0], parts[1]);
			}
			
			map = tmpMap;
		}
		
		String assetName = map.get(name);
		
		InputStream inputStream = NativeScriptContext.getAssets().open("metadata/" + assetName, AssetManager.ACCESS_STREAMING);
		String metadata = FileSystem.readAll(inputStream);
		inputStream.close();
		
		return metadata;
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

	private static Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, final Object[] args) throws NativeScriptException
	{
		return dispatchCallJSMethodNative(javaObjectID, methodName, isConstructor, 0, args);
	}

	private static Object dispatchCallJSMethodNative(final int javaObjectID, final String methodName, boolean isConstructor, long delay, final Object[] args) throws NativeScriptException
	{
		Object ret = null;
		boolean isMainThread = mainThread.equals(Thread.currentThread());
		
		final Object[] tmpArgs = extendConstructorArgs(methodName, isConstructor, args);

		if (isMainThread)
		{
			Object[] packagedArgs = packageArgs(tmpArgs);
			ret = callJSMethodNative(javaObjectID, methodName, packagedArgs);
		}
		else
		{
			Handler mainThreadHandler = new Handler(Looper.getMainLooper());

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
							final Object[] packagedArgs = packageArgs(tmpArgs);
							arr[0] = callJSMethodNative(javaObjectID, methodName, packagedArgs);
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
	
	public static Context getApplicationContext()
	{
		return NativeScriptContext;
	}
	
	private static Class<?> getCachedClass(String className)
	{
		Class<?> clazz = classCache.get(className);
		return clazz;
	}
}