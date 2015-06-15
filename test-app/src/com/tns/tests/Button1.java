package com.tns.tests;

import android.util.Log;
import android.view.View.OnClickListener;

public class Button1 extends ButtonBase
{
	public class InnerButton
	{
		public class InnerClass2
		{
			private int i;

			public InnerClass2()
			{
				this.i = 12345678;
			}

			public InnerClass2(int i)
			{
				this.i = i;
			}

			public String getSomeString2()
			{
				return "this is called from InnerClass2 with " + i;
			}
		}

		public InnerButton()
		{
			Log.d(logTag, "InnerButton ctor() called");
		}

		public InnerButton(String s)
		{
			Log.d(logTag, "InnerButton ctor(String:" + s + ") called");
		}
		
		public InnerButton(int i)
		{
			Log.d(logTag, "InnerButton ctor(int:" + i + ") called");
		}

		public String getSomeString()
		{
			return "this is called from InnerClass";
		}
	}

	public static class InnerStaticClass
	{
		private int i;

		public InnerStaticClass(int i)
		{
			this.i = i;
			Log.d(logTag, "InnerStaticClass ctor called with " + i);
		}

		public int getInt()
		{
			return this.i;
		}
	}

	private OnClickListener mOnClickListener;

	public static final String STATIC_IMAGE_ID = "static image id";
	public final String IMAGE_ID = "image id";

	private String IMAGE_ID_PROP = "image id prop";

	private boolean IMAGE_ID_BOOL_PROP = false;
	
	public String GetStaticImageId()
	{
		return STATIC_IMAGE_ID;
	}
	
	public static String SGetStaticImageId()
	{
		return STATIC_IMAGE_ID;
	}
	
	public Button1()
	{
		Log.d(logTag, "Button instance created from javascript");
	}
	
	public Button1(int value)
	{
		Log.d(logTag, "Button instance created from javascript with number " + value);
	}
	
	public String echo(String s) throws Throwable
	{
		return s;
	}
	
	public String triggerEcho(String s) throws Throwable
	{
		String ret = echo(s);
		return ret;
	}
	
	public Object triggerEchoAsObject(String s) throws Throwable
	{
		String ret = echo(s);
		return ret;
	}

	public static void setText(String text)
	{
		Log.d(logTag, "Button: setText called text:" + text);
	}

	public void setLabel(String text)
	{
		Log.d(logTag, "Button: setLabel called text:" + text);
	}

	public void setOnClickListener(android.view.View.OnClickListener l)
	{
		mOnClickListener = l;
		if (l instanceof OnClickListener)
		{
			Log.d(logTag, "Button: onClickListener called l:" + "TelerikOnClickListener");
		}
		else
		{
			Log.d(logTag, "Button: onClickListener called l:" + l.toString());
		}
	}

	public void click(android.view.View view)
	{
		mOnClickListener.onClick(view);
	}

	public DummyClass getDummy()
	{
		Log.d(logTag, "getDummy called");
		return new DummyClass();
	}

	public Object[] getDummyClassAsObjectArray()
	{
		Log.d(logTag, "getDummy called");
		return new DummyClass[]
		{ new DummyClass() };
	}

	public Object DummyClassAsObjectField = new DummyClass();

	public Object getDummyClassAsObject()
	{
		Log.d(logTag, "getDummyClassAsObject called");
		return new DummyClass("dummy");
	}

	public void methodDummyClassAsObjectInArgs(Object arg)
	{
		Log.d(logTag, "methodDummyClassAsObjectInArgs called");
	}

	public void callMethodDummyClassAsObjectInArgs()
	{
		methodDummyClassAsObjectInArgs(new DummyClass());
	}

	public DummyClass method1(String value)
	{
		Log.d(logTag, "method1(String): " + value);
		return new DummyClass();
	}

	public void method1(int value)
	{
		Log.d(logTag, "method1(int): " + value);
	}

	public void method1(DummyClass value)
	{
		Log.d(logTag, "method1(DummyClass): " + value);
	}

	// public void method1(Byte value)
	public void method1(byte value)
	{
		Log.d(logTag, "method1(byte): " + value);
	}
	
	public String method2(int value)
	{
		return "int=" + value;
	}
	
	public String callMethod2WithInt()
	{
		return method2((int)1);
	}
	
	public String callMethod2WithByte()
	{
		return method2((byte)5);
	}

	public String method2(byte value)
	{
		return "byte=" + value;
	}

	public String getIMAGE_ID_PROP()
	{
		return IMAGE_ID_PROP;
	}
	
	public String getMeANullString()
	{
		return null;
	}

	public void setIMAGE_ID_PROP(String iMAGE_ID_PROP)
	{
		IMAGE_ID_PROP = iMAGE_ID_PROP;
	}

	public boolean isIMAGE_ID_BOOL_PROP()
	{
		return IMAGE_ID_BOOL_PROP;
	}

	public void setIMAGE_ID_BOOL_PROP(boolean iMAGE_ID_BOOL_PROP)
	{
		IMAGE_ID_BOOL_PROP = iMAGE_ID_BOOL_PROP;
	}

	public char[] getSomeArray(int length)
	{
		char[] arr = new char[length];
		if (length > 0)
		{
			arr[0] = 'x';
		}
		return arr;
	}

	public DummyClass[] getDummyInstances()
	{
		return new DummyClass[]
		{ new DummyClass("first"), new DummyClass("second") };
	}

	public void printSomeArray(char[] arr)
	{
		for (int i = 0; i < arr.length; i++)
		{
			Log.d(logTag, "printSomeArray[" + i + "]=" + arr[i]);
		}
	}

	public int[] getIntArray1(int length)
	{
		Log.d(logTag, "called getIntArray1 with " + length);
		int[] arr = new int[length];
		if (length > 0)
		{
			arr[0] = 112233;
		}
		return arr;
	}

	public int[][] getIntArray2(int length)
	{
		int[][] arr = new int[length][];
		if (length > 0)
		{
			arr[0] = new int[10];
			arr[0][5] = 56789;
		}
		return arr;
	}

	public void printArray1(int[] arr)
	{
		if (arr == null)
		{
			Log.d(logTag, "printArray1 null");
		}
		else
		{
			for (int i = 0; i < arr.length; i++)
			{
				Log.d(logTag, "printArray1[" + i + "]=" + arr[i]);
			}
		}
	}

	public void printArray2(int[][] arr)
	{
		if (arr == null)
		{
			Log.d(logTag, "printArray2 null");
		}
		else
		{
			for (int i = 0; i < arr.length; i++)
			{
				int[] inner = arr[i];
				if (inner == null)
				{
					Log.d(logTag, "printArray2 arr[" + i + "] is null");
				}
				else
				{
					for (int j = 0; j < inner.length; j++)
					{
						Log.d(logTag, "printArray2[" + i + "][" + j + "]=" + arr[i][j]);
					}
				}
			}
		}
	}

	public int IntArg(int arg)
	{
		return arg;
	}

	public byte ByteArg(byte arg)
	{
		return arg;
	}

	public short ShortArg(short arg)
	{
		return arg;
	}

	public boolean BooleanArg(boolean arg)
	{
		return arg;
	}

	public long LongArg(long arg)
	{
		return arg;
	}

	public char CharArg(char arg)
	{
		Log.d(logTag, "CharArg called arg:" + arg);
		return arg;
	}

	public float FloatArg(float arg)
	{
		Log.d(logTag, "FloatArg called arg:" + arg);
		return arg;
	}

	public double DoubleArg(double arg)
	{
		Log.d(logTag, "DoubleArg called arg:" + arg);
		return arg;
	}

	public static void someMethod(int i, String s)
	{
		Log.d(logTag, "static someMethod " + i + " " + s);
	}

	public void someMethod(int i, Object o)
	{
		Log.d(logTag, "static someMethod " + i + " " + o);
	}

	public static int getMyStaticIntField()
	{
		return myStaticIntField;
	}

	public static void setMyStaticIntField(int myStaticIntField)
	{
		Button1.myStaticIntField = myStaticIntField;
	}

	private static int myStaticIntField;
	
	private final static String logTag = "TNS.JAVA.TEST";
}
