package com.tns.bindings;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;

import org.junit.Test;
import org.ow2.asmdex.ApplicationReader;
import org.ow2.asmdex.ApplicationVisitor;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.FieldVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.Opcodes;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.PrimitiveIterator;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class DefaultMethodsTest {

    interface Base {
        default String inherited() {
            return "base";
        }

        default String reabstracted() {
            return "base";
        }

        default String redefined() {
            return "base";
        }
    }

    interface Derived extends Base {
        String reabstracted();

        @Override
        default String redefined() {
            return "derived";
        }

        default String overloaded(int value) {
            return "int";
        }

        default String overloaded(String value) {
            return "string";
        }
    }

    interface Other {
        default String inherited() {
            return "other";
        }
    }

    interface Conflicting extends Base, Other {
        @Override
        default String inherited() {
            return "conflicting";
        }
    }

    interface WithoutResolution extends Derived {
        static String helper() {
            return "static";
        }
    }

    interface Covariant extends Iterator<Integer> {
        @Override
        default Integer next() {
            return 1;
        }
    }

    @Test
    public void namesAreTheSortedDistinctNamesOfTheMostSpecificDefaults() {
        assertArrayEquals(new String[]{"inherited", "overloaded", "redefined"}, DefaultMethods.names(Derived.class));
        assertArrayEquals(new String[]{"inherited", "overloaded", "redefined"}, DefaultMethods.names(WithoutResolution.class));
        assertArrayEquals(new String[]{"forEachRemaining", "remove"}, DefaultMethods.names(Iterator.class));
        assertArrayEquals(new String[]{"forEachRemaining", "next", "remove"}, DefaultMethods.names(PrimitiveIterator.OfInt.class));
        assertArrayEquals(new String[0], DefaultMethods.names(Runnable.class));
    }

    @Test
    public void collectsTheOverridingDeclarationAndNoBridges() throws Exception {
        List<Method> derived = DefaultMethods.collect(Derived.class);
        assertTrue(derived.contains(Derived.class.getMethod("redefined")));
        assertFalse(derived.contains(Base.class.getMethod("redefined")));
        assertTrue(derived.contains(Derived.class.getMethod("overloaded", int.class)));
        assertTrue(derived.contains(Derived.class.getMethod("overloaded", String.class)));
        assertEquals(4, derived.size());

        assertTrue(DefaultMethods.collect(Conflicting.class).contains(Conflicting.class.getMethod("inherited")));

        List<Method> covariant = DefaultMethods.collect(Covariant.class);
        for (Method method : covariant) {
            assertFalse(method.toString(), method.isBridge());
        }
        assertTrue(covariant.contains(Covariant.class.getMethod("next")));
        assertEquals(Integer.class, Covariant.class.getMethod("next").getReturnType());
    }

    interface Generic<T> {
        default T same(T value) {
            return value;
        }
    }

    // javac adds the bridge same(Object), which forwards to same(String).
    interface Narrowing extends Generic<String> {
        @Override
        default String same(String value) {
            return "narrowed " + value;
        }
    }

    @Test
    public void leavesADefaultAMoreSpecificBridgeOverridesToTheBridge() throws Exception {
        Method erased = Generic.class.getMethod("same", Object.class);
        assertTrue(Narrowing.class.getDeclaredMethod("same", Object.class).isBridge());

        List<Method> narrowing = DefaultMethods.collect(Narrowing.class);
        assertEquals(Arrays.asList(Narrowing.class.getMethod("same", String.class)), narrowing);
        assertFalse(narrowing.contains(erased));
        assertEquals(Arrays.asList(erased), DefaultMethods.collect(Generic.class));
        assertArrayEquals(new String[]{"same"}, DefaultMethods.names(Narrowing.class));
    }

    static class Implementor implements Derived {
        @Override
        public String reabstracted() {
            return "class";
        }

        @Override
        public String redefined() {
            return "class";
        }
    }

    @Test
    public void aClassInheritsTheDefaultsNoClassInItsHierarchyDeclares() throws Exception {
        assertEquals(Arrays.asList(
                Base.class.getMethod("inherited"),
                Derived.class.getMethod("overloaded", int.class),
                Derived.class.getMethod("overloaded", String.class)),
                DefaultMethods.collect(Implementor.class, new ArrayList<Class<?>>()));

        // Base.inherited() and Other.inherited() conflict: neither is inherited.
        assertEquals(Arrays.asList(
                Derived.class.getMethod("overloaded", int.class),
                Derived.class.getMethod("overloaded", String.class)),
                DefaultMethods.collect(Implementor.class, Arrays.<Class<?>>asList(Other.class)));
    }

    @Test
    public void keyNamesTheDeclaringInterfaceAndTheSignature() throws Exception {
        assertEquals("java.util.Iterator.remove()void", DefaultMethods.key(Iterator.class.getMethod("remove")));
        assertEquals("com.tns.bindings.DefaultMethodsTest$Derived.overloaded(int)java.lang.String",
                DefaultMethods.key(Derived.class.getMethod("overloaded", int.class)));
    }

    private static byte[] generate(Class<?> iface, boolean overrideDefaults) {
        org.ow2.asmdex.ApplicationWriter aw = new org.ow2.asmdex.ApplicationWriter(
                overrideDefaults ? DefaultMethodBodies.INVOKE_SUPER.dexVersion : org.ow2.asmdex.ApplicationWriter.DEFAULT_DEX_VERSION);
        aw.visit();
        ClassDescriptor descriptor = new ClassInfo(iface);
        new Dump().generateProxy(aw, "", descriptor, null, new HashSet<ClassDescriptor>(), null,
                overrideDefaults ? DefaultMethods.collect(iface) : null);
        aw.visitEnd();
        return aw.toByteArray();
    }

    private static final class Shape {
        String className;
        final List<String> interfaces = new ArrayList<>();
        final List<String> fields = new ArrayList<>();
        final List<String> methods = new ArrayList<>();
    }

    private static Shape read(byte[] dex) {
        final Shape shape = new Shape();
        new ApplicationReader(Opcodes.ASM4, dex).accept(new ApplicationVisitor(Opcodes.ASM4) {
            @Override
            public ClassVisitor visitClass(int access, String name, String[] signature, String superName, String[] interfaces) {
                shape.className = name;
                if (interfaces != null) {
                    shape.interfaces.addAll(Arrays.asList(interfaces));
                }
                return new ClassVisitor(Opcodes.ASM4) {
                    @Override
                    public FieldVisitor visitField(int access, String name, String desc, String[] signature, Object value) {
                        shape.fields.add(name + ":" + desc);
                        return null;
                    }

                    @Override
                    public MethodVisitor visitMethod(int access, String name, String desc, String[] signature, String[] exceptions) {
                        shape.methods.add(name + desc);
                        return null;
                    }
                };
            }
        }, ApplicationReader.SKIP_CODE);
        return shape;
    }

    @Test
    public void theDefaultOverridingClassAddsAStubPerDefaultAndTheMask() {
        Shape plain = read(generate(PrimitiveIterator.OfInt.class, false));
        Shape overriding = read(generate(PrimitiveIterator.OfInt.class, true));

        assertEquals("Lcom/tns/gen/java/util/PrimitiveIterator_OfInt;", plain.className);
        assertEquals("Lcom/tns/gen/java/util/PrimitiveIterator_OfInt" + Dump.DEFAULT_OVERRIDING_SUFFIX + ";", overriding.className);

        assertFalse(plain.fields.contains("defaultOverrides__ns:J"));
        assertTrue(overriding.fields.contains("defaultOverrides__ns:J"));

        List<String> added = new ArrayList<>(overriding.methods);
        added.removeAll(plain.methods);
        added.sort(null);
        // dex descriptors: return type first
        assertEquals(Arrays.asList(
                "forEachRemainingVLjava/util/function/Consumer;",
                "forEachRemainingVLjava/util/function/IntConsumer;",
                "nextLjava/lang/Integer;",
                "removeV"), added);
        assertTrue(overriding.methods.containsAll(plain.methods));
    }

    @Test
    public void aNamedInterfaceImplementationStubsOnlyTheGivenDefaults() throws Exception {
        org.ow2.asmdex.ApplicationWriter aw = new org.ow2.asmdex.ApplicationWriter();
        aw.visit();
        List<Method> named = new ArrayList<>();
        named.add(Iterator.class.getMethod("forEachRemaining", java.util.function.Consumer.class));
        new Dump().generateProxy(aw, "a.b.NamedIterator", new ClassInfo(Iterator.class), null, new HashSet<ClassDescriptor>(), null, named);
        aw.visitEnd();
        Shape shape = read(aw.toByteArray());

        assertEquals("La/b/NamedIterator;", shape.className);
        assertFalse(shape.fields.contains("defaultOverrides__ns:J"));
        assertTrue(shape.methods.contains("forEachRemainingVLjava/util/function/Consumer;"));
        assertFalse(shape.methods.contains("removeV"));
    }

    @Test
    public void conflictingDefaultsAreCollectedOnlyWhenAskedFor() throws Exception {
        List<Class<?>> interfaces = Arrays.<Class<?>>asList(Base.class, Other.class);
        assertFalse(DefaultMethods.collect(Object.class, interfaces).contains(Base.class.getMethod("inherited")));

        List<Method> withConflicts = DefaultMethods.collectWithConflicts(Object.class, interfaces);
        assertTrue(withConflicts.contains(Base.class.getMethod("inherited")));
        assertFalse(withConflicts.contains(Other.class.getMethod("inherited")));
        assertTrue(withConflicts.containsAll(DefaultMethods.collect(Object.class, interfaces)));
    }

    interface ObjectSame {
        default Object same(Object value) {
            return "object " + value;
        }
    }

    @Test
    public void aConflictWithABridgedDefaultIsCollectedWhenAskedFor() throws Exception {
        // Narrowing's bridge same(Object) and ObjectSame.same(Object) conflict in a class
        // implementing both, which therefore has to override same(Object) itself.
        List<Class<?>> interfaces = Arrays.<Class<?>>asList(Narrowing.class, ObjectSame.class);
        Method erased = Generic.class.getMethod("same", Object.class);
        Method other = ObjectSame.class.getMethod("same", Object.class);
        Method narrowed = Narrowing.class.getMethod("same", String.class);

        List<Method> withoutConflicts = DefaultMethods.collect(Object.class, interfaces);
        assertEquals(Arrays.asList(narrowed), withoutConflicts);

        List<Method> withConflicts = DefaultMethods.collectWithConflicts(Object.class, interfaces);
        assertTrue(withConflicts.contains(narrowed));
        assertEquals(1, (withConflicts.contains(erased) ? 1 : 0) + (withConflicts.contains(other) ? 1 : 0));
        assertEquals(2, withConflicts.size());
    }

    abstract static class NotAnInterface {
        public abstract String fromClass();
    }

    @Test
    public void anInterfaceImplementationOfOneCallSiteImplementsItsOtherInterfacesOnce() throws Exception {
        HashSet<ClassDescriptor> extras = new HashSet<>();
        extras.add(new ClassInfo(Other.class));
        extras.add(new ClassInfo(Iterator.class));
        extras.add(new ClassInfo(Runnable.class));
        extras.add(new ClassInfo(NotAnInterface.class));
        List<Method> named = new ArrayList<>();
        named.add(Other.class.getMethod("inherited"));

        org.ow2.asmdex.ApplicationWriter aw = new org.ow2.asmdex.ApplicationWriter();
        aw.visit();
        new Dump().generateProxy(aw, "com.tns.gen.java.util.Iterator_site_1_2_", new ClassInfo(Iterator.class), null, extras, null, named);
        aw.visitEnd();
        Shape shape = read(aw.toByteArray());

        assertEquals("Lcom/tns/gen/java/util/Iterator_site_1_2_;", shape.className);
        List<String> interfaces = new ArrayList<>(shape.interfaces);
        interfaces.sort(null);
        assertEquals(Arrays.asList(
                "Lcom/tns/NativeScriptHashCodeProvider;",
                "Lcom/tns/NativeScriptRuntimeBound;",
                "Lcom/tns/bindings/DefaultMethodsTest$Other;",
                "Ljava/lang/Runnable;",
                "Ljava/util/Iterator;"), interfaces);
        assertTrue(shape.methods.contains("runV"));
        assertTrue(shape.methods.contains("inheritedLjava/lang/String;"));
        assertFalse(shape.fields.contains("defaultOverrides__ns:J"));
    }

    interface HiddenBase {
        default String hidden() {
            return "hidden";
        }

        default Object narrowed() {
            return "base";
        }
    }

    public interface VisibleSub extends HiddenBase {
        @Override
        default String narrowed() {
            return "sub";
        }
    }

    private static Method declared(Class<?> clazz, String name, Class<?> returnType) {
        for (Method method : clazz.getDeclaredMethods()) {
            if (method.getName().equals(name) && method.getReturnType() == returnType && !method.isBridge()) {
                return method;
            }
        }
        throw new AssertionError(name);
    }

    @Test
    public void findResolvesAKeyThroughTheImplementedInterfacesHierarchy() {
        Method hidden = declared(HiddenBase.class, "hidden", String.class);
        Method baseNarrowed = declared(HiddenBase.class, "narrowed", Object.class);
        Method subNarrowed = declared(VisibleSub.class, "narrowed", String.class);

        assertEquals(hidden, DefaultMethods.find(VisibleSub.class, DefaultMethods.key(hidden)));
        assertEquals(baseNarrowed, DefaultMethods.find(VisibleSub.class, DefaultMethods.key(baseNarrowed)));
        assertEquals(subNarrowed, DefaultMethods.find(VisibleSub.class, DefaultMethods.key(subNarrowed)));
        assertEquals(null, DefaultMethods.find(Runnable.class, DefaultMethods.key(hidden)));
        assertTrue(DefaultMethods.collect(VisibleSub.class).contains(hidden));
    }

    @Test
    public void theDefaultOverridingFileNameChangesWithTheDefaultMethodNames() throws Exception {
        List<Method> iteratorDefaults = DefaultMethods.collect(Iterator.class);
        List<Method> withoutRemove = new ArrayList<>(iteratorDefaults);
        withoutRemove.remove(Iterator.class.getMethod("remove"));
        List<Method> withAnother = new ArrayList<>(iteratorDefaults);
        withAnother.add(Derived.class.getMethod("redefined"));

        String suffix = ProxyGenerator.defaultOverridingFileSuffix(iteratorDefaults, DefaultMethodBodies.INVOKE_SUPER);
        assertTrue(suffix.startsWith(Dump.DEFAULT_OVERRIDING_SUFFIX + "-"));
        assertEquals(suffix, ProxyGenerator.defaultOverridingFileSuffix(new ArrayList<>(iteratorDefaults), DefaultMethodBodies.INVOKE_SUPER));
        assertFalse(suffix.equals(ProxyGenerator.defaultOverridingFileSuffix(withoutRemove, DefaultMethodBodies.INVOKE_SUPER)));
        assertFalse(suffix.equals(ProxyGenerator.defaultOverridingFileSuffix(withAnother, DefaultMethodBodies.INVOKE_SUPER)));
        List<Method> withOverload = new ArrayList<>(iteratorDefaults);
        withOverload.add(java.util.PrimitiveIterator.OfInt.class.getMethod("forEachRemaining", java.util.function.IntConsumer.class));
        assertFalse(suffix.equals(ProxyGenerator.defaultOverridingFileSuffix(withOverload, DefaultMethodBodies.INVOKE_SUPER)));

        java.io.File dir = java.nio.file.Files.createTempDirectory("proxies").toFile();
        try {
            String path = new ProxyGenerator(dir.getAbsolutePath()).generateProxy("", null, new ClassInfo(Iterator.class), null,
                    new HashSet<ClassDescriptor>(), true, null, iteratorDefaults);
            assertEquals("java.util.Iterator" + suffix + ".dex", new java.io.File(path).getName());
        } finally {
            for (java.io.File file : dir.listFiles()) {
                file.delete();
            }
            dir.delete();
        }
    }

    @Test
    public void generationIsByteStable() {
        assertArrayEquals(generate(Iterator.class, true), generate(Iterator.class, true));
        assertArrayEquals(generate(Iterator.class, false), generate(Iterator.class, false));
    }
}
