package com.extend.generator;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.util.ArrayList;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;
import java.io.OutputStreamWriter;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.extend.generator.NSClassLoader;

public class JarLister {
	private static HashMap<String, HashSet<String>> overridenClasses = new HashMap<String, HashSet<String>>();
	private static NSClassLoader loader;
	private static String LOCATION_SEPARATOR = "_f";

	private static class MethodNameComparator implements Comparator<Method> {
		@Override
		public int compare(Method o1, Method o2) {
			return o1.getName().compareTo(o2.getName());
		}
	}

	private static void ensureDirectories(String outDir, String[] parts) {
		File curDir = new File(outDir);

		for (int i = 0; i < parts.length; i++) {
			String dir = parts[i];

			String nextDir = curDir.getPath() + "/" + dir;

			File next = new File(nextDir);

			if (!next.exists()) {
				next.mkdir();
			}
			curDir = next;
		}
	}

	private static void generateJavaBindings(Class<?> clazz, String outDir) throws Exception {

		if (clazz.isSynthetic()) {
			return;
		}

		int clazzModifiers = clazz.getModifiers();

		if (!Modifier.isPublic(clazzModifiers)) {
			return;
		}
		
		if (Modifier.isStatic(clazzModifiers) && !clazz.isInterface()) {
			return;
		}

		boolean isFinalClass = Modifier.isFinal(clazzModifiers);

		if (isFinalClass) {
			boolean hasNestedInterfaces = checkForPublicNestedInterfaces(clazz);
			boolean hasNestedClasses = checkForPublicNestedStaticClasses(clazz);

			if (!hasNestedInterfaces && !hasNestedClasses)
				return;
		}

		if (clazz.getCanonicalName().startsWith("java.nio.")) {
			return;
		}

		String baseDir = outDir;

		Package classPackage = clazz.getPackage();

		String[] parts = classPackage.getName().split("\\.");

		ensureDirectories(baseDir, parts);

		String path = classPackage.getName().replace('.', '/') + "/";

		String packagePrefix = "com.tns.gen.";

		FileOutputStream fos = null;
		OutputStreamWriter out = null;

		boolean hasPublicCtors = false;

		for (Constructor<?> c : clazz.getConstructors()) {
			if (c.isSynthetic()) {
				continue;
			}

			int modifiers = c.getModifiers();

			boolean isPublic = Modifier.isPublic(modifiers);
			boolean isStatic = Modifier.isStatic(modifiers);

			if (isPublic && !isStatic) {
				hasPublicCtors = checkForPublicSignatureTypes(c.getParameterTypes());
				if (hasPublicCtors) {
					break;
				}
			}
		}

		if (!clazz.isInterface() && !hasPublicCtors && !isFinalClass) {
			return;
		}

		for (String key : overridenClasses.keySet()) {
			String currentCannonicalName = key.substring(0, key.indexOf(LOCATION_SEPARATOR));
			if (currentCannonicalName.equals(clazz.getCanonicalName())) {
				String fullClassName = getFullClassName(clazz, key);
				fos = new FileOutputStream(baseDir + path + fullClassName + ".java");
				out = new OutputStreamWriter(fos, "UTF-8");

				out.write("package " + packagePrefix + classPackage.getName() + ";\n\n");

				boolean hasInitOverride = ((HashSet<String>) overridenClasses.get(key)).contains("init");
				generateJavaBindingsRec(clazz, out, 0, key, hasInitOverride);

				out.flush();
				fos.flush();
			}
		}
	}

	private static String getLocationOfClass(String key) {
		String extendLocation = null;

		int indexOfFileSeparator = key.indexOf(LOCATION_SEPARATOR);
		extendLocation = key.substring(indexOfFileSeparator);

		return extendLocation;
	}

	private static String getFullClassName(Class<?> clazz, String key) {
		Class<?> enclosingClassName = clazz.getEnclosingClass();

		String fullClassName = clazz.getSimpleName();
		if (enclosingClassName != null) {
			fullClassName = enclosingClassName.getSimpleName() + "_" + clazz.getSimpleName();
		}
		String fileLocation = getLocationOfClass(key);
		return fullClassName + fileLocation;
	}

	private static boolean checkForPublicSignatureTypes(Class<?>[] params) {
		boolean allTypesArePublic = true;

		if (params != null) {
			for (Class<?> p : params) {
				int modifiers = p.getModifiers();

				if (!Modifier.isPublic(modifiers)) {
					allTypesArePublic = false;
					break;
				}
			}
		}

		return allTypesArePublic;
	}

	private static boolean checkForPublicNestedInterfaces(Class<?> clazz) {
		boolean found = false;

		Class<?>[] declClasses = clazz.getDeclaredClasses();

		if (declClasses != null) {
			for (Class<?> c : declClasses) {
				int modifiers = c.getModifiers();

				if (Modifier.isPublic(modifiers) && c.isInterface()) {
					found = true;
					break;
				}
			}
		}

		return found;
	}

	private static boolean checkForPublicNestedStaticClasses(Class<?> clazz) {
		boolean found = false;

		Class<?>[] declClasses = clazz.getDeclaredClasses();

		if (declClasses != null) {
			for (Class<?> c : declClasses) {
				int modifiers = c.getModifiers();

				if (Modifier.isPublic(modifiers) && Modifier.isStatic(modifiers) && !c.isInterface()) {

					Constructor<?>[] ctors = c.getConstructors();

					if ((ctors != null) && (ctors.length > 0)) {
						found = true;
						break;
					}
				}
			}
		}

		return found;
	}

	private static boolean equalMethodSignatures(Method x, Method y) {
		if (x.equals(y))
			return true;

		if (!x.getName().equals(y.getName()))
			return false;

		Class<?>[] xParams = x.getParameterTypes();
		Class<?>[] yParams = y.getParameterTypes();

		if (xParams.length != yParams.length)
			return false;

		boolean result = true;

		for (int i = 0; i < xParams.length; i++) {
			if (!xParams[i].equals(yParams[i])) {
				result = false;
				break;
			}
		}

		return result;
	}

	private static String bridge = "com.tns.Platform.";

	private static void getEligibleMethodsHelper(Class<?> clazz, List<Method> methods, List<Method> finals) {
		Method[] declMethods = clazz.getDeclaredMethods();

		if (declMethods != null) {
			for (Method m : declMethods) {
				int modifiers = m.getModifiers();

				boolean isStaticMethod = Modifier.isStatic(modifiers);
				boolean isFinalMethod = Modifier.isFinal(modifiers);
				boolean isPublicMethod = Modifier.isPublic(modifiers);
				boolean isProtectedMethod = Modifier.isProtected(modifiers);
				boolean isVisible = isPublicMethod || isProtectedMethod;

				if (!isVisible)
					continue;

				if (isFinalMethod) {
					boolean found = false;
					for (Method finalMethod : finals) {
						if (equalMethodSignatures(m, finalMethod)) {
							found = true;
							break;
						}
					}

					if (!found) {
						finals.add(m);
					}

					continue;
				} else if (!isStaticMethod) {
					boolean found = false;
					for (Method finalMethod : finals) {
						if (equalMethodSignatures(m, finalMethod)) {
							found = true;
							break;
						}
					}

					if (found)
						continue;

					for (Method addedMethod : methods) {
						if (equalMethodSignatures(m, addedMethod)) {
							found = true;
							break;
						}
					}

					if (!found) {
						methods.add(m);
					}
				}
			}
		}
	}

	private static List<Method> getEligibleMethods(Class<?> clazz) {
		List<Method> result = new ArrayList<Method>();
		List<Method> finals = new ArrayList<Method>();
		LinkedList<Class<?>> interfaces = new LinkedList<Class<?>>();

		LinkedList<Class<?>> q = new LinkedList<Class<?>>();
		q.addLast(clazz);

		Class<?> c;
		while ((c = q.pollFirst()) != null) {
			getEligibleMethodsHelper(c, result, finals);

			//
			Class<?>[] subs = c.getInterfaces();
			if (subs != null) {
				for (Class<?> sub : subs) {
					boolean found = false;
					for (Class<?> i : interfaces) {
						if (sub.equals(i)) {
							found = true;
							break;
						}
					}
					if (!found) {
						interfaces.addLast(sub);
					}
				}
			}
			//

			Class<?> base = c.getSuperclass();
			if (base != null) {
				q.addLast(base);
			}
		}

		q.addLast(clazz);

		for (Class<?> i : interfaces) {
			q.addLast(i);
		}

		while ((c = q.pollFirst()) != null) {
			getEligibleMethodsHelper(c, result, finals);

			Class<?>[] subs = c.getInterfaces();
			if (subs != null) {
				for (Class<?> sub : subs) {
					q.addLast(sub);
				}
			}
		}

		return result;
	}

	private static void generateJavaBindingsRec(Class<?> clazz, OutputStreamWriter out, int level, String currentKey,
			boolean hasInitOverride) throws Exception {
		int clazzModifiers = clazz.getModifiers();

		if (!Modifier.isPublic(clazzModifiers)) {
			return;
		}

		String fullClassName = getFullClassName(clazz, currentKey);

		boolean isFinalClass = Modifier.isFinal(clazzModifiers);

		String tabs = getTabsForLevel(level);

		if (isFinalClass) {
			if (level == 0) {
				out.write(tabs + "public final class " + fullClassName + " {\n");
			} else {
				out.write(tabs + "public static final class " + fullClassName + " {\n");
			}

			out.write(tabs + "\tprivate " + clazz.getSimpleName() + "() {\n");
			out.write(tabs + "\t}\n");

			out.write(tabs + "}\n");

			return;
		}

		boolean hasPublicCtors = false;

		if (clazz.isInterface()) {
			hasPublicCtors = true;

			if (level > 0) {
				out.write(tabs + "public static class " + fullClassName + " implements " + clazz.getCanonicalName()
						+ ", com.tns.NativeScriptHashCodeProvider {\n");
			} else {
				out.write(tabs + "public class " + fullClassName + " implements " + clazz.getCanonicalName()
						+ ", com.tns.NativeScriptHashCodeProvider {\n");
			}

//			for (Class<?> nested : clazz.getDeclaredClasses()) {
//
//				generateJavaBindingsRec(nested, out, level + 1, currentKey, hasInitOverride);
//			}
		} else {
			for (Constructor<?> c : clazz.getConstructors()) {
				if (c.isSynthetic()) {
					continue;
				}

				if (!hasPublicCtors) {
					if (Modifier.isStatic(clazzModifiers)) {
						out.write(tabs + "public static class " + fullClassName + " extends "
								+ clazz.getCanonicalName() + " implements com.tns.NativeScriptHashCodeProvider {\n");
					} else {
						out.write(tabs + "public class " + fullClassName + " extends " + clazz.getCanonicalName() + " implements com.tns.NativeScriptHashCodeProvider {\n");
					}
					hasPublicCtors = true;

					// for (Class<?> nested : clazz.getDeclaredClasses()) {
					// generateJavaBindingsRec(nested, out, level + 1);
					// }
				}

				writeConstructorSignature(out, level, c, fullClassName);
				writeExceptionSignature(out, c);
				writeConstructorBody(out, level, clazz, c, hasInitOverride);
			}
		}

		if (!hasPublicCtors) {
			return;
		}

		boolean isAndroidApplicationClass = isAndroidApplicationClass(clazz);

		if (isAndroidApplicationClass) {
			writeKimeraLoadLibraryStaticSection(out, level);
		}

		List<Method> methods2 = getEligibleMethods(clazz);

		Collections.sort(methods2, new Comparator<Method>() {
			@Override
			public int compare(Method x, Method y) {
				return x.getName().compareTo(y.getName());
			}
		});

		int methodGroupIdx = -1;
		ArrayList<String> methodGroups = new ArrayList<String>();
		String lastMethodGroupName = "";

		for (Method m : methods2) {
			if (m.isSynthetic()) {
				continue;
			}

			int modifiers = m.getModifiers();

			boolean isFinal = Modifier.isFinal(modifiers);
			boolean isStatic = Modifier.isStatic(modifiers);
			boolean isAbstract = Modifier.isAbstract(modifiers);
						
			if (!overridenClasses.get(currentKey).contains(m.getName()) && !clazz.isInterface() && !isAbstract) {
				continue;
			}

			if (!isFinal && !isStatic) {
				//
				if (!m.getName().equals(lastMethodGroupName)) {
					++methodGroupIdx;
					String currentMethodName = m.getName();
					methodGroups.add(currentMethodName);
					lastMethodGroupName = currentMethodName;
				}
				//

				Map<Type, Type> map = getGenericParentsMap2(clazz);

				Type genRetType = m.getGenericReturnType();

				Type res = genRetType;
				while (map.containsKey(res)) {
					res = map.get(res);
				}

				String retType;
				if (res instanceof Class) {
					retType = ((Class<?>) res).getCanonicalName();
				} else {
					retType = m.getReturnType().getCanonicalName();
				}

				Type[] genTypes = m.getGenericParameterTypes();

				boolean isAndroidApplicationOnCreateMethod = isAndroidApplicationClass
						&& m.getName().equals("onCreate");

				String methodName = isAndroidApplicationOnCreateMethod ? "onCreateInternal" : null;

				writeMethodSignature(out, level, m, retType, map, genTypes, methodName);
				writeExceptionSignature(out, m);

				if (clazz.isInterface()) {
					if (!overridenClasses.get(currentKey).contains(m.getName())) {
						writeThrowExceptionImplementation(out, level, m, retType);
					} else {
						writeInterfaceMethodImplementation(out, level, m, retType);
					}
				} else {
					if (isAbstract) {	
						if(!overridenClasses.get(currentKey).contains(m.getName())) {
							writeThrowExceptionImplementation(out, level, m, retType);	
						} else {
							writeMethodBody(out, level, clazz, m, retType, methodGroupIdx);
//							writeAbstractMethodImplementation(out, level, m, retType);	
						}
					} else {
						writeMethodBody(out, level, clazz, m, retType, methodGroupIdx);
					}
				}

				if (isAndroidApplicationOnCreateMethod) {
					writeMethodSignature(out, level, m, retType, map, genTypes, null);
					writeExceptionSignature(out, m);

					writeAndroidApplicationOnCreateMethodBody(out, level);
				}

				if (isAbstract)
					continue;
			}
		}

		writeNativeScriptHashCodeProviderMethods(out, level, clazz, methodGroupIdx, methodGroups);

		out.write(tabs + "}\n");
	}

	private static void writeThrowExceptionImplementation(OutputStreamWriter out, int level, Method m, String retType)
			throws Exception {

		String className = m.getDeclaringClass().getName();
		String methodName = m.getName();
		String errorMessageString = "You haven't overriden " + methodName + " in class " + className;

		Class<?>[] paramTypes = m.getParameterTypes();
		String tabs = getTabsForLevel(level);

		out.write(" {\n");
		out.write(tabs + "\t\tthrow new UnsupportedOperationException(\"" + errorMessageString + "\");\n");
		out.write(tabs + "\t}\n\n");
	}

	private static void writeNativeScriptHashCodeProviderMethods(OutputStreamWriter out, int level, Class<?> clazz,
			int methodGroupIdx, ArrayList<String> methodGroups) throws Exception {
		String tabs = getTabsForLevel(level);

		out.write(tabs + "\tpublic boolean equals__super(java.lang.Object other) {\n");
		out.write(tabs + "\t\treturn super.equals(other);\n");
		out.write(tabs + "\t}\n");

		out.write(tabs + "\tpublic int hashCode__super() {\n");
		out.write(tabs + "\t\treturn super.hashCode();\n");
		out.write(tabs + "\t}\n");

		if (!clazz.isInterface()) {
			writeSetKimeraOverrides(out, level, clazz, methodGroupIdx, methodGroups);
		}
	}

	private static void writeConstructorSignature(OutputStreamWriter out, int level, Constructor<?> c,
			String fullClassName) throws Exception {
		String tabs = getTabsForLevel(level);

		Class<?> declClass = c.getDeclaringClass();

		out.write(tabs + "\tpublic " + fullClassName);
		out.write("(");
		Class<?>[] paramTypes = c.getParameterTypes();

		int modifiers = declClass.getModifiers();

		boolean isStaticType = Modifier.isStatic(modifiers);
		boolean isNestedType = declClass.getEnclosingClass() != null;

		int startIndex = (isNestedType && !isStaticType) ? 1 : 0;
		for (int i = startIndex; i < paramTypes.length; i++) {
			if (i > startIndex) {
				out.write(", ");
			}
			out.write(paramTypes[i].getCanonicalName() + " param_" + i);
		}
		out.write(")");
	}

	private static void writeMethodSignature(OutputStreamWriter out, int level, Method m, String retType,
			Map<Type, Type> map, Type[] genTypes, String methodName) throws Exception {
		String tabs = getTabsForLevel(level);

		Class<?>[] paramTypes = m.getParameterTypes();

		int modifiers = m.getModifiers();

		boolean isPublic = Modifier.isPublic(modifiers);

		if (isPublic) {
			out.write(tabs + "\tpublic " + retType + " " + ((methodName == null) ? m.getName() : methodName));
		} else {
			out.write(tabs + "\tprotected " + retType + " " + ((methodName == null) ? m.getName() : methodName));
		}
		out.write("(");

		for (int i = 0; i < genTypes.length; i++) {
			if (i > 0) {
				out.write(", ");
			}
			if (genTypes[i] instanceof TypeVariable) {
				Type res = genTypes[i];
				while (map.containsKey(res)) {
					res = map.get(res);
				}
				if (res instanceof Class) {
					out.write(((Class<?>) res).getCanonicalName() + " param_" + i);
				} else {
					out.write(paramTypes[i].getCanonicalName() + " param_" + i);
				}
			} else {
				out.write(paramTypes[i].getCanonicalName() + " param_" + i);
			}
		}
		out.write(")");
	}

	private static void writeAndroidApplicationOnCreateMethodBody(OutputStreamWriter out, int level) throws Exception {
		String tabs = getTabsForLevel(level);

		out.write(" {\n");
		out.write(tabs + "\t\tcom.tns.Platform.onCreateApplication(this);\n");
		out.write(tabs + "\t}\n\n");
	}

	private static void writeMethodBody(OutputStreamWriter out, int level, Class<?> clazz, Method m, String retType,
			int methodGroupIdx) throws Exception {
		String tabs = getTabsForLevel(level);

		Class<?>[] paramTypes = m.getParameterTypes();

		out.write(" {\n");

		if (checkIfMustWriteInitializationSection(clazz, m)) {
			boolean shouldInitializeWithIntent = clazz.getName().equals("android.app.Activity")
					&& m.getName().equals("onCreate");

			writeCheckForInitialization(out, level, shouldInitializeWithIntent);
		}

		if (paramTypes.length == 0) {
			out.write(tabs + "\t\t\tjava.lang.Object[] params = null;\n");
		} else {
			out.write(tabs + "\t\t\tjava.lang.Object[] params = new Object[" + paramTypes.length + "];\n");
			for (int i = 0; i < paramTypes.length; i++) {
				out.write(tabs + "\t\t\tparams[" + i + "] = param_" + i + ";\n");
			}
		}
		out.write(tabs + "\t\t\t");
		if (retType != "void") {
			String wrappedRetType = wrapPrimitiveType(retType);
			out.write("return (" + wrappedRetType + ")");
		}
		if (m.getName().equals("init")) {
			out.write(bridge + "callJSMethod(this, \"" + m.getName() + "\", " + retType + ".class, false, params);\n");
		} else {
			out.write(bridge + "callJSMethod(this, \"" + m.getName() + "\", " + retType + ".class, params);\n");
		}

		out.write(tabs + "\t}\n\n");

	}

	private static boolean checkIfMustWriteInitializationSection(Class<?> clazz, Constructor<?> c) {
		String className = clazz.getCanonicalName();

		if (className.equals("android.app.Activity")) {
			return false;
		} else if (isAndroidApplicationClass(clazz)) {
			return false;
		}

		return true;
	}

	private static boolean checkIfMustWriteInitializationSection(Class<?> clazz, Method m) {
		String className = clazz.getCanonicalName();

		if (className.equals("android.app.Activity")) {
			String methodName = m.getName();

			if (methodName.equals("attachBaseContext") || methodName.equals("getSystemService")
					|| methodName.equals("getBaseContext") || methodName.equals("setTheme")
					|| methodName.equals("getResources") || methodName.equals("getApplicationInfo")
					|| methodName.equals("onApplyThemeResource")) {
				return false;
			}
		} else if (className.equals("android.app.Application")) {
			return false;
		}

		return true;
	}

	private static void writeConstructorBody(OutputStreamWriter out, int level, Class<?> clazz, Constructor<?> c,
			boolean hasInitOverride) throws Exception {
		String tabs = getTabsForLevel(level);

		Class<?> declClass = c.getDeclaringClass();

		int modifiers = declClass.getModifiers();

		boolean isStaticType = Modifier.isStatic(modifiers);
		boolean isNestedType = declClass.getEnclosingClass() != null;

		Class<?>[] paramTypes = c.getParameterTypes();

		int startIndex = (isNestedType && !isStaticType) ? 1 : 0;

		out.write(" {\n");

		out.write(tabs + "\t\tsuper(");
		for (int i = startIndex; i < paramTypes.length; i++) {
			if (i > startIndex) {
				out.write(", ");
			}
			out.write("param_" + i);
		}
		out.write(");\n");

		if (checkIfMustWriteInitializationSection(clazz, c)) {
			writeCheckForInitialization(out, level, false /* shouldInitializedWithIntent */);
		}

		if (hasInitOverride) {
			int len1 = paramTypes.length - startIndex;
			if (len1 == 0) {
				out.write(tabs + "\t\t\tjava.lang.Object[] params = null;\n");
			} else {
				out.write(tabs + "\t\t\tjava.lang.Object[] params = new Object[" + paramTypes.length + "];\n");
				for (int i = startIndex; i < paramTypes.length; i++) {
					out.write(tabs + "\t\t\tparams[" + i + "] = param_" + i + ";\n");
				}
			}
			out.write(tabs + "\t\tcom.tns.Platform.callJSMethod(this, \"init\", void.class, true, params);\n");
		}
		out.write(tabs + "\t}\n\n");

	}

	private static void writeCheckForInitialization(OutputStreamWriter out, int level,
			boolean shouldInitializedWithIntent) throws Exception {
		String tabs = getTabsForLevel(level);

		out.write(tabs + "\t\tif (!__initialized) {\n");
		out.write(tabs + "\t\t\t__initialized = true;\n");
		if (shouldInitializedWithIntent) {
			out.write(tabs + "\t\t\tcom.tns.Platform.initInstance(this, super.getIntent());\n");
		} else {
			out.write(tabs + "\t\t\tcom.tns.Platform.initInstance(this);\n");
		}
		out.write(tabs + "\t\t}\n");
	}

	private static void writeExceptionSignature(OutputStreamWriter out, Constructor<?> c) throws Exception {
		Class<?>[] exc = c.getExceptionTypes();
		if (exc.length > 0) {
			out.write(" throws ");
			for (int i = 0; i < exc.length; i++) {
				if (i > 0) {
					out.write(", ");
				}
				out.write(exc[i].getCanonicalName());
			}
		}
	}

	private static void writeExceptionSignature(OutputStreamWriter out, Method m) throws Exception {
		Class<?>[] exc = m.getExceptionTypes();
		if (exc.length > 0) {
			out.write(" throws ");
			for (int i = 0; i < exc.length; i++) {
				if (i > 0) {
					out.write(", ");
				}
				out.write(exc[i].getCanonicalName());
			}
		}
	}

	private static void writeInterfaceMethodImplementation(OutputStreamWriter out, int level, Method m, String retType)
			throws Exception {
		out.write(" {\n");

		Class<?>[] paramTypes = m.getParameterTypes();

		String tabs = getTabsForLevel(level);

		if (paramTypes.length == 0) {
			out.write(tabs + "\t\tjava.lang.Object[] params = null;\n");
		} else {
			out.write(tabs + "\t\tjava.lang.Object[] params = new Object[" + paramTypes.length + "];\n");
			for (int i = 0; i < paramTypes.length; i++) {
				out.write(tabs + "\t\tparams[" + i + "] = param_" + i + ";\n");
			}
		}
		out.write(tabs + "\t\t");
		if (retType != "void") {
			String wrappedRetType = wrapPrimitiveType(retType);
			out.write("return (" + wrappedRetType + ")");
		}
		out.write(bridge + "callJSMethod(this, \"" + m.getName() + "\", " + retType + ".class, params);\n");

		out.write(tabs + "\t}\n\n");
	}

	private static void writeAbstractMethodImplementation(OutputStreamWriter out, int level, Method m, String retType)
			throws Exception {
		out.write(" {\n");

		Class<?>[] paramTypes = m.getParameterTypes();

		String tabs = getTabsForLevel(level);

		if (paramTypes.length == 0) {
			out.write(tabs + "\t\tjava.lang.Object[] params = null;\n");
		} else {
			out.write(tabs + "\t\tjava.lang.Object[] params = new Object[" + paramTypes.length + "];\n");
			for (int i = 0; i < paramTypes.length; i++) {
				out.write(tabs + "\t\tparams[" + i + "] = param_" + i + ";\n");
			}
		}
		out.write(tabs + "\t\t");
		if (retType != "void") {
			String wrappedRetType = wrapPrimitiveType(retType);
			out.write("return (" + wrappedRetType + ")");
		}
		out.write(bridge + "callJSMethod(this, \"" + m.getName() + "\", " + retType + ".class, params);\n");

		out.write(tabs + "\t}\n\n");
	}

	private static boolean isAndroidApplicationClass(Class<?> clazz) {
		return clazz.getCanonicalName().equals("android.app.Application");
	}

	private static void writeKimeraLoadLibraryStaticSection(OutputStreamWriter out, int level) throws Exception {
		String tabs = getTabsForLevel(level);

		out.write(tabs + "\tstatic {\n");
		out.write(tabs + "\t\tSystem.loadLibrary(\"NativeScript\");\n");
		out.write(tabs + "\t\tif (BuildConfig.DEBUG) {\n");
		out.write(tabs + "\t\t\tandroid.os.Debug.waitForDebugger();\n");
		out.write(tabs + "\t\t}\n");
		out.write(tabs + "\t}\n\n");
	}

	private static void writeSetKimeraOverrides(OutputStreamWriter out, int level, Class<?> clazz, int methodGroupIdx,
			List<String> methodGroups) throws Exception {
		String tabs = getTabsForLevel(level);

		if (isAndroidApplicationClass(clazz)) {
			out.write(tabs + "\tprivate boolean __initialized = true;\n");
		} else {
			out.write(tabs + "\tprivate boolean __initialized;\n");
		}
	}

	private static String getTabsForLevel(int level) {
		String tabs = "";
		for (int i = 0; i < level; i++) {
			tabs += "\t";
		}

		return tabs;
	}

	public static Map<Type, Type> getGenericParentsMap2(Class<?> clazz) {
		List<Type> parents = new ArrayList<Type>();

		Class<?> oldParent = clazz;
		Class<?> parentClass = clazz.getSuperclass();

		while ((parentClass != null) && (parentClass != Object.class)) {
			Type nextParent = oldParent.getGenericSuperclass();
			parents.add(nextParent);
			oldParent = parentClass;
			parentClass = parentClass.getSuperclass();
		}

		Map<Type, Type> map = new HashMap<Type, Type>();

		for (Type p : parents) {
			if (p instanceof ParameterizedType) {
				ParameterizedType pt = (ParameterizedType) p;
				Type[] typeParameters = ((Class<?>) pt.getRawType()).getTypeParameters();
				Type[] actualTypeArgs = pt.getActualTypeArguments();
				for (int i = 0; i < typeParameters.length; i++) {
					map.put(typeParameters[i], actualTypeArgs[i]);
				}
			}
		}
		return map;
	}

	private static String wrapPrimitiveType(String type) {
		String wrapped;

		if (type.equals("boolean") || type.equals("byte") || type.equals("short") || type.equals("long")
				|| type.equals("float") || type.equals("double")) {
			wrapped = type.substring(0, 1).toUpperCase() + type.substring(1);
		} else if (type.equals("char")) {
			wrapped = "Character";
		} else if (type.equals("int")) {
			wrapped = "Integer";
		} else {
			wrapped = type;
		}

		return wrapped;
	}

	public static void startGenerateBindings(String[] args, HashMap<String, HashSet<String>> proxyNames)
			throws Exception {
		overridenClasses = proxyNames;

		loader = NSClassLoader.getInstance();
		loader.loadJars(args);

		String[] jars = loader.getJarNames();

		String[] outDirs = new String[jars.length];

		for (int i = 0; i < jars.length; i++) {
			String[] dirs = { "com/", "tns/", "gen/" };
			String outputDir = ExtendClassGenerator.outFilesDir + '/';
			for (String d : dirs) {
				outputDir += d;
				File fd = new File(outputDir);
				if (!fd.exists()) {
					fd.mkdir();
				}
			}
			outDirs[i] = outputDir;
		}

		start(args, jars, outDirs);
	}

	private static void start(String[] args, String[] jars, String[] outDirs) throws Exception {

		for (int i = 0; i < jars.length; i++) {
			String jarFile = jars[i];
			String outDir = outDirs[i];

			JarInputStream input = null;

			String jarFilename = new File(jarFile).getCanonicalPath();
			try {
				input = new JarInputStream(new FileInputStream(jarFilename));	
			}
			catch(FileNotFoundException e) {
				System.out.println("The jar could not be found: " + e.getMessage());
				continue;
			}

			JarEntry entry = input.getNextJarEntry();
			ArrayList<String> classes = new ArrayList<String>();
			while (entry != null) {
				try {
					String name = entry.getName();

					if (!name.endsWith(".class"))
						continue;

					name = name.substring(0, name.length() - 6).replace('/', '.');
					classes.add(name);
				} finally {
					entry = input.getNextJarEntry();
				}				
			}

			//
			Collections.sort(classes);
			//

			for (String className : classes) {
				//
				if (jarFile.endsWith("nativescript.jar") && className.startsWith("com.tns.com.tns.tests.")) {
					continue;
				}
				//

				try {
					Class<?> clazz = Class.forName(className, false, loader);
					generateJavaBindings(clazz, outDir);
				} catch (NoClassDefFoundError e) {
					System.out.println("No deffinition could be found for: " + e.getMessage());
					continue;
				} finally {
					if (input != null) {
						input.close();
					}
				}
			}
		}
	}

}
