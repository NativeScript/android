package com.tns;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

class MethodResolver
{
	private static Map<String, String> primitiveTypesSignature = new HashMap<String, String>();
	
	static
	{
		// Boolean
		primitiveTypesSignature.put("boolean", "Z");
		primitiveTypesSignature.put("Boolean", "Z");
		// Integer
		primitiveTypesSignature.put("int", "I");
		primitiveTypesSignature.put("Integer", "I");
		// Double
		primitiveTypesSignature.put("double", "D");
		primitiveTypesSignature.put("Double", "D");
		// Float
		primitiveTypesSignature.put("float", "F");
		primitiveTypesSignature.put("Float", "F");
		// Short
		primitiveTypesSignature.put("short", "S");
		primitiveTypesSignature.put("Short", "S");
		// Long
		primitiveTypesSignature.put("long", "J");
		primitiveTypesSignature.put("Long", "J");
		// Char
		primitiveTypesSignature.put("char", "C");
		primitiveTypesSignature.put("Character", "C");
		// Byte
		primitiveTypesSignature.put("byte", "B");
		primitiveTypesSignature.put("Byte", "B");
		// Void
		primitiveTypesSignature.put("void", "V");
	}
	
	private static class Tuple<X, Y>
	{ 
	  public final X x; 
	  public final Y y;
	  
	  public Tuple(X x, Y y)
	  { 
	    this.x = x; 
	    this.y = y; 
	  } 
	}
	
	private static class DistanceComparator implements Comparator<Tuple<?, Integer>> {
	    @Override
	    public int compare(Tuple<?, Integer> left, Tuple<?, Integer> right) {
	        return left.y.compareTo(right.y);
	    }
	}
	
	private static DistanceComparator distanceComparator = new DistanceComparator();
	
	private static Map<Constructor<?>, Class<?>[]> constructorParamTypeCache = new HashMap<Constructor<?>, Class<?>[]>();
	
	public static String getMethodSignature(Class<?> retType, Class<?>[] params)
	{
		StringBuilder ret = new StringBuilder();
		ret.append('(');
		for (int i = 0; i < params.length; i++)
		{
			Class<?> type = params[i];
			ret.append(getTypeSignature(type));
		}
		ret.append(')');
		ret.append(getTypeSignature(retType));
		return ret.toString();
	}

	public static String getTypeSignature(Class<?> type)
	{
		if (type == null)
		{
			return "V";
		}

		Class<?> t = type;
		
		String array = "";
		while (t.isArray())
		{
			array += "[";
			t = type.getComponentType();
		}
		
		String signature = primitiveTypesSignature.get(t.getName());
		if(signature == null)
		{
			signature = "L" + t.getName().replace('.', '/') + ";";
		}
		
		return array + signature;
	}

	static String resolveMethodOverload(HashMap<String, Class<?>> classCache, String className, String methodName, Object[] args) throws ClassNotFoundException
	{
		String methodSig = null;
		Class<?> clazz = classCache.get(className);
		if (clazz == null)
		{
			clazz = Class.forName(className);
		}
		int argLength = (args != null) ? args.length : 0;
		
		ArrayList<Tuple<Method, Integer>> candidates = new ArrayList<Tuple<Method, Integer>>();
		
		Class<?> c = clazz;
		int iterationIndex = 0;
		while (c != null)
		{
			tryFindMatches(methodName, candidates, args, argLength, c.getDeclaredMethods());
			if(candidates.size() > iterationIndex && candidates.get(iterationIndex).y == 0)
			{
				// direct matching (distance 0) found
				break;
			}
			
			c = c.getSuperclass();
			iterationIndex++;
		}
		
		if (!candidates.isEmpty())
		{
			if (candidates.size() > 1)
				Collections.sort(candidates, distanceComparator);
			Method method = candidates.get(0).x;
			methodSig = getMethodSignature(method.getReturnType(), method.getParameterTypes());
		}

		return methodSig;
	}
	
	static void tryFindMatches(String methodName, ArrayList<Tuple<Method, Integer>> candidates, Object[] args, int argLength, Method[] methods)
	{
		for (Method method : methods)
		{
			if (!method.getName().equals(methodName))
			{
				continue;
			}
				
			int modifiers = method.getModifiers();
			if (!Modifier.isPublic(modifiers) && !Modifier.isProtected(modifiers))
			{
				continue;
			}
			
			Class<?>[] params = method.getParameterTypes();

			boolean success = false;
			
			if (params.length == argLength)
			{
				int dist = 0;
				if (argLength == 0)
				{
					success = true;
				}
				else
				{
					for (int i = 0; i < params.length; i++)
					{
						if (args[i] != null)
						{
							Tuple<Boolean, Integer> res = isAssignableFrom(params[i], args[i].getClass()); 
							success = res.x.booleanValue();
							dist += res.y;
						}
						else
						{
							success = !params[i].isPrimitive();
						}

						if (!success)
							break;
					}
				}

				if (success)
				{
					candidates.add(new Tuple<Method, Integer>(method, Integer.valueOf(dist)));
					if (dist == 0) 
						break;
				}
			}
		}
	}
	
	static Constructor<?> resolveConstructor(Class<?> clazz, Object[] args) throws ClassNotFoundException, IOException
	{
		Constructor<?>[] constructors = clazz.getConstructors();

		if (constructors.length == 1)
		{
			return constructors[0];
		}
		
		ArrayList<Tuple<Constructor<?>, Integer>> candidates = new ArrayList<Tuple<Constructor<?>, Integer>>(); 

		int argLen = (args != null) ? args.length : 0;

		for (Constructor<?> constructor : constructors)
		{
			int dist = 0;
			Class<?>[] paramTypes = constructor.getParameterTypes();

			boolean success = true;

			if (args == null)
			{

				if (paramTypes.length == 0)
				{
					return constructor;
				}
				success = false;
			}
			else
			{
				if (paramTypes.length != argLen)
				{
					continue;
				}

				for (int i = 0; i < args.length; i++)
				{
					if (args[i] != null)
					{
						Tuple<Boolean, Integer> res = isAssignableFrom(paramTypes[i], args[i].getClass()); 
						success = res.x.booleanValue();
						dist += res.y;
					}
					else
					{
						success = !paramTypes[i].isPrimitive();
					}

					if (!success)
					{
						break;
					}
				}
			}

			if (success)
			{
				if (dist == 0) 
				{
					return constructor;
				}
				candidates.add(new Tuple<Constructor<?>, Integer>(constructor, Integer.valueOf(dist)));
			}
		}
		
		if (!candidates.isEmpty())
		{
			Collections.sort(candidates, distanceComparator);
			Constructor<?> selectedCtor = candidates.get(0).x;
			
			boolean success = convertConstructorArgs(selectedCtor, args);
			
			return success ? selectedCtor : null;
		}

		return null;
	}

	private static Tuple<Boolean, Integer> isAssignableFrom(Class<?> assignTo, Class<?> assignFrom)
	{
		boolean success = false;
		int dist = 0;

		if (assignTo.isPrimitive())
		{
			if (assignTo.equals(byte.class))
			{
				if ((success = assignFrom.equals(Byte.class)))
				{
					dist += 0;
				}
				else if ((success = assignFrom.equals(Short.class)))
				{
					dist += 1001;
				}
				else if ((success = assignFrom.equals(Integer.class)))
				{
					dist += 1002;
				}
				else if ((success = assignFrom.equals(Long.class)))
				{
					dist += 1003;
				}
				else if ((success = assignFrom.equals(Float.class)))
				{
					dist += 1004;
				}
				else if ((success = assignFrom.equals(Double.class)))
				{
					dist += 1005;
				}
			}
			else if (assignTo.equals(short.class))
			{
				if ((success = assignFrom.equals(Short.class)))
				{
					dist += 0;
				}
				else if ((success = assignFrom.equals(Byte.class)))
				{
					dist += 1;
				}
				else if ((success = assignFrom.equals(Integer.class)))
				{
					dist += 2;
				}
				else if ((success = assignFrom.equals(Long.class)))
				{
					dist += 3;
				}
				else if ((success = assignFrom.equals(Float.class)))
				{
					dist += 4;
				}
				else if ((success = assignFrom.equals(Double.class)))
				{
					dist += 5;
				}
			}
			else if (assignTo.equals(int.class))
			{
				if ((success = assignFrom.equals(Integer.class)))
				{
					dist += 0;
				}
				else if ((success = assignFrom.equals(Short.class)))
				{
					dist += 1;
				}
				else if ((success = assignFrom.equals(Byte.class)))
				{
					dist += 2;
				}
				else if ((success = assignFrom.equals(Long.class)))
				{
					dist += 3;
				}
				else if ((success = assignFrom.equals(Float.class)))
				{
					dist += 4;
				}
				else if ((success = assignFrom.equals(Double.class)))
				{
					dist += 5;
				}
			}
			else if (assignTo.equals(long.class))
			{
				if ((success = assignFrom.equals(Long.class)))
				{
					dist += 0;
				}
				else if ((success = assignFrom.equals(Integer.class)))
				{
					dist += 1;
				}
				else if ((success = assignFrom.equals(Short.class)))
				{
					dist += 2;
				}
				else if ((success = assignFrom.equals(Byte.class)))
				{
					dist += 3;
				}
				else if ((success = assignFrom.equals(Float.class)))
				{
					dist += 4;
				}
				else if ((success = assignFrom.equals(Double.class)))
				{
					dist += 5;
				}
			}
			else if (assignTo.equals(float.class))
			{
				if ((success = assignFrom.equals(Float.class)))
				{
					dist += 0;
				}
				else if ((success = assignFrom.equals(Long.class)))
				{
					dist += 1;
				}
				else if ((success = assignFrom.equals(Integer.class)))
				{
					dist += 2;
				}
				else if ((success = assignFrom.equals(Short.class)))
				{
					dist += 3;
				}
				else if ((success = assignFrom.equals(Byte.class)))
				{
					dist += 4;
				}
				else if ((success = assignFrom.equals(Double.class)))
				{
					dist += 5;
				}
			}
			else if (assignTo.equals(double.class))
			{
				if ((success = assignFrom.equals(Double.class)))
				{
					dist += 0;
				}
				else if ((success = assignFrom.equals(Float.class)))
				{
					dist += 1;
				}
				else if ((success = assignFrom.equals(Long.class)))
				{
					dist += 2;
				}
				else if ((success = assignFrom.equals(Integer.class)))
				{
					dist += 3;
				}
				else if ((success = assignFrom.equals(Short.class)))
				{
					dist += 4;
				}
				else if ((success = assignFrom.equals(Byte.class)))
				{
					dist += 5;
				}
			}
			else if (assignTo.equals(boolean.class))
			{
				success = assignFrom.equals(Boolean.class);
			}
			else if (assignTo.equals(char.class))
			{
				success = assignFrom.equals(Character.class);
			}
		}
		else
		{
			success = assignTo.isAssignableFrom(assignFrom);

			if (success)
			{
				// TODO: consider interfaces as well
				Class<?> currClass = assignFrom;
				while (!assignTo.equals(currClass) && (currClass != null))
				{
					dist += 10 * 1000;
					currClass = currClass.getSuperclass();
				}
			}
		}
		
		Tuple<Boolean, Integer> ret = new Tuple<Boolean, Integer>(Boolean.valueOf(success), Integer.valueOf(dist));
		
		return ret;
	}
	
	public static boolean convertConstructorArgs(Constructor<?> ctor, Object[] args)
	{
		boolean success = true;
		
		if (ctor == null)
		{
			success = false;
			return  success;
		}

		Class<?>[] paramTypes;
		if (constructorParamTypeCache.containsKey(ctor))
		{
			paramTypes = constructorParamTypeCache.get(ctor);
		}
		else
		{
			paramTypes = ctor.getParameterTypes();
			constructorParamTypeCache.put(ctor, paramTypes);
		}
		
		for (int i=0; i<paramTypes.length; i++)
		{
			Class<?> cuurParamType = paramTypes[i];
			
			if (cuurParamType.isPrimitive())
			{
				success = convertPrimitiveArg(cuurParamType, args, i);
			}
			
			if (!success)
				break;
		}
		
		return success;
	}
	
	private static boolean convertPrimitiveArg(Class<?> primitiveType, Object[] args, int argIndex)
	{
		boolean success = false;
		
		Object currentArg = args[argIndex];
		Class<?> currentArgClass = currentArg.getClass();
		Number n;
		
		if (primitiveType.equals(byte.class))
		{
			if (currentArgClass.equals(Byte.class)
				|| currentArgClass.equals(Short.class)
				|| currentArgClass.equals(Integer.class)
				|| currentArgClass.equals(Long.class)
				|| currentArgClass.equals(Float.class)
				|| currentArgClass.equals(Double.class))
			{
				n = (Number)currentArg;
				args[argIndex] = Byte.valueOf(n.byteValue());
				success = true;
			}
		}
		else if (primitiveType.equals(short.class))
		{
			if (currentArgClass.equals(Byte.class)
				|| currentArgClass.equals(Short.class)
				|| currentArgClass.equals(Integer.class)
				|| currentArgClass.equals(Long.class)
				|| currentArgClass.equals(Float.class)
				|| currentArgClass.equals(Double.class))
			{
				n = (Number)currentArg;
				args[argIndex] = Short.valueOf(n.shortValue());
				success = true;
			}
		}
		else if (primitiveType.equals(int.class))
		{
			if (currentArgClass.equals(Byte.class)
				|| currentArgClass.equals(Short.class)
				|| currentArgClass.equals(Integer.class)
				|| currentArgClass.equals(Long.class)
				|| currentArgClass.equals(Float.class)
				|| currentArgClass.equals(Double.class))
			{
				n = (Number)currentArg;
				args[argIndex] = Integer.valueOf(n.intValue());
				success = true;
			}
		}
		else if (primitiveType.equals(long.class))
		{
			if (currentArgClass.equals(Byte.class)
				|| currentArgClass.equals(Short.class)
				|| currentArgClass.equals(Integer.class)
				|| currentArgClass.equals(Long.class)
				|| currentArgClass.equals(Float.class)
				|| currentArgClass.equals(Double.class))
			{
				n = (Number)currentArg;
				args[argIndex] = Long.valueOf(n.longValue());
				success = true;
			}
		}
		else if (primitiveType.equals(float.class))
		{
			if (currentArgClass.equals(Byte.class)
				|| currentArgClass.equals(Short.class)
				|| currentArgClass.equals(Integer.class)
				|| currentArgClass.equals(Long.class)
				|| currentArgClass.equals(Float.class)
				|| currentArgClass.equals(Double.class))
			{
				n = (Number)currentArg;
				args[argIndex] = Float.valueOf(n.floatValue());
				success = true;
			}
		}
		else if (primitiveType.equals(double.class))
		{
			if (currentArgClass.equals(Byte.class)
				|| currentArgClass.equals(Short.class)
				|| currentArgClass.equals(Integer.class)
				|| currentArgClass.equals(Long.class)
				|| currentArgClass.equals(Float.class)
				|| currentArgClass.equals(Double.class))
			{
				n = (Number)currentArg;
				args[argIndex] = Double.valueOf(n.doubleValue());
				success = true;
			}
		}
		else if (primitiveType.equals(char.class))
		{
			success = currentArgClass.equals(Character.class);
		}
		else if (primitiveType.equals(boolean.class))
		{
			success = currentArgClass.equals(Boolean.class);
		}
		
		return success;
	}
}
