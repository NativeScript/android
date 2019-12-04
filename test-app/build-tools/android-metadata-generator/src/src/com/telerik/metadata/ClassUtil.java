package com.telerik.metadata;

import com.telerik.metadata.parsing.NativeClassDescriptor;
import com.telerik.metadata.parsing.NativeMethodDescriptor;
import com.telerik.metadata.parsing.NativeTypeDescriptor;

import java.util.ArrayList;

public class ClassUtil {
    private ClassUtil() {
    }

    public static boolean isPrimitive(NativeClassDescriptor clazz) {
        boolean isPrimitive = !clazz.isClass() && !clazz.isEnum()
                              && !clazz.isInterface();
        return isPrimitive;
    }

    public static boolean isPrimitive(NativeTypeDescriptor type) {
        boolean isPrimitive = type.equals(NativeTypeDescriptor.Companion.getBOOLEAN())
                              || type.equals(NativeTypeDescriptor.Companion.getCHAR()) || type.equals(NativeTypeDescriptor.Companion.getBYTE())
                              || type.equals(NativeTypeDescriptor.Companion.getSHORT()) || type.equals(NativeTypeDescriptor.Companion.getINT())
                              || type.equals(NativeTypeDescriptor.Companion.getLONG()) || type.equals(NativeTypeDescriptor.Companion.getFLOAT())
                              || type.equals(NativeTypeDescriptor.Companion.getDOUBLE()) || type.equals(NativeTypeDescriptor.Companion.getVOID());

        return isPrimitive;
    }

    public static boolean isPrimitive(String name) {
        boolean isPrimitive = name.equals("C") || name.equals("Z")
                              || name.equals("B") || name.equals("S") || name.equals("I")
                              || name.equals("J") || name.equals("F") || name.equals("D")
                              || name.equals("V");
        return isPrimitive;
    }

    public static boolean isArray(NativeClassDescriptor clazz) {
        boolean isArray = isArray(clazz.getClassName());
        return isArray;
    }

    public static boolean isArray(String className) {
        boolean isArray = className.startsWith("[");
        return isArray;
    }

    public static NativeClassDescriptor getEnclosingClass(NativeClassDescriptor clazz) {
        NativeClassDescriptor enclosingClass = null;

        String className = clazz.getClassName();
        int idx = className.lastIndexOf("$");
        if (idx > 0) {
            String name = className.substring(0, idx);
            enclosingClass = ClassRepo.findClass(name);
        }

        return enclosingClass;
    }

    public static String getSimpleName(NativeClassDescriptor clazz) {
        String className = clazz.getClassName();
        int idx = className.lastIndexOf("$");
        if (idx < 0) {
            idx = className.lastIndexOf(".");
        }
        String simpleName = className.substring(idx + 1);
        return simpleName;
    }

    public static NativeMethodDescriptor[] getAllMethods(NativeClassDescriptor clazz) {
        ArrayList<NativeMethodDescriptor> methods = new ArrayList<NativeMethodDescriptor>();
        NativeClassDescriptor currentClass = clazz;
        while (currentClass != null) {
            NativeMethodDescriptor[] currentClassMethods = currentClass.getMethods();
            for (NativeMethodDescriptor m : currentClassMethods) {
                if ((m.isPublic() || m.isProtected()) && !m.isSynthetic()) {
                    methods.add(m);
                }
            }
            currentClass = getSuperclass(currentClass);
        }
        return methods.toArray(new NativeMethodDescriptor[0]);
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

    public static NativeClassDescriptor getSuperclass(NativeClassDescriptor clazz) {
        NativeClassDescriptor superClass = null;
        if (!clazz.getClassName().equals("java.lang.Object")) {
            String superClassName = clazz.getSuperclassName();
            superClass = ClassRepo.findClass(superClassName);
        }
        return superClass;
    }

    public static NativeClassDescriptor getClassByName(String className) {
        NativeClassDescriptor clazz = ClassRepo.findClass(className);
        return clazz;
    }

}
