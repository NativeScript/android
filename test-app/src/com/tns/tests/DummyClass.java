package com.tns.tests;

import android.util.Log;

public class DummyClass
{
	private static class DummyClassImpl extends DummyClass
	{
	}
	
	public static class DummyDerivedClass extends DummyClass
	{
		@Override
		public String dummyMethod(DummyClass dummy)
		{
			return super.dummyMethod(dummy);
		}
	}
	
	public static interface MyInterface
	{
		int echoInt(int i);
		
		void doSomething();
	}

	public String nameField;
	
	public DummyClass()
	{
		nameField = "dummy";
		
		arrIntField = new int[] { 11, 22, 33, 44, 55 };
		arrIntField2 = null;
		arrStringField = new String[] { "aa", "bb", "cc", "dd", "ee" };
		arrLongField = new long[] { (long)1 << 0, (long)1 << 10, (long)1 << 20, (long)1 << 30, (long)1 << 40, (long)1 << 50, (long)1 << 60 }; 
	}
	
	public DummyClass(boolean throwException) throws Exception
	{
		if (throwException)
		{
			try
			{
				someMethodThatThrowsException();	
			}
			catch (Exception e)
			{
				throw new Exception("This is wrapped exception from constructor", e);
			}
		}
	}
	
	public DummyClass(String name)
	{
		this.nameField = name;
	}
	
	public int dummyMethod(int value)
	{
		return value;
	}
	
	public String executeCallback()
	{
		return dummyMethod(new DummyClassImpl());
	}
	
	public String dummyMethod(DummyClass dummy)
	{
		return dummy.nameField;
	}
	
	public String getName()
	{
		return nameField;
	}
	
	public Object getNameAsObject()
	{
		return nameField;
	}
	
	public CharSequence getNameAsCharSequence()
	{
		return nameField;
	}

	public void setName(String value)
	{
		nameField = value;
	}
	
	public String getname2()
	{
		return nameField;
	}

	public void setname2(String value)
	{
		nameField = value;
	}

	public Object getDummyClassAsObject()
	{
		return new DummyClassImpl();
	}
	
	public Object getDummyClassArrayAsObject()
	{
		DummyClassImpl[] arr = new DummyClassImpl[1];
		arr[0] = new DummyClassImpl();
		return arr;
	}
	
	public void setDummyField()
	{
		dummyField = new DummyClassImpl();
	}
	
	public Object dummyField;
	
	public void methodThatThrowsException() throws Exception
	{
		try
		{
			someMethodThatThrowsException();
		}
		catch (Exception e)
		{
			throw new Exception("This is wrapped exception throwns on purpose", e);
		}
	}
	
	private void someMethodThatThrowsException() throws Exception
	{
		throw new Exception("This exception is thrown on purpose");
	}
	
	public Object nullField = null;
	
	public Object getNull()
	{
		return null;
	}
	
	public String getNullString()
	{
		return null;
	}
	
	public Object[] getArrayWithNullElement()
	{
		Object[] arr = new Object[1];
		arr[0] = null;
		return arr;
	}
	
	public String concatIntArrayAsString(int... params)
	{
		String s = "";
		
		if (params != null)
		{
			for (int i=0; i<params.length; i++)
			{
				s += params[i];
			}
		}
		
		return s;
	}
	
	public String concatCharArrayAsString(char... params)
	{
		String s = "";
		
		if (params != null)
		{
			for (int i=0; i<params.length; i++)
			{
				s += params[i];
			}
		}
		
		return s;
	}

	public String concatObjectArrayAsString(Object... params)
	{
		String s = "";
		
		if (params != null)
		{
			for (int i=0; i<params.length; i++)
			{
				if (i > 0)
				{
					s += ", ";
				}

				Object obj = params[i];
				
				if (obj != null)
				{
					s += obj.toString();
				}
			}
		}
		
		return s;
	}
	
	public double getMaxDouble()
	{
		double d = Double.MAX_VALUE;
		Log.d(logTag, "Double.MAX_VALUE=" + d);
		return d;
	}
	
	public double getMinDouble()
	{
		double d = Double.MIN_VALUE;
		Log.d(logTag, "Double.MIN_VALUE=" + d);
		return d;
	}
	
	public long getMaxLong()
	{
		long l = Long.MAX_VALUE;
		Log.d(logTag, "Long.MAX_VALUE=" + l);
		return l;
	}
	
	public long getMinLong()
	{
		long l = Long.MIN_VALUE;
		Log.d(logTag, "Long.MIN_VALUE=" + l);
		return l;
	}

	public int[] arrIntField;
	
	public int[] arrIntField2;
	
	public String[] arrStringField;
	
	public long[] arrLongField;
	
	public long longField;
	
	public String getLongAsString(long value)
	{
		return "" + value;
	}
	
	public String method1(int value)
	{
		return "int=" + value;
	}

	public String method1(byte value)
	{
		return "byte=" + value;
	}

	public String method1(long value)
	{
		return "long=" + value;
	}

	public String method1(short value)
	{
		return "short=" + value;
	}

	public String method1(boolean value)
	{
		return "boolean=" + value;
	}
	
	public String method2(Object obj)
	{
		String ret = "obj=" + ((obj != null) ? obj.toString() : "<null>");
		Log.d("NativeScript.Java", "inside DummyClass.method2 with ret=" + ret);
		return ret;
	}
	
	public String methodWithoutOverloads(float value)
	{
		return "float=" + value;
	}
	
	public int triggerEchoInt(MyInterface impl, int i)
	{
		int ret = impl.echoInt(i);
		return ret;
	}

	public void triggerDoSomething(MyInterface impl)
	{
		impl.doSomething();
	}
	
	private final String logTag = "TNS.Java";
}
