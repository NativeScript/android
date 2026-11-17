package com.tns;

import android.os.Build;

import com.tns.bindings.DefaultMethods;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Behaves like the class the runtime generates for an anonymous interface
 * implementation (see DexFactory.resolveClass): the interface's abstract methods
 * reach JS, and so does each default method whose name has its bit set in the
 * instance's mask (MetadataNode::GetDefaultOverrides); any other default method runs
 * its Java body. Object's methods keep identity semantics.
 */
final class InterfaceProxyHandler implements InvocationHandler {
    // Per proxy class: the bit of each default method the interface's default-overriding
    // class stubs, by name, parameter and return types. Bits follow DefaultMethods.names.
    private static final ConcurrentHashMap<Class<?>, Map<String, Integer>> defaultMethodBits = new ConcurrentHashMap<Class<?>, Map<String, Integer>>();

    private final int runtimeId;
    private final int objectId;
    private final long defaultOverrides;

    InterfaceProxyHandler(int runtimeId, int objectId, long defaultOverrides) {
        this.runtimeId = runtimeId;
        this.objectId = objectId;
        this.defaultOverrides = defaultOverrides;
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

        if (Build.VERSION.SDK_INT >= 24 && method.isDefault() && !isOverriddenInJs(proxy.getClass(), method)) {
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

    private boolean isOverriddenInJs(Class<?> proxyClass, Method method) {
        if (defaultOverrides == 0) {
            return false;
        }
        Integer bit = defaultMethodBitsOf(proxyClass).get(descriptor(method));
        return bit != null && (defaultOverrides & (1L << bit)) != 0;
    }

    private static Map<String, Integer> defaultMethodBitsOf(Class<?> proxyClass) {
        Map<String, Integer> bits = defaultMethodBits.get(proxyClass);
        if (bits != null) {
            return bits;
        }

        bits = new HashMap<String, Integer>();
        for (Class<?> iface : proxyClass.getInterfaces()) {
            if (iface == NativeScriptRuntimeBound.class) {
                continue;
            }
            String[] names = DefaultMethods.names(iface);
            List<Method> methods = DefaultMethods.collect(iface);
            for (Method defaultMethod : methods) {
                int bit = Arrays.binarySearch(names, defaultMethod.getName());
                if (bit >= 0 && bit < DefaultMethods.MAX_DISPATCHED_NAMES) {
                    bits.put(descriptor(defaultMethod), bit);
                }
            }
        }
        Map<String, Integer> published = defaultMethodBits.putIfAbsent(proxyClass, bits);
        return published != null ? published : bits;
    }

    private static String descriptor(Method method) {
        return method.getName() + Arrays.toString(method.getParameterTypes()) + method.getReturnType().getName();
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
