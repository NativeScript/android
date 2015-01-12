package com.tns;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

import android.renderscript.Float2;

class MethodResolver
{
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
			return "V";

		Class<?> t = type;
		
		String typeName = t.getName().toLowerCase();
		String array = "";
		while (t.isArray())
		{
			array += "[";
			t = type.getComponentType();
			typeName = t.getName().toLowerCase();
		}

		if ("boolean".equals(typeName))
		{
			return array + "Z";
		}
		else if ("int".equals(typeName))
		{
			return array + "I";
		}
		else if ("double".equals(typeName))
		{
			return array + "D";
		}
		else if ("float".equals(typeName))
		{
			return array + "F";
		}
		else if ("short".equals(typeName))
		{
			return array + "S";
		}
		else if ("long".equals(typeName))
		{
			return array + "J";
		}
		else if ("char".equals(typeName))
		{
			return array + "C";
		}
		else if ("byte".equals(typeName))
		{
			return array + "B";
		}
		else if ("void".equals(typeName))
		{
			return array + "V";
		}
		else
		{
			return array + "L" + t.getName().replace('.', '/') + ";";
		}
	}

	static String resolveMethodOverload(String className, String methodName, Object[] args) throws ClassNotFoundException
	{
		String methodSig = null;
		
		Class<?> clazz = Class.forName(className);

		ArrayList<Tuple<Method, Integer>> candidates = new ArrayList<Tuple<Method, Integer>>();
		
		int argLength = (args != null) ? args.length : 0;
		
		tryFindMatches(methodName, candidates, args, argLength, clazz.getMethods());
		
		Class<?> c = clazz;
		while (candidates.isEmpty() && (c != null))
		{
			tryFindMatches(methodName, candidates, args, argLength, c.getDeclaredMethods());
			c = c.getSuperclass();
		}
		
		if (!candidates.isEmpty())
		{
			if (candidates.size() > 1)
				Collections.sort(candidates, distanceComparator);
			Method selectedMethod = candidates.get(0).x;
			methodSig = getMethodSignature(selectedMethod.getReturnType(), selectedMethod.getParameterTypes());
		}

		return methodSig;
	}
	
	static void tryFindMatches(String methodName, ArrayList<Tuple<Method, Integer>> candidates, Object[] args, int argLength, Method[] methods)
	{
		for (Method method : methods)
		{
			int modifiers = method.getModifiers();
			if (!Modifier.isPublic(modifiers) && !Modifier.isProtected(modifiers))
				continue;
			
			if (method.getName().equals(methodName))
			{
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
	}
	
	static Constructor<?> resolveConstructor(String classPath, Object[] args) throws ClassNotFoundException
	{
		String cannonicalClassName = classPath.replace('/', '.');
		
		Class<?> clazz = Class.forName(cannonicalClassName);

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
