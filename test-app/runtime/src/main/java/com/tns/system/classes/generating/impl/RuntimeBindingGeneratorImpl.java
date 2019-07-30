package com.tns.system.classes.generating.impl;

import com.android.dx.stock.ProxyBuilder;
import com.tns.system.classes.generating.RuntimeBindingGenerator;
import com.tns.system.classes.loading.ClassStorageService;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;

public class RuntimeBindingGeneratorImpl implements RuntimeBindingGenerator {

    private final ClassStorageService classStorageService;

    public RuntimeBindingGeneratorImpl(ClassStorageService classStorageService) {
        this.classStorageService = classStorageService;
    }

    @Override
    public Class<?> generateBinding(String extendedClassName, List<String> implementedInterfacesNames, List<String> overriddenMethodsNames) {
        Class<?> extendedClass = getExtendedClass(extendedClassName);
        List<Class<?>> implementedInterfaces = getImplementedInterfaces(implementedInterfacesNames);
        List<Method> overriddenMethods = getOverridenMethods(overriddenMethodsNames, extendedClass, implementedInterfaces);

        List<Class<?>> allImplementedInterfaces = new ArrayList<>(implementedInterfaces);
        if (extendedClass.isInterface()) {
            allImplementedInterfaces.add(extendedClass);
            extendedClass = Object.class;
        }

        try {
            return ProxyBuilder
                    .forClass(extendedClass)
                    .implementing(allImplementedInterfaces.toArray(new Class[0]))
                    .onlyMethods(overriddenMethods.toArray(new Method[0]))
                    .buildProxyClass();

        } catch (Exception e) {
            throw new RuntimeException("Couldn't generate binding", e);
        }
    }

    @Override
    public boolean isRuntimeGeneratedBinding(Object object) {
        return ProxyBuilder.isProxyClass(object.getClass());
    }

    @Override
    public void setInvocationHandlerToBinding(Object object, InvocationHandler invocationHandler) {
        ProxyBuilder.setInvocationHandler(object, invocationHandler);
    }

    private Class<?> getExtendedClass(String extendedClassName) {
        return classStorageService.retrieveClass(extendedClassName);
    }

    private List<Class<?>> getImplementedInterfaces(List<String> implementedInterfacesNames) {
        List<Class<?>> implementedInterfaces = new ArrayList<>();

        for (String implementedInterfaceName : implementedInterfacesNames) {
            Class<?> implementedInterface = classStorageService.retrieveClass(implementedInterfaceName);
            implementedInterfaces.add(implementedInterface);
        }

        return implementedInterfaces;
    }

    private List<Method> getOverridenMethods(List<String> methodNames, Class<?> extendedClass, List<Class<?>> implementedInterfaces) {
        List<Method> overriddenMethods = new ArrayList<>();

        methodLoop:
        for (String methodName : methodNames) {
            if ("init".equals(methodName)) {
                methodName = "<init>";
            }

            for (Method extendedClassMethod : extendedClass.getMethods()) {
                if (extendedClassMethod.getName().equals(methodName) && isOverridableMethod(extendedClassMethod)) {
                    overriddenMethods.add(extendedClassMethod);
                    continue methodLoop;
                }
            }

            for (Class<?> implementedInterface : implementedInterfaces) {
                for (Method implementedInterfaceMethod : implementedInterface.getMethods()) {
                    if (implementedInterfaceMethod.getName().equals(methodName) && isOverridableMethod(implementedInterfaceMethod)) {
                        overriddenMethods.add(implementedInterfaceMethod);
                        continue methodLoop;
                    }
                }
            }

            System.out.println("!!! VM: method not found: " + methodName);
        }

        return overriddenMethods;
    }

    private boolean isOverridableMethod(Method method) {
        int modifiers = method.getModifiers();
        return !Modifier.isNative(modifiers)
                && (Modifier.isPublic(modifiers) || Modifier.isProtected(modifiers))
                && !Modifier.isStatic(modifiers)
                && !method.isSynthetic()
                && !method.isBridge();
    }
}
