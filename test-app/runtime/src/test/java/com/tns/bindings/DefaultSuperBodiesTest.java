package com.tns.bindings;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;

import org.junit.Test;
import org.ow2.asmdex.ApplicationReader;
import org.ow2.asmdex.ApplicationVisitor;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.Opcodes;

import java.io.File;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

public class DefaultSuperBodiesTest {

    public interface WideBase {
        default long wide(long a, double b, int c) {
            return a + (long) b + c;
        }
    }

    public interface Wide extends WideBase {
        void abstractOne();

        default Object same(Object value) {
            return value;
        }

        default char letter() {
            return 'x';
        }

        default void nothing() {
        }
    }

    private static final String WIDE = "Lcom/tns/bindings/DefaultSuperBodiesTest$Wide;";
    private static final String OVERRIDING = "Lcom/tns/gen/com/tns/bindings/DefaultSuperBodiesTest_Wide-defaults;";
    private static final String RUNTIME_CALLING = "Lcom/tns/gen/com/tns/bindings/DefaultSuperBodiesTest_Wide-defaults-jni;";

    private static final class Call {
        final int opcode;
        final String owner;
        final String name;
        final String desc;
        final int[] arguments;

        Call(int opcode, String owner, String name, String desc, int[] arguments) {
            this.opcode = opcode;
            this.owner = owner;
            this.name = name;
            this.desc = desc;
            this.arguments = arguments;
        }
    }

    private static final class Dex {
        String version;
        final Map<String, String[]> interfaces = new HashMap<>();
        // "Class.methodDesc" -> its method calls
        final Map<String, List<Call>> calls = new HashMap<>();
    }

    private static Dex read(byte[] bytes) {
        final Dex dex = new Dex();
        dex.version = new String(bytes, 4, 3, java.nio.charset.StandardCharsets.US_ASCII);
        new ApplicationReader(Opcodes.ASM4, bytes).accept(new ApplicationVisitor(Opcodes.ASM4) {
            @Override
            public ClassVisitor visitClass(int access, final String className, String[] signature, String superName, String[] interfaces) {
                dex.interfaces.put(className, interfaces);
                return new ClassVisitor(Opcodes.ASM4) {
                    @Override
                    public MethodVisitor visitMethod(int access, String name, String desc, String[] signature, String[] exceptions) {
                        final List<Call> calls = new ArrayList<>();
                        dex.calls.put(className + "." + name + desc, calls);
                        return new MethodVisitor(Opcodes.ASM4) {
                            @Override
                            public void visitMethodInsn(int opcode, String owner, String name, String desc, int[] arguments) {
                                calls.add(new Call(opcode, owner, name, desc, arguments));
                            }
                        };
                    }
                };
            }
        }, 0);
        return dex;
    }

    private static byte[] generate(DefaultMethodBodies bodies, String[] fileName) throws Exception {
        File dir = Files.createTempDirectory("proxies").toFile();
        try {
            String path = new ProxyGenerator(dir.getAbsolutePath()).generateProxy("", null, new ClassInfo(Wide.class), null,
                    new HashSet<ClassDescriptor>(), true, null, DefaultMethods.collect(Wide.class), bodies);
            fileName[0] = new File(path).getName();
            return Files.readAllBytes(new File(path).toPath());
        } finally {
            for (File file : dir.listFiles()) {
                file.delete();
            }
            dir.delete();
        }
    }

    private static Call superCall(Dex dex, String method) {
        Call found = null;
        for (Call call : dex.calls.get(OVERRIDING + "." + method)) {
            assertFalse(call.name.equals(Dump.callInterfaceDefaultMethodName));
            if (call.opcode == Opcodes.INSN_INVOKE_SUPER_RANGE) {
                found = call;
            }
        }
        assertNotNull(method, found);
        return found;
    }

    @Test
    public void invokeSuperStubsNameTheImplementedInterfaceAndPassTheirOwnRegisters() throws Exception {
        String[] fileName = new String[1];
        Dex dex = read(generate(DefaultMethodBodies.INVOKE_SUPER, fileName));

        assertEquals("037", dex.version);

        // v0-v2 are locals, v3 is this
        Call wide = superCall(dex, "wideJJDI");
        assertEquals(WIDE, wide.owner);
        assertEquals("wide", wide.name);
        assertEquals("JJDI", wide.desc);
        assertArrayEquals(new int[] { 3, 4, 5, 6, 7, 8 }, wide.arguments);

        assertArrayEquals(new int[] { 3, 4 }, superCall(dex, "sameLjava/lang/Object;Ljava/lang/Object;").arguments);
        assertArrayEquals(new int[] { 3 }, superCall(dex, "letterC").arguments);
        assertEquals(WIDE, superCall(dex, "nothingV").owner);

        assertEquals(new HashSet<>(Arrays.asList(OVERRIDING, DefaultSuperProbe.descriptor(OVERRIDING))), dex.interfaces.keySet());
        assertTrue(Arrays.asList(dex.interfaces.get(DefaultSuperProbe.descriptor(OVERRIDING)))
                .containsAll(Arrays.asList("Ljava/util/PrimitiveIterator$OfInt;", "Ljava/util/Comparator;")));
        assertEquals(DefaultSuperBodiesTest.class.getName() + "_Wide" + Dump.DEFAULT_OVERRIDING_SUFFIX + "-"
                + DefaultMethods.signaturesHash(DefaultMethods.collect(Wide.class)) + ".dex", fileName[0]);
    }

    @Test
    public void runtimeCallStubsHaveTheirOwnNameAndADex035FileWithoutAProbe() throws Exception {
        String[] fileName = new String[1];
        Dex dex = read(generate(DefaultMethodBodies.RUNTIME_CALL, fileName));

        assertEquals("035", dex.version);
        assertEquals(new HashSet<>(Arrays.asList(RUNTIME_CALLING)), dex.interfaces.keySet());
        boolean callsRuntime = false;
        for (Call call : dex.calls.get(RUNTIME_CALLING + ".wideJJDI")) {
            assertFalse(call.opcode == Opcodes.INSN_INVOKE_SUPER_RANGE);
            callsRuntime |= call.name.equals(Dump.callInterfaceDefaultMethodName);
        }
        assertTrue(callsRuntime);
        assertEquals(DefaultSuperBodiesTest.class.getName() + "_Wide" + Dump.DEFAULT_OVERRIDING_SUFFIX + "-jni-"
                + DefaultMethods.signaturesHash(DefaultMethods.collect(Wide.class)) + ".dex", fileName[0]);
    }

    @Test
    public void theTwoKindsNeverShareADexFile() {
        List<Method> defaults = DefaultMethods.collect(Wide.class);
        assertFalse(ProxyGenerator.defaultOverridingFileSuffix(defaults, DefaultMethodBodies.RUNTIME_CALL)
                .equals(ProxyGenerator.defaultOverridingFileSuffix(defaults, DefaultMethodBodies.INVOKE_SUPER)));
    }

    @Test
    public void theWriterKeepsItsVersionToItself() {
        org.ow2.asmdex.ApplicationWriter v37 = new org.ow2.asmdex.ApplicationWriter(37);
        org.ow2.asmdex.ApplicationWriter v35 = new org.ow2.asmdex.ApplicationWriter();
        v37.visit();
        v37.visitEnd();
        v35.visit();
        v35.visitEnd();
        assertEquals("037", new String(v37.toByteArray(), 4, 3, java.nio.charset.StandardCharsets.US_ASCII));
        assertEquals("035", new String(v35.toByteArray(), 4, 3, java.nio.charset.StandardCharsets.US_ASCII));
    }
}
