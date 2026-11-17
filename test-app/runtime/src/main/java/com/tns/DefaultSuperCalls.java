package com.tns;

import android.util.Log;

import com.tns.bindings.DefaultMethodBodies;
import com.tns.bindings.DefaultSuperProbe;

import java.util.Arrays;
import java.util.Collections;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * How default-overriding classes run the defaults their JS object does not implement, decided
 * once per process by the DefaultSuperProbe in the dex file of the first INVOKE_SUPER class
 * loaded. When it fails, that and every later default-overriding class uses RUNTIME_CALL
 * stubs, whose class names differ, since the class that failed keeps its name in its loader.
 */
final class DefaultSuperCalls {
    private static final int UNDECIDED = 0;
    private static final int PASSED = 1;
    private static final int FAILED = 2;

    private static final Object decisionLock = new Object();
    private static volatile int decision = UNDECIDED;

    private static final Set<Class<?>> runtimeCallInterfaces = Collections.newSetFromMap(new ConcurrentHashMap<Class<?>, Boolean>());

    private DefaultSuperCalls() {
    }

    static DefaultMethodBodies bodies(Class<?> iface) {
        if (decision == FAILED || (!runtimeCallInterfaces.isEmpty() && runtimeCallInterfaces.contains(iface))) {
            return DefaultMethodBodies.RUNTIME_CALL;
        }
        return DefaultMethodBodies.INVOKE_SUPER;
    }

    /**
     * Whether an INVOKE_SUPER class may be used: the first such class of the process runs its
     * dex file's probe, which decides for every class.
     */
    static boolean verify(Class<?> defaultOverridingClass) {
        int current = decision;
        if (current != UNDECIDED) {
            return current == PASSED;
        }
        synchronized (decisionLock) {
            if (decision == UNDECIDED) {
                String outcome;
                int result;
                try {
                    Class<?> probe = Class.forName(DefaultSuperProbe.className(defaultOverridingClass.getName()), true, defaultOverridingClass.getClassLoader());
                    DefaultSuperProbe.Call[] calls = DefaultSuperProbe.check(probe);
                    result = DefaultSuperProbe.passed(calls) ? PASSED : FAILED;
                    outcome = Arrays.toString(calls);
                } catch (Throwable e) {
                    result = FAILED;
                    outcome = e.toString();
                }
                decision = result;
                if (result == PASSED) {
                    Log.d("JS", "Default methods a JS object lacks run with invoke-super: " + outcome);
                } else {
                    Log.w("JS", "Default methods a JS object lacks run through the runtime: the invoke-super probe failed: " + outcome);
                }
            }
            return decision == PASSED;
        }
    }

    /*
     * Makes the iface's default-overriding class use RUNTIME_CALL stubs whatever the probe
     * decides. For tests of those stubs only, which reach it by reflection; it must come before
     * the class is first resolved.
     */
    static void useRuntimeCallsFor(Class<?> iface) {
        runtimeCallInterfaces.add(iface);
    }
}
