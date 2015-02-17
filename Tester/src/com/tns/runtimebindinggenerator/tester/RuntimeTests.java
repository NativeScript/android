package com.tns.runtimebindinggenerator.tester;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;
import java.util.Random;

import org.ow2.asmdex.ApplicationWriter;

import com.proxy.Button1;
import com.proxy.TargetObject;
import com.tns.NativeScriptHashCodeProvider;
import com.tns.Platform;
import com.tns.bindings.Dump;

import dalvik.system.DexClassLoader;

import android.content.Context;
import android.test.AndroidTestCase;
import junit.framework.Assert;
import junit.framework.TestCase;
import junit.framework.TestResult;

import android.test.InstrumentationTestRunner;
import android.test.InstrumentationTestSuite;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;


public class RuntimeTests extends AndroidTestCase 
{
	
	private ApplicationWriter aw;
	private Dump dump;
	private File optimizedDexOutputPath;

	
	public RuntimeTests()
	{
	}
	
	
	@Override
	protected void setUp() throws Exception
	{
		super.setUp();
		
		dump = new Dump();
		
		aw = new ApplicationWriter();
		aw.visit();
		
		optimizedDexOutputPath =  getContext().getDir("odex", Context.MODE_PRIVATE);
		
		if (!optimizedDexOutputPath.exists())
		{
			try
			{
				optimizedDexOutputPath.createNewFile();
			}
			catch (IOException e)
			{
		
				e.printStackTrace();
			}
		}
	}
	
	@Override
	protected void runTest() throws Throwable
	{
		// TODO Auto-generated method stub
		super.runTest();
	}
	
	@Override
	protected void tearDown() throws Exception
	{
		//delete created files
		super.tearDown();
	}
	
	private void testMethodOverride(String methodName, String dexfile)
	{
		
	}
	
	private File saveProxy(byte[] proxyBytes) throws IOException
	{
		String testName = getName();
		File file = new File(getContext().getFilesDir() + "/" + testName + ".dex");
		file.createNewFile();
		FileOutputStream stream = new FileOutputStream(file);
		stream.write(proxyBytes);
		stream.flush();
		stream.close();
		return file;
	}
	
	private File saveProxy(String name, byte[] proxyBytes) throws IOException
	{
		File file = new File(getContext().getFilesDir() + "/" + name + ".dex");
		file.createNewFile();
		FileOutputStream stream = new FileOutputStream(file);
		stream.write(proxyBytes);
		stream.flush();
		stream.close();
		return file;
	}
	
	private TargetObject loadProxy(File proxyFile, TargetObject implementationObject) throws Exception
	{
		Platform.implementationObject = implementationObject;	
		
		DexClassLoader dexClassLoader = new DexClassLoader(proxyFile.getAbsolutePath(), optimizedDexOutputPath.getAbsolutePath(), null, getContext().getClassLoader());
		Class<?> clazz = dexClassLoader.loadClass("com.tns.gen.com.proxy.TargetObject-0");
		assertNotNull("Error loading proxy class com.tns.gen.com.proxy.TargetObject", clazz);
		
		Constructor<?> ctor = clazz.getConstructor();
		TargetObject proxy = (TargetObject)ctor.newInstance(); //this cast should succeed
		return proxy;
	}
	
	private <T> T loadAnyProxy(File proxyFile, T implementationObject) throws Exception
	{
		Platform.implementationObject = (Object)implementationObject;	
		
		DexClassLoader dexClassLoader = new DexClassLoader(proxyFile.getAbsolutePath(), optimizedDexOutputPath.getAbsolutePath(), null, getContext().getClassLoader());
		Class<?> clazz = dexClassLoader.loadClass("com.tns.gen." + implementationObject.getClass().getSuperclass().getCanonicalName() + "-0");
		assertNotNull("Error loading proxy class com.tns.gen.com.proxy.Button1", clazz);
		
		Constructor<?> ctor = clazz.getConstructor();
		T proxy = (T)ctor.newInstance(); //this cast should succeed
		return proxy;
	}
	
	private <T> T loadAnyProxyByClass(File proxyFile, Class<?> proxyClass) throws Exception
	{
		DexClassLoader dexClassLoader = new DexClassLoader(proxyFile.getAbsolutePath(), optimizedDexOutputPath.getAbsolutePath(), null, getContext().getClassLoader());
		Class<?> clazz = dexClassLoader.loadClass("com.tns.gen." + proxyClass.getName().replace('$', '_') + "-0");
		assertNotNull("Error loading proxy class com.tns.gen.com.proxy.Button1", clazz);
		
		if (!proxyClass.getName().contains("ViewGroup"))
		{
			Constructor<?> ctor = clazz.getConstructor();
			T proxy = (T)ctor.newInstance(); //this cast should succeed
			return proxy;
		}
		else
		{
			Constructor<?> ctor = clazz.getConstructor(android.content.Context.class);
			Context arg = getContext();
			T proxy = (T)ctor.newInstance(arg); //this cast should succeed
			return proxy;
		}
	}
	
	
	private Button1 loadButton1Proxy(File proxyFile, Button1 implementationObject) throws Exception
	{
		Platform.implementationObject = implementationObject;	
		
		DexClassLoader dexClassLoader = new DexClassLoader(proxyFile.getAbsolutePath(), optimizedDexOutputPath.getAbsolutePath(), null, getContext().getClassLoader());
		Class<?> clazz = dexClassLoader.loadClass("com.tns.gen.com.proxy.Button1-0");
		assertNotNull("Error loading proxy class com.tns.gen.com.proxy.Button1", clazz);
		
		Constructor<?> ctor = clazz.getConstructor();
		Button1 proxy = (Button1)ctor.newInstance(); //this cast should succeed
		return proxy;
	}
	
	private TargetObject.TargetInterface loadInterfaceProxy(File proxyFile, TargetObject.TargetInterface implementationObject) throws Exception
	{
		Platform.implementationObject = implementationObject;	
		
		DexClassLoader dexClassLoader = new DexClassLoader(proxyFile.getAbsolutePath(), optimizedDexOutputPath.getAbsolutePath(), null, getContext().getClassLoader());
		Class<?> clazz = dexClassLoader.loadClass("com.tns.gen.com.proxy.TargetObject_TargetInterface-0");
		assertNotNull("Error loading proxy class com.tns.gen.com.proxy.TargetObject", clazz);
		
		Constructor<?> ctor = clazz.getConstructor();
		TargetObject.TargetInterface proxy = (TargetObject.TargetInterface)ctor.newInstance(); //this cast should succeed
		return proxy;
	}
	
	public void test_When_generating_proxy_on_method_with_object_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithObjectArg(Object o)
			{
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithObjectArg(new Object());
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithObjectArg"); //this cast should succeed

		proxy.methodWithObjectArg(new Object());
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
	}
	
	public void test_When_generating_proxy_on_method_with_string_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithStringArg(String o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithStringArg("not overriden");
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals("not overriden", proxy.arg);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithStringArg"); //this cast should succeed

		proxy.methodWithStringArg("overriden");
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals("overriden", implementationObject.arg);
	}
	
	public void test_When_generating_proxy_on_method_with_int_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithIntArg(int o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithIntArg(5);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals(5, proxy.arg); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithIntArg"); //this cast should succeed

		proxy.methodWithIntArg(6);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals(6, implementationObject.arg); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_boolean_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithBooleanArg(boolean o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithBooleanArg(true);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals(true, proxy.arg); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithBooleanArg"); //this cast should succeed

		proxy.methodWithBooleanArg(true);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals(true, implementationObject.arg); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_char_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithCharArg(char o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithCharArg('x');
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals('x', proxy.arg); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithCharArg"); //this cast should succeed

		proxy.methodWithCharArg('b');
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals('b', implementationObject.arg); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_long_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithLongArg(long o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithLongArg(6L);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals(6L, proxy.arg); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithLongArg"); //this cast should succeed

		proxy.methodWithLongArg(7L);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals(7L, implementationObject.arg); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_double_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithDoubleArg(double o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithDoubleArg(4.0);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals(4.0, proxy.arg); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithDoubleArg"); //this cast should succeed

		proxy.methodWithDoubleArg(5.0);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals(5.0, implementationObject.arg); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_float_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithFloatArg(float o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithFloatArg((float)33.0);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((float)33.0, proxy.arg); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithFloatArg"); //this cast should succeed

		proxy.methodWithFloatArg((float)44.0);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((float)44.0, implementationObject.arg); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_byte_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithByteArg(byte o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithByteArg((byte)154);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((byte)154, proxy.arg); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithByteArg"); //this cast should succeed

		proxy.methodWithByteArg((byte)164);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((byte)164, implementationObject.arg); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_byte_array_argument() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithByteArrayArg(byte[] o)
			{
				arg = o;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		proxy.methodWithByteArrayArg(new byte[] { (byte)141 });
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		
		assertEquals((byte)141, ((byte[])proxy.arg)[0]); //asserting against the proxy instance. Override should not be called but proxy instance should be
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithByteArrayArg"); //this cast should succeed

		proxy.methodWithByteArrayArg(new byte[] { (byte)111 });
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((byte)111, ((byte[]) implementationObject.arg)[0]); //asserting against the implementation instance. Override should be called and implementation instance is updated
	}
	
	public void test_When_generating_proxy_on_method_with_long_and_double_arguments() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithLongAndDoubleArgs(Object arg0, long arg1, Object arg2, double arg3)
			{
				this.arg0 = arg0;
				this.arg1 = arg1 + 1;
				this.arg2 = arg2;
				this.arg3 = arg3 + 1;
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		Object first = new Object();
		Object second = new Object();
		proxy.methodWithLongAndDoubleArgs(first, 334534L, second, 345.345);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals(first, proxy.arg0);
		assertEquals(334534L, proxy.arg1);
		assertEquals(second, proxy.arg2);
		assertEquals(345.345, proxy.arg3);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithLongAndDoubleArgs"); //this cast should succeed

		proxy.methodWithLongAndDoubleArgs(first, 11222L, second, 346.3456);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals(first, implementationObject.arg0);
		assertEquals(11222L + 1L, implementationObject.arg1);
		assertEquals(second, implementationObject.arg2); 
		assertEquals(346.3456D + 1D, implementationObject.arg3);
	}
	
	
	
	//Return values tests
	
	
	
	public void test_When_generating_proxy_on_method_returning_byte() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public byte methodReturningByte(byte o)
			{
				arg = (byte)(o + 3);
				overrideCalled = true;
				return (byte)(o + 4);
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		byte result = proxy.methodReturningByte((byte)154);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((byte)154, proxy.arg);
		assertEquals((byte)154, result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningByte"); //this cast should succeed

		result = proxy.methodReturningByte((byte)164);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((byte)167, implementationObject.arg);
		assertEquals((byte)168, result);
	}
	
	public void test_When_generating_proxy_on_method_returning_char() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public char methodReturningChar(char o)
			{
				arg = 'v';
				overrideCalled = true;
				return 'c';
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		char result = proxy.methodReturningChar('j');
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals('j', proxy.arg);
		assertEquals('j', result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningChar"); //this cast should succeed

		result = proxy.methodReturningChar('b');
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals('v', implementationObject.arg);
		assertEquals('c', result);
	}
	
	public void test_When_generating_proxy_on_method_returning_short() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public short methodReturningShort(short o)
			{
				arg = (short)(o + 3);
				overrideCalled = true;
				return (short)(o + 4);
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		short result = proxy.methodReturningShort((short)154);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((short)154, proxy.arg);
		assertEquals((short)154, result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningShort"); //this cast should succeed

		result = proxy.methodReturningShort((short)164);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((short)167, implementationObject.arg);
		assertEquals((short)168, result);
	}
	
	public void test_When_generating_proxy_on_method_returning_int() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public int methodReturningInt(int o)
			{
				arg = (int)(o + 3);
				overrideCalled = true;
				return (int)(o + 4);
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		int result = proxy.methodReturningInt((int)154);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((int)154, proxy.arg);
		assertEquals((int)154, result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningInt"); //this cast should succeed

		result = proxy.methodReturningInt((int)164);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((int)167, implementationObject.arg);
		assertEquals((int)168, result);
	}
	
	public void test_When_generating_proxy_on_method_returning_float() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public float methodReturningFloat(float o)
			{
				arg = (float)(o + 3);
				overrideCalled = true;
				return (float)(o + 4);
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		float result = proxy.methodReturningFloat((float)154);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((float)154, proxy.arg);
		assertEquals((float)154, result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningFloat"); //this cast should succeed

		result = proxy.methodReturningFloat((float)164);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((float)167, implementationObject.arg);
		assertEquals((float)168, result);
	}
	
	public void test_When_generating_proxy_on_method_returning_double() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public double methodReturningDouble(double o)
			{
				arg = (double)(o + 3);
				overrideCalled = true;
				return (double)(o + 4);
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		double result = proxy.methodReturningDouble((double)154);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((double)154, proxy.arg);
		assertEquals((double)154, result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningDouble"); //this cast should succeed

		result = proxy.methodReturningDouble((float)164);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((double)167, implementationObject.arg);
		assertEquals((double)168, result);
	}
	
	public void test_When_generating_proxy_on_method_returning_long() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public long methodReturningLong(long o)
			{
				arg = (long)(o + 3);
				overrideCalled = true;
				return (long)(o + 4);
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		long result = proxy.methodReturningLong((long)154);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals((long)154, proxy.arg);
		assertEquals((long)154, result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningLong"); //this cast should succeed

		result = proxy.methodReturningLong((long)164);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals((long)167, implementationObject.arg);
		assertEquals((long)168, result);
	}
	
	public void test_When_generating_proxy_on_method_returning_String() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		
		class ImplementationObject extends TargetObject
		{
			@Override
			public String methodReturningString(String s)
			{
				arg = s + " indeed";
				overrideCalled = true;
				return s + " indeed";
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		String result = proxy.methodReturningString("return string");
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals("return string", proxy.arg);
		assertEquals("return string", result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningString"); //this cast should succeed

		result = proxy.methodReturningString("override string");
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals("override string indeed", implementationObject.arg);
		assertEquals("override string indeed", result);
	}
	
	public void test_When_generating_proxy_on_method_returning_Object() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		final Object overrideResult = new Object();
		class ImplementationObject extends TargetObject
		{
			@Override
			public Object methodReturningObject(Object s)
			{
				arg = s;
				overrideCalled = true;
				return overrideResult;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		Object first = new Object();
		Object result = proxy.methodReturningObject(first);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals(first, proxy.arg);
		assertEquals(first, result);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodReturningObject"); //this cast should succeed

		Object second = new Object();
		result = proxy.methodReturningObject(second);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals(second, implementationObject.arg);
		assertEquals(overrideResult, result);
	}
	
	public void test_When_generating_proxy_on_an_interface() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.TargetInterface.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		final Object overrideResult = new Object();
		final boolean overrideCalled = false;
		
		class ImplementationObject implements TargetObject.TargetInterface
		{
			public boolean overrideCalled;
			public Object arg;

			@Override
			public void methodWithObjectArg(Object a)
			{
				arg = a;
				overrideCalled = true;
			}
		}
		
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject.TargetInterface proxy = loadInterfaceProxy(proxyFile, implementationObject);
		
		Object first = new Object();
		proxy.methodWithObjectArg(first);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals("Override should be called", first, implementationObject.arg);
		
	}
	
	public void test_When_generating_proxy_on_an_button1_class() throws Throwable
	{
		dump.generateProxy(aw, com.proxy.Button1.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		final Object overrideResult = new Object();
		final boolean overrideCalled = false;
		
		class Button1Implementation extends  com.proxy.Button1
		{
			public boolean overrideCalled;
			public Object arg;
		}
		
		Button1Implementation implementationObject = new Button1Implementation();
		Button1 proxy = loadButton1Proxy(proxyFile, implementationObject);
		
		//Object first = new Object();
		//proxy.methodWithObjectArg(first);
		//assertTrue("Override should be called", implementationObject.overrideCalled == true);
		//assertEquals("Override should be called", first, implementationObject.arg);
		
	}
	
	public void test_When_generating_proxy_on_an_android_widget_BaseAdapter_class() throws Throwable
	{
		dump.generateProxy(aw, android.widget.BaseAdapter.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		File proxyFile = saveProxy(generatedBytes);
		final Object overrideResult = new Object();
		final boolean overrideCalled = false;
		
		class BaseAdapterImpl extends android.widget.BaseAdapter
		{
			public boolean overrideCalled;
			public Object arg;
			
			@Override
			public int getCount()
			{
				return 0;
			}
			@Override
			public Object getItem(int arg0)
			{
				return null;
			}
			@Override
			public long getItemId(int arg0)
			{
				return 0;
			}
			@Override
			public View getView(int arg0, View arg1, ViewGroup arg2)
			{
				return null;
			}
		}
		
		BaseAdapterImpl implementationObject = new BaseAdapterImpl();
		android.widget.BaseAdapter proxy = loadAnyProxy(proxyFile, implementationObject);
		
		//Object first = new Object();
		//proxy.methodWithObjectArg(first);
		//assertTrue("Override should be called", implementationObject.overrideCalled == true);
		//assertEquals("Override should be called", first, implementationObject.arg);
		
	}
	
	public void test_When_generating_proxy_on_method_with_all_types_of_args() throws Throwable
	{
		dump.generateProxy(aw, TargetObject.class);
		aw.visitEnd();
		byte[] generatedBytes = aw.toByteArray();
		
		
		File proxyFile = saveProxy(generatedBytes);
		final Object overrideResult = new Object();
		class ImplementationObject extends TargetObject
		{
			@Override
			public void methodWithAllTypesOfArgs(
					Object arg0, 
					int arg1, 
					Integer arg2, 
					char arg3, 
					Character arg4,
					byte arg5,
					Byte arg6,
					short arg7,
					Short arg8,
					boolean arg9,
					Boolean arg10,
					long arg11,
					Long arg12,
					float arg13,
					Float arg14,
					double arg15,
					Double arg16,
					String arg17)
			{
				this.arg0 = overrideResult;
				this.arg1 = arg1 + 1;
				this.arg2 = arg2 + 1;
				this.arg3 = 'b';
				this.arg4 = 'z';
				this.arg5 = arg5 + 1;
				this.arg6 = arg6 + 1;
				this.arg7 = arg7 + 1;
				this.arg8 = arg8 + 1;
				this.arg9 = true;
				this.arg10 = true;
				this.arg11 = arg11 + 1;
				this.arg12 = arg12 + 1;
				this.arg13 = arg13 + 1;
				this.arg14 = arg14 + 1;
				this.arg15 = arg15 + 1;
				this.arg16 = arg16 + 1;
				this.arg17 = "overriden string indeed";
				
				overrideCalled = true;
			}
		}
		ImplementationObject implementationObject = new ImplementationObject();
		TargetObject proxy = loadProxy(proxyFile, implementationObject);
		
		Object param0 = new Object(); 
		int param1 = 42; 
		Integer param2 = 43;
		char param3 = 'p';
		Character param4 = 'j';
		byte param5 = 3;
		Byte param6 = 4;
		short param7 = 60;
		Short param8 = 61;
		boolean param9 = true;
		Boolean param10 = true;
		long param11 = 123456L;
		Long param12 = 1234567L;
		float param13 = 45.0F;
		Float param14 = 46.0F;
		double param15 = 456.45;
		Double param16 = 456.47;
		String param17 = "the string";
		
		
		proxy.methodWithAllTypesOfArgs(param0, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10, param11, param12, param13, param14, param15, param16, param17);
		assertTrue("Override should NOT be called", implementationObject.overrideCalled == false);
		assertEquals(param0, proxy.arg0);
		assertEquals(param1, proxy.arg1);
		assertEquals(param2, proxy.arg2);
		assertEquals(param3, proxy.arg3);
		assertEquals(param4, proxy.arg4);
		assertEquals(param5, proxy.arg5);
		assertEquals(param6, proxy.arg6);
		assertEquals(param7, proxy.arg7);
		assertEquals(param8, proxy.arg8);
		assertEquals(param9, proxy.arg9);
		assertEquals(param10, proxy.arg10);
		assertEquals(param11, proxy.arg11);
		assertEquals(param12, proxy.arg12);
		assertEquals(param13, proxy.arg13);
		assertEquals(param14, proxy.arg14);
		assertEquals(param15, proxy.arg15);
		assertEquals(param16, proxy.arg16);
		assertEquals(param17, proxy.arg17);
		
		((NativeScriptHashCodeProvider)proxy).setNativeScriptOverride("methodWithAllTypesOfArgs"); //this cast should succeed

		param0 = new Object(); 
		param1 = 43; 
		param2 = 44;
		param3 = 'a';
		param4 = 'c';
		param5 = 4;
		param6 = 5;
		param7 = 61;
		param8 = 62;
		param9 = true;
		param10 = true;
		param11 = 1123456L;
		param12 = 11234567L;
		param13 = 46.0F;
		param14 = 47.0F;
		param15 = 1456.45;
		param16 = 1456.47;
		param17 = "overriden string";
		
		proxy.methodWithAllTypesOfArgs(param0, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10, param11, param12, param13, param14, param15, param16, param17);
		assertTrue("Override should be called", implementationObject.overrideCalled == true);
		assertEquals(overrideResult, implementationObject.arg0);
		assertEquals(param1 + 1, implementationObject.arg1);
		assertEquals(param2 + 1, implementationObject.arg2);
		assertEquals('b', implementationObject.arg3);
		assertEquals('z', implementationObject.arg4);
		assertEquals(param5 + 1, implementationObject.arg5);
		assertEquals(param6 + 1, implementationObject.arg6);
		assertEquals(param7 + 1, implementationObject.arg7);
		assertEquals(param8 + 1, implementationObject.arg8);
		assertEquals(param9, implementationObject.arg9);
		assertEquals(param10, implementationObject.arg10);
		assertEquals(param11 + 1, implementationObject.arg11);
		assertEquals(param12 +1, implementationObject.arg12);
		assertEquals(param13 + 1, implementationObject.arg13);
		assertEquals(param14 + 1, implementationObject.arg14);
		assertEquals(param15 + 1, implementationObject.arg15);
		assertEquals(param16 + 1, implementationObject.arg16);
		assertEquals("overriden string indeed", implementationObject.arg17);
	}
	
//	public void test_When_generating_proxy_on_method_with_throws_statement() throws Throwable
//	{
//		assertTrue("Not implemented", false);
//	}
	
	public void test_When_generating_prox_for_every_cuttenessio_class() throws Throwable
	{
		 //Package[] packages = android.app.Application.class.getPackage().getPackages();
		//android.app.Application.class.getPackage().getName();
		
		//List<Class<?>> classes = ClasspathScanner.run(getContext().getPackageCodePath()); //ClassFinder.getClassesForPackage(android.app.Application.class.getPackage()); //ClassFinder.find(android.app.Application.class.getPackage().getName());
		String[] classes = new String[] {
				"android.view.ViewGroup",
				
				
				"android.accessibilityservice.AccessibilityServiceInfo",
				"org.xmlpull.v1.XmlSerializer",
				"android.app.ActionBar$TabListener",
				"android.app.Application$ActivityLifecycleCallbacks",
				"android.app.Fragment",
				"android.support.v4.view.PagerAdapter",
				"android.support.v4.view.ViewPager$OnPageChangeListener",
				
				"android.widget.AbsListView$OnScrollListener",
				"android.widget.AdapterView$OnItemClickListener",
				"android.widget.BaseAdapter",
		};
		
		Log.d("Binding.Gen.Test", "Starting generating_prox_for_every_cuttenessio_class");
		long startTime = System.nanoTime();
		ArrayList<File> files = new ArrayList<File>();
		ArrayList<Class<?>> proxyClassNames = new ArrayList<Class<?>>();
		
		for (String clazz : classes)
		{
			ApplicationWriter aw = new ApplicationWriter();
			aw.visit();
			Dump dump = new Dump();
			Class<?> classForName = Class.forName(clazz);
			//Log.d("Binding.Gen.Test", "Generating Class " +  classForName.getName());
			dump.generateProxy(aw, classForName);
			aw.visitEnd();
			byte[] generatedBytes = aw.toByteArray();
			File proxyFile = saveProxy(classForName.getName().replace('$', '_'), generatedBytes);
			files.add(proxyFile);
			proxyClassNames.add(classForName);
		}

		long stopTime = System.nanoTime();
		Log.d("Binding.Gen.Test", "Finished generating_prox_for_every_cuttenessio_class took: " + (stopTime - startTime) / 1000000.0 + "ms");
		
		System.gc();
		System.gc();
		System.gc();
		
		Log.d("Binding.Gen.Test", "Starting loading proxies of generating_prox_for_every_cuttenessio_class");
		startTime = System.nanoTime();
		for (int i = 0; i < files.size(); i++)
		{
			File proxyFile = files.get(i);
			Class<?> classForName = proxyClassNames.get(i);
			//Log.d("Binding.Gen.Test", classForName.getName());
			Object proxy = loadAnyProxyByClass(proxyFile, classForName);
		}
		stopTime = System.nanoTime();
		Log.d("Binding.Gen.Test", "Finished loading proxies of generating_prox_for_every_cuttenessio_class took: " + (stopTime - startTime) / 1000000.0 + "ms");
	}
	
	public void disabled_test_When_generating_prox_for_every_cuttenessio_class_with_exact_methods() throws Throwable
	{
		 //Package[] packages = android.app.Application.class.getPackage().getPackages();
		//android.app.Application.class.getPackage().getName();
		
		//List<Class<?>> classes = ClasspathScanner.run(getContext().getPackageCodePath()); //ClassFinder.getClassesForPackage(android.app.Application.class.getPackage()); //ClassFinder.find(android.app.Application.class.getPackage().getName());
		
		HashMap<String, String[]> exactMethodOverrides = new HashMap<String, String[]>();
		exactMethodOverrides.put("android.app.Application$ActivityLifecycleCallbacks", new String[] 
		{
				"onActivitySaveInstanceState",
				"onActivityResumed",
				"onActivityStarted",
				"onActivityStopped",
				"onActivityCreated",
				"onActivityPaused",
				"onActivityDestroyed",
		});
		
		exactMethodOverrides.put("android.view.ViewGroup", new String[] 
		{
				"onMeasure",
				"onLayout",
		});
		
		exactMethodOverrides.put("android.support.v4.view.PagerAdapter", new String[] 
		{
				"getPageTitle",
				"instantiateItem",
				"destroyItem",
				"isViewFromObject",
				"getCount",
		});
		
		exactMethodOverrides.put("android.app.ActionBar$TabListener", new String[] 
		{
				"onTabUnselected",
				"onTabSelected",
				"onTabReselected",
		});
		
		exactMethodOverrides.put("android.support.v4.view.ViewPager$OnPageChangeListener", new String[] 
		{
				"onPageScrolled",
				"onPageScrollStateChanged",
				"onPageSelected",
		});
		
		exactMethodOverrides.put("android.widget.BaseAdapter", new String[] 
		{
				"getView",
				"getItem",
				"getItemId",
				"getCount",
		});
		
		exactMethodOverrides.put("android.widget.AbsListView$OnScrollListener", new String[] 
		{
				"onScrollStateChanged",
				"onScroll"
		});
		
		exactMethodOverrides.put("android.widget.AdapterView$OnItemClickListener", new String[] 
		{
				"onItemClick"
		});
		
		exactMethodOverrides.put("android.app.Fragment", new String[] 
		{
				"onPause",
				"onCreateView",
				"onResume",
				"onCreate",
				"onActivityCreated",
				"onDestroyView",
				"onStop",
				"onDestroy",
				"onSaveInstanceState",
				"onStart",
				"onViewStateRestored",
				"onDetach",
				"onAttach",
		});

		Log.d("Binding.Gen.Test", "Starting test_When_generating_prox_for_every_cuttenessio_class_with_exact_methods");
		long startTime = System.nanoTime();
		ArrayList<File> files = new ArrayList<File>();
    	ArrayList<Class<?>> proxyClassNames = new ArrayList<Class<?>>();
		
		for (Entry<String, String[]> classWithOverrides : exactMethodOverrides.entrySet())
		{
			ApplicationWriter aw = new ApplicationWriter();
			aw.visit();
			Dump dump = new Dump();
			Class<?> classForName = Class.forName(classWithOverrides.getKey());
			//Log.d("Binding.Gen.Test", "Generating Class " +  classForName.getName());
			dump.generateProxy(aw, classForName, classWithOverrides.getValue(), 0);
			aw.visitEnd();
			byte[] generatedBytes = aw.toByteArray();
			File proxyFile = saveProxy(classForName.getName().replace('$', '_'), generatedBytes);
		
			files.add(proxyFile);
			proxyClassNames.add(classForName);
		}
		
		long stopTime = System.nanoTime();
		Log.d("Binding.Gen.Test", "Finished test_When_generating_prox_for_every_cuttenessio_class_with_exact_methods took: " + (stopTime - startTime) / 1000000.0 + "ms");


		System.gc();
		System.gc();
		System.gc();
		
		
		
		Log.d("Binding.Gen.Test", "Starting loading proxies of test_When_generating_prox_for_every_cuttenessio_class_with_exact_methods");
		startTime = System.nanoTime();
		for (int i = 0; i < files.size(); i++)
		{
			File proxyFile = files.get(i);
			Class<?> classForName = proxyClassNames.get(i);
			//Log.d("Binding.Gen.Test", classForName.getName());
			Object proxy = loadAnyProxyByClass(proxyFile, classForName);
		}
		stopTime = System.nanoTime();
		Log.d("Binding.Gen.Test", "Finished loading proxies of test_When_generating_prox_for_every_cuttenessio_class_with_exact_methods took: " + (stopTime - startTime) / 1000000.0 + "ms");
	}
	
	public void disabled_test_When_generating_prox_for_every_class() throws Throwable
	{
		 //Package[] packages = android.app.Application.class.getPackage().getPackages();
		//android.app.Application.class.getPackage().getName();
		
		//List<Class<?>> classes = ClasspathScanner.run(getContext().getPackageCodePath()); //ClassFinder.getClassesForPackage(android.app.Application.class.getPackage()); //ClassFinder.find(android.app.Application.class.getPackage().getName());
		String[] classes = new String[] {
				"android.accessibilityservice.AccessibilityServiceInfo",
				"android.accessibilityservice.AccessibilityService",
				"android.accounts.AbstractAccountAuthenticator",
				"android.accounts.Account",
				"android.accounts.AccountAuthenticatorActivity",
				"android.accounts.AccountAuthenticatorResponse",
				"android.accounts.AccountManagerCallback",
				"android.accounts.AccountManagerFuture",
				"android.accounts.AccountsException",
				"android.accounts.AuthenticatorDescription",
				"android.accounts.AuthenticatorException",
				"android.accounts.NetworkErrorException",
				"android.accounts.OnAccountsUpdateListener",
				"android.accounts.OperationCanceledException",
				"android.animation.Animator",
				"android.animation.AnimatorInflater",
				"android.animation.AnimatorListenerAdapter",
				"android.animation.ArgbEvaluator",
				"android.animation.FloatEvaluator",
				"android.animation.IntEvaluator",
				"android.animation.Keyframe",
				"android.animation.LayoutTransition",
				"android.animation.TimeAnimator",
				"android.animation.TimeInterpolator",
				"android.animation.TypeEvaluator",
				"android.animation.ValueAnimator",
				"android.annotation.SuppressLint",
				"android.annotation.TargetApi",
				"android.app.ActionBar",
				"android.app.Activity",
				"android.app.ActivityGroup",
				"android.app.admin.DeviceAdminReceiver",
				"android.app.AliasActivity",
				"android.app.Application",
				"android.app.ApplicationErrorReport",
				"android.app.backup.BackupAgent",
				"android.app.backup.BackupAgentHelper",
				"android.app.backup.BackupHelper",
				"android.app.backup.BackupManager",
				"android.app.backup.FileBackupHelper",
				"android.app.backup.RestoreObserver",
				"android.app.backup.SharedPreferencesBackupHelper",
				"android.app.DatePickerDialog",
				"android.app.Dialog",
				"android.app.DialogFragment",
				"android.app.ExpandableListActivity",
				"android.app.Fragment",
				"android.app.FragmentBreadCrumbs",
				"android.app.FragmentManager",
				"android.app.FragmentTransaction",
				"android.app.Instrumentation",
				"android.app.IntentService",
				"android.app.LauncherActivity",
				"android.app.ListActivity",
				"android.app.ListFragment",
				"android.app.LoaderManager",
				"android.app.LocalActivityManager",
				"android.app.MediaRouteActionProvider",
				"android.app.MediaRouteButton",
				"android.app.NativeActivity",
				"android.app.Notification",
				"android.app.PendingIntent",
				"android.app.Presentation",
				"android.app.ProgressDialog",
				"android.app.Service",
				"android.app.TabActivity",
				"android.app.TimePickerDialog",
				"android.appwidget.AppWidgetHost",
				"android.appwidget.AppWidgetHostView",
				"android.appwidget.AppWidgetProvider",
				"android.appwidget.AppWidgetProviderInfo",
				"android.bluetooth.BluetoothClass",
				"android.bluetooth.BluetoothHealthCallback",
				"android.bluetooth.BluetoothProfile",
				"android.content.AbstractThreadedSyncAdapter",
				"android.content.ActivityNotFoundException",
				"android.content.AsyncQueryHandler",
				"android.content.AsyncTaskLoader",
				"android.content.BroadcastReceiver",
				"android.content.ClipData",
				"android.content.ClipDescription",
				"android.content.ComponentCallbacks",
				"android.content.ComponentCallbacks2",
				"android.content.ContentProvider",
				"android.content.ContentProviderResult",
				"android.content.ContentQueryMap",
				"android.content.ContentResolver",
				"android.content.ContentUris",
				"android.content.Context",
				"android.content.ContextWrapper",
				"android.content.CursorLoader",
				"android.content.DialogInterface",
				"android.content.Entity",
				"android.content.EntityIterator",
				"android.content.Intent",
				"android.content.IntentFilter",
				"android.content.Loader",
				"android.content.MutableContextWrapper",
				"android.content.OperationApplicationException",
				"android.content.PeriodicSync",
				"android.content.pm.ActivityInfo",
				"android.content.pm.ApplicationInfo",
				"android.content.pm.ComponentInfo",
				"android.content.pm.ConfigurationInfo",
				"android.content.pm.FeatureInfo",
				"android.content.pm.InstrumentationInfo",
				"android.content.pm.LabeledIntent",
				"android.content.pm.PackageInfo",
				"android.content.pm.PackageItemInfo",
				"android.content.pm.PackageManager",
				"android.content.pm.PackageStats",
				"android.content.pm.PathPermission",
				"android.content.pm.PermissionGroupInfo",
				"android.content.pm.PermissionInfo",
				"android.content.pm.ResolveInfo",
				"android.content.pm.ServiceInfo",
				"android.content.pm.Signature",
				"android.content.ReceiverCallNotAllowedException",
				"android.content.res.AssetFileDescriptor",
				"android.content.res.ColorStateList",
				"android.content.res.Resources",
				"android.content.res.XmlResourceParser",
				"android.content.SearchRecentSuggestionsProvider",
				"android.content.ServiceConnection",
				"android.content.SharedPreferences",
				"android.content.SyncAdapterType",
				"android.content.SyncStats",
				"android.content.SyncStatusObserver",
				"android.content.UriMatcher",
				"android.database.AbstractCursor",
				"android.database.AbstractWindowedCursor",
				"android.database.ContentObservable",
				"android.database.ContentObserver",
				"android.database.CrossProcessCursor",
				"android.database.CrossProcessCursorWrapper",
				"android.database.Cursor",
				"android.database.CursorIndexOutOfBoundsException",
				"android.database.CursorWindow",
				"android.database.CursorWrapper",
				"android.database.DatabaseErrorHandler",
				"android.database.DatabaseUtils",
				"android.database.DataSetObservable",
				"android.database.DataSetObserver",
				"android.database.MatrixCursor",
				"android.database.MergeCursor",
				"android.database.Observable",
				"android.database.SQLException",
				"android.database.sqlite.SQLiteAbortException",
				"android.database.sqlite.SQLiteAccessPermException",
				"android.database.sqlite.SQLiteBindOrColumnIndexOutOfRangeException",
				"android.database.sqlite.SQLiteBlobTooBigException",
				"android.database.sqlite.SQLiteCantOpenDatabaseException",
				"android.database.sqlite.SQLiteClosable",
				"android.database.sqlite.SQLiteConstraintException",
				"android.database.sqlite.SQLiteCursor",
				"android.database.sqlite.SQLiteCursorDriver",
				"android.database.sqlite.SQLiteDatabase",
				"android.database.sqlite.SQLiteDatabaseCorruptException",
				"android.database.sqlite.SQLiteDatabaseLockedException",
				"android.database.sqlite.SQLiteDatatypeMismatchException",
				"android.database.sqlite.SQLiteDiskIOException",
				"android.database.sqlite.SQLiteDoneException",
				"android.database.sqlite.SQLiteException",
				"android.database.sqlite.SQLiteFullException",
				"android.database.sqlite.SQLiteMisuseException",
				"android.database.sqlite.SQLiteOpenHelper",
				"android.database.sqlite.SQLiteOutOfMemoryException",
				"android.database.sqlite.SQLiteQueryBuilder",
				"android.database.sqlite.SQLiteReadOnlyDatabaseException",
				"android.database.sqlite.SQLiteTableLockedException",
				"android.database.sqlite.SQLiteTransactionListener",
				"android.database.StaleDataException",
				"android.drm.DrmConvertedStatus",
				"android.drm.DrmErrorEvent",
				"android.drm.DrmInfo",
				"android.drm.DrmInfoEvent",
				"android.drm.DrmInfoRequest",
				"android.drm.DrmInfoStatus",
				"android.drm.DrmManagerClient",
				"android.drm.DrmRights",
				"android.drm.DrmStore",
				"android.drm.DrmSupportInfo",
				"android.drm.DrmUtils",
				"android.gesture.Gesture",
				"android.gesture.GestureOverlayView",
				"android.gesture.GesturePoint",
				"android.gesture.GestureStore",
				"android.gesture.GestureStroke",
				"android.graphics.AvoidXfermode",
				"android.graphics.BitmapFactory",
				"android.graphics.BitmapShader",
				"android.graphics.BlurMaskFilter",
				"android.graphics.Camera",
				"android.graphics.Canvas",
				"android.graphics.Color",
				"android.graphics.ColorFilter",
				"android.graphics.ColorMatrix",
				"android.graphics.ColorMatrixColorFilter",
				"android.graphics.ComposePathEffect",
				"android.graphics.ComposeShader",
				"android.graphics.CornerPathEffect",
				"android.graphics.DashPathEffect",
				"android.graphics.DiscretePathEffect",
				"android.graphics.drawable.Animatable",
				"android.graphics.drawable.AnimationDrawable",
				"android.graphics.drawable.BitmapDrawable",
				"android.graphics.drawable.ClipDrawable",
				"android.graphics.drawable.ColorDrawable",
				"android.graphics.drawable.Drawable",
				"android.graphics.drawable.DrawableContainer",
				"android.graphics.drawable.GradientDrawable",
				"android.graphics.drawable.InsetDrawable",
				"android.graphics.drawable.LayerDrawable",
				"android.graphics.drawable.LevelListDrawable",
				"android.graphics.drawable.NinePatchDrawable",
				"android.graphics.drawable.PaintDrawable",
				"android.graphics.drawable.PictureDrawable",
				"android.graphics.drawable.RotateDrawable",
				"android.graphics.drawable.ScaleDrawable",
				"android.graphics.drawable.ShapeDrawable",
				"android.graphics.drawable.shapes.ArcShape",
				"android.graphics.drawable.shapes.OvalShape",
				"android.graphics.drawable.shapes.PathShape",
				"android.graphics.drawable.shapes.RectShape",
				"android.graphics.drawable.shapes.RoundRectShape",
				"android.graphics.drawable.shapes.Shape",
				"android.graphics.drawable.StateListDrawable",
				"android.graphics.drawable.TransitionDrawable",
				"android.graphics.DrawFilter",
				"android.graphics.EmbossMaskFilter",
				"android.graphics.ImageFormat",
				"android.graphics.Interpolator",
				"android.graphics.LayerRasterizer",
				"android.graphics.LightingColorFilter",
				"android.graphics.LinearGradient",
				"android.graphics.MaskFilter",
				"android.graphics.Matrix",
				"android.graphics.NinePatch",
				"android.graphics.Paint",
				"android.graphics.PaintFlagsDrawFilter",
				"android.graphics.Path",
				"android.graphics.PathDashPathEffect",
				"android.graphics.PathEffect",
				"android.graphics.PathMeasure",
				"android.graphics.Picture",
				"android.graphics.PixelFormat",
				"android.graphics.PixelXorXfermode",
				"android.graphics.Point",
				"android.graphics.PointF",
				"android.graphics.PorterDuff",
				"android.graphics.PorterDuffColorFilter",
				"android.graphics.PorterDuffXfermode",
				"android.graphics.RadialGradient",
				"android.graphics.Rasterizer",
				"android.graphics.RectF",
				"android.graphics.Region",
				"android.graphics.RegionIterator",
				"android.graphics.Shader",
				"android.graphics.SumPathEffect",
				"android.graphics.SurfaceTexture",
				"android.graphics.SweepGradient",
				"android.graphics.Xfermode",
				"android.graphics.YuvImage",
				"android.hardware.display.DisplayManager",
				"android.hardware.GeomagneticField",
				"android.hardware.input.InputManager",
				"android.hardware.SensorEventListener",
				"android.hardware.SensorListener",
				"android.hardware.usb.UsbRequest",
				"android.inputmethodservice.AbstractInputMethodService",
				"android.inputmethodservice.ExtractEditText",
				"android.inputmethodservice.InputMethodService",
				"android.inputmethodservice.Keyboard",
				"android.inputmethodservice.KeyboardView",
				"android.location.Address",
				"android.location.Criteria",
				"android.location.GpsStatus",
				"android.location.Location",
				"android.location.LocationListener",
				"android.Manifest",
				"android.media.AsyncPlayer",
				"android.media.AudioFormat",
				"android.media.audiofx.BassBoost",
				"android.media.audiofx.EnvironmentalReverb",
				"android.media.audiofx.Equalizer",
				"android.media.audiofx.PresetReverb",
				"android.media.audiofx.Virtualizer",
				"android.media.audiofx.Visualizer",
				"android.media.AudioRecord",
				"android.media.AudioTrack",
				"android.media.CameraProfile",
				"android.media.effect.Effect",
				"android.media.effect.EffectUpdateListener",
				"android.media.ExifInterface",
				"android.media.FaceDetector",
				"android.media.MediaActionSound",
				"android.media.MediaCodec",
				"android.media.MediaCodecInfo",
				"android.media.MediaMetadataRetriever",
				"android.media.MediaPlayer",
				"android.media.MediaRecorder",
				"android.media.MediaScannerConnection",
				"android.media.RemoteControlClient",
				"android.media.RingtoneManager",
				"android.media.SoundPool",
				"android.media.ThumbnailUtils",
				"android.media.ToneGenerator",
				"android.net.Credentials",
				"android.net.DhcpInfo",
				"android.net.http.SslCertificate",
				"android.net.http.SslError",
				"android.net.http.X509TrustManagerExtensions",
				"android.net.LocalServerSocket",
				"android.net.LocalSocket",
				"android.net.LocalSocketAddress",
				"android.net.nsd.NsdManager",
				"android.net.rtp.AudioGroup",
				"android.net.rtp.AudioStream",
				"android.net.sip.SipAudioCall",
				"android.net.sip.SipException",
				"android.net.sip.SipRegistrationListener",
				"android.net.sip.SipSession",
				"android.net.SSLCertificateSocketFactory",
				"android.net.TrafficStats",
				"android.net.UrlQuerySanitizer",
				"android.net.VpnService",
				"android.net.wifi.p2p.WifiP2pConfig",
				"android.net.wifi.p2p.WifiP2pDevice",
				"android.net.wifi.p2p.WifiP2pDeviceList",
				"android.net.wifi.p2p.WifiP2pGroup",
				"android.net.wifi.p2p.WifiP2pInfo",
				"android.net.wifi.WifiConfiguration",
				"android.net.wifi.WpsInfo",
				"android.nfc.FormatException",
				"android.nfc.NfcAdapter",
				"android.nfc.TagLostException",
				"android.nfc.tech.TagTechnology",
				"android.opengl.EGL14",
				"android.opengl.ETC1",
				"android.opengl.ETC1Util",
				"android.opengl.GLDebugHelper",
				"android.opengl.GLES10",
				"android.opengl.GLES10Ext",
				"android.opengl.GLES11",
				"android.opengl.GLES11Ext",
				"android.opengl.GLES20",
				"android.opengl.GLException",
				"android.opengl.GLSurfaceView",
				"android.opengl.GLU",
				"android.opengl.Matrix",
				"android.opengl.Visibility",
				"android.os.AsyncTask",
				"android.os.BadParcelableException",
				"android.os.BatteryManager",
				"android.os.Binder",
				"android.os.Build",
				"android.os.CancellationSignal",
				"android.os.ConditionVariable",
				"android.os.CountDownTimer",
				"android.os.DeadObjectException",
				"android.os.Debug",
				"android.os.Environment",
				"android.os.FileObserver",
				"android.os.Handler",
				"android.os.HandlerThread",
				"android.os.IBinder",
				"android.os.IInterface",
				"android.os.MemoryFile",
				"android.os.NetworkOnMainThreadException",
				"android.os.OperationCanceledException",
				"android.os.Parcelable",
				"android.os.ParcelFileDescriptor",
				"android.os.ParcelFormatException",
				"android.os.PatternMatcher",
				"android.os.Process",
				"android.os.RecoverySystem",
				"android.os.RemoteCallbackList",
				"android.os.RemoteException",
				"android.os.ResultReceiver",
				"android.os.StatFs",
				"android.os.storage.OnObbStateChangeListener",
				"android.os.TokenWatcher",
				"android.os.TransactionTooLargeException",
				"android.os.WorkSource",
				"android.preference.CheckBoxPreference",
				"android.preference.DialogPreference",
				"android.preference.EditTextPreference",
				"android.preference.ListPreference",
				"android.preference.MultiSelectListPreference",
				"android.preference.Preference",
				"android.preference.PreferenceActivity",
				"android.preference.PreferenceCategory",
				"android.preference.PreferenceFragment",
				"android.preference.PreferenceGroup",
				"android.preference.RingtonePreference",
				"android.preference.SwitchPreference",
				"android.preference.TwoStatePreference",
				"android.provider.BaseColumns",
				"android.provider.Browser",
				"android.provider.CallLog",
				"android.provider.ContactsContract",
				"android.provider.MediaStore",
				"android.provider.OpenableColumns",
				"android.provider.SearchRecentSuggestions",
				"android.provider.Settings",
				"android.provider.SyncStateContract",
				"android.provider.UserDictionary",
				"android.R",
				"android.renderscript.Byte2",
				"android.renderscript.Byte3",
				"android.renderscript.Byte4",
				"android.renderscript.Double2",
				"android.renderscript.Double3",
				"android.renderscript.Double4",
				"android.renderscript.FieldPacker",
				"android.renderscript.Float2",
				"android.renderscript.Float3",
				"android.renderscript.Float4",
				"android.renderscript.Int2",
				"android.renderscript.Int3",
				"android.renderscript.Int4",
				"android.renderscript.Long2",
				"android.renderscript.Long3",
				"android.renderscript.Long4",
				"android.renderscript.Matrix2f",
				"android.renderscript.Matrix3f",
				"android.renderscript.Matrix4f",
				"android.renderscript.RenderScriptGL",
				"android.renderscript.RSDriverException",
				"android.renderscript.RSIllegalArgumentException",
				"android.renderscript.RSInvalidStateException",
				"android.renderscript.RSRuntimeException",
				"android.renderscript.RSSurfaceView",
				"android.renderscript.RSTextureView",
				"android.renderscript.ScriptGroup",
				"android.renderscript.Short2",
				"android.renderscript.Short3",
				"android.renderscript.Short4",
				"android.sax.ElementListener",
				"android.sax.EndElementListener",
				"android.sax.EndTextElementListener",
				"android.sax.RootElement",
				"android.sax.StartElementListener",
				"android.sax.TextElementListener",
				"android.security.KeyChainAliasCallback",
				"android.security.KeyChainException",
				"android.service.dreams.DreamService",
				"android.service.textservice.SpellCheckerService",
				"android.service.wallpaper.WallpaperService",
				"android.speech.RecognitionListener",
				"android.speech.RecognitionService",
				"android.speech.tts.SynthesisCallback",
				"android.speech.tts.TextToSpeech",
				"android.speech.tts.TextToSpeechService",
				"android.speech.tts.UtteranceProgressListener",
				"android.telephony.cdma.CdmaCellLocation",
				"android.telephony.CellLocation",
				"android.telephony.gsm.GsmCellLocation",
				"android.telephony.gsm.SmsMessage",
				"android.telephony.NeighboringCellInfo",
				"android.telephony.PhoneNumberFormattingTextWatcher",
				"android.telephony.PhoneNumberUtils",
				"android.telephony.PhoneStateListener",
				"android.telephony.ServiceState",
				"android.test.ActivityInstrumentationTestCase",
				"android.test.ActivityInstrumentationTestCase2",
				"android.test.ActivityTestCase",
				"android.test.ActivityUnitTestCase",
				"android.test.AndroidTestCase",
				"android.test.AndroidTestRunner",
				"android.test.ApplicationTestCase",
				"android.test.AssertionFailedError",
				"android.test.ComparisonFailure",
				"android.test.FlakyTest",
				"android.test.InstrumentationTestCase",
				"android.test.InstrumentationTestRunner",
				"android.test.InstrumentationTestSuite",
				"android.test.IsolatedContext",
				"android.test.LoaderTestCase",
				"android.test.mock.MockApplication",
				"android.test.mock.MockContentProvider",
				"android.test.mock.MockContentResolver",
				"android.test.mock.MockContext",
				"android.test.mock.MockCursor",
				"android.test.mock.MockDialogInterface",
				"android.test.mock.MockPackageManager",
				"android.test.mock.MockResources",
				"android.test.PerformanceTestCase",
				"android.test.ProviderTestCase",
				"android.test.ProviderTestCase2",
				"android.test.RenamingDelegatingContext",
				"android.test.ServiceTestCase",
				"android.test.SingleLaunchActivityTestCase",
				"android.test.suitebuilder.annotation.LargeTest",
				"android.test.suitebuilder.annotation.MediumTest",
				"android.test.suitebuilder.annotation.SmallTest",
				"android.test.suitebuilder.annotation.Smoke",
				"android.test.suitebuilder.annotation.Suppress",
				"android.test.suitebuilder.TestMethod",
				"android.test.suitebuilder.TestSuiteBuilder",
				"android.test.SyncBaseInstrumentation",
				"android.test.TestSuiteProvider",
				"android.test.TouchUtils",
				"android.test.UiThreadTest",
				"android.text.AndroidCharacter",
				"android.text.Annotation",
				"android.text.BoringLayout",
				"android.text.ClipboardManager",
				"android.text.DynamicLayout",
				"android.text.Editable",
				"android.text.format.DateFormat",
				"android.text.format.DateUtils",
				"android.text.format.Time",
				"android.text.GetChars",
				"android.text.InputFilter",
				"android.text.InputType",
				"android.text.method.ArrowKeyMovementMethod",
				"android.text.method.BaseKeyListener",
				"android.text.method.BaseMovementMethod",
				"android.text.method.CharacterPickerDialog",
				"android.text.method.DateKeyListener",
				"android.text.method.DateTimeKeyListener",
				"android.text.method.DialerKeyListener",
				"android.text.method.DigitsKeyListener",
				"android.text.method.HideReturnsTransformationMethod",
				"android.text.method.KeyListener",
				"android.text.method.LinkMovementMethod",
				"android.text.method.MetaKeyKeyListener",
				"android.text.method.MovementMethod",
				"android.text.method.MultiTapKeyListener",
				"android.text.method.NumberKeyListener",
				"android.text.method.PasswordTransformationMethod",
				"android.text.method.QwertyKeyListener",
				"android.text.method.ReplacementTransformationMethod",
				"android.text.method.ScrollingMovementMethod",
				"android.text.method.SingleLineTransformationMethod",
				"android.text.method.TextKeyListener",
				"android.text.method.TimeKeyListener",
				"android.text.method.TransformationMethod",
				"android.text.NoCopySpan",
				"android.text.ParcelableSpan",
				"android.text.Spannable",
				"android.text.SpannableString",
				"android.text.SpannableStringBuilder",
				"android.text.Spanned",
				"android.text.SpanWatcher",
				"android.text.StaticLayout",
				"android.text.style.AbsoluteSizeSpan",
				"android.text.style.AlignmentSpan",
				"android.text.style.BackgroundColorSpan",
				"android.text.style.BulletSpan",
				"android.text.style.CharacterStyle",
				"android.text.style.ClickableSpan",
				"android.text.style.DrawableMarginSpan",
				"android.text.style.DynamicDrawableSpan",
				"android.text.style.EasyEditSpan",
				"android.text.style.ForegroundColorSpan",
				"android.text.style.IconMarginSpan",
				"android.text.style.ImageSpan",
				"android.text.style.LeadingMarginSpan",
				"android.text.style.LineBackgroundSpan",
				"android.text.style.LineHeightSpan",
				"android.text.style.LocaleSpan",
				"android.text.style.MaskFilterSpan",
				"android.text.style.MetricAffectingSpan",
				"android.text.style.ParagraphStyle",
				"android.text.style.QuoteSpan",
				"android.text.style.RasterizerSpan",
				"android.text.style.RelativeSizeSpan",
				"android.text.style.ReplacementSpan",
				"android.text.style.ScaleXSpan",
				"android.text.style.StrikethroughSpan",
				"android.text.style.StyleSpan",
				"android.text.style.SubscriptSpan",
				"android.text.style.SuggestionSpan",
				"android.text.style.SuperscriptSpan",
				"android.text.style.TabStopSpan",
				"android.text.style.TextAppearanceSpan",
				"android.text.style.TypefaceSpan",
				"android.text.style.UnderlineSpan",
				"android.text.style.UpdateAppearance",
				"android.text.style.UpdateLayout",
				"android.text.style.URLSpan",
				"android.text.style.WrapTogetherSpan",
				"android.text.TextPaint",
				"android.text.TextWatcher",
				"android.text.util.Linkify",
				"android.text.util.Rfc822Token",
				"android.text.util.Rfc822Tokenizer",
				"android.util.AndroidException",
				"android.util.AndroidRuntimeException",
				"android.util.AtomicFile",
				"android.util.AttributeSet",
				"android.util.Base64DataException",
				"android.util.Base64InputStream",
				"android.util.Base64OutputStream",
				"android.util.DisplayMetrics",
				"android.util.EventLogTags",
				"android.util.LogPrinter",
				"android.util.LongSparseArray",
				"android.util.LruCache",
				"android.util.MonthDisplayHelper",
				"android.util.NoSuchPropertyException",
				"android.util.Pair",
				"android.util.Printer",
				"android.util.PrintStreamPrinter",
				"android.util.PrintWriterPrinter",
				"android.util.Property",
				"android.util.SparseArray",
				"android.util.SparseBooleanArray",
				"android.util.SparseIntArray",
				"android.util.StringBuilderPrinter",
				"android.util.TimingLogger",
				"android.util.TypedValue",
				"android.view.accessibility.AccessibilityEventSource",
				"android.view.accessibility.AccessibilityManager",
				"android.view.accessibility.AccessibilityNodeProvider",
				"android.view.ActionMode",
				"android.view.ActionProvider",
				"android.view.animation.AccelerateDecelerateInterpolator",
				"android.view.animation.AccelerateInterpolator",
				"android.view.animation.AlphaAnimation",
				"android.view.animation.Animation",
				"android.view.animation.AnimationSet",
				"android.view.animation.AnimationUtils",
				"android.view.animation.AnticipateInterpolator",
				"android.view.animation.AnticipateOvershootInterpolator",
				"android.view.animation.BounceInterpolator",
				"android.view.animation.CycleInterpolator",
				"android.view.animation.DecelerateInterpolator",
				"android.view.animation.GridLayoutAnimationController",
				"android.view.animation.Interpolator",
				"android.view.animation.LayoutAnimationController",
				"android.view.animation.LinearInterpolator",
				"android.view.animation.OvershootInterpolator",
				"android.view.animation.RotateAnimation",
				"android.view.animation.ScaleAnimation",
				"android.view.animation.Transformation",
				"android.view.animation.TranslateAnimation",
				"android.view.Choreographer",
				"android.view.CollapsibleActionView",
				"android.view.ContextMenu",
				"android.view.ContextThemeWrapper",
				"android.view.GestureDetector",
				"android.view.Gravity",
				"android.view.InflateException",
				"android.view.inputmethod.BaseInputConnection",
				"android.view.inputmethod.EditorInfo",
				"android.view.inputmethod.ExtractedText",
				"android.view.inputmethod.ExtractedTextRequest",
				"android.view.inputmethod.InputConnection",
				"android.view.inputmethod.InputConnectionWrapper",
				"android.view.inputmethod.InputMethod",
				"android.view.inputmethod.InputMethodSession",
				"android.view.InputQueue",
				"android.view.KeyEvent",
				"android.view.Menu",
				"android.view.MenuInflater",
				"android.view.MenuItem",
				"android.view.MotionEvent",
				"android.view.OrientationEventListener",
				"android.view.OrientationListener",
				"android.view.ScaleGestureDetector",
				"android.view.SubMenu",
				"android.view.Surface",
				"android.view.SurfaceHolder",
				"android.view.SurfaceView",
				"android.view.TextureView",
				"android.view.TouchDelegate",
				"android.view.View",
				"android.view.ViewConfiguration",
				"android.view.ViewDebug",
				"android.view.ViewGroup",
				"android.view.ViewManager",
				"android.view.ViewParent",
				"android.view.ViewStub",
				"android.view.ViewTreeObserver",
				"android.view.Window",
				"android.view.WindowManager",
				"android.webkit.ConsoleMessage",
				"android.webkit.DateSorter",
				"android.webkit.DownloadListener",
				"android.webkit.PluginStub",
				"android.webkit.ValueCallback",
				"android.webkit.WebChromeClient",
				"android.webkit.WebResourceResponse",
				"android.webkit.WebView",
				"android.webkit.WebViewClient",
				"android.webkit.WebViewFragment",
				"android.widget.AbsListView",
				"android.widget.AbsoluteLayout",
				"android.widget.AbsSeekBar",
				"android.widget.AbsSpinner",
				"android.widget.Adapter",
				"android.widget.AdapterView",
				"android.widget.AdapterViewAnimator",
				"android.widget.AdapterViewFlipper",
				"android.widget.Advanceable",
				"android.widget.AlphabetIndexer",
				"android.widget.AnalogClock",
				"android.widget.ArrayAdapter",
				"android.widget.AutoCompleteTextView",
				"android.widget.BaseAdapter",
				"android.widget.BaseExpandableListAdapter",
				"android.widget.Button",
				"android.widget.CalendarView",
				"android.widget.Checkable",
				"android.widget.CheckBox",
				"android.widget.CheckedTextView",
				"android.widget.Chronometer",
				"android.widget.CompoundButton",
				"android.widget.CursorAdapter",
				"android.widget.CursorTreeAdapter",
				"android.widget.DatePicker",
				"android.widget.DialerFilter",
				"android.widget.DigitalClock",
				"android.widget.EdgeEffect",
				"android.widget.EditText",
				"android.widget.ExpandableListAdapter",
				"android.widget.ExpandableListView",
				"android.widget.Filter",
				"android.widget.Filterable",
				"android.widget.FilterQueryProvider",
				"android.widget.FrameLayout",
				"android.widget.Gallery",
				"android.widget.GridLayout",
				"android.widget.GridView",
				"android.widget.HeaderViewListAdapter",
				"android.widget.HeterogeneousExpandableList",
				"android.widget.HorizontalScrollView",
				"android.widget.ImageButton",
				"android.widget.ImageSwitcher",
				"android.widget.ImageView",
				"android.widget.LinearLayout",
				"android.widget.ListAdapter",
				"android.widget.ListPopupWindow",
				"android.widget.ListView",
				"android.widget.MediaController",
				"android.widget.MultiAutoCompleteTextView",
				"android.widget.NumberPicker",
				"android.widget.OverScroller",
				"android.widget.PopupMenu",
				"android.widget.PopupWindow",
				"android.widget.ProgressBar",
				"android.widget.QuickContactBadge",
				"android.widget.RadioButton",
				"android.widget.RadioGroup",
				"android.widget.RatingBar",
				"android.widget.RelativeLayout",
				"android.widget.RemoteViews",
				"android.widget.RemoteViewsService",
				"android.widget.ResourceCursorAdapter",
				"android.widget.ResourceCursorTreeAdapter",
				"android.widget.Scroller",
				"android.widget.ScrollView",
				"android.widget.SearchView",
				"android.widget.SectionIndexer",
				"android.widget.SeekBar",
				"android.widget.ShareActionProvider",
				"android.widget.SimpleAdapter",
				"android.widget.SimpleCursorAdapter",
				"android.widget.SimpleCursorTreeAdapter",
				"android.widget.SimpleExpandableListAdapter",
				"android.widget.SlidingDrawer",
				"android.widget.Spinner",
				"android.widget.SpinnerAdapter",
				"android.widget.StackView",
				"android.widget.Switch",
				"android.widget.TabHost",
				"android.widget.TableLayout",
				"android.widget.TableRow",
				"android.widget.TabWidget",
				"android.widget.TextClock",
				"android.widget.TextSwitcher",
				"android.widget.TextView",
				"android.widget.TimePicker",
				"android.widget.Toast",
				"android.widget.ToggleButton",
				"android.widget.TwoLineListItem",
				"android.widget.VideoView",
				"android.widget.ViewAnimator",
				"android.widget.ViewFlipper",
				"android.widget.ViewSwitcher",
				"android.widget.WrapperListAdapter",
				"android.widget.ZoomButton",
				"android.widget.ZoomButtonsController",
				"android.widget.ZoomControls",
				"dalvik.annotation.TestTarget",
				"dalvik.annotation.TestTargetClass",
				"dalvik.bytecode.Opcodes",
				"dalvik.system.BaseDexClassLoader",
				"dalvik.system.DexClassLoader",
				"dalvik.system.PathClassLoader",
				"java.beans.IndexedPropertyChangeEvent",
				"java.beans.PropertyChangeEvent",
				"java.beans.PropertyChangeListener",
				"java.beans.PropertyChangeListenerProxy",
				"java.beans.PropertyChangeSupport",
				"java.io.BufferedInputStream",
				"java.io.BufferedOutputStream",
				"java.io.BufferedReader",
				"java.io.BufferedWriter",
				"java.io.ByteArrayInputStream",
				"java.io.ByteArrayOutputStream",
				"java.io.CharArrayReader",
				"java.io.CharArrayWriter",
				"java.io.CharConversionException",
				"java.io.Closeable",
				"java.io.DataInput",
				"java.io.DataInputStream",
				"java.io.DataOutput",
				"java.io.DataOutputStream",
				"java.io.EOFException",
				"java.io.Externalizable",
				"java.io.File",
				"java.io.FileFilter",
				"java.io.FileInputStream",
				"java.io.FilenameFilter",
				"java.io.FileNotFoundException",
				"java.io.FileOutputStream",
				"java.io.FileReader",
				"java.io.FileWriter",
				"java.io.FilterOutputStream",
				"java.io.Flushable",
				"java.io.InputStream",
				"java.io.InputStreamReader",
				"java.io.InterruptedIOException",
				"java.io.InvalidClassException",
				"java.io.InvalidObjectException",
				"java.io.IOError",
				"java.io.IOException",
				"java.io.LineNumberInputStream",
				"java.io.LineNumberReader",
				"java.io.NotActiveException",
				"java.io.NotSerializableException",
				"java.io.ObjectInput",
				"java.io.ObjectInputStream",
				"java.io.ObjectInputValidation",
				"java.io.ObjectOutput",
				"java.io.ObjectOutputStream",
				"java.io.ObjectStreamConstants",
				"java.io.ObjectStreamField",
				"java.io.OutputStream",
				"java.io.OutputStreamWriter",
				"java.io.PipedInputStream",
				"java.io.PipedOutputStream",
				"java.io.PipedReader",
				"java.io.PipedWriter",
				"java.io.PrintStream",
				"java.io.PrintWriter",
				"java.io.PushbackInputStream",
				"java.io.PushbackReader",
				"java.io.RandomAccessFile",
				"java.io.SequenceInputStream",
				"java.io.Serializable",
				"java.io.StreamCorruptedException",
				"java.io.StreamTokenizer",
				"java.io.StringBufferInputStream",
				"java.io.StringReader",
				"java.io.StringWriter",
				"java.io.SyncFailedException",
				"java.io.UnsupportedEncodingException",
				"java.io.UTFDataFormatException",
				"java.io.WriteAbortedException",
				"java.lang.AbstractMethodError",
				"java.lang.annotation.Annotation",
				"java.lang.annotation.AnnotationFormatError",
				"java.lang.annotation.AnnotationTypeMismatchException",
				"java.lang.annotation.Documented",
				"java.lang.annotation.IncompleteAnnotationException",
				"java.lang.annotation.Inherited",
				"java.lang.annotation.Retention",
				"java.lang.annotation.Target",
				"java.lang.Appendable",
				"java.lang.ArithmeticException",
				"java.lang.ArrayIndexOutOfBoundsException",
				"java.lang.ArrayStoreException",
				"java.lang.AssertionError",
				"java.lang.CharSequence",
				"java.lang.ClassCastException",
				"java.lang.ClassCircularityError",
				"java.lang.ClassFormatError",
				"java.lang.ClassNotFoundException",
				"java.lang.Cloneable",
				"java.lang.CloneNotSupportedException",
				"java.lang.Comparable",
				"java.lang.Deprecated",
				"java.lang.EnumConstantNotPresentException",
				"java.lang.Error",
				"java.lang.Exception",
				"java.lang.ExceptionInInitializerError",
				"java.lang.IllegalAccessError",
				"java.lang.IllegalAccessException",
				"java.lang.IllegalArgumentException",
				"java.lang.IllegalMonitorStateException",
				"java.lang.IllegalStateException",
				"java.lang.IllegalThreadStateException",
				"java.lang.IncompatibleClassChangeError",
				"java.lang.IndexOutOfBoundsException",
				"java.lang.InheritableThreadLocal",
				"java.lang.InstantiationError",
				"java.lang.InstantiationException",
				"java.lang.InternalError",
				"java.lang.InterruptedException",
				"java.lang.Iterable",
				"java.lang.LinkageError",
				"java.lang.NegativeArraySizeException",
				"java.lang.NoClassDefFoundError",
				"java.lang.NoSuchFieldError",
				"java.lang.NoSuchFieldException",
				"java.lang.NoSuchMethodError",
				"java.lang.NoSuchMethodException",
				"java.lang.NullPointerException",
				"java.lang.Number",
				"java.lang.NumberFormatException",
				"java.lang.Object",
				"java.lang.OutOfMemoryError",
				"java.lang.Override",
				"java.lang.Process",
				"java.lang.Readable",
				"java.lang.ref.PhantomReference",
				"java.lang.ref.ReferenceQueue",
				"java.lang.ref.SoftReference",
				"java.lang.ref.WeakReference",
				"java.lang.reflect.AnnotatedElement",
				"java.lang.reflect.GenericArrayType",
				"java.lang.reflect.GenericDeclaration",
				"java.lang.reflect.GenericSignatureFormatError",
				"java.lang.reflect.InvocationHandler",
				"java.lang.reflect.InvocationTargetException",
				"java.lang.reflect.MalformedParameterizedTypeException",
				"java.lang.reflect.Member",
				"java.lang.reflect.Modifier",
				"java.lang.reflect.ParameterizedType",
				"java.lang.reflect.Type",
				"java.lang.reflect.TypeVariable",
				"java.lang.reflect.UndeclaredThrowableException",
				"java.lang.reflect.WildcardType",
				"java.lang.Runnable",
				"java.lang.RuntimeException",
				"java.lang.SecurityException",
				"java.lang.SecurityManager",
				"java.lang.StackOverflowError",
				"java.lang.StringIndexOutOfBoundsException",
				"java.lang.SuppressWarnings",
				"java.lang.Thread",
				"java.lang.ThreadDeath",
				"java.lang.ThreadGroup",
				"java.lang.ThreadLocal",
				"java.lang.Throwable",
				"java.lang.TypeNotPresentException",
				"java.lang.UnknownError",
				"java.lang.UnsatisfiedLinkError",
				"java.lang.UnsupportedClassVersionError",
				"java.lang.UnsupportedOperationException",
				"java.lang.VerifyError",
				"java.lang.VirtualMachineError",
				"java.math.BigDecimal",
				"java.math.BigInteger",
				"java.net.Authenticator",
				"java.net.BindException",
				"java.net.CacheRequest",
				"java.net.CacheResponse",
				"java.net.ConnectException",
				"java.net.ContentHandler",
				"java.net.ContentHandlerFactory",
				"java.net.CookieHandler",
				"java.net.CookieManager",
				"java.net.CookiePolicy",
				"java.net.CookieStore",
				"java.net.DatagramSocket",
				"java.net.DatagramSocketImpl",
				"java.net.DatagramSocketImplFactory",
				"java.net.FileNameMap",
				"java.net.HttpRetryException",
				"java.net.InetSocketAddress",
				"java.net.MalformedURLException",
				"java.net.MulticastSocket",
				"java.net.NoRouteToHostException",
				"java.net.PortUnreachableException",
				"java.net.ProtocolException",
				"java.net.Proxy",
				"java.net.ProxySelector",
				"java.net.ResponseCache",
				"java.net.SecureCacheResponse",
				"java.net.ServerSocket",
				"java.net.Socket",
				"java.net.SocketAddress",
				"java.net.SocketException",
				"java.net.SocketImpl",
				"java.net.SocketImplFactory",
				"java.net.SocketOptions",
				"java.net.SocketTimeoutException",
				"java.net.UnknownHostException",
				"java.net.UnknownServiceException",
				"java.net.URISyntaxException",
				"java.net.URLClassLoader",
				"java.net.URLDecoder",
				"java.net.URLStreamHandler",
				"java.net.URLStreamHandlerFactory",
				"java.security.AccessControlException",
				"java.security.acl.Acl",
				"java.security.acl.AclEntry",
				"java.security.acl.AclNotFoundException",
				"java.security.acl.Group",
				"java.security.acl.LastOwnerException",
				"java.security.acl.NotOwnerException",
				"java.security.acl.Owner",
				"java.security.acl.Permission",
				"java.security.AlgorithmParameterGeneratorSpi",
				"java.security.AlgorithmParametersSpi",
				"java.security.BasicPermission",
				"java.security.cert.CertificateEncodingException",
				"java.security.cert.CertificateException",
				"java.security.cert.CertificateExpiredException",
				"java.security.cert.CertificateFactorySpi",
				"java.security.cert.CertificateNotYetValidException",
				"java.security.cert.CertificateParsingException",
				"java.security.cert.CertPathBuilderException",
				"java.security.cert.CertPathBuilderResult",
				"java.security.cert.CertPathBuilderSpi",
				"java.security.cert.CertPathParameters",
				"java.security.cert.CertPathValidatorException",
				"java.security.cert.CertPathValidatorResult",
				"java.security.cert.CertPathValidatorSpi",
				"java.security.cert.CertSelector",
				"java.security.cert.CertStoreException",
				"java.security.cert.CertStoreParameters",
				"java.security.cert.CertStoreSpi",
				"java.security.cert.CollectionCertStoreParameters",
				"java.security.cert.CRLException",
				"java.security.cert.CRLSelector",
				"java.security.cert.LDAPCertStoreParameters",
				"java.security.cert.PKIXBuilderParameters",
				"java.security.cert.PKIXCertPathBuilderResult",
				"java.security.cert.PKIXCertPathValidatorResult",
				"java.security.cert.PKIXParameters",
				"java.security.cert.PolicyNode",
				"java.security.cert.PolicyQualifierInfo",
				"java.security.cert.TrustAnchor",
				"java.security.cert.X509CertSelector",
				"java.security.cert.X509CRLEntry",
				"java.security.cert.X509CRLSelector",
				"java.security.cert.X509Extension",
				"java.security.Certificate",
				"java.security.CodeSource",
				"java.security.DigestException",
				"java.security.DigestInputStream",
				"java.security.DigestOutputStream",
				"java.security.DomainCombiner",
				"java.security.GeneralSecurityException",
				"java.security.Guard",
				"java.security.GuardedObject",
				"java.security.Identity",
				"java.security.IdentityScope",
				"java.security.interfaces.DSAKey",
				"java.security.interfaces.DSAKeyPairGenerator",
				"java.security.interfaces.DSAParams",
				"java.security.interfaces.DSAPrivateKey",
				"java.security.interfaces.DSAPublicKey",
				"java.security.interfaces.ECKey",
				"java.security.interfaces.ECPrivateKey",
				"java.security.interfaces.ECPublicKey",
				"java.security.interfaces.RSAKey",
				"java.security.interfaces.RSAMultiPrimePrivateCrtKey",
				"java.security.interfaces.RSAPrivateCrtKey",
				"java.security.interfaces.RSAPrivateKey",
				"java.security.interfaces.RSAPublicKey",
				"java.security.InvalidAlgorithmParameterException",
				"java.security.InvalidKeyException",
				"java.security.InvalidParameterException",
				"java.security.Key",
				"java.security.KeyException",
				"java.security.KeyFactorySpi",
				"java.security.KeyManagementException",
				"java.security.KeyPairGeneratorSpi",
				"java.security.KeyRep",
				"java.security.KeyStoreException",
				"java.security.KeyStoreSpi",
				"java.security.MessageDigestSpi",
				"java.security.NoSuchAlgorithmException",
				"java.security.NoSuchProviderException",
				"java.security.Permission",
				"java.security.PermissionCollection",
				"java.security.Policy",
				"java.security.PolicySpi",
				"java.security.Principal",
				"java.security.PrivateKey",
				"java.security.PrivilegedAction",
				"java.security.PrivilegedActionException",
				"java.security.PrivilegedExceptionAction",
				"java.security.ProtectionDomain",
				"java.security.ProviderException",
				"java.security.PublicKey",
				"java.security.SecureRandom",
				"java.security.SecureRandomSpi",
				"java.security.SignatureException",
				"java.security.SignatureSpi",
				"java.security.Signer",
				"java.security.spec.AlgorithmParameterSpec",
				"java.security.spec.DSAParameterSpec",
				"java.security.spec.DSAPrivateKeySpec",
				"java.security.spec.DSAPublicKeySpec",
				"java.security.spec.ECField",
				"java.security.spec.ECFieldF2m",
				"java.security.spec.ECFieldFp",
				"java.security.spec.ECGenParameterSpec",
				"java.security.spec.ECParameterSpec",
				"java.security.spec.ECPoint",
				"java.security.spec.ECPrivateKeySpec",
				"java.security.spec.ECPublicKeySpec",
				"java.security.spec.EllipticCurve",
				"java.security.spec.EncodedKeySpec",
				"java.security.spec.InvalidKeySpecException",
				"java.security.spec.InvalidParameterSpecException",
				"java.security.spec.KeySpec",
				"java.security.spec.MGF1ParameterSpec",
				"java.security.spec.PKCS8EncodedKeySpec",
				"java.security.spec.PSSParameterSpec",
				"java.security.spec.RSAKeyGenParameterSpec",
				"java.security.spec.RSAMultiPrimePrivateCrtKeySpec",
				"java.security.spec.RSAOtherPrimeInfo",
				"java.security.spec.RSAPrivateCrtKeySpec",
				"java.security.spec.RSAPrivateKeySpec",
				"java.security.spec.RSAPublicKeySpec",
				"java.security.spec.X509EncodedKeySpec",
				"java.security.UnrecoverableEntryException",
				"java.security.UnrecoverableKeyException",
				"java.sql.Array",
				"java.sql.BatchUpdateException",
				"java.sql.Blob",
				"java.sql.CallableStatement",
				"java.sql.Clob",
				"java.sql.Connection",
				"java.sql.DatabaseMetaData",
				"java.sql.DataTruncation",
				"java.sql.Date",
				"java.sql.Driver",
				"java.sql.DriverPropertyInfo",
				"java.sql.NClob",
				"java.sql.ParameterMetaData",
				"java.sql.PreparedStatement",
				"java.sql.Ref",
				"java.sql.ResultSet",
				"java.sql.ResultSetMetaData",
				"java.sql.RowId",
				"java.sql.Savepoint",
				"java.sql.SQLClientInfoException",
				"java.sql.SQLData",
				"java.sql.SQLDataException",
				"java.sql.SQLException",
				"java.sql.SQLFeatureNotSupportedException",
				"java.sql.SQLInput",
				"java.sql.SQLIntegrityConstraintViolationException",
				"java.sql.SQLInvalidAuthorizationSpecException",
				"java.sql.SQLNonTransientConnectionException",
				"java.sql.SQLNonTransientException",
				"java.sql.SQLOutput",
				"java.sql.SQLRecoverableException",
				"java.sql.SQLSyntaxErrorException",
				"java.sql.SQLTimeoutException",
				"java.sql.SQLTransactionRollbackException",
				"java.sql.SQLTransientConnectionException",
				"java.sql.SQLTransientException",
				"java.sql.SQLWarning",
				"java.sql.SQLXML",
				"java.sql.Statement",
				"java.sql.Struct",
				"java.sql.Time",
				"java.sql.Timestamp",
				"java.sql.Wrapper",
				"java.text.Annotation",
				"java.text.AttributedCharacterIterator",
				"java.text.AttributedString",
				"java.text.CharacterIterator",
				"java.text.ChoiceFormat",
				"java.text.DateFormatSymbols",
				"java.text.DecimalFormat",
				"java.text.DecimalFormatSymbols",
				"java.text.FieldPosition",
				"java.text.MessageFormat",
				"java.text.ParseException",
				"java.text.ParsePosition",
				"java.text.RuleBasedCollator",
				"java.text.SimpleDateFormat",
				"java.util.ArrayDeque",
				"java.util.ArrayList",
				"java.util.BitSet",
				"java.util.Collection",
				"java.util.Comparator",
				"java.util.concurrent.AbstractExecutorService",
				"java.util.concurrent.ArrayBlockingQueue",
				"java.util.concurrent.atomic.AtomicBoolean",
				"java.util.concurrent.atomic.AtomicInteger",
				"java.util.concurrent.atomic.AtomicIntegerArray",
				"java.util.concurrent.atomic.AtomicLong",
				"java.util.concurrent.atomic.AtomicLongArray",
				"java.util.concurrent.atomic.AtomicMarkableReference",
				"java.util.concurrent.atomic.AtomicReference",
				"java.util.concurrent.atomic.AtomicReferenceArray",
				"java.util.concurrent.atomic.AtomicStampedReference",
				"java.util.concurrent.BlockingDeque",
				"java.util.concurrent.BlockingQueue",
				"java.util.concurrent.BrokenBarrierException",
				"java.util.concurrent.Callable",
				"java.util.concurrent.CancellationException",
				"java.util.concurrent.CompletionService",
				"java.util.concurrent.ConcurrentHashMap",
				"java.util.concurrent.ConcurrentLinkedQueue",
				"java.util.concurrent.ConcurrentMap",
				"java.util.concurrent.ConcurrentNavigableMap",
				"java.util.concurrent.ConcurrentSkipListMap",
				"java.util.concurrent.ConcurrentSkipListSet",
				"java.util.concurrent.CopyOnWriteArrayList",
				"java.util.concurrent.CopyOnWriteArraySet",
				"java.util.concurrent.CountDownLatch",
				"java.util.concurrent.CyclicBarrier",
				"java.util.concurrent.Delayed",
				"java.util.concurrent.DelayQueue",
				"java.util.concurrent.Exchanger",
				"java.util.concurrent.ExecutionException",
				"java.util.concurrent.Executor",
				"java.util.concurrent.ExecutorCompletionService",
				"java.util.concurrent.ExecutorService",
				"java.util.concurrent.Future",
				"java.util.concurrent.FutureTask",
				"java.util.concurrent.LinkedBlockingDeque",
				"java.util.concurrent.LinkedBlockingQueue",
				"java.util.concurrent.locks.Condition",
				"java.util.concurrent.locks.Lock",
				"java.util.concurrent.locks.ReadWriteLock",
				"java.util.concurrent.locks.ReentrantLock",
				"java.util.concurrent.locks.ReentrantReadWriteLock",
				"java.util.concurrent.PriorityBlockingQueue",
				"java.util.concurrent.RejectedExecutionException",
				"java.util.concurrent.RejectedExecutionHandler",
				"java.util.concurrent.RunnableFuture",
				"java.util.concurrent.RunnableScheduledFuture",
				"java.util.concurrent.ScheduledExecutorService",
				"java.util.concurrent.ScheduledFuture",
				"java.util.concurrent.ScheduledThreadPoolExecutor",
				"java.util.concurrent.Semaphore",
				"java.util.concurrent.SynchronousQueue",
				"java.util.concurrent.ThreadFactory",
				"java.util.concurrent.ThreadPoolExecutor",
				"java.util.concurrent.TimeoutException",
				"java.util.ConcurrentModificationException",
				"java.util.Date",
				"java.util.Deque",
				"java.util.Dictionary",
				"java.util.DuplicateFormatFlagsException",
				"java.util.EmptyStackException",
				"java.util.Enumeration",
				"java.util.EnumMap",
				"java.util.EventListener",
				"java.util.EventListenerProxy",
				"java.util.EventObject",
				"java.util.FormatFlagsConversionMismatchException",
				"java.util.Formattable",
				"java.util.FormatterClosedException",
				"java.util.GregorianCalendar",
				"java.util.HashMap",
				"java.util.HashSet",
				"java.util.Hashtable",
				"java.util.IdentityHashMap",
				"java.util.IllegalFormatCodePointException",
				"java.util.IllegalFormatConversionException",
				"java.util.IllegalFormatFlagsException",
				"java.util.IllegalFormatPrecisionException",
				"java.util.IllegalFormatWidthException",
				"java.util.InputMismatchException",
				"java.util.InvalidPropertiesFormatException",
				"java.util.Iterator",
				"java.util.jar.Attributes",
				"java.util.jar.JarEntry",
				"java.util.jar.JarException",
				"java.util.jar.JarFile",
				"java.util.jar.JarInputStream",
				"java.util.jar.JarOutputStream",
				"java.util.jar.Manifest",
				"java.util.LinkedHashMap",
				"java.util.LinkedHashSet",
				"java.util.LinkedList",
				"java.util.List",
				"java.util.ListIterator",
				"java.util.ListResourceBundle",
				"java.util.logging.ConsoleHandler",
				"java.util.logging.ErrorManager",
				"java.util.logging.FileHandler",
				"java.util.logging.Filter",
				"java.util.logging.LoggingMXBean",
				"java.util.logging.LogRecord",
				"java.util.logging.MemoryHandler",
				"java.util.logging.SimpleFormatter",
				"java.util.logging.SocketHandler",
				"java.util.logging.StreamHandler",
				"java.util.logging.XMLFormatter",
				"java.util.Map",
				"java.util.MissingFormatArgumentException",
				"java.util.MissingFormatWidthException",
				"java.util.MissingResourceException",
				"java.util.NavigableMap",
				"java.util.NavigableSet",
				"java.util.NoSuchElementException",
				"java.util.Observable",
				"java.util.Observer",
				"java.util.prefs.BackingStoreException",
				"java.util.prefs.InvalidPreferencesFormatException",
				"java.util.prefs.NodeChangeEvent",
				"java.util.prefs.NodeChangeListener",
				"java.util.prefs.PreferenceChangeEvent",
				"java.util.prefs.PreferenceChangeListener",
				"java.util.prefs.PreferencesFactory",
				"java.util.PriorityQueue",
				"java.util.Properties",
				"java.util.PropertyResourceBundle",
				"java.util.Queue",
				"java.util.Random",
				"java.util.RandomAccess",
				"java.util.regex.MatchResult",
				"java.util.regex.PatternSyntaxException",
				"java.util.ResourceBundle",
				"java.util.ServiceConfigurationError",
				"java.util.Set",
				"java.util.SimpleTimeZone",
				"java.util.SortedMap",
				"java.util.SortedSet",
				"java.util.Stack",
				"java.util.StringTokenizer",
				"java.util.Timer",
				"java.util.TimeZone",
				"java.util.TooManyListenersException",
				"java.util.TreeMap",
				"java.util.TreeSet",
				"java.util.UnknownFormatConversionException",
				"java.util.UnknownFormatFlagsException",
				"java.util.Vector",
				"java.util.WeakHashMap",
				"java.util.zip.Adler32",
				"java.util.zip.CheckedInputStream",
				"java.util.zip.CheckedOutputStream",
				"java.util.zip.Checksum",
				"java.util.zip.CRC32",
				"java.util.zip.DataFormatException",
				"java.util.zip.Deflater",
				"java.util.zip.DeflaterInputStream",
				"java.util.zip.DeflaterOutputStream",
				"java.util.zip.GZIPInputStream",
				"java.util.zip.GZIPOutputStream",
				"java.util.zip.Inflater",
				"java.util.zip.InflaterInputStream",
				"java.util.zip.InflaterOutputStream",
				"java.util.zip.ZipEntry",
				"java.util.zip.ZipError",
				"java.util.zip.ZipException",
				"java.util.zip.ZipFile",
				"java.util.zip.ZipInputStream",
				"java.util.zip.ZipOutputStream",
				"javax.crypto.BadPaddingException",
				"javax.crypto.CipherInputStream",
				"javax.crypto.CipherOutputStream",
				"javax.crypto.CipherSpi",
				"javax.crypto.EncryptedPrivateKeyInfo",
				"javax.crypto.ExemptionMechanismException",
				"javax.crypto.ExemptionMechanismSpi",
				"javax.crypto.IllegalBlockSizeException",
				"javax.crypto.interfaces.DHKey",
				"javax.crypto.interfaces.DHPrivateKey",
				"javax.crypto.interfaces.DHPublicKey",
				"javax.crypto.interfaces.PBEKey",
				"javax.crypto.KeyAgreementSpi",
				"javax.crypto.KeyGeneratorSpi",
				"javax.crypto.MacSpi",
				"javax.crypto.NoSuchPaddingException",
				"javax.crypto.NullCipher",
				"javax.crypto.SealedObject",
				"javax.crypto.SecretKey",
				"javax.crypto.SecretKeyFactorySpi",
				"javax.crypto.ShortBufferException",
				"javax.crypto.spec.DESedeKeySpec",
				"javax.crypto.spec.DESKeySpec",
				"javax.crypto.spec.DHGenParameterSpec",
				"javax.crypto.spec.DHParameterSpec",
				"javax.crypto.spec.DHPrivateKeySpec",
				"javax.crypto.spec.DHPublicKeySpec",
				"javax.crypto.spec.IvParameterSpec",
				"javax.crypto.spec.OAEPParameterSpec",
				"javax.crypto.spec.PBEKeySpec",
				"javax.crypto.spec.PBEParameterSpec",
				"javax.crypto.spec.RC2ParameterSpec",
				"javax.crypto.spec.RC5ParameterSpec",
				"javax.crypto.spec.SecretKeySpec",
				"javax.microedition.khronos.egl.EGL",
				"javax.microedition.khronos.egl.EGL10",
				"javax.microedition.khronos.egl.EGL11",
				"javax.microedition.khronos.egl.EGLConfig",
				"javax.microedition.khronos.egl.EGLContext",
				"javax.microedition.khronos.egl.EGLDisplay",
				"javax.microedition.khronos.egl.EGLSurface",
				"javax.microedition.khronos.opengles.GL",
				"javax.microedition.khronos.opengles.GL10",
				"javax.microedition.khronos.opengles.GL10Ext",
				"javax.microedition.khronos.opengles.GL11",
				"javax.microedition.khronos.opengles.GL11Ext",
				"javax.microedition.khronos.opengles.GL11ExtensionPack",
				"javax.net.ssl.CertPathTrustManagerParameters",
				"javax.net.ssl.HandshakeCompletedEvent",
				"javax.net.ssl.HandshakeCompletedListener",
				"javax.net.ssl.HostnameVerifier",
				"javax.net.ssl.KeyManager",
				"javax.net.ssl.KeyManagerFactorySpi",
				"javax.net.ssl.KeyStoreBuilderParameters",
				"javax.net.ssl.ManagerFactoryParameters",
				"javax.net.ssl.SSLContextSpi",
				"javax.net.ssl.SSLEngineResult",
				"javax.net.ssl.SSLException",
				"javax.net.ssl.SSLHandshakeException",
				"javax.net.ssl.SSLKeyException",
				"javax.net.ssl.SSLParameters",
				"javax.net.ssl.SSLPeerUnverifiedException",
				"javax.net.ssl.SSLProtocolException",
				"javax.net.ssl.SSLSession",
				"javax.net.ssl.SSLSessionBindingEvent",
				"javax.net.ssl.SSLSessionBindingListener",
				"javax.net.ssl.SSLSessionContext",
				"javax.net.ssl.SSLSocketFactory",
				"javax.net.ssl.TrustManager",
				"javax.net.ssl.TrustManagerFactorySpi",
				"javax.net.ssl.X509KeyManager",
				"javax.net.ssl.X509TrustManager",
				"javax.security.auth.callback.Callback",
				"javax.security.auth.callback.CallbackHandler",
				"javax.security.auth.callback.PasswordCallback",
				"javax.security.auth.callback.UnsupportedCallbackException",
				"javax.security.auth.Destroyable",
				"javax.security.auth.DestroyFailedException",
				"javax.security.auth.login.LoginException",
				"javax.security.auth.SubjectDomainCombiner",
				"javax.security.cert.Certificate",
				"javax.security.cert.CertificateEncodingException",
				"javax.security.cert.CertificateException",
				"javax.security.cert.CertificateExpiredException",
				"javax.security.cert.CertificateNotYetValidException",
				"javax.security.cert.CertificateParsingException",
				"javax.security.cert.X509Certificate",
				"javax.sql.CommonDataSource",
				"javax.sql.ConnectionEvent",
				"javax.sql.ConnectionEventListener",
				"javax.sql.ConnectionPoolDataSource",
				"javax.sql.DataSource",
				"javax.sql.PooledConnection",
				"javax.sql.RowSet",
				"javax.sql.RowSetEvent",
				"javax.sql.RowSetInternal",
				"javax.sql.RowSetListener",
				"javax.sql.RowSetMetaData",
				"javax.sql.RowSetReader",
				"javax.sql.RowSetWriter",
				"javax.sql.StatementEvent",
				"javax.sql.StatementEventListener",
				"javax.xml.datatype.DatatypeConfigurationException",
				"javax.xml.datatype.Duration",
				"javax.xml.datatype.XMLGregorianCalendar",
				"javax.xml.namespace.NamespaceContext",
				"javax.xml.namespace.QName",
				"javax.xml.parsers.FactoryConfigurationError",
				"javax.xml.parsers.ParserConfigurationException",
				"javax.xml.transform.dom.DOMLocator",
				"javax.xml.transform.dom.DOMResult",
				"javax.xml.transform.dom.DOMSource",
				"javax.xml.transform.ErrorListener",
				"javax.xml.transform.Result",
				"javax.xml.transform.sax.SAXResult",
				"javax.xml.transform.sax.SAXSource",
				"javax.xml.transform.sax.TemplatesHandler",
				"javax.xml.transform.sax.TransformerHandler",
				"javax.xml.transform.Source",
				"javax.xml.transform.SourceLocator",
				"javax.xml.transform.stream.StreamResult",
				"javax.xml.transform.stream.StreamSource",
				"javax.xml.transform.Templates",
				"javax.xml.transform.TransformerConfigurationException",
				"javax.xml.transform.TransformerException",
				"javax.xml.transform.TransformerFactoryConfigurationError",
				"javax.xml.transform.URIResolver",
				"javax.xml.xpath.XPath",
				"javax.xml.xpath.XPathException",
				"javax.xml.xpath.XPathExpression",
				"javax.xml.xpath.XPathExpressionException",
				"javax.xml.xpath.XPathFactoryConfigurationException",
				"javax.xml.xpath.XPathFunction",
				"javax.xml.xpath.XPathFunctionException",
				"javax.xml.xpath.XPathFunctionResolver",
				"javax.xml.xpath.XPathVariableResolver",
				"junit.framework.AssertionFailedError",
				"junit.framework.ComparisonFailure",
				"junit.framework.Protectable",
				"junit.framework.Test",
				"junit.framework.TestCase",
				"junit.framework.TestFailure",
				"junit.framework.TestListener",
				"junit.framework.TestResult",
				"junit.framework.TestSuite",
				"junit.runner.BaseTestRunner",
				"junit.runner.TestSuiteLoader",
				"org.apache.commons.logging.Log",
				"org.apache.http.auth.AuthenticationException",
				"org.apache.http.auth.AuthScheme",
				"org.apache.http.auth.AuthSchemeFactory",
				"org.apache.http.auth.AuthScope",
				"org.apache.http.auth.AuthState",
				"org.apache.http.auth.Credentials",
				"org.apache.http.auth.InvalidCredentialsException",
				"org.apache.http.auth.MalformedChallengeException",
				"org.apache.http.auth.NTCredentials",
				"org.apache.http.auth.NTUserPrincipal",
				"org.apache.http.auth.params.AuthParamBean",
				"org.apache.http.auth.params.AuthPNames",
				"org.apache.http.auth.UsernamePasswordCredentials",
				"org.apache.http.client.AuthenticationHandler",
				"org.apache.http.client.CircularRedirectException",
				"org.apache.http.client.ClientProtocolException",
				"org.apache.http.client.CookieStore",
				"org.apache.http.client.CredentialsProvider",
				"org.apache.http.client.entity.UrlEncodedFormEntity",
				"org.apache.http.client.HttpClient",
				"org.apache.http.client.HttpRequestRetryHandler",
				"org.apache.http.client.HttpResponseException",
				"org.apache.http.client.methods.AbortableHttpRequest",
				"org.apache.http.client.methods.HttpDelete",
				"org.apache.http.client.methods.HttpEntityEnclosingRequestBase",
				"org.apache.http.client.methods.HttpGet",
				"org.apache.http.client.methods.HttpHead",
				"org.apache.http.client.methods.HttpOptions",
				"org.apache.http.client.methods.HttpPost",
				"org.apache.http.client.methods.HttpPut",
				"org.apache.http.client.methods.HttpRequestBase",
				"org.apache.http.client.methods.HttpTrace",
				"org.apache.http.client.methods.HttpUriRequest",
				"org.apache.http.client.NonRepeatableRequestException",
				"org.apache.http.client.params.AllClientPNames",
				"org.apache.http.client.params.ClientParamBean",
				"org.apache.http.client.params.ClientPNames",
				"org.apache.http.client.protocol.ClientContext",
				"org.apache.http.client.protocol.ClientContextConfigurer",
				"org.apache.http.client.protocol.RequestAddCookies",
				"org.apache.http.client.protocol.RequestDefaultHeaders",
				"org.apache.http.client.protocol.RequestProxyAuthentication",
				"org.apache.http.client.protocol.RequestTargetAuthentication",
				"org.apache.http.client.protocol.ResponseProcessCookies",
				"org.apache.http.client.RedirectException",
				"org.apache.http.client.RedirectHandler",
				"org.apache.http.client.RequestDirector",
				"org.apache.http.client.ResponseHandler",
				"org.apache.http.client.UserTokenHandler",
				"org.apache.http.client.utils.URLEncodedUtils",
				"org.apache.http.conn.BasicEofSensorWatcher",
				"org.apache.http.conn.BasicManagedEntity",
				"org.apache.http.conn.ClientConnectionManager",
				"org.apache.http.conn.ClientConnectionManagerFactory",
				"org.apache.http.conn.ClientConnectionOperator",
				"org.apache.http.conn.ClientConnectionRequest",
				"org.apache.http.conn.ConnectionKeepAliveStrategy",
				"org.apache.http.conn.ConnectionPoolTimeoutException",
				"org.apache.http.conn.ConnectionReleaseTrigger",
				"org.apache.http.conn.ConnectTimeoutException",
				"org.apache.http.conn.EofSensorInputStream",
				"org.apache.http.conn.EofSensorWatcher",
				"org.apache.http.conn.HttpHostConnectException",
				"org.apache.http.conn.ManagedClientConnection",
				"org.apache.http.conn.OperatedClientConnection",
				"org.apache.http.conn.params.ConnConnectionParamBean",
				"org.apache.http.conn.params.ConnConnectionPNames",
				"org.apache.http.conn.params.ConnManagerParamBean",
				"org.apache.http.conn.params.ConnManagerPNames",
				"org.apache.http.conn.params.ConnPerRoute",
				"org.apache.http.conn.params.ConnRouteParamBean",
				"org.apache.http.conn.params.ConnRoutePNames",
				"org.apache.http.conn.routing.BasicRouteDirector",
				"org.apache.http.conn.routing.HttpRouteDirector",
				"org.apache.http.conn.routing.HttpRoutePlanner",
				"org.apache.http.conn.routing.RouteInfo",
				"org.apache.http.conn.scheme.HostNameResolver",
				"org.apache.http.conn.scheme.LayeredSocketFactory",
				"org.apache.http.conn.scheme.SocketFactory",
				"org.apache.http.conn.ssl.AbstractVerifier",
				"org.apache.http.conn.ssl.AllowAllHostnameVerifier",
				"org.apache.http.conn.ssl.BrowserCompatHostnameVerifier",
				"org.apache.http.conn.ssl.SSLSocketFactory",
				"org.apache.http.conn.ssl.StrictHostnameVerifier",
				"org.apache.http.conn.ssl.X509HostnameVerifier",
				"org.apache.http.ConnectionClosedException",
				"org.apache.http.ConnectionReuseStrategy",
				"org.apache.http.cookie.ClientCookie",
				"org.apache.http.cookie.Cookie",
				"org.apache.http.cookie.CookieAttributeHandler",
				"org.apache.http.cookie.CookieIdentityComparator",
				"org.apache.http.cookie.CookiePathComparator",
				"org.apache.http.cookie.CookieSpec",
				"org.apache.http.cookie.CookieSpecFactory",
				"org.apache.http.cookie.MalformedCookieException",
				"org.apache.http.cookie.params.CookieSpecParamBean",
				"org.apache.http.cookie.params.CookieSpecPNames",
				"org.apache.http.cookie.SetCookie",
				"org.apache.http.cookie.SetCookie2",
				"org.apache.http.cookie.SM",
				"org.apache.http.entity.BasicHttpEntity",
				"org.apache.http.entity.BufferedHttpEntity",
				"org.apache.http.entity.ByteArrayEntity",
				"org.apache.http.entity.ContentLengthStrategy",
				"org.apache.http.entity.ContentProducer",
				"org.apache.http.entity.EntityTemplate",
				"org.apache.http.entity.FileEntity",
				"org.apache.http.entity.HttpEntityWrapper",
				"org.apache.http.entity.InputStreamEntity",
				"org.apache.http.entity.SerializableEntity",
				"org.apache.http.entity.StringEntity",
				"org.apache.http.FormattedHeader",
				"org.apache.http.Header",
				"org.apache.http.HeaderElement",
				"org.apache.http.HeaderElementIterator",
				"org.apache.http.HeaderIterator",
				"org.apache.http.HttpClientConnection",
				"org.apache.http.HttpConnection",
				"org.apache.http.HttpConnectionMetrics",
				"org.apache.http.HttpEntity",
				"org.apache.http.HttpEntityEnclosingRequest",
				"org.apache.http.HttpException",
				"org.apache.http.HttpInetConnection",
				"org.apache.http.HttpMessage",
				"org.apache.http.HttpRequest",
				"org.apache.http.HttpRequestFactory",
				"org.apache.http.HttpRequestInterceptor",
				"org.apache.http.HttpResponse",
				"org.apache.http.HttpResponseFactory",
				"org.apache.http.HttpResponseInterceptor",
				"org.apache.http.HttpServerConnection",
				"org.apache.http.HttpStatus",
				"org.apache.http.impl.AbstractHttpClientConnection",
				"org.apache.http.impl.AbstractHttpServerConnection",
				"org.apache.http.impl.auth.AuthSchemeBase",
				"org.apache.http.impl.auth.BasicScheme",
				"org.apache.http.impl.auth.BasicSchemeFactory",
				"org.apache.http.impl.auth.DigestScheme",
				"org.apache.http.impl.auth.DigestSchemeFactory",
				"org.apache.http.impl.auth.NTLMEngine",
				"org.apache.http.impl.auth.NTLMEngineException",
				"org.apache.http.impl.auth.NTLMScheme",
				"org.apache.http.impl.auth.RFC2617Scheme",
				"org.apache.http.impl.auth.UnsupportedDigestAlgorithmException",
				"org.apache.http.impl.client.AbstractAuthenticationHandler",
				"org.apache.http.impl.client.BasicCookieStore",
				"org.apache.http.impl.client.BasicCredentialsProvider",
				"org.apache.http.impl.client.BasicResponseHandler",
				"org.apache.http.impl.client.ClientParamsStack",
				"org.apache.http.impl.client.DefaultConnectionKeepAliveStrategy",
				"org.apache.http.impl.client.DefaultHttpClient",
				"org.apache.http.impl.client.DefaultHttpRequestRetryHandler",
				"org.apache.http.impl.client.DefaultProxyAuthenticationHandler",
				"org.apache.http.impl.client.DefaultRedirectHandler",
				"org.apache.http.impl.client.DefaultRequestDirector",
				"org.apache.http.impl.client.DefaultTargetAuthenticationHandler",
				"org.apache.http.impl.client.DefaultUserTokenHandler",
				"org.apache.http.impl.client.EntityEnclosingRequestWrapper",
				"org.apache.http.impl.client.RedirectLocations",
				"org.apache.http.impl.client.RequestWrapper",
				"org.apache.http.impl.client.RoutedRequest",
				"org.apache.http.impl.client.TunnelRefusedException",
				"org.apache.http.impl.conn.DefaultClientConnection",
				"org.apache.http.impl.conn.DefaultClientConnectionOperator",
				"org.apache.http.impl.conn.DefaultHttpRoutePlanner",
				"org.apache.http.impl.conn.DefaultResponseParser",
				"org.apache.http.impl.conn.IdleConnectionHandler",
				"org.apache.http.impl.conn.LoggingSessionInputBuffer",
				"org.apache.http.impl.conn.LoggingSessionOutputBuffer",
				"org.apache.http.impl.conn.ProxySelectorRoutePlanner",
				"org.apache.http.impl.conn.SingleClientConnManager",
				"org.apache.http.impl.conn.tsccm.BasicPoolEntry",
				"org.apache.http.impl.conn.tsccm.BasicPoolEntryRef",
				"org.apache.http.impl.conn.tsccm.ConnPoolByRoute",
				"org.apache.http.impl.conn.tsccm.PoolEntryRequest",
				"org.apache.http.impl.conn.tsccm.RefQueueHandler",
				"org.apache.http.impl.conn.tsccm.RefQueueWorker",
				"org.apache.http.impl.conn.tsccm.RouteSpecificPool",
				"org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager",
				"org.apache.http.impl.conn.tsccm.WaitingThread",
				"org.apache.http.impl.conn.tsccm.WaitingThreadAborter",
				"org.apache.http.impl.conn.Wire",
				"org.apache.http.impl.cookie.AbstractCookieAttributeHandler",
				"org.apache.http.impl.cookie.AbstractCookieSpec",
				"org.apache.http.impl.cookie.BasicClientCookie",
				"org.apache.http.impl.cookie.BasicClientCookie2",
				"org.apache.http.impl.cookie.BasicCommentHandler",
				"org.apache.http.impl.cookie.BasicDomainHandler",
				"org.apache.http.impl.cookie.BasicExpiresHandler",
				"org.apache.http.impl.cookie.BasicMaxAgeHandler",
				"org.apache.http.impl.cookie.BasicPathHandler",
				"org.apache.http.impl.cookie.BasicSecureHandler",
				"org.apache.http.impl.cookie.BestMatchSpec",
				"org.apache.http.impl.cookie.BestMatchSpecFactory",
				"org.apache.http.impl.cookie.BrowserCompatSpec",
				"org.apache.http.impl.cookie.BrowserCompatSpecFactory",
				"org.apache.http.impl.cookie.CookieSpecBase",
				"org.apache.http.impl.cookie.DateParseException",
				"org.apache.http.impl.cookie.NetscapeDomainHandler",
				"org.apache.http.impl.cookie.NetscapeDraftHeaderParser",
				"org.apache.http.impl.cookie.NetscapeDraftSpec",
				"org.apache.http.impl.cookie.NetscapeDraftSpecFactory",
				"org.apache.http.impl.cookie.RFC2109DomainHandler",
				"org.apache.http.impl.cookie.RFC2109Spec",
				"org.apache.http.impl.cookie.RFC2109SpecFactory",
				"org.apache.http.impl.cookie.RFC2109VersionHandler",
				"org.apache.http.impl.cookie.RFC2965CommentUrlAttributeHandler",
				"org.apache.http.impl.cookie.RFC2965DiscardAttributeHandler",
				"org.apache.http.impl.cookie.RFC2965DomainAttributeHandler",
				"org.apache.http.impl.cookie.RFC2965PortAttributeHandler",
				"org.apache.http.impl.cookie.RFC2965Spec",
				"org.apache.http.impl.cookie.RFC2965SpecFactory",
				"org.apache.http.impl.cookie.RFC2965VersionAttributeHandler",
				"org.apache.http.impl.DefaultConnectionReuseStrategy",
				"org.apache.http.impl.DefaultHttpClientConnection",
				"org.apache.http.impl.DefaultHttpRequestFactory",
				"org.apache.http.impl.DefaultHttpResponseFactory",
				"org.apache.http.impl.DefaultHttpServerConnection",
				"org.apache.http.impl.entity.EntityDeserializer",
				"org.apache.http.impl.entity.EntitySerializer",
				"org.apache.http.impl.entity.LaxContentLengthStrategy",
				"org.apache.http.impl.entity.StrictContentLengthStrategy",
				"org.apache.http.impl.HttpConnectionMetricsImpl",
				"org.apache.http.impl.io.AbstractMessageParser",
				"org.apache.http.impl.io.AbstractMessageWriter",
				"org.apache.http.impl.io.AbstractSessionInputBuffer",
				"org.apache.http.impl.io.AbstractSessionOutputBuffer",
				"org.apache.http.impl.io.ChunkedInputStream",
				"org.apache.http.impl.io.ChunkedOutputStream",
				"org.apache.http.impl.io.ContentLengthInputStream",
				"org.apache.http.impl.io.ContentLengthOutputStream",
				"org.apache.http.impl.io.HttpRequestParser",
				"org.apache.http.impl.io.HttpRequestWriter",
				"org.apache.http.impl.io.HttpResponseParser",
				"org.apache.http.impl.io.HttpResponseWriter",
				"org.apache.http.impl.io.HttpTransportMetricsImpl",
				"org.apache.http.impl.io.IdentityInputStream",
				"org.apache.http.impl.io.IdentityOutputStream",
				"org.apache.http.impl.io.SocketInputBuffer",
				"org.apache.http.impl.io.SocketOutputBuffer",
				"org.apache.http.impl.NoConnectionReuseStrategy",
				"org.apache.http.impl.SocketHttpClientConnection",
				"org.apache.http.impl.SocketHttpServerConnection",
				"org.apache.http.io.HttpMessageParser",
				"org.apache.http.io.HttpMessageWriter",
				"org.apache.http.io.HttpTransportMetrics",
				"org.apache.http.io.SessionInputBuffer",
				"org.apache.http.io.SessionOutputBuffer",
				"org.apache.http.MalformedChunkCodingException",
				"org.apache.http.message.BasicHeader",
				"org.apache.http.message.BasicHeaderElement",
				"org.apache.http.message.BasicHeaderElementIterator",
				"org.apache.http.message.BasicHeaderIterator",
				"org.apache.http.message.BasicHeaderValueFormatter",
				"org.apache.http.message.BasicHeaderValueParser",
				"org.apache.http.message.BasicHttpEntityEnclosingRequest",
				"org.apache.http.message.BasicHttpRequest",
				"org.apache.http.message.BasicHttpResponse",
				"org.apache.http.message.BasicLineFormatter",
				"org.apache.http.message.BasicLineParser",
				"org.apache.http.message.BasicListHeaderIterator",
				"org.apache.http.message.BasicNameValuePair",
				"org.apache.http.message.BasicRequestLine",
				"org.apache.http.message.BasicStatusLine",
				"org.apache.http.message.BasicTokenIterator",
				"org.apache.http.message.BufferedHeader",
				"org.apache.http.message.HeaderGroup",
				"org.apache.http.message.HeaderValueFormatter",
				"org.apache.http.message.HeaderValueParser",
				"org.apache.http.message.LineFormatter",
				"org.apache.http.message.LineParser",
				"org.apache.http.message.ParserCursor",
				"org.apache.http.MethodNotSupportedException",
				"org.apache.http.NameValuePair",
				"org.apache.http.NoHttpResponseException",
				"org.apache.http.params.CoreConnectionPNames",
				"org.apache.http.params.CoreProtocolPNames",
				"org.apache.http.params.HttpAbstractParamBean",
				"org.apache.http.params.HttpConnectionParamBean",
				"org.apache.http.params.HttpParams",
				"org.apache.http.params.HttpProtocolParamBean",
				"org.apache.http.ParseException",
				"org.apache.http.protocol.BasicHttpContext",
				"org.apache.http.protocol.ExecutionContext",
				"org.apache.http.protocol.HttpContext",
				"org.apache.http.protocol.HttpDateGenerator",
				"org.apache.http.protocol.HttpExpectationVerifier",
				"org.apache.http.protocol.HttpProcessor",
				"org.apache.http.protocol.HttpRequestExecutor",
				"org.apache.http.protocol.HttpRequestHandler",
				"org.apache.http.protocol.HttpRequestHandlerRegistry",
				"org.apache.http.protocol.HttpRequestHandlerResolver",
				"org.apache.http.protocol.HttpRequestInterceptorList",
				"org.apache.http.protocol.HttpResponseInterceptorList",
				"org.apache.http.protocol.HttpService",
				"org.apache.http.protocol.RequestConnControl",
				"org.apache.http.protocol.RequestContent",
				"org.apache.http.protocol.RequestDate",
				"org.apache.http.protocol.RequestExpectContinue",
				"org.apache.http.protocol.RequestTargetHost",
				"org.apache.http.protocol.RequestUserAgent",
				"org.apache.http.protocol.ResponseConnControl",
				"org.apache.http.protocol.ResponseContent",
				"org.apache.http.protocol.ResponseDate",
				"org.apache.http.protocol.ResponseServer",
				"org.apache.http.protocol.SyncBasicHttpContext",
				"org.apache.http.protocol.UriPatternMatcher",
				"org.apache.http.ProtocolException",
				"org.apache.http.ProtocolVersion",
				"org.apache.http.ReasonPhraseCatalog",
				"org.apache.http.RequestLine",
				"org.apache.http.StatusLine",
				"org.apache.http.TokenIterator",
				"org.apache.http.UnsupportedHttpVersionException",
				"org.json.JSONArray",
				"org.json.JSONException",
				"org.json.JSONObject",
				"org.json.JSONStringer",
				"org.json.JSONTokener",
				"org.w3c.dom.Attr",
				"org.w3c.dom.CDATASection",
				"org.w3c.dom.CharacterData",
				"org.w3c.dom.Comment",
				"org.w3c.dom.Document",
				"org.w3c.dom.DocumentFragment",
				"org.w3c.dom.DocumentType",
				"org.w3c.dom.DOMConfiguration",
				"org.w3c.dom.DOMError",
				"org.w3c.dom.DOMErrorHandler",
				"org.w3c.dom.DOMException",
				"org.w3c.dom.DOMImplementation",
				"org.w3c.dom.DOMImplementationList",
				"org.w3c.dom.DOMImplementationSource",
				"org.w3c.dom.DOMLocator",
				"org.w3c.dom.DOMStringList",
				"org.w3c.dom.Element",
				"org.w3c.dom.Entity",
				"org.w3c.dom.EntityReference",
				"org.w3c.dom.ls.DOMImplementationLS",
				"org.w3c.dom.ls.LSException",
				"org.w3c.dom.ls.LSInput",
				"org.w3c.dom.ls.LSOutput",
				"org.w3c.dom.ls.LSParser",
				"org.w3c.dom.ls.LSParserFilter",
				"org.w3c.dom.ls.LSResourceResolver",
				"org.w3c.dom.ls.LSSerializer",
				"org.w3c.dom.NamedNodeMap",
				"org.w3c.dom.NameList",
				"org.w3c.dom.Node",
				"org.w3c.dom.NodeList",
				"org.w3c.dom.Notation",
				"org.w3c.dom.ProcessingInstruction",
				"org.w3c.dom.Text",
				"org.w3c.dom.TypeInfo",
				"org.w3c.dom.UserDataHandler",
				"org.xml.sax.AttributeList",
				"org.xml.sax.Attributes",
				"org.xml.sax.ContentHandler",
				"org.xml.sax.DocumentHandler",
				"org.xml.sax.DTDHandler",
				"org.xml.sax.EntityResolver",
				"org.xml.sax.ErrorHandler",
				"org.xml.sax.ext.Attributes2",
				"org.xml.sax.ext.Attributes2Impl",
				"org.xml.sax.ext.DeclHandler",
				"org.xml.sax.ext.DefaultHandler2",
				"org.xml.sax.ext.EntityResolver2",
				"org.xml.sax.ext.LexicalHandler",
				"org.xml.sax.ext.Locator2",
				"org.xml.sax.ext.Locator2Impl",
				"org.xml.sax.HandlerBase",
				"org.xml.sax.helpers.AttributeListImpl",
				"org.xml.sax.helpers.AttributesImpl",
				"org.xml.sax.helpers.DefaultHandler",
				"org.xml.sax.helpers.LocatorImpl",
				"org.xml.sax.helpers.NamespaceSupport",
				"org.xml.sax.helpers.ParserAdapter",
				"org.xml.sax.helpers.XMLFilterImpl",
				"org.xml.sax.helpers.XMLReaderAdapter",
				"org.xml.sax.InputSource",
				"org.xml.sax.Locator",
				"org.xml.sax.Parser",
				"org.xml.sax.SAXException",
				"org.xml.sax.SAXNotRecognizedException",
				"org.xml.sax.SAXNotSupportedException",
				"org.xml.sax.SAXParseException",
				"org.xml.sax.XMLFilter",
				"org.xml.sax.XMLReader",
				"org.xmlpull.v1.sax2.Driver",
				"org.xmlpull.v1.XmlPullParser",
				"org.xmlpull.v1.XmlPullParserException",
				"org.xmlpull.v1.XmlSerializer"
		};
		
		
		for (String clazz : classes)
		{
			ApplicationWriter aw = new ApplicationWriter();
			aw.visit();
			Dump dump = new Dump();
			Class<?> classForName = Class.forName(clazz);
			Log.d("Binding.Gen.Test", "Generating Class " +  classForName.getName());
			dump.generateProxy(aw, classForName);
			aw.visitEnd();
			byte[] generatedBytes = aw.toByteArray();
		}
	}
}
