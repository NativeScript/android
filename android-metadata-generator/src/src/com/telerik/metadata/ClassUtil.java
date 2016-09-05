package com.telerik.metadata;

import java.util.ArrayList;

import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;

class ClassUtil {
	private ClassUtil() {
	}

	public static boolean isPrimitive(JavaClass clazz) {
		boolean isPrimitive = !clazz.isClass() && !clazz.isEnum()
				&& !clazz.isInterface();
		return isPrimitive;
	}

	public static boolean isPrimitive(Type type) {
		boolean isPrimitive = type.equals(Type.BOOLEAN)
				|| type.equals(Type.CHAR) || type.equals(Type.BYTE)
				|| type.equals(Type.SHORT) || type.equals(Type.INT)
				|| type.equals(Type.LONG) || type.equals(Type.FLOAT)
				|| type.equals(Type.DOUBLE) || type.equals(Type.VOID);

		return isPrimitive;
	}

	public static boolean isPrimitive(String name) {
		boolean isPrimitive = name.equals("C") || name.equals("Z")
				|| name.equals("B") || name.equals("S") || name.equals("I")
				|| name.equals("J") || name.equals("F") || name.equals("D")
				|| name.equals("V");
		return isPrimitive;
	}

	public static boolean isArray(JavaClass clazz) {
		boolean isArray = isArray(clazz.getClassName());
		return isArray;
	}

	public static boolean isArray(String className) {
		boolean isArray = className.startsWith("[");
		return isArray;
	}

	public static JavaClass getEnclosingClass(JavaClass clazz) {
		JavaClass enclosingClass = null;

		String className = clazz.getClassName();
		int idx = className.lastIndexOf("$");
		if (idx > 0) {
			String name = className.substring(0, idx);
			enclosingClass = ClassRepo.findClass(name);
		}

		return enclosingClass;
	}

	public static String getSimpleName(JavaClass clazz) {
		String className = clazz.getClassName();
		int idx = className.lastIndexOf("$");
		if (idx < 0) {
			idx = className.lastIndexOf(".");
		}
		String simpleName = className.substring(idx + 1);
		return simpleName;
	}

	public static Method[] getAllMethods(JavaClass clazz) {
		ArrayList<Method> methods = new ArrayList<Method>();
		JavaClass currentClass = clazz;
		while (currentClass != null) {
			Method[] currentClassMethods = currentClass.getMethods();
			for (Method m : currentClassMethods) {
				if ((m.isPublic() || m.isProtected()) && !m.isSynthetic()) {
					methods.add(m);
				}
			}
			currentClass = getSuperclass(currentClass);
		}
		return methods.toArray(new Method[methods.size()]);
	}

	public static String getCanonicalName(String className) {
		String canonicalName;
		if (className.startsWith("L") && className.endsWith(";")) {
			canonicalName = className.substring(1, className.length() - 1)
					.replace('/', '.');
		} else {
			canonicalName = className;
		}
		return canonicalName;
	}

	public static JavaClass getSuperclass(JavaClass clazz) {
		JavaClass superClass = null;
		if (!clazz.getClassName().equals("java.lang.Object")) {
			String superClassName = clazz.getSuperclassName();
			superClass = ClassRepo.findClass(superClassName);
		}
		return superClass;
	}

	public static JavaClass getClassByName(String className) {
		JavaClass clazz = ClassRepo.findClass(className);
		return clazz;
	}

}
