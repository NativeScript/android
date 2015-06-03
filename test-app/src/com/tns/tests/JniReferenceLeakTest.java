package com.tns.tests;

public class JniReferenceLeakTest {
	
	public static interface Processor
	{
		char echoChar(char c);
		int echoInt(int i);
		String echoString(String s);
		Object echoObject(Object o);
		char[] echoCharArray(char[] arr);
		int[] echoIntArray(int[] arr);
		String[] echoStringArray(String[] arr);
		Object[] echoObjectArray(Object[] arr);
	}
	
	private final Processor processor;
	
	public JniReferenceLeakTest(Processor processor)
	{
		this.processor = processor;
	}
	
	public byte byteField;
	public short shortField;
	public int intField;
	public long longField;
	public float floatField;
	public double doubleField;
	public char charField;
	public boolean booleanField;
	public String stringField;
	public Object objectField;
	
	public char[] charArray;
	public int[] intArray;
	public String[] stringArray;
	public Object[] objectArray;
	
	public char[][] charArray2;
	public int[][] intArray2;
	public String[][] stringArray2;
	public Object[][] objectArray2;
	
	public char echoChar(char c)
	{
		return c;
	}
	
	public int echoInt(int i)
	{
		return i;
	}
	
	public String echoString(String s)
	{
		return s;
	}
	
	public Object echoObject(Object o)
	{
		return o;
	}
	
	public char[] echoCharArray(char[] arr)
	{
		return arr;
	}
	
	public int[] echoIntArray(int[] arr)
	{
		return arr;
	}
	
	public String[] echoStringArray(String[] arr)
	{
		return arr;
	}
	
	public Object[] echoObjectArray(Object[] arr)
	{
		return arr;
	}
	
	//
	
	public char triggerEchoChar(char c)
	{
		return processor.echoChar(c);
	}
	
	public int triggerEchoInt(int i)
	{
		return processor.echoInt(i);
	}
	
	public String triggerEchoString(String s)
	{
		return processor.echoString(s);
	}
	
	public Object triggerEchoObject(Object o)
	{
		return processor.echoObject(o);
	}
	
	public char[] triggerEchoCharArray(char[] arr)
	{
		return processor.echoCharArray(arr);
	}
	
	public int[] triggerEchoIntArray(int[] arr)
	{
		return processor.echoIntArray(arr);
	}
	
	public String[] triggerEchoStringArray(String[] arr)
	{
		return processor.echoStringArray(arr);
	}
	
	public Object[] triggerEchoObjectArray(Object[] arr)
	{
		return processor.echoObjectArray(arr);
	}
}
