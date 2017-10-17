package com.tns.binding.tests;

import java.util.Random;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.ApplicationWriter;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.FieldVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.structureCommon.Label;

class Dummy {
    public static void dummyMethod(Object thiz, String name, Object... args) {

    }

    public static void dummyMethod(Object thiz, String name, Object arg1, Object arg2, Object arg3, Object arg4) {

    }

    public static Object doMethod() {
        return new Object();
    }

    public static <T,T1, T2> T2 dummyGenMethod(Object thiz, String name, T arg1, T1 arg2) {
        return (T2)doMethod();
    }

    public static void callDummyMethod() {
        dummyMethod(new Object(), "someName", new Object(), 5);
    }

    public static void calldummyMethodParams() {
        dummyMethod(new Object(), "paramsName", new Object(), new Object(), new Object(), "some string param");
    }

    public static void callDummyGenMethod() {
        Random val = Dummy.<Object, Integer, Random>dummyGenMethod(new Object(), "someName", new Object(), 5);

    }

    public static void dumpBuildConfig(ApplicationWriter aw) {
        ClassVisitor cv;
        FieldVisitor fv;
        MethodVisitor mv;
        AnnotationVisitor av0;

        cv = aw.visitClass(org.ow2.asmdex.Opcodes.ACC_PUBLIC + org.ow2.asmdex.Opcodes.ACC_FINAL, "Ltelerik/kimera/kimeraskeleton/BuildConfig;", null, "Ljava/lang/Object;", null);
        cv.visit(0, org.ow2.asmdex.Opcodes.ACC_PUBLIC + org.ow2.asmdex.Opcodes.ACC_FINAL, "Ltelerik/kimera/kimeraskeleton/BuildConfig;", null, "Ljava/lang/Object;", null);
        cv.visitSource("BuildConfig.java", null);
        {
            fv = cv.visitField(org.ow2.asmdex.Opcodes.ACC_PUBLIC + org.ow2.asmdex.Opcodes.ACC_STATIC + org.ow2.asmdex.Opcodes.ACC_FINAL, "DEBUG", "Z", null, true);
            fv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC + org.ow2.asmdex.Opcodes.ACC_CONSTRUCTOR, "<init>", "V", null, null);
            mv.visitCode();
            mv.visitMaxs(2, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 1 });
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_INSTANCE, 0, 0, 0, "Ljava/lang/Object;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 0 });
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_OBJECT, "Ldummy;", "arg1", "Ljava/lang/Object;", 0, 1);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_INSTANCE, 0, 0, 0, "Ljava/lang/Object;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 0 });
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_OBJECT, "Ldummy;", "arg2", "Ljava/lang/Object;", 0, 1);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_INSTANCE, 0, 0, 0, "Ljava/lang/Object;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 0 });
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_OBJECT, "Ldummy;", "arg3", "Ljava/lang/Object;", 0, 1);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "some string param");
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_OBJECT, "Ldummy;", "arg4", "Ljava/lang/String;", 0, 1);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "callDummyGenMethod", "V", null, null);
            mv.visitCode();
            mv.visitMaxs(6, 0);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_INSTANCE, 1, 0, 0, "Ljava/lang/Object;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 1 });
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 2, "someName");
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_INSTANCE, 3, 0, 0, "Ljava/lang/Object;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 3 });
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 4, 5);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Integer;", "valueOf", "Ljava/lang/Integer;I", new int[] { 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 4);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ldummy;", "dummyGenMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;", new int[] { 5, 1, 2, 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 0);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, 0, 0, "Ljava/util/Random;");
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "callJsMethodsWithArgs", "VLjava/lang/String;[Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "calldummyMethodWith0Args", "V", null, null);
            mv.visitCode();
            mv.visitMaxs(2, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/Object;", "toString", "Ljava/lang/String;", new int[] { 1 });
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "paramsName");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ldummy;", "dummyMethod0", "VLjava/lang/Object;Ljava/lang/String;", new int[] { 1, 1, 0 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "calldummyMethodWith1Args", "VLjava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/Object;", "toString", "Ljava/lang/String;", new int[] { 1 });
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "paramsName");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ldummy;", "dummyMethod1", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;", new int[] { 1, 1, 0, 2 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "calldummyMethodWith20Args", "VLjava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(44, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL_RANGE, "Ljava/lang/Object;", "toString", "Ljava/lang/String;", new int[] { 23 });
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 2, "paramsName");
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 0, 23);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 1, 23);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 3, 24);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 4, 25);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 5, 26);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 6, 27);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 7, 28);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 8, 29);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 9, 30);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 10, 31);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 11, 32);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 12, 33);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 13, 34);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 14, 35);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 15, 36);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 16, 37);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 17, 38);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 18, 39);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 19, 40);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 20, 41);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 21, 42);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT_FROM16, 22, 43);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL_RANGE, "Ldummy;", "dummyMethod20", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;", new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "calldummyMethodWith2Args", "VLjava/lang/Object;Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(4, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/Object;", "toString", "Ljava/lang/String;", new int[] { 1 });
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "paramsName");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ldummy;", "dummyMethod2", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;", new int[] { 1, 1, 0, 2, 3 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "calldummyMethodWith4Args", "VLjava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(12, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/Object;", "toString", "Ljava/lang/String;", new int[] { 7 });
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 2, "paramsName");
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 0, 7);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 1, 7);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 3, 8);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 4, 9);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 5, 10);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 6, 11);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL_RANGE, "Ldummy;", "dummyMethod", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;", new int[] { 0, 1, 2, 3, 4, 5, 6 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "calldummyMethodWithParams", "V", null, null);
            mv.visitCode();
            mv.visitMaxs(8, 0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 2, "paramsName");
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_OBJECT, "Ldummy;", "arg1", "Ljava/lang/Object;", 3, 7);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_OBJECT, "Ldummy;", "arg2", "Ljava/lang/Object;", 4, 7);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_OBJECT, "Ldummy;", "arg3", "Ljava/lang/Object;", 5, 7);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_OBJECT, "Ldummy;", "arg4", "Ljava/lang/String;", 6, 7);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 0, 7);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 1, 7);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL_RANGE, "Ldummy;", "dummyMethod", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;", new int[] { 0, 1, 2, 3, 4, 5, 6 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "doMethod", "Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(2, 0);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_INSTANCE, 0, 0, 0, "Ljava/lang/Object;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "dummyGenMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;", new String[] { "<T:", "Ljava/lang/Object;", "T1:", "Ljava/lang/Object;", "T2:", "Ljava/lang/Object;", ">(", "Ljava/lang/Object;", "Ljava/lang/String;", "TT;TT1;)TT2;" }, null);
            mv.visitCode();
            mv.visitMaxs(6, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ldummy;", "doMethod", "Ljava/lang/Object;", new int[] { 1 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 0);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "dummyMethod", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(7, 0);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "dummyMethod0", "VLjava/lang/Object;Ljava/lang/String;", null, null);
            mv.visitCode();
            mv.visitMaxs(17, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 2, 10);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 0, 128);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Integer;", "valueOf", "Ljava/lang/Integer;I", new int[] { 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 3);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 4, 97);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 5, 3);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 6, 4);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 7, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_WIDE_16, 8, 89);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_HIGH16, 10, 1091567616);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_WIDE, 11, 4636666922610458624L);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 13, "astring");
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 0, 14);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL_RANGE, "Ldummy;", "dummyMethodWithAllTypesOfArgs", "VLjava/lang/Object;ILjava/lang/Integer;CBSZJFDLjava/lang/String;", new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "dummyMethod1", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(4, 0);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "dummyMethod2", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(5, 0);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "dummyMethod20", "VLjava/lang/Object;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(23, 0);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "dummyMethodWithAllTypesOfArgs", "VLjava/lang/Object;ILjava/lang/Integer;CBSZJFDLjava/lang/String;", null, null);
            mv.visitCode();
            mv.visitMaxs(17, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 1, 11);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_ARRAY, 0, 0, 1, "[Ljava/lang/Object;");
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 0);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 4, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 1);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Integer;", "valueOf", "Ljava/lang/Integer;I", new int[] { 5 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 6, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 3);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Character;", "valueOf", "Ljava/lang/Character;C", new int[] { 7 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 4);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Byte;", "valueOf", "Ljava/lang/Byte;B", new int[] { 8 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 5);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Short;", "valueOf", "Ljava/lang/Short;S", new int[] { 9 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 6);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Boolean;", "valueOf", "Ljava/lang/Boolean;Z", new int[] { 10 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 7);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Long;", "valueOf", "Ljava/lang/Long;J", new int[] { 11, 12 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 1, 8);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Float;", "valueOf", "Ljava/lang/Float;F", new int[] { 13 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 1, 9);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, "Ljava/lang/Double;", "valueOf", "Ljava/lang/Double;D", new int[] { 14, 15 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 1, 10);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 16, 0, 1);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "none");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ldummy;", "callJsMethodsWithArgs", "VLjava/lang/String;[Ljava/lang/Object;", new int[] { 3, 1, 0 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "setNativeScriptOverride", "VLjava/lang/String;", null, null);
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "init");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 2, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            Label l0 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l0, 0, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 1);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Ldummy;", "__ctorOverridden", "Z", 0, 1);
            Label l1 = new Label();
            mv.visitLabel(l1);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitLabel(l0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "clone");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 2, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            Label l2 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l2, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 0, 0, 0, 1);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 0, 0, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l1, 0, 0);
            mv.visitLabel(l2);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "equals");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 2, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            Label l3 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l3, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 0, 0, 0, 2);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 0, 0, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l1, 0, 0);
            mv.visitLabel(l3);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "finalize");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 2, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            Label l4 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l4, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 0, 0, 0, 4);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 0, 0, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l1, 0, 0);
            mv.visitLabel(l4);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "hashCode");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 2, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            Label l5 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l5, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 0, 0, 0, 8);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 0, 0, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l1, 0, 0);
            mv.visitLabel(l5);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 0, "toString");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 2, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l1, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 0, 0, 0, 16);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 0, 0, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Ldummy;", "__ho0", "B", 0, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l1, 0, 0);
            mv.visitEnd();
        }
        cv.visitEnd();
    }
}
