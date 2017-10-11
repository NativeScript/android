/* Software Name : AsmDex
 * Version : 1.0
 *
 * Copyright © 2012 France Télécom
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

package org.ow2.asmdex;

import java.util.List;

import org.ow2.asmdex.structureCommon.Label;

/**
 * A visitor to visit a Java method. The methods of this interface must be
 * called in the following order: [ <tt>visitAnnotationDefault</tt> ] [ <tt>visitParameters</tt> ] (
 * <tt>visitAnnotation</tt> | <tt>visitParameterAnnotation</tt> |
 * <tt>visitAttribute</tt> )* [ <tt>visitCode</tt> <tt>visitMaxs</tt> ( <tt>visitFrame</tt> |
 * <tt>visit</tt><i>X</i>Insn</tt> | <tt>visitLabel</tt> | <tt>visitTryCatchBlock</tt> |
 * <tt>visitLocalVariable</tt> | <tt>visitLineNumber</tt>)* ]
 * <tt>visitEnd</tt>. In addition, the <tt>visit</tt><i>X</i>Insn</tt>
 * and <tt>visitLabel</tt> methods must be called in the sequential order of
 * the bytecode instructions of the visited code, <tt>visitTryCatchBlock</tt>
 * must be called <i>before</i> the labels passed as arguments have been
 * visited, and the <tt>visitLocalVariable</tt> and <tt>visitLineNumber</tt>
 * methods must be called <i>after</i> the labels passed as arguments have been
 * visited.
 * <br /><br />
 * Changes from ASM :
 * <ul>
 * <li> visitMaxs : the MaxLocals parameter is ignored, only MaxStack is used. It must be called
 * at the beginning of the visit of the code, not before the visitEnd.</li>
 * <li> visitFrame is ignored.</li>
 * <li> visitParameters added, because this debug information would have been lost.</li>
 * <li> visitOperationInsn added.</li>
 * <li> visitArrayOperationInsn added, because the visitOperationInsn description and
 *   parameters didn't fit.</li>
 * <li> visitArrayLengthInsn added, because no instruction fitted it accurately in
 *   their description or parameters.</li>
 * <li> visitAttribute has no effect, as Attributes are not managed.</li>
 * <li> visitFillArrayDataInsn added.</li>
 * <li> visitTypeInsn has its signature changed. Added two parameters
 *   (int destinationRegister, int referenceBearingRegister).</li>
 * <li> visitMethodInsn: added the registers parameter.</li>
 * <li> visitFieldInsn: added valueRegister and objectRegister.</li>
 * <li> visitJumpInsn: added two registers (registerA, registerB).</li>
 * <li> visitTableSwitchInsn: added a register. Max value and Default Label are ignored by the Writer,
 *   as they are not used in the encoded table. The Reader generates them, though.</li>
 * <li> visitLookupSwitchInsn: added a register. Default Label is ignored, it doesn't appear in
 *   the table. The Reader generates it, though.</li>
 * <li> visitMultiANewArrayInsn: second parameter changed to a list of registers.</li>
 * <li> removed the visitLdcInsn method.</li>
 * <li> removed the visitIincInsn method.</li>
 * </ul>
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public abstract class MethodVisitor {

    /**
     * The method visitor to which this visitor must delegate method calls. May
     * be null.
     */
    protected MethodVisitor mv;

    /**
     * The ASM API version implemented by this visitor.
     */
    final protected int api;

    /**
     * Constructs a new {@link MethodVisitor}.
     */
    public MethodVisitor(final int api) {
        this.api = api;
    }

    /**
     * Constructs a new {@link MethodVisitor}.
     *
     * @param mv the method visitor to which this visitor must delegate method
     *        calls. May be null.
     */
    public MethodVisitor(final int api, final MethodVisitor mv) {
        this.api = api;
        this.mv = mv;
    }

    // -------------------------------------------------------------------------
    // Annotations and non standard attributes
    // -------------------------------------------------------------------------

    /**
     * Visits the default value of this annotation interface method.
     *
     * @return a visitor to the visit the actual default value of this
     *         annotation interface method, or <tt>null</tt> if this visitor
     *         is not interested in visiting this default value. The 'name'
     *         parameters passed to the methods of this annotation visitor are
     *         ignored. Moreover, exactly one visit method must be called on this
     *         annotation visitor, followed by visitEnd.
     */
    public AnnotationVisitor visitAnnotationDefault() {
        if (mv!=null) {
            return mv.visitAnnotationDefault();
        }
        return null;
    }

    /**
     * Visits an annotation of this method.
     *
     * @param desc the class descriptor of the annotation class.
     * @param visible <tt>true</tt> if the annotation is visible at runtime.
     * @return a visitor to visit the annotation values, or <tt>null</tt> if
     *         this visitor is not interested in visiting this annotation.
     */
    public AnnotationVisitor visitAnnotation(String desc, boolean visible) {
        if (mv!=null) {
            return mv.visitAnnotation(desc,visible);
        }
        return null;
    }

    /**
     * Visits an annotation of a parameter this method.
     *
     * @param parameter the parameter index.
     * @param desc the class descriptor of the annotation class.
     * @param visible <tt>true</tt> if the annotation is visible at runtime.
     * @return a visitor to visit the annotation values, or <tt>null</tt> if
     *         this visitor is not interested in visiting this annotation.
     */
    public AnnotationVisitor visitParameterAnnotation(
        int parameter,
        String desc,
        boolean visible) {
        if (mv!=null) {
            return mv.visitParameterAnnotation(parameter,desc,visible);
        }
        return null;

    }
    /**
     * Visits a non standard attribute of this method. This visit is <i>ignored</i> as
     * Attributes are not supported by AsmDex.
     *
     * @param attr an attribute.
     */
    public void visitAttribute(Object attr) {
        if (mv!= null) {
            mv.visitAttribute(attr);
        }
    }

    /**
     * Starts the visit of the method's code, if any (i.e. non abstract method).
     */
    public void visitCode() {
        if (mv !=null) {
            mv.visitCode();
        }
    }

    /**
     * Visits the parameters of the method, if any.
     *
     * @param parameters a list of parameters.
     */
    public void visitParameters(String[] parameters) {
        if (mv != null) {
            mv.visitParameters(parameters);
        }
    }

    /**
     * Visits the current state of the local variables and operand stack
     * elements. <i>Ignored by AsmDex. Only used for compatibility with ASM</i>.
     *
     * @param type the type of this stack map frame. <i>Ignored by AsmDex.</i>
     * @param nLocal the number of local variables in the visited frame. <i>Ignored by AsmDex.</i>
     * @param local the local variable types in this frame. <i>Ignored by AsmDex.</i>
     * @param nStack the number of operand stack elements in the visited frame. <i>Ignored by AsmDex.</i>
     * @param stack the operand stack types in this frame. <i>Ignored by AsmDex.</i>
     */
    public void visitFrame(
        int type,
        int nLocal,
        Object[] local,
        int nStack,
        Object[] stack) {
        if (mv != null) {
            mv.visitFrame(type, nLocal, local, nStack, stack);
        }
    }

    // -------------------------------------------------------------------------
    // Normal instructions
    // -------------------------------------------------------------------------

    /**
     * Visits a zero operand instruction.
     *
     * @param opcode the opcode of the instruction to be visited. This opcode is
     *        either NOP, RETURN-VOID, or unused.
     */
    public void visitInsn(int opcode) {
        if (mv !=null) {
            mv.visitInsn(opcode);
        }
    }

    /**
     * Visits an instruction with a single register.
     *
     * @param opcode the opcode of the instruction to be visited.
     *        This opcode is either RETURN, RETURN-WIDE, RETURN-OBJECT,
     *        MOVE-RESULT, MOVE-RESULT-WIDE, MOVE-RESULT-OBJECT, MOVE-EXCEPTION,
     *        THROW vAA.
     *
     * @param register the operand of the instruction to be visited.
     */
    public void visitIntInsn(int opcode, int register) {
        if (mv !=null) {
            mv.visitIntInsn(opcode, register);
        }
    }

    /**
     * Visits a local variable instruction. A local variable instruction is an
     * instruction that loads or stores the value of a local variable.
     *
     * @param opcode the opcode of the local variable instruction to be visited.
     *        This opcode is either MOVE, MOVE/FROM16, MOVE/16, MOVE-WIDE,
     *        MOVE-WIDE/FROM16, MOVE-WIDE/16, MOVE-OBJECT, MOVE-OBJECT/FROM16,
     *        MOVE-OBJECT/16,
     *        CONST/4, CONST/16, CONST, CONST/HIGH16, CONST-WIDE/16, CONST-WIDE/32.
     * @param destinationRegister the destination register.
     * @param var the operand of the instruction to be visited. This operand is
     *        either a value or a source Register.
     */
    public void visitVarInsn(int opcode, int destinationRegister, int var) {
        if (mv != null) {
            mv.visitVarInsn(opcode, destinationRegister, var);
        }
    }

    /**
     * Visits a local variable instruction, using a Long. A local variable instruction is an
     * instruction that loads or stores the value of a local variable.
     *
     * @param opcode the opcode of the local variable instruction to be visited.
     *        This opcode is either CONST-WIDE, CONST-WIDE/HIGH16.
     * @param destinationRegister the destination register.
     * @param var the operand of the instruction to be visited. This operand is
     *        either a value or a source Register.
     */
    public void visitVarInsn(int opcode, int destinationRegister, long var) {
        if (mv != null) {
            mv.visitVarInsn(opcode, destinationRegister, var);
        }
    }
    /**
     * Visits a type instruction. A type instruction is an instruction that
     * takes the internal name of a class as parameter.
     *
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is either CONST-CLASS, NEW-ARRAY, NEW-INSTANCE, CHECK-CAST
     *        or INSTANCE-OF.
     * @param destinationRegister the destination register. Used only by CONST-CLASS,
     * 		  INSTANCE-OF, NEW-INSTANCE, NEW-ARRAY.
     * @param referenceBearingRegister reference bearing register. Only used for
     * 		  CHECK-CAST, INSTANCE-OF instructions.
     * @param sizeRegister size register. Used only for NEW-ARRAY.
     * @param type the operand of the instruction to be visited. This operand
     *        must be the internal name of an object or array class.
     */
    public void visitTypeInsn(int opcode, int destinationRegister,
                              int referenceBearingRegister, int sizeRegister, String type) {
        if (mv != null) {
            mv.visitTypeInsn(opcode, destinationRegister, referenceBearingRegister, sizeRegister, type);
        }
    }

    /**
     * Visits a field instruction. A field instruction is an instruction that
     * loads or stores the value of a field of an object.
     *
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is either IinstanceOP (IGETxx/IPUTxx) and SstaticOP (SGETXX/IPUTxx).
     * @param owner the internal name of the field's owner class.
     * @param name the field's name.
     * @param desc the field's descriptor.
     * @param valueRegister the value register.
     * @param objectRegister the objectRegister. Ignored for static operations.
     */
    public void visitFieldInsn(int opcode, String owner, String name, String desc,
                               int valueRegister, int objectRegister) {
        if (mv!=null) {
            mv.visitFieldInsn(opcode, owner, name, desc, valueRegister, objectRegister);
        }
    }

    /**
     * Visits a method instruction. A method instruction is an instruction that
     * invokes a method.
     *
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is either INVOKE-VIRTUAL, INVOKE-SUPER, INVOKE-DIRECT,
     *        INVOKE-STATIC or INVOKE-INTERFACE.
     * @param owner the internal name of the method's owner class.
     * @param name the method's name.
     * @param desc the method's descriptor.
     * @param arguments the arguments to give to the method.
     */
    public void visitMethodInsn(int opcode, String owner, String name, String desc,
                                int[] arguments) {
        if (mv != null) {
            mv.visitMethodInsn(opcode, owner, name, desc, arguments);
        }
    }

    /**
     * Visits a string instruction. A string instruction is an instruction that
     * takes the internal name of a string as parameter.
     *
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is either CONST-STRING or CONST-STRING/JUMBO.
     * @param destinationRegister the destination register.
     * @param string the string of the instruction to be visited.
     */
    public void visitStringInsn(int opcode, int destinationRegister, String string) {
        if (mv !=null) {
            mv.visitStringInsn(opcode, destinationRegister, string);
        }
    }

    /**
     * Visits an instruction that performs an unary or binary operation on one or two
     * sources or one array, possibly a literal value, to a destination register.
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is in either of these categories : UNOP, BINOP, BINOP/2ADDR,
     *        BINOP/LIT16, BINOP/LIT8.
     * @param destinationRegister the destination register or pair. For BINOP/2ADDR,
     * 		  it should be the same as firstSourceRegister.
     * @param firstSourceRegister the first source register or pair. For BINOP/2ADDR,
     * 		  it should be the same as destinationRegister.
     * @param secondSourceRegister the second source register or pair. Ignored for
     * 		  UNOP, BINOP/LIT16, BINOP/LIT8.
     * @param value literal value. Only useful for BINOP/LIT16, BINOP/LIT8.
     */
    public void visitOperationInsn(int opcode, int destinationRegister,
                                   int firstSourceRegister, int secondSourceRegister, int value) {
        if (mv!=null) {
            mv.visitOperationInsn(opcode, destinationRegister, firstSourceRegister, secondSourceRegister, value);
        }
    }

    /**
     * Visits an instruction that performs an operation on an array.
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is either AGET(-xxx) or APUT(-xxx).
     * @param valueRegister the value register. May be source or destination register/pair.
     * @param arrayRegister the array register.
     * @param indexRegister the index register.
     */
    public void visitArrayOperationInsn(int opcode, int valueRegister,
                                        int arrayRegister, int indexRegister) {
        if (mv != null) {
            mv.visitArrayOperationInsn(opcode, valueRegister, arrayRegister, indexRegister);
        }
    }

    /**
     * Visits a Fill Array Data instruction. It is only used for primitive areas. Reference
     * areas are filled programmatically.
     * @param arrayReference array reference.
     * @param arrayData array of the primitives to encode.
     */
    public void visitFillArrayDataInsn(int arrayReference, Object[] arrayData) {
        if (mv != null) {
            mv.visitFillArrayDataInsn(arrayReference, arrayData);
        }
    }

    /**
     * Visits a jump operation, conditional or not. A jump instruction is an instruction that may
     * jump to another instruction.
     *
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is either GOTO, IF-EQ, IF-NE, IF-LT, IF-GE, IF-GT, IF-LE,
     *        or IF-EQZ, IF-NEZ, IF-LTZ, IF-GEZ, IF-GTZ, IF-LEZ.
     * @param label the operand of the instruction to be visited. This operand
     *        is a label that designates the instruction to which the jump
     *        instruction may jump.
     * @param registerA (4 bits). Ignored for unconditional jump.
     * @param registerB (4 bits). Ignored for unconditional jump or zero tests (IF-EQZ,
     * 		  IF-NEZ...)
     */
    public void visitJumpInsn(int opcode, Label label, int registerA, int registerB) {
        if (mv != null) {
            mv.visitJumpInsn(opcode, label, registerA, registerB);
        }
    }

    /**
     * Visits a label. A label designates the instruction that will be visited
     * just after it.
     *
     * @param label a {@link Label Label} object.
     */
    public void visitLabel(Label label) {
        if (mv!=null) {
            mv.visitLabel(label);
        }
    }


    // -------------------------------------------------------------------------
    // Special instructions
    // -------------------------------------------------------------------------

    /**
     * Visits a TABLESWITCH instruction.
     *
     * @param register the register to test.
     * @param min the minimum key value.
     * @param max the maximum key value (ignored by the Writer).
     * @param dflt beginning of the default handler block (ignored by the Writer).
     * @param labels beginnings of the handler blocks. <tt>labels[i]</tt> is
     *        the beginning of the handler block for the <tt>min + i</tt> key.
     */
    public void visitTableSwitchInsn(int register, int min, int max, Label dflt, Label[] labels) {
        if (mv != null) {
            mv.visitTableSwitchInsn(register, min, max, dflt, labels);
        }
    }

    /**
     * Visits a LOOKUPSWITCH instruction.
     *
     * @param register the register to test.
     * @param dflt beginning of the default handler block (ignored by the Writer).
     * @param keys the values of the keys.
     * @param labels beginnings of the handler blocks. <tt>labels[i]</tt> is
     *        the beginning of the handler block for the <tt>keys[i]</tt> key.
     */
    public void visitLookupSwitchInsn(int register, Label dflt, int[] keys, Label[] labels) {
        if (mv != null) {
            mv.visitLookupSwitchInsn(register, dflt, keys, labels);
        }
    }

    /**
     * Visits a MULTIANEWARRAY instruction.
     *
     * @param desc an array type descriptor.
     * @param registers array of registers containing the values of the array.
     */
    public void visitMultiANewArrayInsn(String desc, int[] registers) {
        if (mv!=null) {
            mv.visitMultiANewArrayInsn(desc, registers);
        }
    }

    /**
     * Visits an ARRAY-LENGTH instruction.
     *
     * @param destinationRegister the destination register.
     * @param arrayReferenceBearing register referencing the array.
     */
    public void visitArrayLengthInsn(int destinationRegister, int arrayReferenceBearing) {
        if (mv !=null) {
            mv.visitArrayLengthInsn(destinationRegister, arrayReferenceBearing);
        }
    }


    // -------------------------------------------------------------------------
    // Exceptions table entries, debug information, max stack and max locals
    // -------------------------------------------------------------------------

    /**
     * Visits a try catch block.
     *
     * @param start beginning of the exception handler's scope (inclusive).
     * @param end end of the exception handler's scope (exclusive).
     * @param handler beginning of the exception handler's code.
     * @param type internal name of the type of exceptions handled by the
     *        handler, or <tt>null</tt> to catch any exceptions (for "finally"
     *        blocks).
     * @throws IllegalArgumentException if one of the labels has already been
     *         visited by this visitor (by the {@link #visitLabel visitLabel}
     *         method).
     */
    public void visitTryCatchBlock(Label start, Label end, Label handler, String type) {
        if (mv !=null) {
            mv.visitTryCatchBlock(start, end, handler, type);
        }
    }

    /**
     * Visits a local variable declaration.
     *
     * @param name the name of a local variable.
     * @param desc the type descriptor of this local variable.
     * @param signature the type signature of this local variable. May be
     *        <tt>null</tt> if the local variable type does not use generic
     *        types.
     * @param start the first instruction corresponding to the scope of this
     *        local variable (inclusive).
     * @param end the last instruction corresponding to the scope of this local
     *        variable (exclusive). May be <tt>null</tt> if we don't know it.
     * @param index the local variable's index.
     * @throws IllegalArgumentException if one of the labels has not already
     *         been visited by this visitor (by the
     *         {@link #visitLabel visitLabel} method).
     */
    public void visitLocalVariable(
        String name,
        String desc,
        String signature,
        Label start,
        Label end,
        int index) {
        if (mv!=null) {
            mv.visitLocalVariable(name, desc, signature, start, end, index);
        }
    }

    /**
     * Visits a local variable declaration.
     *
     * @param name the name of a local variable.
     * @param desc the type descriptor of this local variable.
     * @param signature the type signature of this local variable. May be
     *        <tt>null</tt> if the local variable type does not use generic
     *        types.
     * @param start the first instruction corresponding to the scope of this
     *        local variable (inclusive).
     * @param ends the last instruction corresponding to the scope of this local
     *        variable (exclusive). May be <tt>null</tt> if we don't know it. As Variables can be
     *        restarted, we may have several ends.
     * @param restarts the instruction where the local variable is restarted. May be
     * 		  <tt>null</tt> if it never happens. Variables can be restarted as many times as
     * 		  needed, hence the array.
     * @param index the local variable's index.
     * @throws IllegalArgumentException if one of the labels has not already
     *         been visited by this visitor (by the
     *         {@link #visitLabel visitLabel} method).
     */
    public void visitLocalVariable(
        String name,
        String desc,
        String signature,
        Label start,
        List<Label> ends,
        List<Label> restarts,
        int index) {
        if (mv != null) {
            mv.visitLocalVariable(name, desc, signature, start, ends, restarts, index);
        }
    }

    /**
     * Visits a line number declaration.
     *
     * @param line a line number. This number refers to the source file from
     *        which the class was compiled.
     * @param start the first instruction corresponding to this line number.
     * @throws IllegalArgumentException if <tt>start</tt> has not already been
     *         visited by this visitor (by the {@link #visitLabel visitLabel}
     *         method).
     */
    public void visitLineNumber(int line, Label start) {
        if (mv != null) {
            mv.visitLineNumber(line, start);
        }
    }

    /**
     * Visits the maximum stack size and the maximum number of local variables
     * of the method.
     *
     * @param maxStack maximum stack size of the method.
     * @param maxLocals <i>ignored</i> in AsmDex. Maximum number of local variables
     * 		  for the method.
     */
    public void visitMaxs(int maxStack, int maxLocals) {
        if (mv != null) {
            mv.visitMaxs(maxStack, maxLocals);
        }
    }

    /**
     * Visits the end of the method. This method, which is the last one to be
     * called, is used to inform the visitor that all the annotations and
     * attributes of the method have been visited.
     */
    public void visitEnd() {
        if (mv != null) {
            mv.visitEnd();
        }
    }


}
