package com.telerik.metadata;

import com.telerik.metadata.desc.ClassDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;

import java.util.ArrayList;

import org.apache.bcel.generic.Type;

public class ClassUtil {
	private ClassUtil() {
	}

	public static boolean isPrimitive(ClassDescriptor clazz) {
		boolean isPrimitive = !clazz.isClass() && !clazz.isEnum()
				&& !clazz.isInterface();
		return isPrimitive;
	}

	public static boolean isPrimitive(TypeDescriptor type) {
		boolean isPrimitive = type.equals(TypeDescriptor.BOOLEAN)
				|| type.equals(TypeDescriptor.CHAR) || type.equals(TypeDescriptor.BYTE)
				|| type.equals(TypeDescriptor.SHORT) || type.equals(TypeDescriptor.INT)
				|| type.equals(TypeDescriptor.LONG) || type.equals(TypeDescriptor.FLOAT)
				|| type.equals(TypeDescriptor.DOUBLE) || type.equals(TypeDescriptor.VOID);

		return isPrimitive;
	}

	public static boolean isPrimitive(String name) {
		boolean isPrimitive = name.equals("C") || name.equals("Z")
				|| name.equals("B") || name.equals("S") || name.equals("I")
				|| name.equals("J") || name.equals("F") || name.equals("D")
				|| name.equals("V");
		return isPrimitive;
	}

	public static boolean isArray(ClassDescriptor clazz) {
		boolean isArray = isArray(clazz.getClassName());
		return isArray;
	}

	public static boolean isArray(String className) {
		boolean isArray = className.startsWith("[");
		return isArray;
	}

	public static ClassDescriptor getEnclosingClass(ClassDescriptor clazz) {
		ClassDescriptor enclosingClass = null;

		String className = clazz.getClassName();
		int idx = className.lastIndexOf("$");
		if (idx > 0) {
			String name = className.substring(0, idx);
			enclosingClass = ClassRepo.findClass(name);
		}

		return enclosingClass;
	}

	public static String getSimpleName(ClassDescriptor clazz) {
		String className = clazz.getClassName();
		int idx = className.lastIndexOf("$");
		if (idx < 0) {
			idx = className.lastIndexOf(".");
		}
		String simpleName = className.substring(idx + 1);
		return simpleName;
	}

	public static MethodDescriptor[] getAllMethods(ClassDescriptor clazz) {
		ArrayList<MethodDescriptor> methods = new ArrayList<MethodDescriptor>();
		ClassDescriptor currentClass = clazz;
		while (currentClass != null) {
			MethodDescriptor[] currentClassMethods = currentClass.getMethods();
			for (MethodDescriptor m : currentClassMethods) {
				if ((m.isPublic() || m.isProtected()) && !m.isSynthetic()) {
					methods.add(m);
				}
			}
			currentClass = getSuperclass(currentClass);
		}
		return methods.toArray(new MethodDescriptor[methods.size()]);
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

	public static ClassDescriptor getSuperclass(ClassDescriptor clazz) {
		ClassDescriptor superClass = null;
		if (!clazz.getClassName().equals("java.lang.Object")) {
			String superClassName = clazz.getSuperclassName();
			superClass = ClassRepo.findClass(superClassName);
		}
		return superClass;
	}

	public static ClassDescriptor getClassByName(String className) {
		ClassDescriptor clazz = ClassRepo.findClass(className);
		return clazz;
	}

}
