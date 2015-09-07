package com.tns;

//Make this enum equal values in native code
public class TypeIDs
{
	public static int Boolean = 0;
	public static int Char = 1;
	public static int Byte = 2;
	public static int Short = 3;
	public static int Int = 4;
	public static int Long = 5;
	public static int Float = 6;
	public static int Double = 7;

	public static int string = 8;
	public static int JsObject = 9;
	public static int Null = 10;

	// TODO: handle array types

	// public static int IntArray = 10;
	// public static int CharArray = 10;
	// public static int ByteArray = 10;
	// public static int ByteArray = 10;

	public static int GetObjectTypeId(Object object)
	{
		if (object == null)
		{
			return TypeIDs.Null;
		}
		else if (object instanceof Integer || object.toString() == int.class.getName())
		{
			return TypeIDs.Int;
		}
		else if (object instanceof Boolean)
		{
			return TypeIDs.Boolean;
		}
		else if (object instanceof Character || object.toString() == char.class.getName())
		{
			return TypeIDs.Char;
		}
		else if (object instanceof Byte || object.toString() == byte.class.getName())
		{
			return TypeIDs.Byte;
		}
		else if (object instanceof Short || object.toString() == short.class.getName())
		{
			return TypeIDs.Short;
		}
		else if (object instanceof Long || object.toString() == long.class.getName())
		{
			return TypeIDs.Long;
		}
		else if (object instanceof Float || object.toString() == float.class.getName())
		{
			return TypeIDs.Float;
		}
		else if (object instanceof Double || object.toString() == double.class.getName())
		{
			return TypeIDs.Double;
		}
		else if (object instanceof String)
		{
			return TypeIDs.string;
		}
		// else if (object instanceof int[])
		// {
		// return TypeIDs.Array;
		// }
		else
		{
			return TypeIDs.JsObject;
		}
	}
}
