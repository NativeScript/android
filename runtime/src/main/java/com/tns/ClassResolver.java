package com.tns;

import java.io.IOException;

class ClassResolver {
    private final Runtime runtime;

    public ClassResolver(Runtime runtime) {
        this.runtime = runtime;
    }

    public Class<?> resolveClass(String fullClassName, DexFactory dexFactory, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) throws ClassNotFoundException, IOException {
        String cannonicalClassName = fullClassName.replace('/', '.');
        String name = null;
        String className = cannonicalClassName;

        int classExtendSeparatorIndex = cannonicalClassName.indexOf("_f");
        if (classExtendSeparatorIndex != -1) {
            className = cannonicalClassName.substring(0, classExtendSeparatorIndex);
            name = cannonicalClassName.substring(classExtendSeparatorIndex + 1);
        } else {
            classExtendSeparatorIndex = cannonicalClassName.indexOf("_unknown_location");
            if (classExtendSeparatorIndex != -1) {
                className = cannonicalClassName.substring(0, classExtendSeparatorIndex);
                name = cannonicalClassName.substring(classExtendSeparatorIndex + 1);
            }
        }

        Class<?> clazz = null;
        boolean isBindingClass = cannonicalClassName.startsWith("com.tns.gen") && !cannonicalClassName.startsWith("com.tns.tests.");

        // if binding generate proxy or load pregenerated
        if (isBindingClass) {
            if (name == null || name == "") {
                if (isInterface) {
                    name = "";
                } else {
                    name = "0";
                }
            }

            clazz = dexFactory.resolveClass(name, className, methodOverrides, implementedInterfaces, isInterface);
        }

        if (clazz == null) {
            clazz = Runtime.getClassForName(className);
        }

        return clazz;
    }
}
