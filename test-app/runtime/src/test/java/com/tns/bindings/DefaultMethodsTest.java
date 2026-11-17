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

    @Test
    public void keyNamesTheDeclaringInterfaceAndTheSignature() throws Exception {
        assertEquals("java.util.Iterator.remove()void", DefaultMethods.key(Iterator.class.getMethod("remove")));
        assertEquals("com.tns.bindings.DefaultMethodsTest$Derived.overloaded(int)java.lang.String",
                DefaultMethods.key(Derived.class.getMethod("overloaded", int.class)));
    }

    private static byte[] generate(Class<?> iface, boolean overrideDefaults) {
        org.ow2.asmdex.ApplicationWriter aw = new org.ow2.asmdex.ApplicationWriter();
        aw.visit();
        ClassDescriptor descriptor = new ClassInfo(iface);
        new Dump().generateProxy(aw, "", descriptor, null, new HashSet<ClassDescriptor>(), null,
                overrideDefaults ? DefaultMethods.collect(iface) : null);
        aw.visitEnd();
        return aw.toByteArray();
    }

    private static final class Shape {
        String className;
        final List<String> fields = new ArrayList<>();
        final List<String> methods = new ArrayList<>();
    }

    private static Shape read(byte[] dex) {
        final Shape shape = new Shape();
        new ApplicationReader(Opcodes.ASM4, dex).accept(new ApplicationVisitor(Opcodes.ASM4) {
            @Override
            public ClassVisitor visitClass(int access, String name, String[] signature, String superName, String[] interfaces) {
                shape.className = name;
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
    public void generationIsByteStable() {
        assertArrayEquals(generate(Iterator.class, true), generate(Iterator.class, true));
        assertArrayEquals(generate(Iterator.class, false), generate(Iterator.class, false));
    }
}
