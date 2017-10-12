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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.ow2.asmdex.instruction.IOffsetInstruction;
import org.ow2.asmdex.instruction.IPseudoInstruction;
import org.ow2.asmdex.instruction.Instruction;
import org.ow2.asmdex.instruction.InstructionFormat10X;
import org.ow2.asmdex.instruction.InstructionFormat20T;
import org.ow2.asmdex.instruction.InstructionFormat30T;
import org.ow2.asmdex.instruction.PseudoInstructionFillArrayData;
import org.ow2.asmdex.instruction.PseudoInstructionPackedSwitch;
import org.ow2.asmdex.instruction.PseudoInstructionSparseSwitch;
import org.ow2.asmdex.lowLevelUtils.InstructionEncoder;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureCommon.LocalVariable;
import org.ow2.asmdex.structureWriter.AnnotationItem;
import org.ow2.asmdex.structureWriter.CodeItem;
import org.ow2.asmdex.structureWriter.ConstantPool;
import org.ow2.asmdex.structureWriter.ExceptionHandler;
import org.ow2.asmdex.structureWriter.Field;
import org.ow2.asmdex.structureWriter.Method;
import org.ow2.asmdex.structureWriter.Prototype;
import org.ow2.asmdex.structureWriter.TryCatch;

/**
 * Class that takes care of visiting all the elements of a method.
 * <br /><br />
 * RegisterSize, Incoming Arguments Size, Outgoing Arguments Size<br />
 * --------------------------------------------------------------<br />
 * The RegisterSize should be calculated like that :
 * <ul>
 * <li> First, the local registers (watching the pair registers according to the opcodes !).</li>
 * <li> Then, "this" if the method is non-static AND not a constructor.</li>
 * <li> Then, the parameters (1 (word) for int... 2 for double, long, references).</li>
 * </ul>
 * But we can't calculate it because we don't know if "this" or the parameters are used.
 * So we have to rely on visitMaxs() to give us the RegisterSize.
 * <br /><br />
 * The Incoming Argument Size : calculating it consists in parsing the descriptor, minus the
 * return type, and counting how many bytes are allocated to each type (I = 2 (bytes), D = 4...).
 * Add 2 for "this", unless it's a Static method.
 * <br /><br />
 * The Outgoing Arguments Size : a bit the same. Each time a Method is called inside the current
 * Method, calculates how many bytes are allocated to each type (I = 2 bytes, D = 4, L = 4...) in the
 * descriptor, minus the return type. Add 2 for "this", unless the called method is Static.
 * <br /><br />
 * Labels Management<br />
 * -----------------<br />
 * <ul>
 * <li>After the first pass, at the end of the visit of the Method (visitEnd), we know where all the labels
 *   are, IN THE ORIGINAL CODE.</li>
 * <li> The unconditional Jump instructions are encoded as they are given. They will of course be
 *   extended if needed.</li>
 * <li> We must know what are the instructions that refer to the Labels, in order to check if their
 *   range is enough. So each Label has a list of the instructions making reference to it and that are
 *   subject to be too far (Unconditional Jumps only. Conditional jumps are always 16 bits which should
 *   be enough, if not, there is no 32 bits jump, this case is NOT handled for now. Switch cases are 32 bits).</li>
 * <li> So such instruction must know what is its offset inside the code. They derive from OffsetInstruction
 *   which declares the offsetInstruction field.</li>
 * <li> We scan all the OffsetInstructions inside each Label, and checks if the range to their target Label
 *   is valid. If not, we transform the Instruction to a one with a bigger range (if possible), shift all
 *   the Labels and OffsetInstructions that are beyond this Instruction accordingly, and make the scan
 *   once again.</li>
 * </ul>
 *
 * @author Julien Névo
 */
public class MethodWriter extends MethodVisitor {

    static final int MAXIMUM_SIGNED_VALUE_8_BITS = 0x7f;
    static final int MAXIMUM_SIGNED_VALUE_16_BITS = 0x7fff;
    static final int MINIMUM_SIGNED_VALUE_16_BITS = -0x8000;
    static final int MAXIMUM_SIGNED_VALUE_32_BITS = 0x7fffffff;

    /**
     * The Class Writer of this Method Writer.
     */
    private ClassWriter classWriter;

    /**
     * The Constant Pool of the Application.
     */
    private ConstantPool constantPool;

    /**
     * Contains all the information about the Method.
     */
    private Method method;

    /**
     * List of the Pseudo Instruction encountered, linked with a Label whose offset must be set.
     * Elements are added as we encounter switches and arrays to fill, and will be parsed at
     * the end of the Method, in order to actually write the structures and resolve the Labels.
     */
    private ArrayList<Instruction> pseudoInstructionList = new ArrayList<Instruction>();

    /**
     * Indicates the next Line Number. It is assigned to the next Instruction found, then set to 0 at
     * this moment, as a Line Number must be superior to 0. This Line Number is set when visiting a
     * Line Number.
     */
    private int nextLineNumber = 0;

    /**
     * Constructor of the Method Writer. Requires the data of the Method invocation.
     * @param classWriter the classWriter of the method.
     * @param access the access flags of the method.
     * @param name the name of the method.
     * @param desc the descriptor of the method.
     * @param signature the signature of the method. May be Null.
     * @param exceptions the exceptions of the method. May be Null.
     */
    public MethodWriter(ClassWriter classWriter, int access, String name,
                        String desc, String[] signature, String[] exceptions) {
        super(Opcodes.ASM4);
        this.classWriter = classWriter;

        constantPool = classWriter.getConstantPool();

        // Adds the method to the constant pool.
        method = constantPool.addMethodToConstantPool(name, classWriter.getName(), desc,
                 access, signature, exceptions);
        CodeItem codeItem = getCodeItem();

        if (codeItem != null) {
            // Calculate the Incoming Argument Size. This consists in parsing the descriptor, minus the
            // return type, and counting how many bytes are allocated to each type (I = 2, D = 4...).
            // Static methods don't have a "this" argument (in bytes).
            int ias = (access & Opcodes.ACC_STATIC) > 0 ? 0 : 2;
            // We now parse the descriptor, minus the return type.
            ias += Prototype.getSizeOfDescriptor(desc, true);
            codeItem.setIncomingArgumentsSizeInWord(ias / 2);
        }
    }


    // -----------------------------------------------------
    // Getters and Setters.
    // -----------------------------------------------------

    /**
     * Returns the ClassWriter of this MethodWriter.
     */
    public ClassWriter getClassWriter() {
        return classWriter;
    }

    /**
     * Sets the start of the bytecode to copy from the input Dex file to the output.
     * This is only useful when using the optimization that consists in copying part of
     * the Constant Pool and the bytecode of methods that doesn't change, if the Reader is linked
     * to the Writer with no Adapter to modify the methods in between.
     * @param start start in bytes from the beginning of the Dex file where the bytecode is. This
     *        includes the code_item header.
     */
    public void setStartBytecodeToCopy(int start) {
        method.setStartBytecodeToCopy(start);
    }

    /**
     * Returns the Method linked to this Writer.
     * @return the Method linked to this Writer.
     */
    public Method getMethod() {
        return method;
    }

    /**
     * Returns the Code Item linked to this Writer. May be Null if it hasn't any (if abstract or interface).
     * @return the Code Item linked to this Writer, or Null.
     */
    public CodeItem getCodeItem() {
        return method.getCodeItem();
    }


    // -----------------------------------------------------
    // Public Methods.
    // -----------------------------------------------------

    /**
     * Adds a label to the set of used labels. If Null, nothing is done.
     * @param label the label to add.
     */
    public void addLabel(Label label) {
        getCodeItem().addLabel(label);
    }

    /**
     * Adds an instruction to the current Code Item. Also adds the Line Number to the
     * Instruction, and resets it.
     * @param insn instruction to add to the Code Item.
     */
    public void addInstruction(Instruction insn) {
        getCodeItem().addInstruction(insn);

        if (nextLineNumber > 0) {
            insn.setLineNumber(nextLineNumber);
            nextLineNumber = 0;
        }
    }

    /**
     * Adds padding at the end of the file, using an alignment of 4, if necessary.
     * The padding is done using a NOP Instruction.
     * @return the padding added, in bytes.
     */
    public int addPadding() {
        CodeItem codeItem = getCodeItem();
        int padding = codeItem.getSize() % 4;
        if (padding != 0) {
            if (padding == 2) {
                codeItem.addInstruction(new InstructionFormat10X(0));
            } else {
                throw new RuntimeException("Padding can only be 0 or 2 ! (" + method.getMethodName() + " " + method.getClassName() + " " + codeItem.getSize() + " " + padding + ")");
            }
        }
        return padding;
    }


    // -----------------------------------------------------
    // Visitor Methods.
    // -----------------------------------------------------

    @Override
    public void visitCode() {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitCode.");
        }
    }

    @Override
    public void visitEnd() {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitEnd.");
        }

        // Now that the annotations are fully known, we can register them to the Constant Pool.
        closeAnnotations();

        // There's no need to check the references, parse the pseudo-instructions and generate bytecode
        // if there aren't any code.
        // This can also be the case when the ConstantPool optimization is on, as no bytecode is parsed.
        // We can free the code_item and debug_item from their stored instructions, labels and so on, in
        // order to save memory.
        CodeItem codeItem = getCodeItem();
        if (codeItem == null) {
            return;
        }
        if (codeItem.getSize() == 0) {
            method.free();
            return;
        }

        // We check if the Instructions referring the Labels all have a valid range.
        // If not, the Instruction are modified.
        checkAndCorrectLabelReferences();

        // Then, we may have Pseudo Instructions to add at the end.
        // We do that after the Labels correction on purpose : the Pseudo Instructions may need
        // padding. It is simpler to have the final code structure before adding these Instructions,
        // so that we don't have to add/remove padding each time an Instruction is modified.
        //CodeItem codeItem = getCodeItem();
        for (Instruction insn : pseudoInstructionList) {
            addPadding();
            IPseudoInstruction ps = (IPseudoInstruction)insn;
            // Resolves the Label. Gets it from the Instruction that is linked to the Pseudo
            // Instruction and sets its offset now that we know it.
            Label label = ps.getSourceInstruction().getLabel();
            label.setOffset(codeItem.getSize());
            addInstruction(insn);
        }

        // We generate the bytecode now, so that we can free the code_item and debug_item from their
        // stored instructions, labels and so on, in order to save memory.
        method.generateCodeItemCode();
        method.free();
    }

    @Override
    public void visitMethodInsn(int opcode, String owner, String name,
                                String desc, int[] arguments) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("          MethodWriter : visitMethodInsn(). Opcode = 0x" +
                             Integer.toHexString(opcode) + ", owner = " + owner +
                             ", name = " + name + ", desc = " + desc + ", parameters = ");
            for (int parameter : arguments) {
                System.out.print(parameter + ", ");
            }
            System.out.println();
        }

        // We found an instruction related to a Method (call...). This Method could have been found before, or
        // not, in which case Constant Pool adds it. In both cases, we make the newly found Instruction
        // refer to this Method.
        Method newMethod = constantPool.addMethodToConstantPool(name, owner, desc, Opcodes.ACC_UNKNOWN, null, null);
        Instruction insn = InstructionEncoder.encodeMethodInsn(opcode, newMethod, arguments);
        addInstruction(insn);

        CodeItem codeItem = getCodeItem();

        // Calculates the Outgoing Arguments Size.
        int storedOutgoingSize = codeItem.getOutgoingArgumentsSizeInWord();
        // We now parse the descriptor, minus the return type.
        int outgoingSize = (Prototype.getSizeOfDescriptor(desc, true) / 2);
        // If the opcode of the call shows a non-static call, we have to count "this" in
        // the current Outgoing Arguments size.
        if (opcode != Opcodes.INSN_INVOKE_STATIC && opcode != Opcodes.INSN_INVOKE_STATIC_RANGE) {
            outgoingSize++;
        }

        if (outgoingSize > storedOutgoingSize) {
            codeItem.setOutgoingArgumentsSizeInWord(outgoingSize);
        }

        // We add the Prototype to the Prototype pool.
        Prototype prototype = constantPool.addPrototypeToConstantPool(desc);

        // Adds the Types to the constant pool. Then can be found in the newly created Prototype.
        constantPool.addTypeListToConstantPool(prototype.getParameterTypes());

    }

    @Override
    public void visitParameters(String[] parameters) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("          MethodWriter : visitParameters(). Parameters = ");
            if (parameters != null) {
                for (String parameter : parameters) {
                    System.out.print(parameter + ", ");
                }
            }
            System.out.println();
        }

        // Adds the Parameters as Strings in the constant pool, but ONLY if there are not empty.
        // As we don't refer to them in the Debug Item, it is not useful to encode an empty String.
        if (parameters != null) {
            for (String parameter : parameters) {
                if (!parameter.equals("")) {
                    constantPool.addStringToConstantPool(parameter);
                }
            }
        }

        method.setParameters(parameters);
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, int var) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitVarInsn(). Opcode = 0x" +
                               Integer.toHexString(opcode) + ", destinationRegister = " + destinationRegister +
                               ", var = 0x" + Integer.toHexString(var)
                              );
        }

        Instruction insn = InstructionEncoder.encodeVarInsn(opcode, destinationRegister, var);
        addInstruction(insn);
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, long var) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitVarInsn(). Opcode = 0x" +
                               Integer.toHexString(opcode) + ", destinationRegister = " + destinationRegister +
                               ", var (long) = 0x" + Long.toHexString(var)
                              );
        }

        Instruction insn = InstructionEncoder.encodeVarInsn(opcode, destinationRegister, var);
        addInstruction(insn);
    }


    @Override
    public void visitInsn(int opcode) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitInsn(). Opcode = 0x" +
                               Integer.toHexString(opcode));
        }

        addInstruction(InstructionEncoder.encodeInsn(opcode));
    }

    @Override
    public void visitLabel(Label label) {
        CodeItem codeItem = getCodeItem();
        label.setOffset(codeItem.getSize());
        addLabel(label);

        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitLabel(). Label = " + label);
        }
    }




    @Override
    public void visitOperationInsn(int opcode, int destinationRegister,
                                   int firstSourceRegister, int secondSourceRegister, int value) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitOperationInsn(). Opcode = 0x" +
                               Integer.toHexString(opcode)
                               + ", destinationRegister = " + destinationRegister
                               + ", firstSourceRegister = " + firstSourceRegister
                               + ", secondSourceRegister = " + secondSourceRegister
                               + ", value = 0x" + Integer.toHexString(value)
                              );
        }

        Instruction insn = InstructionEncoder.encodeOperationInsn(opcode, destinationRegister,
                           firstSourceRegister, secondSourceRegister, value);
        addInstruction(insn);
    }

    @Override
    public void visitIntInsn(int opcode, int register) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitIntInsn(). Opcode = 0x" +
                               Integer.toHexString(opcode)
                               + ", operand = " + register);
        }
        Instruction insn = InstructionEncoder.encodeIntInsn(opcode, register);
        addInstruction(insn);
    }

    @Override
    public void visitJumpInsn(int opcode, Label label, int registerA,
                              int registerB) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitJumpInsn(). Opcode = 0x" +
                               Integer.toHexString(opcode)
                               + ", label = " + label
                               + ", destinationRegister = " + registerA
                               + ", firstSourceRegister = " + registerB);
        }

        // Note that we also provide the instruction offset so that the instruction knows its offset,
        // to help calculate later if the Label it refers is within range or not.
        CodeItem codeItem = getCodeItem();
        Instruction insn = InstructionEncoder.encodeJumpInsn(opcode, label, registerA, registerB, codeItem.getSize());

        // Make the label refer to the instruction, at this specific offset.
        label.addReferringInstruction(insn);

        addInstruction(insn);
        addLabel(label);
    }

    @Override
    public void visitFillArrayDataInsn(int arrayReference, Object[] arrayData) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("          MethodWriter : visitFillArrayDataInsn(). "
                             + "ArrayReference = " + Integer.toHexString(arrayReference)
                             + ", arrayData = ");
            for (Object data : arrayData) {
                System.out.print(data + ", ");
            }
            System.out.println();
        }

        // The array data are given by the parameters. However, they are not encoded directly in the
        // Dalvik instruction now, but as a reference. First we encode the fill-array-data instruction,
        // with a link to a Label where we will encode the array.
        CodeItem codeItem = getCodeItem();
        Label arrayLabel = new Label();
        Instruction insn = InstructionEncoder.encodeFillArrayDataInsn(0x26, arrayReference, arrayLabel, codeItem.getSize());
        addInstruction(insn);
        // Adds the fill-array-data pseudo instruction in a List to be parsed at the end of the
        // method. We'll encode the array here.
        Instruction ps = new PseudoInstructionFillArrayData(arrayData, (IOffsetInstruction)insn);
        pseudoInstructionList.add(ps);
    }

    @Override
    public void visitTypeInsn(int opcode, int destinationRegister,
                              int referenceBearingRegister, int sizeRegister, String type) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitTypeInsn(). Opcode = 0x" +
                               Integer.toHexString(opcode)
                               + ", destinationRegister = " + destinationRegister
                               + ", refBearingRegister = " + referenceBearingRegister
                               + ", sizeRegister = " + sizeRegister
                               + ", type = " + type);
        }

        // Registers the given Type.
        constantPool.addTypeToConstantPool(type);

        Instruction insn = InstructionEncoder.encodeTypeInsn(opcode, destinationRegister,
                           referenceBearingRegister, sizeRegister, type);
        addInstruction(insn);
    }

    @Override
    public void visitMultiANewArrayInsn(String desc, int[] registers) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("          MethodWriter : visitMultiANewArrayInsn(). "
                             + "Desc = " + desc + ", registers = ");
            for (int register : registers) {
                System.out.print("0x" + Integer.toHexString(register) + ", ");
            }
            System.out.println();
        }

        // Registers the given Type.
        constantPool.addTypeToConstantPool(desc);

        Instruction insn = InstructionEncoder.encodeMultiANewArrayInsn(desc, registers);
        addInstruction(insn);
    }

    @Override
    public void visitTableSwitchInsn(int register, int min, int max,
                                     Label dflt, Label[] labels) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("          MethodWriter : visitTableSwitchInsn(). "
                             + "Register = 0x" + Integer.toHexString(register)
                             + ", min = " + min
                             + ", max = " + max
                             + ", default = " + dflt + ". Labels = "
                            );
            for (Label label : labels) {
                System.out.print(label + ", ");
            }
            System.out.println();
        }

        // The max parameter is not used (can be calculated if needed).
        // The Default Label is also not used. It is not encoded in the Table (unless to fill the "holes"
        // in a Switch/Case, but it then appears as normal entries in the Table). The Default code is
        // encoded just after the Switch/Case.

        // The Table Switch is composed of the Instruction, plus the Table (the Pseudo Instruction).
        // However, this one is not encoded directly in the Instruction, but as a reference.
        // First we encode the Instruction, with a Label that has no offset for now.
        Label switchLabel = new Label();
        CodeItem codeItem = getCodeItem();
        Instruction insn = InstructionEncoder.encodeTableSwitchInsn(register, switchLabel, codeItem.getSize());
        addInstruction(insn);

        // Adds the packed-switch pseudo instruction in a List to be parsed at the end of the
        // method. We'll encode the table here.
        Instruction ps = new PseudoInstructionPackedSwitch(min, labels, (IOffsetInstruction)insn);
        pseudoInstructionList.add(ps);
    }

    @Override
    public void visitLookupSwitchInsn(int register, Label dflt, int[] keys, Label[] labels) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("          MethodWriter : visitLookupSwitchInsn(). "
                             + "Register = 0x" + Integer.toHexString(register)
                             + ", default = " + dflt + ". Keys+Labels = "
                            );
            for (int i = 0; i < keys.length; i++) {
                System.out.print(keys[i] +"-->" + labels[i] + ", ");
            }
            System.out.println();
        }

        // The Default Label is not used, as it is not encoded in the Table.

        // The Sparse Switch is composed of the Instruction, plus the Table. However, this one is not encoded
        // directly in the Instruction, but as a reference. First we encode the Instruction,
        // with a link to a Label where we will encode the Table.
        CodeItem codeItem = getCodeItem();
        Label switchLabel = new Label();
        Instruction insn = InstructionEncoder.encodeSparseSwitchInsn(register, switchLabel, codeItem.getSize());
        addInstruction(insn);

        // Adds the sparse-switch pseudo instruction in a List to be parsed at the end of the
        // method. We'll encode the table here.
        Instruction ps = new PseudoInstructionSparseSwitch(keys, labels, (IOffsetInstruction)insn);
        pseudoInstructionList.add(ps);
    }

    @Override
    public void visitTryCatchBlock(Label start, Label end, Label handler,
                                   String type) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitTryCatchBlock(). "
                               + "start = " + start
                               + ", end = " + end
                               + ", handler = " + handler);
        }

        // Registers the given Type.
        constantPool.addTypeToConstantPool(type);

        addLabel(end);
        addLabel(handler);

        // A Try/Catch block isn't encoded as an Instruction. We store it as is for it to be encoded
        // when the code_item has to be. The Try/Catch are encoded after the Instructions.
        CodeItem codeItem = getCodeItem();
        codeItem.addTryCatch(new TryCatch(start, end, new ExceptionHandler(handler, type)));
    }

    @Override
    public AnnotationVisitor visitAnnotation(String desc, boolean visible) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitAnnotation(). "
                               + "Desc = " + desc
                               + ", visible = " + visible
                              );
        }

        // Creates an AnnotationItem, incomplete for now (no Elements), and registers it to the Method.
        // We do not add it to the Constant Pool now because of this incompleteness.
        AnnotationWriter annotationWriter = AnnotationWriter.createAnnotationWriter(desc, visible, constantPool, null);
        method.addAnnotationItem(annotationWriter.getAnnotationItem());

        return annotationWriter;
    }

    @Override
    public AnnotationVisitor visitParameterAnnotation(int parameter,
            String desc, boolean visible) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("    MethodWriter : visitParameterAnnotation. Parameter = " + parameter
                               + ", desc = " + desc
                               + ", visible = " + visible
                              );
        }

        // Creates an AnnotationItem, incomplete for now (no Elements), and registers it to the Method.
        // We do not add it to the Constant Pool now because of this incompleteness.
        AnnotationItem annotationItem = new AnnotationItem(visible, desc);
        method.addParameterAnnotationItem(parameter, annotationItem);

        // Adds the desc to the Constant Pool.
        constantPool.addTypeToConstantPool(desc);

        return new AnnotationWriter(constantPool, annotationItem);
    }

    @Override
    public void visitArrayLengthInsn(int destinationRegister,
                                     int arrayReferenceBearing) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitArrayLength(). "
                               + "destinationRegister = " + destinationRegister
                               + ", arrayReferenceBearing = " + arrayReferenceBearing);
        }

        Instruction insn = InstructionEncoder.encodeArrayLength(destinationRegister, arrayReferenceBearing);
        addInstruction(insn);
    }


    @Override
    public void visitArrayOperationInsn(int opcode, int valueRegister,
                                        int arrayRegister, int indexRegister) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitArrayOperationInsn(). "
                               + "opcode = " + Integer.toHexString(opcode)
                               + ", valueRegister = " + valueRegister
                               + ", arrayRegister = " + arrayRegister
                               + ", indexRegister = " + indexRegister
                              );
        }

        Instruction insn = InstructionEncoder.encodeArrayOperation(opcode, valueRegister, arrayRegister, indexRegister);
        addInstruction(insn);
    }

    @Override
    public void visitStringInsn(int opcode, int destinationRegister, String string) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitStringInsn(). "
                               + "opcode = " + Integer.toHexString(opcode)
                               + ", destinationRegister = " + destinationRegister
                               + ", string = " + string
                              );
        }

        // Adds the string to the Constant Pool.
        constantPool.addStringToConstantPool(string);

        Instruction insn = InstructionEncoder.encodeStringOperation(opcode, destinationRegister, string);
        addInstruction(insn);
    }

    @Override
    public void visitFieldInsn(int opcode, String owner, String name,
                               String desc, int valueRegister, int objectRegister) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitFieldInsn(). "
                               + "opcode = " + Integer.toHexString(opcode)
                               + ", owner = " + owner
                               + ", name = " + name
                               + ", desc = " + desc
                               + ", valueRegister = " + valueRegister
                               + ", objectRegister = " + objectRegister
                              );
        }

        // We found an instruction related to a Field. This Field could have been found before, or not, in
        // which case the Constant Pool will add it. In both cases, we make the newly found instruction
        // refer to this Field.
        Field field = constantPool.addFieldToConstantPool(name, desc, owner, Opcodes.ACC_UNKNOWN, null, null);

        Instruction insn = InstructionEncoder.encodeFieldInsn(opcode, valueRegister, objectRegister, field);
        if (insn != null) {
            addInstruction(insn);
        } else {
            throw new RuntimeException("MethodWriter.visitFieldInsn");
        }
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, Label end, int index) {
        List<Label> ends = null;
        if (end != null) {
            ends = new ArrayList<Label>(1);
            ends.add(end);
        }
        visitLocalVariable(name, desc, signature, start, ends, null, index);
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, List<Label> ends, List<Label> restarts, int index) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitLocalVariable(). "
                               + "Name = " + name
                               + ", desc = " + desc
                               + ", signature = " + signature
                               + ", start = " + start
                               + ", index = " + index
                              );
            System.out.print("             LabelEnds = ");
            if (ends != null) {
                for (Label l: ends) {
                    System.out.print(l +" --- ");
                }
            }
            System.out.println();
            System.out.print("             LabelRestarts = ");
            if (restarts != null) {
                for (Label l: restarts) {
                    System.out.print(l +" --- ");
                }
            }
            System.out.println();
        }

        constantPool.addStringToConstantPool(name);
        constantPool.addTypeToConstantPool(desc);
        constantPool.addStringToConstantPool(signature);

        LocalVariable localVariable = new LocalVariable(index, name, desc, signature, start, ends, restarts);
        method.addLocalVariable(localVariable);
    }

    @Override
    public void visitLineNumber(int line, Label start) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitLineNumber(). "
                               + "Line = " + line
                               + ", start = " + start
                              );
        }

        // We don't use the Start Label. There's always a visitLabel before...
        nextLineNumber = line;
        method.setFirstLineNumberIfNeeded(nextLineNumber);
    }

    @Override
    public void visitMaxs(int maxStack, int maxLocals) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitMaxs(). MaxStack = " + maxStack);
        }

        // maxLocals is ignored by AsmDex.
        method.getCodeItem().setRegisterSize(maxStack);
    }

    @Override
    public AnnotationVisitor visitAnnotationDefault() {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          MethodWriter : visitAnnotationDefault.");
        }

        return AnnotationWriter.createAnnotationWriter(Constants.ANNOTATION_DEFAULT_INTERNAL_NAME, false, constantPool,
                classWriter.getClassDefinitionItem());
    }

    @Override
    public void visitAttribute(Object attr) {
        // Method ignored by AsmDex. Attributes are not supported.
    }

    @Override
    public void visitFrame(int type, int nLocal, Object[] local, int nStack,
                           Object[] stack) {
        // Method ignored by AsmDex. Attributes are not supported.
    }


    // ---------------------------------------------------
    // Private methods.
    // ---------------------------------------------------

    /**
     * Checks if the Instructions making references to each Label is valid. That is, that the range of
     * its instruction is enough. If not, corrects it by modifying the Instruction.
     */
    private void checkAndCorrectLabelReferences() {

        boolean madeChange;
        CodeItem codeItem = getCodeItem();
        List<Label> labels = codeItem.getLabels();

        // Parses all the Labels, and all the OffsetInstructions inside the Labels.
        // We stop the parsing whenever a change of instruction has been made, to restart the parsing
        // once again till everything is valid.
        do {
            madeChange = false;
            Iterator<Label> labelIterator = labels.iterator();
            while (!madeChange && labelIterator.hasNext()) {
                Label label = labelIterator.next();
                ArrayList<Instruction> instructions = label.getReferringInstructions();

                int insnIndex = 0;
                int nbInsn = instructions.size();
                while (!madeChange && (insnIndex < nbInsn)) {
                    Instruction insn = instructions.get(insnIndex);

                    IOffsetInstruction offsetInsn = (IOffsetInstruction)insn;
                    int instructionOffset = offsetInsn.getInstructionOffset();
                    // The offset have to be divided by two, because the encoded offset is word based.
                    // The relativeOffset is the offset for the Instruction to reach the Label.
                    // It is negative if the Label is before the Instruction.
                    int relativeOffset = (label.getOffset() - instructionOffset) / 2;

                    int opcode = insn.getOpcodeByte();
                    int maximumOffset = 0;
                    // Check if the relative offset is valid for the instruction range.
                    switch (opcode) {
                    case 0x28: // Goto 8 bits.
                        maximumOffset = MAXIMUM_SIGNED_VALUE_8_BITS;
                        break;
                    case 0x29: // Goto 16 bits.
                    case 0x32: // If-test.
                    case 0x33:
                    case 0x34:
                    case 0x35:
                    case 0x36:
                    case 0x37:
                    case 0x38: // If-testz.
                    case 0x39:
                    case 0x3a:
                    case 0x3b:
                    case 0x3c:
                    case 0x3d:
                        maximumOffset = MAXIMUM_SIGNED_VALUE_16_BITS;
                        break;
                    case 0x2a: // Goto 32 bits.
                    case 0x2b: // Packed Switch.
                    case 0x2c: // Sparse Switch.
                        maximumOffset = MAXIMUM_SIGNED_VALUE_32_BITS;
                        break;
                    default:
                        try {
                            throw new Exception("Opcode error : 0x" + Integer.toHexString(opcode));
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }

                    int minimumOffset = -maximumOffset - 1;

                    if ((relativeOffset < minimumOffset) || (relativeOffset > maximumOffset)) {
                        // Must change to an Instruction with a bigger range. This is only possible for
                        // the GOTO 8/16 bits.
                        if ((opcode == Opcodes.INSN_GOTO) || (opcode == Opcodes.INSN_GOTO_16)) {
                            Instruction newInsn;
                            // Change to 16 or 32 bits ?
                            if ((relativeOffset > MAXIMUM_SIGNED_VALUE_16_BITS) || (relativeOffset < MINIMUM_SIGNED_VALUE_16_BITS)) {
                                // 32 bits.
                                newInsn = new InstructionFormat30T(Opcodes.INSN_GOTO_32, label, instructionOffset);
                            } else {
                                // 16 bits.
                                newInsn = new InstructionFormat20T(Opcodes.INSN_GOTO_16, label, instructionOffset);
                            }

                            // Removes the instruction from the codeItem and replaces it with the new one.
                            codeItem.replaceInstructions(insn, newInsn);

                            // Replaces the old instruction with the new one in the Label Instructions list.
                            instructions.remove(insnIndex);
                            instructions.add(insnIndex, newInsn);

                            // Shifts all the Jump related instructions and the labels AFTER this instruction.
                            shiftOffsetInstructionsAndLabels(instructionOffset, newInsn.getSize() - insn.getSize());
                            madeChange = true;

                        } else {
                            try {
                                throw new IllegalArgumentException("Instruction Range extension unhandled. Opcode : 0x" + Integer.toHexString(opcode));
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }

                    insnIndex++;
                }

            }
        } while (madeChange);
    }

    /**
     * Shifts all the Offset Instructions and Labels of a given number of bytes, from the <i>exclusive</i>
     * offset given. All labels and instructions below or equal are ignored.
     * @param shiftOffset <i>exclusive</i> offset from which the shift begins.
     * @param shiftSize shift in bytes to add.
     */
    private void shiftOffsetInstructionsAndLabels(int shiftOffset, int shiftSize) {
        if (shiftSize != 0) {
            CodeItem codeItem = getCodeItem();
            List<Label> labels = codeItem.getLabels();
            for (Label label : labels) {
                // Shifts the Label offset if needed.
                int labelOffset = label.getOffset();
                if (labelOffset > shiftOffset) {
                    label.setOffset(labelOffset + shiftSize);
                }
                // Scans all its instructions and shifts them if needed.
                for (Instruction instruction : label.getReferringInstructions()) {
                    IOffsetInstruction offsetInstruction = (IOffsetInstruction)instruction;
                    int instructionOffset = offsetInstruction.getInstructionOffset();
                    if (instructionOffset > shiftOffset) {
                        offsetInstruction.setInstructionOffset(instructionOffset + shiftSize);
                    }
                }
            }
        }
    }

    /**
     * Closes and registers the annotation_set_items and annotation_set_ref_items of this Method.
     * This must only be done once when the Method has been fully visited.
     */
    public void closeAnnotations() {
        method.closeAnnotations(constantPool);
    }
}
