package com.tns;

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
                // HMR / dev fallback. The Static Binding Generator pre-generates
                // a Java stub for every `.extend('com.tns.Name', { ... })` call
                // it sees at build time, and production bakes those stubs into
                // the APK dex. In Vite HMR mode the bundle is just the boot
                // loader and modules are fetched over HTTP at runtime, so the
                // SBG never sees those `.extend(...)` calls and the class is
                // missing from the dex.
                //
                // With a baseClassName, JS is extending a known Java type, so
                // route through `DexFactory.resolveClass` (the same path as
                // `com.tns.gen.*` bindings) to generate + load a dex at runtime.
                // Lookups without a baseClassName are plain `Class.forName`-style
                // requests and must still fail loudly.
                //
                // Gated on `isDebuggable()`: runtime dex generation and
                // parent-classloader injection are only needed in dev. In
                // release every stub is baked in, so a miss is a genuinely
                // missing class that should fail rather than silently mutate
                // the app classloader.
                if (!canonicalBaseClassName.isEmpty() && com.tns.Runtime.isDebuggable()) {
                    clazz = dexFactory.resolveClass(canonicalBaseClassName, name, className, methodOverrides, implementedInterfaces, isInterface);
                }
                if (clazz == null) {
                    throw notFound;
                }
            }
        }

        return clazz;
    }
}
