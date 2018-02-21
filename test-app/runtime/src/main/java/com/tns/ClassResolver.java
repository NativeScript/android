package com.tns;

import java.io.IOException;

class ClassResolver {
    private final Runtime runtime;

    public ClassResolver(Runtime runtime) {
        this.runtime = runtime;
    }

    public Class<?> resolveClass(String baseClassName, String fullClassName, DexFactory dexFactory, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) throws ClassNotFoundException, IOException {
        String canonicalClassName = fullClassName.replace('/', '.');
        String canonicalBaseClassName = baseClassName.replace('/', '.');
        String name = "";
        String className = canonicalClassName;

        Class<?> clazz = null;
        boolean isBindingClass = canonicalClassName.startsWith("com.tns.gen") && !canonicalClassName.startsWith("com.tns.tests.");

        // if binding generate proxy or load pregenerated
        if (isBindingClass) {
            clazz = dexFactory.resolveClass(canonicalBaseClassName, name, className, methodOverrides, implementedInterfaces, isInterface);
        }

        if (clazz == null) {
            clazz = Runtime.getClassForName(className);
        }

        return clazz;
    }
}
