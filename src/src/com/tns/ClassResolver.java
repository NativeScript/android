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

class ClassResolver
{
	public static Class<?> resolveClass(String fullClassName, DexFactory dexFactory, String[] methodOverrides) throws ClassNotFoundException, IOException
	{
		String cannonicalClassName = fullClassName.replace('/', '.');
		String name = null;
		String className = cannonicalClassName;

		int classExtendSeparatorIndex = cannonicalClassName.indexOf("_f");
		if (classExtendSeparatorIndex != -1)
		{
			className = cannonicalClassName.substring(0, classExtendSeparatorIndex);
			name = cannonicalClassName.substring(classExtendSeparatorIndex + 1);
		}

		Class<?> clazz = null;
		boolean isBindingClass = cannonicalClassName.startsWith("com.tns.gen") && !cannonicalClassName.startsWith("com.tns.tests.");

		// if binding generate proxy or load pregenerated
		if (isBindingClass)
		{
			if (name == null || name == "")
			{
				name = "0";
			}

			clazz = dexFactory.resolveClass(name, className, methodOverrides);
		}

		if (clazz == null)
		{
			clazz = Class.forName(className);
		}

		return clazz;
	}
}
