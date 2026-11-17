package com.tns.tests.interfaces.extras;

import android.content.Context;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

// Java callers for testInterfaceExtraInterfaces.js. Every method but runExtra takes Object, so
// an instance that lacks an interface reaches Java and is reported instead of crossing JNI
// as an argument of a type it does not have.
public final class ExtrasFixtures {
    private ExtrasFixtures() {
    }

    public static String className(Object instance) {
        return instance.getClass().getName();
    }

    public static boolean isInstance(Object instance, String interfaceName) throws ClassNotFoundException {
        return Class.forName(interfaceName).isInstance(instance);
    }

    // The interfaces the instance's class declares, sorted, without the runtime's own.
    public static String interfaceNames(Object instance) {
        List<String> names = new ArrayList<String>();
        for (Class<?> iface : instance.getClass().getInterfaces()) {
            if (!iface.getName().startsWith("com.tns.NativeScript")) {
                names.add(iface.getName());
            }
        }
        Collections.sort(names);
        StringBuilder joined = new StringBuilder();
        for (String name : names) {
            if (joined.length() > 0) {
                joined.append(',');
            }
            joined.append(name);
        }
        return joined.toString();
    }

    // Calls the interface's no-argument method through the interface, as a Java caller holding
    // the instance as that type would.
    public static String call(Object instance, String interfaceName, String methodName) {
        try {
            Class<?> iface = Class.forName(interfaceName);
            if (!iface.isInstance(instance)) {
                return "not a " + interfaceName;
            }
            return String.valueOf(iface.getMethod(methodName).invoke(instance));
        } catch (java.lang.reflect.InvocationTargetException e) {
            return e.getCause().getClass().getName();
        } catch (Exception e) {
            return e.getClass().getName();
        }
    }

    public static String runExtra(Extra extra) {
        return extra.extra();
    }

    // Below minSdk 24, D8 turns this module's default methods into abstract ones, whose
    // bodies a class generated on the device cannot reach. Method.isDefault needs API 24.
    public static boolean areDefaultsDesugared() throws NoSuchMethodException {
        return !Extra.class.getMethod("extraDefault").isDefault();
    }

    public static boolean isRuntimeGenerated(Object instance) {
        try {
            instance.getClass().getDeclaredField("__initialized");
            return true;
        } catch (NoSuchFieldException e) {
            return false;
        }
    }

    // The runtime's cached dex files whose names start with prefix.
    public static String[] cachedDexFiles(String prefix) throws Exception {
        Context context = (Context) Class.forName("android.app.ActivityThread").getMethod("currentApplication").invoke(null);
        List<String> found = new ArrayList<String>();
        File[] dirs = {
                new File(context.getApplicationInfo().dataDir, "code_cache/secondary-dexes"),
                new File(context.getFilesDir(), "secondary-dexes")
        };
        for (File dir : dirs) {
            String[] names = dir.list();
            if (names == null) {
                continue;
            }
            for (String name : names) {
                if (name.startsWith(prefix) && name.endsWith(".dex")) {
                    found.add(name);
                }
            }
        }
        Collections.sort(found);
        return found.toArray(new String[0]);
    }
}
