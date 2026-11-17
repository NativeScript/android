package com.tns.bindings;

import java.lang.reflect.Field;
import java.util.Comparator;
import java.util.Iterator;
import java.util.function.Consumer;

import org.ow2.asmdex.ApplicationWriter;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.FieldVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.structureCommon.Label;

/**
 * A class whose methods call default methods of boot class path interfaces with invoke-super,
 * the way a default-overriding class runs the defaults its JS object does not implement, and
 * the checks that tell whether the runtime executed them correctly. Its interfaces are
 * java.util.PrimitiveIterator.OfInt and java.util.Comparator:
 * <ul>
 * <li>remove() is declared by Iterator, a super-interface of OfInt;</li>
 * <li>next() is OfInt's covariant redeclaration of Iterator.next(), returning Integer;</li>
 * <li>forEachRemaining(Consumer) is OfInt's default, which calls back into hasNext();</li>
 * <li>reversed() returns an object that calls back into compare().</li>
 * </ul>
 * Every such method fails with IllegalStateException when it is entered again before it
 * returned, so an invoke-super that dispatches back to the class shows up as a failed check
 * rather than as unbounded recursion.
 */
public final class DefaultSuperProbe {
    static final int NEXT_INT = 42;
    static final int COMPARE = 7;

    private static final String OBJECT = "Ljava/lang/Object;";
    private static final String OF_INT = "Ljava/util/PrimitiveIterator$OfInt;";
    private static final String COMPARATOR = "Ljava/util/Comparator;";
    private static final String ILLEGAL_STATE = "Ljava/lang/IllegalStateException;";

    private static final String ENTERED = "entered";
    private static final String HAS_NEXT_CALLS = "hasNextCalls";

    private DefaultSuperProbe() {
    }

    /** The descriptor the probe gets in the dex file of a default-overriding class. */
    public static String descriptor(String defaultOverridingDescriptor) {
        return defaultOverridingDescriptor.substring(0, defaultOverridingDescriptor.length() - 1) + "-probe;";
    }

    /** The name the probe gets in the dex file of a default-overriding class. */
    public static String className(String defaultOverridingClassName) {
        return defaultOverridingClassName + "-probe";
    }

    /**
     * Adds the probe class, named by its dex descriptor ("Lcom/x/Y;"), to aw. Like a
     * default-overriding class's stubs, its invoke-super instructions name the interface the
     * class implements, also for remove(), which Iterator declares.
     */
    public static void generate(ApplicationWriter aw, String descriptor) {
        String[] interfaces = { OF_INT, COMPARATOR };
        ClassVisitor cv = aw.visitClass(Opcodes.ACC_PUBLIC, descriptor, null, OBJECT, interfaces);
        cv.visit(0, Opcodes.ACC_PUBLIC, descriptor, null, OBJECT, interfaces);

        FieldVisitor fv = cv.visitField(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, ENTERED, "Z", null, null);
        fv.visitEnd();
        fv = cv.visitField(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, HAS_NEXT_CALLS, "I", null, null);
        fv.visitEnd();

        generateConstructor(cv);
        generateHasNext(cv, descriptor);
        generateConstant(cv, "nextInt", "I", 0, NEXT_INT);
        generateConstant(cv, "compare", "I" + OBJECT + OBJECT, 2, COMPARE);

        generateSuperCall(cv, descriptor, OF_INT, "remove", "V", 0, Opcodes.INSN_RETURN_VOID);
        generateSuperCall(cv, descriptor, OF_INT, "next", "Ljava/lang/Integer;", 0, Opcodes.INSN_RETURN_OBJECT);
        generateSuperCall(cv, descriptor, OF_INT, "forEachRemaining", "VLjava/util/function/Consumer;", 1, Opcodes.INSN_RETURN_VOID);
        generateSuperCall(cv, descriptor, COMPARATOR, "reversed", COMPARATOR, 0, Opcodes.INSN_RETURN_OBJECT);

        cv.visitEnd();
    }

    private static void generateConstructor(ClassVisitor cv) {
        MethodVisitor mv = cv.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_CONSTRUCTOR, "<init>", "V", null, null);
        mv.visitCode();
        mv.visitMaxs(1, 0);
        mv.visitMethodInsn(Opcodes.INSN_INVOKE_DIRECT, OBJECT, "<init>", "V", new int[] { 0 });
        mv.visitInsn(Opcodes.INSN_RETURN_VOID);
        mv.visitEnd();
    }

    private static void generateHasNext(ClassVisitor cv, String descriptor) {
        MethodVisitor mv = cv.visitMethod(Opcodes.ACC_PUBLIC, "hasNext", "Z", null, null);
        mv.visitCode();
        mv.visitMaxs(2, 0);
        mv.visitFieldInsn(Opcodes.INSN_SGET, descriptor, HAS_NEXT_CALLS, "I", 0, 0);
        mv.visitOperationInsn(Opcodes.INSN_ADD_INT_LIT8, 0, 0, 0, 1);
        mv.visitFieldInsn(Opcodes.INSN_SPUT, descriptor, HAS_NEXT_CALLS, "I", 0, 0);
        mv.visitVarInsn(Opcodes.INSN_CONST_4, 0, 0);
        mv.visitIntInsn(Opcodes.INSN_RETURN, 0);
        mv.visitEnd();
    }

    private static void generateConstant(ClassVisitor cv, String name, String desc, int parameterWords, int value) {
        MethodVisitor mv = cv.visitMethod(Opcodes.ACC_PUBLIC, name, desc, null, null);
        mv.visitCode();
        mv.visitMaxs(1 + 1 + parameterWords, 0);
        mv.visitVarInsn(Opcodes.INSN_CONST_16, 0, value);
        mv.visitIntInsn(Opcodes.INSN_RETURN, 0);
        mv.visitEnd();
    }

    // v0-v2 are locals, v3 is this and the parameters follow it.
    private static void generateSuperCall(ClassVisitor cv, String descriptor, String owner, String name, String desc, int parameterWords, int returnOpcode) {
        MethodVisitor mv = cv.visitMethod(Opcodes.ACC_PUBLIC, name, desc, null, null);
        mv.visitCode();
        int thisRegister = 3;
        mv.visitMaxs(thisRegister + 1 + parameterWords, 0);

        Label start = new Label();
        Label end = new Label();
        Label handler = new Label();
        Label enter = new Label();
        mv.visitTryCatchBlock(start, end, handler, null);

        mv.visitFieldInsn(Opcodes.INSN_SGET_BOOLEAN, descriptor, ENTERED, "Z", 0, 0);
        mv.visitJumpInsn(Opcodes.INSN_IF_EQZ, enter, 0, 0);
        mv.visitTypeInsn(Opcodes.INSN_NEW_INSTANCE, 0, 0, 0, ILLEGAL_STATE);
        mv.visitStringInsn(Opcodes.INSN_CONST_STRING, 1, "re-entered " + name);
        mv.visitMethodInsn(Opcodes.INSN_INVOKE_DIRECT, ILLEGAL_STATE, "<init>", "VLjava/lang/String;", new int[] { 0, 1 });
        mv.visitIntInsn(Opcodes.INSN_THROW, 0);

        mv.visitLabel(enter);
        mv.visitVarInsn(Opcodes.INSN_CONST_4, 0, 1);
        mv.visitFieldInsn(Opcodes.INSN_SPUT_BOOLEAN, descriptor, ENTERED, "Z", 0, 0);
        mv.visitLabel(start);
        int[] arguments = new int[1 + parameterWords];
        for (int i = 0; i < arguments.length; i++) {
            arguments[i] = thisRegister + i;
        }
        mv.visitMethodInsn(Opcodes.INSN_INVOKE_SUPER_RANGE, owner, name, desc, arguments);
        mv.visitLabel(end);
        if (returnOpcode == Opcodes.INSN_RETURN_OBJECT) {
            mv.visitIntInsn(Opcodes.INSN_MOVE_RESULT_OBJECT, 0);
        }
        mv.visitVarInsn(Opcodes.INSN_CONST_4, 2, 0);
        mv.visitFieldInsn(Opcodes.INSN_SPUT_BOOLEAN, descriptor, ENTERED, "Z", 2, 0);
        if (returnOpcode == Opcodes.INSN_RETURN_VOID) {
            mv.visitInsn(returnOpcode);
        } else {
            mv.visitIntInsn(returnOpcode, 0);
        }

        mv.visitLabel(handler);
        mv.visitIntInsn(Opcodes.INSN_MOVE_EXCEPTION, 0);
        mv.visitVarInsn(Opcodes.INSN_CONST_4, 2, 0);
        mv.visitFieldInsn(Opcodes.INSN_SPUT_BOOLEAN, descriptor, ENTERED, "Z", 2, 0);
        mv.visitIntInsn(Opcodes.INSN_THROW, 0);
        mv.visitEnd();
    }

    /** The outcome of one call into a probe instance. */
    public static final class Call {
        public final String name;
        /** Whether the default ran the interface's body and returned what it should. */
        public final boolean passed;
        public final String detail;

        Call(String name, boolean passed, String detail) {
            this.name = name;
            this.passed = passed;
            this.detail = detail;
        }

        @Override
        public String toString() {
            return name + (passed ? " ok " : " FAILED ") + detail;
        }
    }

    /**
     * Creates and initializes the probe class (a verification failure surfaces here as a
     * LinkageError), then makes each of its default calls once.
     */
    public static Call[] check(Class<?> probeClass) throws ReflectiveOperationException {
        Object probe = probeClass.getConstructor().newInstance();
        Field hasNextCalls = probeClass.getField(HAS_NEXT_CALLS);
        return new Call[] {
            remove(probe),
            next(probe),
            forEachRemaining(probe, hasNextCalls),
            reversed(probe)
        };
    }

    /** Whether every call of {@link #check} passed. */
    public static boolean passed(Call[] calls) {
        for (Call call : calls) {
            if (!call.passed) {
                return false;
            }
        }
        return true;
    }

    private static Call remove(Object probe) {
        try {
            ((Iterator<?>) probe).remove();
            return new Call("remove", false, "returned");
        } catch (Throwable e) {
            boolean passed = e.getClass() == UnsupportedOperationException.class;
            return new Call("remove", passed, describe(e));
        }
    }

    private static Call next(Object probe) {
        try {
            Object next = ((Iterator<?>) probe).next();
            return new Call("next", Integer.valueOf(NEXT_INT).equals(next), String.valueOf(next));
        } catch (Throwable e) {
            return new Call("next", false, describe(e));
        }
    }

    private static Call forEachRemaining(Object probe, Field hasNextCalls) {
        try {
            int before = hasNextCalls.getInt(null);
            final int[] accepted = new int[1];
            ((Iterator<?>) probe).forEachRemaining(new Consumer<Object>() {
                @Override
                public void accept(Object o) {
                    accepted[0]++;
                }
            });
            int calls = hasNextCalls.getInt(null) - before;
            return new Call("forEachRemaining", calls == 1 && accepted[0] == 0, "hasNext calls " + calls);
        } catch (Throwable e) {
            return new Call("forEachRemaining", false, describe(e));
        }
    }

    @SuppressWarnings({"unchecked", "rawtypes"})
    private static Call reversed(Object probe) {
        try {
            Comparator reversed = ((Comparator) probe).reversed();
            int result = reversed.compare("a", "b");
            boolean passed = reversed != probe && result == COMPARE;
            return new Call("reversed", passed, (reversed == null ? "null" : reversed.getClass().getName()) + " compare " + result);
        } catch (Throwable e) {
            return new Call("reversed", false, describe(e));
        }
    }

    private static String describe(Throwable e) {
        return e.getClass().getName() + ": " + e.getMessage();
    }
}
