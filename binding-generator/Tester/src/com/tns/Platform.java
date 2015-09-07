package com.tns;

import java.io.InvalidObjectException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import com.tns.runtimebindinggenerator.tester.MethodResolver;

public class Platform
{
	public static Object implementationObject;

	public static void initInstance(Object instance)
	{
		// initInstance(instance, null);
	}

	public static void initInstance(Object instance, Object... args)
	{
	}

	public static Object callJSMethod(Object javaObject, String methodName, Object... args) throws Exception
	{
		return callJSMethod(javaObject, methodName, false /* isConstructor */, args);
	}
	
	public static Object callJSMethod(Object javaObject, String methodName, Class<?> returnTypeClass, Object... args) throws Exception
	{
		return callJSMethod(javaObject, methodName, false /* isConstructor */, args);
	}

	public static Object callJSMethod(Object javaObject, String methodName, boolean isConstructor, Object... args) throws Exception
	{
		Method method = MethodResolver.resolveMethodOverload(implementationObject.getClass(), methodName, args);
		Object result = method.invoke(implementationObject, args);
		return result;
	}
}
