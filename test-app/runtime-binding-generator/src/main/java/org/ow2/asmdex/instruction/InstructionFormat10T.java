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

package org.ow2.asmdex.instruction;

import java.util.LinkedList;

import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Encodes and decodes an instruction built with the Dalvik format 10T.
 *
 * @author Julien Névo
 */
public class InstructionFormat10T extends Instruction implements IOffsetInstruction {

    private static final int INSTRUCTION_SIZE = 2; // Instruction size in bytes.

    /**
     * Offset to the pointed Instruction.
     */
    private int offset;

    /**
     * Offset in bytes of the instruction from the beginning of the method.
     */
    private int instructionOffset;

    /**
     * Returns the offset pointing to the beginning of the branch, from the start of the Dex file.
     * @return the offset pointing to the beginning of the branch, from the start of the Dex file.
     */
    @Override
    public int getOffset() {
        return offset;
    }

    /**
     * Label to which to jump.
     */
    private Label label;

    /**
     * Returns the label to which the instruction jumps.
     * @return the label to which the instruction jumps.
     */
    @Override
    public Label getLabel() {
        return label;
    }

    /**
     * Constructor of the Instruction, using a Reader to parse its bytecode.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode 16-bit opcode.
     */
//	public InstructionFormat10T(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//
//		// The format is AA|op.
//		int readOffset = ((byte)(opcodeHighOrderByte & 0xff)) * 2; // * 2 because branch offsets are word based.
//		// Offset points to the beginning of the branch, from the start of the Dex file.
//		// It is related to the current opcode (hence the -2).
//		offset = reader.getPos() - 2 + readOffset;
//	}

    /**
     * Returns the Offset encoded in the given 16-bit opcode. <i>It must be performed only once after
     * having read the opcode.</i>
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode the 16-bit opcode.
     * @return the Offset from the beginning of the Dex file.
     */
    public static int getOffset(IDalvikValueReader reader, int opcode) {
        // The format is AA|op.
        int readOffset = ((byte)((opcode >> 8) & 0xff)) * 2; // * 2 because branch offsets are word based.
        // Offset points to the beginning of the branch, from the start of the Dex file.
        // It is related to the current opcode (hence the -2).
        return reader.getPos() - 2 + readOffset;
    }



    /**
     * Constructor of the Instruction by providing all the elements it's composed of.
     * @param opcode 8 or 16 bits opcode.
     * @param label Label to which is linked this Instruction.
     * @param instructionOffset offset of this Instruction from the beginning of its Method.
     */
    public InstructionFormat10T(int opcode, Label label, int instructionOffset) {
        super(opcode);

        this.label = label;
        this.instructionOffset = instructionOffset;
    }

    protected static final LinkedList<InstructionFormat10T> instances = new LinkedList<InstructionFormat10T>();
    public static final InstructionFormat10T obtain(int opcode, Label label, int instructionOffset) {
        InstructionFormat10T instruction = instances.poll();
        if (instruction == null) {
            return new InstructionFormat10T(opcode, label, instructionOffset);
        }

        instruction.opcodeHighOrderByte = (opcode >> 8) & 0xff;
        instruction.opcodeByte = opcode & 0xff;

        instruction.label = label;
        instruction.instructionOffset = instructionOffset;

        return instruction;
    }

    public void recycle(Instruction instruction) {
        instances.add((InstructionFormat10T)instruction);
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

//	@Override
//	public int getNbEncodedRegisters() {
//		return 0;
//	}

//	@Override
//	public int[] getUsedRegisters() {
//		return null;
//	}

    @Override
    public int getInstructionOffset() {
        return instructionOffset;
    }

    @Override
    public void setInstructionOffset(int instructionOffset) {
        this.instructionOffset = instructionOffset;
    };

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        // The format is AA|op.
        int firstShort = ((((label.getOffset() - instructionOffset) / 2)
                           & 0xff) << 8) + opcodeByte; // / 2 because offsets are word-based.
        out.putShort(firstShort);
    }
}
