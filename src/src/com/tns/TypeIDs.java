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

	public static int GetObjectTypeId(Object object)
	{
		if (object == null)
		{
			return TypeIDs.Null;
		}
		else 
		{
			Class<?> clazz = object.getClass();
			if (object instanceof Integer || clazz.equals(int.class))
			{
				return TypeIDs.Int;
			}
			else if (object instanceof Boolean)
			{
				return TypeIDs.Boolean;
			}
			else if (object instanceof Character || clazz.equals(char.class))
			{
				return TypeIDs.Char;
			}
			else if (object instanceof Byte || clazz.equals(byte.class))
			{
				return TypeIDs.Byte;
			}
			else if (object instanceof Short || clazz.equals(short.class))
			{
				return TypeIDs.Short;
			}
			else if (object instanceof Long || clazz.equals(long.class))
			{
				return TypeIDs.Long;
			}
			else if (object instanceof Float || clazz.equals(float.class))
			{
				return TypeIDs.Float;
			}
			else if (object instanceof Double || clazz.equals(double.class))
			{
				return TypeIDs.Double;
			}
			else if (object instanceof String)
			{
				return TypeIDs.string;
			}
			else
			{
				return TypeIDs.JsObject;
			}
		}
	}
}
