package com.tns.bindings;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.Deque;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

/**
 * The default methods a class implementing an interface inherits, and the bit each one's
 * name gets in the per-instance mask of the class generated to dispatch them to JS. Both
 * sides derive the bits from the interface alone. Method.isDefault() needs API 24.
 */
public final class DefaultMethods {
    /** Names past this many keep their Java bodies: the mask is a long. */
    public static final int MAX_DISPATCHED_NAMES = 64;

    private DefaultMethods() {
    }

    /**
     * The most specific declaration of every method signature of the interface's hierarchy
     * that is a default method, sorted by name and then by key. Bridges are left out: they
     * forward to the method they bridge.
     */
    public static List<Method> collect(Class<?> iface) {
        return collect(Collections.<Class<?>>singletonList(iface), Collections.<String>emptySet(), false);
    }

    /**
     * The default methods a subclass of the class that also implements the given interfaces
     * inherits: as {@link #collect(Class)}, less those a class in the hierarchy declares.
     */
    public static List<Method> collect(Class<?> clazz, Collection<Class<?>> interfaces) {
        return collect(clazz, interfaces, false);
    }

    /**
     * As {@link #collect(Class, Collection)}, plus one of the default declarations of each
     * signature that several unrelated interfaces declare. The subclass has to override such
     * a method itself, so it is a candidate only for a class that implements it in JS.
     */
    public static List<Method> collectWithConflicts(Class<?> clazz, Collection<Class<?>> interfaces) {
        return collect(clazz, interfaces, true);
    }

    private static List<Method> collect(Class<?> clazz, Collection<Class<?>> interfaces, boolean withConflicts) {
        // A class listed among the interfaces is not implemented, so it overrides nothing.
        List<Class<?>> roots = new ArrayList<>();
        for (Class<?> iface : interfaces) {
            if (iface.isInterface()) {
                roots.add(iface);
            }
        }
        // A class method overrides a default of the same return type only: Java calls one of
        // another return type through a descriptor the class does not declare.
        Set<String> classDescriptors = new HashSet<>();
        for (Class<?> current = clazz; current != null; current = current.getSuperclass()) {
            roots.addAll(Arrays.asList(current.getInterfaces()));
            for (Method method : current.getDeclaredMethods()) {
                if (!Modifier.isStatic(method.getModifiers())) {
                    classDescriptors.add(descriptor(method));
                }
            }
        }
        return collect(roots, classDescriptors, withConflicts);
    }

    private static String signature(Method method) {
        return method.getName() + Arrays.toString(method.getParameterTypes());
    }

    private static String descriptor(Method method) {
        return signature(method) + method.getReturnType().getName();
    }

    private static List<Method> collect(Collection<Class<?>> roots, Set<String> excludedDescriptors, boolean withConflicts) {
        Map<String, List<Method>> declarationsBySignature = new LinkedHashMap<>();
        List<Method> bridges = new ArrayList<>();
        Set<Class<?>> visited = new HashSet<>();
        Deque<Class<?>> pending = new ArrayDeque<>(roots);
        while (!pending.isEmpty()) {
            Class<?> current = pending.poll();
            if (!visited.add(current)) {
                continue;
            }
            for (Method method : current.getDeclaredMethods()) {
                int modifiers = method.getModifiers();
                if (Modifier.isStatic(modifiers) || !Modifier.isPublic(modifiers)) {
                    continue;
                }
                if (method.isBridge()) {
                    bridges.add(method);
                    continue;
                }
                if (method.isSynthetic()) {
                    continue;
                }
                if (excludedDescriptors.contains(descriptor(method))) {
                    continue;
                }
                String signature = signature(method);
                List<Method> declarations = declarationsBySignature.get(signature);
                if (declarations == null) {
                    declarations = new ArrayList<>();
                    declarationsBySignature.put(signature, declarations);
                }
                declarations.add(method);
            }
            pending.addAll(Arrays.asList(current.getInterfaces()));
        }

        List<Method> result = new ArrayList<>();
        for (List<Method> declarations : declarationsBySignature.values()) {
            Method mostSpecific = null;
            List<Method> conflictingDefaults = new ArrayList<>();
            int maximal = 0;
            for (Method candidate : declarations) {
                boolean overridden = false;
                for (Method other : declarations) {
                    if (other != candidate && candidate.getDeclaringClass() != other.getDeclaringClass()
                            && candidate.getDeclaringClass().isAssignableFrom(other.getDeclaringClass())) {
                        overridden = true;
                        break;
                    }
                }
                if (!overridden) {
                    mostSpecific = candidate;
                    maximal++;
                    if (candidate.isDefault() && !hasReturnTypeOf(conflictingDefaults, candidate)) {
                        conflictingDefaults.add(candidate);
                    }
                }
            }
            // Several maximal declarations conflict, and the class must override them
            // itself; unless asked for, that case keeps whatever it does without this class.
            // A bridge covering one of them is itself one side of the conflict, so only an
            // override resolves it, bridged or not. Java calls each return type's declaration
            // by its own descriptor, so one declaration per return type is kept.
            if (maximal == 1 && mostSpecific.isDefault() && !isBridged(mostSpecific, bridges)) {
                result.add(mostSpecific);
            } else if (maximal > 1 && withConflicts) {
                result.addAll(conflictingDefaults);
            }
        }

        Collections.sort(result, new Comparator<Method>() {
            @Override
            public int compare(Method a, Method b) {
                int byName = a.getName().compareTo(b.getName());
                return byName != 0 ? byName : key(a).compareTo(key(b));
            }
        });
        return result;
    }

    private static boolean hasReturnTypeOf(List<Method> methods, Method method) {
        for (Method other : methods) {
            if (other.getReturnType() == method.getReturnType()) {
                return true;
            }
        }
        return false;
    }

    /*
     * Whether a sub-interface of the method's declaring interface has a bridge of the same
     * name, parameter and return types: a call of that signature runs the bridge, which forwards
     * to the narrowed method the sub-interface declares, so the method itself must not be stubbed.
     */
    private static boolean isBridged(Method method, List<Method> bridges) {
        for (Method bridge : bridges) {
            if (bridge.getDeclaringClass() != method.getDeclaringClass()
                    && method.getDeclaringClass().isAssignableFrom(bridge.getDeclaringClass())
                    && bridge.getName().equals(method.getName())
                    && bridge.getReturnType() == method.getReturnType()
                    && Arrays.equals(bridge.getParameterTypes(), method.getParameterTypes())) {
                return true;
            }
        }
        return false;
    }

    /** Sorted distinct names of {@link #collect}; a name's index is its bit. */
    public static String[] names(Class<?> iface) {
        TreeSet<String> names = new TreeSet<>();
        for (Method method : collect(iface)) {
            names.add(method.getName());
        }
        return names.toArray(new String[0]);
    }

    /** 8 hex digits of FNV-1a 32 over the sorted keys of the methods. */
    public static String signaturesHash(List<Method> methods) {
        TreeSet<String> keys = new TreeSet<>();
        for (Method method : methods) {
            keys.add(key(method));
        }
        int hash = 0x811c9dc5;
        for (String key : keys) {
            for (int i = 0; i < key.length(); i++) {
                hash ^= key.charAt(i);
                hash *= 0x01000193;
            }
            hash ^= ',';
            hash *= 0x01000193;
        }
        return String.format("%08x", hash);
    }

    /** The method of iface's hierarchy whose {@link #key} is key, or null. */
    public static Method find(Class<?> iface, String key) {
        Set<Class<?>> visited = new HashSet<>();
        Deque<Class<?>> pending = new ArrayDeque<>();
        pending.add(iface);
        while (!pending.isEmpty()) {
            Class<?> current = pending.poll();
            if (!visited.add(current)) {
                continue;
            }
            for (Method method : current.getDeclaredMethods()) {
                if (key(method).equals(key)) {
                    return method;
                }
            }
            pending.addAll(Arrays.asList(current.getInterfaces()));
        }
        return null;
    }

    /** Identifies a default method to com.tns.Runtime.callInterfaceDefault. */
    public static String key(Method method) {
        StringBuilder key = new StringBuilder(method.getDeclaringClass().getName()).append('.').append(method.getName()).append('(');
        Class<?>[] parameterTypes = method.getParameterTypes();
        for (int i = 0; i < parameterTypes.length; i++) {
            if (i > 0) {
                key.append(',');
            }
            key.append(parameterTypes[i].getName());
        }
        return key.append(')').append(method.getReturnType().getName()).toString();
    }
}
