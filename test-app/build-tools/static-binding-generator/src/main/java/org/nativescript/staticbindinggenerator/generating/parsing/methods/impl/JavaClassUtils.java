package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.JavaMethod;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;
import org.nativescript.staticbindinggenerator.naming.JavaClassNames;

import java.util.Map;

class JavaClassUtils {
    private final Map<String, JavaClass> classesCache;

    JavaClassUtils(Map<String, JavaClass> classesCache) {
        this.classesCache = classesCache;
    }

    JavaClass[] getInterfaces(JavaClass javaClass) {
        String[] interfacesNames = javaClass.getInterfaceNames();
        JavaClass[] interfaces = new JavaClass[interfacesNames.length];

        for (int i = 0; i < interfacesNames.length; i += 1) {
            interfaces[i] = getClassFromCache(interfacesNames[i]);
        }

        return interfaces;
    }

    ReifiedJavaMethod[] getReifiedMethodsFromClass(JavaClass javaClass, GenericHierarchyView initialClassGenericHierarchyView){
        Method[] classMethods = javaClass.getMethods();
        MethodSignatureReifier methodSignatureReifier = new MethodSignatureReifier(initialClassGenericHierarchyView);
        ReifiedJavaMethod[] methods = new ReifiedJavaMethod[classMethods.length];
        for (int i = 0; i < classMethods.length; i += 1) {
            JavaMethod javaMethod = new JavaMethodImpl(classMethods[i], javaClass);
            methods[i] = methodSignatureReifier.transformJavaMethod(javaMethod);
        }

        return methods;
    }

    JavaClass getSuperClass(JavaClass javaClass){
        if(javaClass.getClassName().equals(JavaClassNames.BASE_JAVA_CLASS_NAME)){
            return null;
        }

        return getClassFromCache(javaClass.getSuperclassName());
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