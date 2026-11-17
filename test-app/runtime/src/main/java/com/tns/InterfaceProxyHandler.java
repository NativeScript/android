package com.tns;

import android.os.Build;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

/**
 * Behaves like the class the runtime generates for an anonymous interface
 * implementation (see DexFactory.resolveClass): only the interface's abstract
 * methods reach JS. Object's methods keep identity semantics and a default method
 * runs its Java body, even when the JS implementation defines a function of that name.
 */
final class InterfaceProxyHandler implements InvocationHandler {
    private final int runtimeId;
    private final int objectId;

    InterfaceProxyHandler(int runtimeId, int objectId) {
        this.runtimeId = runtimeId;
        this.objectId = objectId;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        String name = method.getName();

        // The owner is fixed at construction, so the setters have nothing to record.
        if (method.getDeclaringClass() == NativeScriptRuntimeBound.class) {
            if (name.equals("getRuntimeTag__ns")) {
                return Runtime.runtimeTagOf(runtimeId);
            } else if (name.equals("getObjectId__ns")) {
                return objectId;
            }
            return null;
        }

        // Matched by signature: an interface that redeclares one of these, as
        // Comparator does equals, is the declaring class of the Method passed here.
        int argCount = args == null ? 0 : args.length;
        if (argCount == 0) {
            if (name.equals("hashCode")) {
                return System.identityHashCode(proxy);
            } else if (name.equals("toString")) {
                return proxy.getClass().getName() + "@" + Integer.toHexString(System.identityHashCode(proxy));
            }
        } else if (argCount == 1 && name.equals("equals") && method.getParameterTypes()[0] == Object.class) {
            return proxy == args[0];
        }

        if (Build.VERSION.SDK_INT >= 24 && method.isDefault()) {
            return Runtime.invokeInterfaceDefault(proxy, method, args);
        }

        Class<?> returnType = method.getReturnType();
        Object result = Runtime.callJSMethodOfOwner(runtimeId, objectId, proxy, name, returnType, false, 0, args);

        // A generated class casts the result to the return type, or to its box for a
        // primitive; the proxy would instead widen a box or throw IllegalArgumentException.
        if (result != null && returnType != void.class) {
            Class<?> expected = returnType.isPrimitive() ? boxOf(returnType) : returnType;
            if (returnType.isPrimitive() ? result.getClass() != expected : !expected.isInstance(result)) {
                throw new ClassCastException(result.getClass().getName() + " cannot be cast to " + expected.getName());
            }
        }
        return result;
    }

    private static Class<?> boxOf(Class<?> primitive) {
        if (primitive == boolean.class) {
            return Boolean.class;
        } else if (primitive == byte.class) {
            return Byte.class;
        } else if (primitive == char.class) {
            return Character.class;
        } else if (primitive == short.class) {
            return Short.class;
        } else if (primitive == int.class) {
            return Integer.class;
        } else if (primitive == long.class) {
            return Long.class;
        } else if (primitive == float.class) {
            return Float.class;
        }
        return Double.class;
    }
}
