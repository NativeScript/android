package com.proxy;

import java.lang.reflect.Array;

import com.tns.Platform;

import android.content.Context;
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
			Log.d("TNS.JAVA.TEST", "InnerButton ctor() called");
		}

		public InnerButton(String s)
		{
			Log.d("TNS.JAVA.TEST", "InnerButton ctor(String:" + s + ") called");
		}
		
		public InnerButton(int i)
		{
			Log.d("TNS.JAVA.TEST", "InnerButton ctor(int:" + i + ") called");
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
			Log.d("TNS.Java", "InnerStaticClass ctor called with " + i);
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
	

	public Button1()
	{
		Log.d("TNS.JAVA.TEST", "Button instance created from javascript");
	}
	
	public Button1(int value)
	{
		Log.d("TNS.JAVA.TEST", "Button instance created from javascript with number " + value);
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
		Log.d("TNS.JAVA.TEST", "Button: setText called text:" + text);
	}

	public void setLabel(String text)
	{
		Log.d("TNS.JAVA.TEST", "Button: setLabel called text:" + text);
	}

	public void setOnClickListener(android.view.View.OnClickListener l)
	{
		mOnClickListener = l;
		if (l instanceof OnClickListener)
		{
			Log.d("TNS.JAVA.TEST", "Button: onClickListener called l:" + "TelerikOnClickListener");
		}
		else
		{
			Log.d("TNS.JAVA.TEST", "Button: onClickListener called l:" + l.toString());
		}
	}

	public void click(android.view.View view)
	{
		mOnClickListener.onClick(view);
	}

	
	

	
	public void methodDummyClassAsObjectInArgs(Object arg)
	{
		Log.d("TNS.JAVA.TEST", "methodDummyClassAsObjectInArgs called");
	}

	

	

	public void method1(int value)
	{
		Log.d("TNS.JAVA.TEST", "method1(int): " + value);
	}

	
	// public void method1(Byte value)
	public void method1(byte value)
	{
		Log.d("TNS.JAVA.TEST", "method1(byte): " + value);
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


	public void printSomeArray(char[] arr)
	{
		for (int i = 0; i < arr.length; i++)
		{
			Log.d("TNS.JAVA.TEST", "printSomeArray[" + i + "]=" + arr[i]);
		}
	}

	public int[] getIntArray1(int length)
	{
		Log.d("TNS.JAVA.TEST", "called getIntArray1 with " + length);
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
			Log.d("TNS.JAVA.TEST", "printArray1 null");
		}
		else
		{
			for (int i = 0; i < arr.length; i++)
			{
				Log.d("TNS.JAVA.TEST", "printArray1[" + i + "]=" + arr[i]);
			}
		}
	}

	public void printArray2(int[][] arr)
	{
		if (arr == null)
		{
			Log.d("TNS.JAVA.TEST", "printArray2 null");
		}
		else
		{
			for (int i = 0; i < arr.length; i++)
			{
				int[] inner = arr[i];
				if (inner == null)
				{
					Log.d("TNS.JAVA.TEST", "printArray2 arr[" + i + "] is null");
				}
				else
				{
					for (int j = 0; j < inner.length; j++)
					{
						Log.d("TNS.JAVA.TEST", "printArray2[" + i + "][" + j + "]=" + arr[i][j]);
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
		Log.d("TNS.JAVA.TEST", "CharArg called arg:" + arg);
		return arg;
	}

	public float FloatArg(float arg)
	{
		Log.d("TNS.JAVA.TEST", "FloatArg called arg:" + arg);
		return arg;
	}

	public double DoubleArg(double arg)
	{
		Log.d("TNS.JAVA.TEST", "DoubleArg called arg:" + arg);
		return arg;
	}

	public static void someMethod(int i, String s)
	{
		Log.d("TNS.JAVA.TEST", "static someMethod " + i + " " + s);
	}

	public void someMethod(int i, Object o)
	{
		Log.d("TNS.JAVA.TEST", "static someMethod " + i + " " + o);
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
}
