package com.tns;

import android.util.Log;

import com.tns.system.classes.loading.ClassStorageService;
import com.tns.system.classes.loading.LookedUpClassNotFound;

import java.io.IOException;

class ClassResolver {
    private final ClassStorageService classStorageService;

    ClassResolver(ClassStorageService classStorageService) {
        this.classStorageService = classStorageService;
    }

    Class<?> resolveClass(String baseClassName, String fullClassName, DexFactory dexFactory, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) throws ClassNotFoundException, IOException {
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
            try {
                clazz = classStorageService.retrieveClass(className);
            } catch (LookedUpClassNotFound notFound) {
                // A named proxy (`Base.extend('a.b.C', {...})` / @JavaProxy)
                // whose class the static binding generator never compiled — it
                // only scans assets/app, and dev servers keep most source off
                // disk. The proxy generator accepts dotted names for classes
                // and interfaces alike, so supply the class the same way
                // anonymous extends are supplied.
                Log.w("JS", "Class " + className + " not precompiled; generating at runtime. Framework references resolve only if dex injection into the app class loader succeeds.");
                clazz = dexFactory.resolveClass(canonicalBaseClassName, name, className, methodOverrides, implementedInterfaces, isInterface);
                if (clazz == null) {
                    throw notFound;
                }
            }
        }

        return clazz;
    }
}
