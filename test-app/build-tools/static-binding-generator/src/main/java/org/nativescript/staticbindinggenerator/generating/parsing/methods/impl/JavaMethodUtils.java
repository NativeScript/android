package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.generic.Type;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.ClassHierarchyParser;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.HierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.GenericSignatureView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.impl.ClassHierarchyParserImpl;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

class JavaMethodUtils {

    private static final Map<String, String> PRIMITIVE_TO_BOXED_TYPE_NAME = new HashMap<>();
    private static final Pattern GENERICS_ERASURE_PATTERN = Pattern.compile("<[^\\.]*>");

    static {
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.BOOLEAN.toString(), "java.lang.Boolean");
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.BYTE.toString(), "java.lang.Byte");
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.CHAR.toString(), "java.lang.Character");
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.DOUBLE.toString(), "java.lang.Double");
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.FLOAT.toString(), "java.lang.Float");
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.INT.toString(), "java.lang.Integer");
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.LONG.toString(), "java.lang.Long");
        PRIMITIVE_TO_BOXED_TYPE_NAME.put(Type.SHORT.toString(), "java.lang.Short");
    }

    private static final String BASE_CLASS_NAME = "java.lang.Object";

    private final Map<String, JavaClass> classesCache;

    JavaMethodUtils(Map<String, JavaClass> classesCache) {
        this.classesCache = classesCache;
    }

    boolean isVisibleInHierarchy(JavaMethod member, String packageName) {
        if (member.isPublic() || member.isProtected()) {
            return true;
        } else if (member.isPrivate()) {
            return false;
        }
        return member.getDeclaringClassPackageName().equals(packageName);
    }

    boolean isSubTypeCompatible(Type overriddenType, Type overridingType) {

        String overriddenTypeSignature = overriddenType.toString();
        String overridingTypeSignature = overridingType.toString();

        boolean isOverriddenTypeArray = isArrayType(overriddenTypeSignature);
        boolean isOverridingTypeArray = isArrayType(overridingTypeSignature);

        if (isOverriddenTypeArray && isOverridingTypeArray) {
            overriddenTypeSignature = overriddenTypeSignature.substring(0, overriddenTypeSignature.length() - 2);
            overridingTypeSignature = overridingTypeSignature.substring(0, overridingTypeSignature.length() - 2);
        } else if (isOverriddenTypeArray != isOverridingTypeArray) {
            return false;
        }

        if (overriddenType.equals(Type.VOID)) {
            return overridingType.equals(Type.VOID);
        }

        if (overridingType.equals(Type.VOID)) {
            return overriddenType.equals(Type.VOID);
        }

        boolean isOverriddenTypePrimitive = isPrimitiveType(overriddenTypeSignature);
        boolean isOverridingTypePrimitive = isPrimitiveType(overridingTypeSignature);

        if (isOverriddenTypePrimitive) {
            overriddenTypeSignature = PRIMITIVE_TO_BOXED_TYPE_NAME.get(overriddenTypeSignature);
        }

        if (isOverridingTypePrimitive) {
            overridingTypeSignature = PRIMITIVE_TO_BOXED_TYPE_NAME.get(overridingTypeSignature);
        }

        JavaClass overriddenClass = getTypeClassFromCache(overriddenTypeSignature);
        JavaClass overridingClass = getTypeClassFromCache(overridingTypeSignature);
        return isSubTypeCompatible(overriddenClass, overridingClass);
    }

    private boolean isPrimitiveType(String typeSignature) {
        return PRIMITIVE_TO_BOXED_TYPE_NAME.containsKey(typeSignature);
    }

    private boolean isArrayType(String signature) {
        return signature.endsWith("[]");
    }

    private boolean isSubTypeCompatible(JavaClass overriddenType, JavaClass overridingType) {
        if (overriddenType.isClass() && overridingType.isInterface()) {
            return false;
        }

        String overriddenTypeClassName = overriddenType.getClassName();

        if (overriddenTypeClassName.equals(BASE_CLASS_NAME)) {
            return true;
        }

        if (overriddenType.getPackageName().equals(overridingType.getPackageName()) && overriddenTypeClassName.equals(overridingType.getClassName())) {
            return true;
        }

        ClassHierarchyParser classHierarchyParser = new ClassHierarchyParserImpl(classesCache);
        HierarchyView hierarchyView = classHierarchyParser.getClassHierarchy(overridingType);

        return hierarchyView.getAllParentClassesNames().contains(overriddenTypeClassName)
                || hierarchyView.getAllImplementedInterfacesNames().contains(overriddenTypeClassName);
    }

    boolean isSubSignature(ReifiedJavaMethod m1, ReifiedJavaMethod m2) {
        /*
            From JLS 8.4.2:
            The signature of a method m1 is a subsignature of the signature of a method m2 if either:
            - m2 has the same signature as m1, or
            - the signature of m1 is the same as the erasure (§4.6) of the signature of m2.
        */

        if (!m1.getName().equals(m2.getName())) {
            return false;
        }


        List<String> m1ReifiedArguments = m1.getReifiedArguments();
        List<String> m2ReifiedArguments = m2.getReifiedArguments();

        if (m1ReifiedArguments.size() != m2ReifiedArguments.size()) {
            return false;
        }

        int argumentsCount = m2ReifiedArguments.size();
        boolean isSubSignature = true;

        for (int i = 0; i < argumentsCount; i += 1) {
            String m2MethodArgumentType = m2ReifiedArguments.get(i);
            String m1MethodArgumentType = m1ReifiedArguments.get(i);

            if (!m2MethodArgumentType.equals(m1MethodArgumentType)) {
                isSubSignature = false;
                break;
            }
        }

        if(isSubSignature){
            return true;
        }

        for (int i = 0; i < argumentsCount; i += 1) {
            String m1MethodArgumentType = m1ReifiedArguments.get(i);
            String m2MethodArgumentType = m2ReifiedArguments.get(i);
            if(m2.hasGenericSignature()){
                m2MethodArgumentType = getGenericsErasure(m2MethodArgumentType);
            }

            if (!m2MethodArgumentType.equals(m1MethodArgumentType)) {
                return false;
            }
        }

        return true;
    }

    private String getGenericsErasure(String genericArgument) {
        return GENERICS_ERASURE_PATTERN.matcher(genericArgument).replaceAll("");
    }

    private JavaClass getTypeClassFromCache(String typeSignature) {
        return tryGetClassFromCache(BcelNamingUtil.resolveBcelTypeName(typeSignature));
    }

    private JavaClass getClassFromCache(String className) {
        return tryGetClassFromCache(BcelNamingUtil.resolveClassName(className));
    }

    private JavaClass tryGetClassFromCache(String name) {
        JavaClass javaClass = classesCache.get(name);

        if (javaClass == null) {
            throw new RuntimeException("Class not found: " + name);
        }

        return javaClass;
    }

}
