package com.tns.binding.tests;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.ApplicationWriter;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.FieldVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.structureCommon.Label;

public class DumpObject {

    public static void dump(ApplicationWriter aw) {
        ClassVisitor cv;
        FieldVisitor fv;
        MethodVisitor mv;
        AnnotationVisitor av0;

        cv = aw.visitClass(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "Lcom/tns/java/lang/Object;", null, "Ljava/lang/Object;", new String[] { "Lcom/tns/NativeScriptHashCodeProvider;" });
        cv.visit(0, org.ow2.asmdex.Opcodes.ACC_PUBLIC, "Lcom/tns/java/lang/Object;", null, "Ljava/lang/Object;", new String[] { "Lcom/tns/NativeScriptHashCodeProvider;" });
        cv.visitSource("Object.java", null);
        {
            fv = cv.visitField(org.ow2.asmdex.Opcodes.ACC_PRIVATE, "__ctorOverridden", "Z", null, null);
            fv.visitEnd();
        }
        {
            fv = cv.visitField(org.ow2.asmdex.Opcodes.ACC_PRIVATE, "__ho0", "B", null, null);
            fv.visitEnd();
        }
        {
            fv = cv.visitField(org.ow2.asmdex.Opcodes.ACC_PRIVATE, "__ho1", "B", null, null);
            fv.visitEnd();
        }
        {
            fv = cv.visitField(org.ow2.asmdex.Opcodes.ACC_PRIVATE, "__initialized", "Z", null, null);
            fv.visitEnd();
        }
        String runtimeClass = "Lcom/tns/Runtime;";
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC + org.ow2.asmdex.Opcodes.ACC_CONSTRUCTOR, "<init>", "V", null, null);
            mv.visitCode();
            mv.visitMaxs(4, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 2, 1);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT, "Ljava/lang/Object;", "<init>", "V", new int[] { 3 });
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 3);
            Label l0 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, l0, 1, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 2, 3);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "initInstance", "VLjava/lang/Object;", new int[] { 3 });
            mv.visitLabel(l0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, "Lcom/tns/java/lang/Object;", "__ctorOverridden", "Z", 1, 3);
            Label l1 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l1, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "init");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "callJSMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Z[Ljava/lang/Object;", new int[] { 3, 1, 2, 0 });
            mv.visitLabel(l1);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PROTECTED, "clone", "Ljava/lang/Object;", null, new String[] { "Ljava/lang/CloneNotSupportedException;" });
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            Label l0 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, l0, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 1);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "initInstance", "VLjava/lang/Object;", new int[] { 2 });
            mv.visitLabel(l0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 1, 2);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_AND_INT_LIT8, 1, 1, 0, 1);
            Label l1 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_LEZ, l1, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "clone");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "callJSMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Object;", new int[] { 2, 1, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 1);
            Label l2 = new Label();
            mv.visitLabel(l2);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, 1);
            mv.visitLabel(l1);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "clone", "Ljava/lang/Object;", new int[] { 2 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l2, 0, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "clone__super", "Ljava/lang/Object;", null, new String[] { "Ljava/lang/CloneNotSupportedException;" });
            mv.visitCode();
            mv.visitMaxs(2, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "clone", "Ljava/lang/Object;", new int[] { 1 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 0);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "equals", "ZLjava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(5, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 2, 1);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 3);
            Label l0 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, l0, 1, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 2, 3);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "initInstance", "VLjava/lang/Object;", new int[] { 3 });
            mv.visitLabel(l0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 1, 3);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_AND_INT_LIT8, 1, 1, 0, 2);
            Label l1 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_LEZ, l1, 1, 0);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_ARRAY, 0, 0, 2, "[Lcom/tns/java/lang/Object;");
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 0);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 4, 0, 1);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "equals");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "callJSMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Object;", new int[] { 3, 1, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 1);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, 1, 0, "Ljava/lang/Boolean;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/Boolean;", "booleanValue", "Z", new int[] { 1 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 1);
            Label l2 = new Label();
            mv.visitLabel(l2);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 1);
            mv.visitLabel(l1);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "equals", "ZLjava/lang/Object;", new int[] { 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l2, 0, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "equals__super", "ZLjava/lang/Object;", null, null);
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "equals", "ZLjava/lang/Object;", new int[] { 1, 2 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PROTECTED, "finalize", "V", null, new String[] { "Ljava/lang/Throwable;" });
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            Label l0 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, l0, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 1);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "initInstance", "VLjava/lang/Object;", new int[] { 2 });
            mv.visitLabel(l0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 1, 2);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_AND_INT_LIT8, 1, 1, 0, 4);
            Label l1 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_LEZ, l1, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "finalize");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "callJSMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Object;", new int[] { 2, 1, 0 });
            Label l2 = new Label();
            mv.visitLabel(l2);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitLabel(l1);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "finalize", "V", new int[] { 2 });
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l2, 0, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "finalize__super", "V", null, new String[] { "Ljava/lang/Throwable;" });
            mv.visitCode();
            mv.visitMaxs(1, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "finalize", "V", new int[] { 0 });
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "hashCode", "I", null, null);
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            Label l0 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, l0, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 1);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "initInstance", "VLjava/lang/Object;", new int[] { 2 });
            mv.visitLabel(l0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 1, 2);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_AND_INT_LIT8, 1, 1, 0, 8);
            Label l1 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_LEZ, l1, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "hashCode");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "callJSMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Object;", new int[] { 2, 1, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 1);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, 1, 0, "Ljava/lang/Integer;");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/Integer;", "intValue", "I", new int[] { 1 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 1);
            Label l2 = new Label();
            mv.visitLabel(l2);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 1);
            mv.visitLabel(l1);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "hashCode", "I", new int[] { 2 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l2, 0, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "hashCode__super", "I", null, null);
            mv.visitCode();
            mv.visitMaxs(2, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "hashCode", "I", new int[] { 1 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "setNativeScriptOverrides", "V[Ljava/lang/String;", null, null);
            mv.visitCode();
            mv.visitMaxs(7, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_OBJECT, 0, 6);
            mv.visitArrayLengthInsn(2, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 0);
            Label l0 = new Label();
            mv.visitLabel(l0);
            Label l1 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_GE, l1, 1, 2);
            mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_AGET_OBJECT, 3, 0, 1);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 4, "init");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 4);
            Label l2 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l2, 4, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 4, 1);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Lcom/tns/java/lang/Object;", "__ctorOverridden", "Z", 4, 5);
            mv.visitLabel(l2);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 4, "clone");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 4);
            Label l3 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l3, 4, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 4, 4, 0, 1);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 4, 4, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            Label l4 = new Label();
            mv.visitLabel(l4);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_ADD_INT_LIT8, 1, 1, 0, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l0, 0, 0);
            mv.visitLabel(l3);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 4, "equals");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 4);
            Label l5 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l5, 4, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 4, 4, 0, 2);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 4, 4, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l4, 0, 0);
            mv.visitLabel(l5);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 4, "finalize");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 4);
            Label l6 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l6, 4, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 4, 4, 0, 4);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 4, 4, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l4, 0, 0);
            mv.visitLabel(l6);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 4, "hashCode");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 4);
            Label l7 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l7, 4, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 4, 4, 0, 8);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 4, 4, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l4, 0, 0);
            mv.visitLabel(l7);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 4, "toString");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, "Ljava/lang/String;", "equals", "ZLjava/lang/Object;", new int[] { 3, 4 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 4);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_EQZ, l4, 4, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_OR_INT_LIT8, 4, 4, 0, 16);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_INT_TO_BYTE, 4, 4, 0, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 4, 5);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l4, 0, 0);
            mv.visitLabel(l1);
            mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "toString", "Ljava/lang/String;", null, null);
            mv.visitCode();
            mv.visitMaxs(3, 0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            Label l0 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, l0, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 1, 1);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, "Lcom/tns/java/lang/Object;", "__initialized", "Z", 1, 2);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "initInstance", "VLjava/lang/Object;", new int[] { 2 });
            mv.visitLabel(l0);
            mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BYTE, "Lcom/tns/java/lang/Object;", "__ho0", "B", 1, 2);
            mv.visitOperationInsn(org.ow2.asmdex.Opcodes.INSN_AND_INT_LIT8, 1, 1, 0, 16);
            Label l1 = new Label();
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_LEZ, l1, 1, 0);
            mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 0);
            mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "toString");
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, runtimeClass, "callJSMethod", "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Object;", new int[] { 2, 1, 0 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 1);
            mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, 1, 0, "Ljava/lang/String;");
            Label l2 = new Label();
            mv.visitLabel(l2);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, 1);
            mv.visitLabel(l1);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "toString", "Ljava/lang/String;", new int[] { 2 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 1);
            mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_GOTO, l2, 0, 0);
            mv.visitEnd();
        }
        {
            mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "toString__super", "Ljava/lang/String;", null, null);
            mv.visitCode();
            mv.visitMaxs(2, 0);
            mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "toString", "Ljava/lang/String;", new int[] { 1 });
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 0);
            mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, 0);
            mv.visitEnd();
        }
        cv.visitEnd();
    }
}
