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

import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Encodes and decodes an instruction built with the Dalvik format 21T.
 *
 * @author Julien Névo
 */
public class InstructionFormat21T extends Instruction
    implements IOneRegisterInstruction, IOffsetInstruction {

    private static final int INSTRUCTION_SIZE = 4; // Instruction size in bytes.

    private int registerA;
    private int offset;

    /**
     * Offset in bytes of the instruction from the beginning of the method.
     */
    private int instructionOffset;

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
//	public InstructionFormat21T(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//
//		// The format is AA|op BBBB.
//		registerA = opcodeHighOrderByte;
//		int readOffset = reader.sshort() * 2; // * 2 because branch offsets are word based.
//		// Offset points to the beginning of the branch, from the start of the Dex file.
//		offset = reader.getPos() - 4 + readOffset;
//	}

    /**
     * Returns the RegisterA encoded in the given 16-bit opcode.
     * @return the RegisterA.
     */
    public static int getRegisterA(int opcode) {
        return (opcode >> 8) & 0xff;
    }

    /**
     * Returns the Offset encoded in the given 16-bit opcode. <i>It must be performed only once after
     * having read the opcode.</i>
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode the 16-bit opcode.
     * @return the Offset from the beginning of the Dex file.
     */
    public static int getOffset(IDalvikValueReader reader, int opcode) {
        int readOffset = reader.sshort() * 2; // * 2 because branch offsets are word based.
        // Offset points to the beginning of the branch, from the start of the Dex file.
        return reader.getPos() - 4 + readOffset;
    }

    /**
     * Skips the data of this Instruction.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     */
    public static void skip(IDalvikValueReader reader) {
        reader.relativeSeek(INSTRUCTION_SIZE - 2); // The first word has already been read.
    }

    /**
     * Constructor of the Instruction by providing all the elements it's composed of.
     * @param opcode 8 or 16 bits opcode.
     * @param label Label to which is linked this Instruction.
     * @param registerA the register to store.
     * @param instructionOffset offset of this Instruction from the beginning of its Method.
     */
    public InstructionFormat21T(int opcode, Label label, int registerA, int instructionOffset) {
        super(opcode);

        this.label = label;
        this.registerA = registerA;
        this.instructionOffset = instructionOffset;
    }

    @Override
    public int getRegisterA() {
        return registerA;
    }

    @Override
    public int getOffset() {
        return offset;
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

//	@Override
//	public int[] getUsedRegisters() {
//		return new int[] { registerA };
//	}

//	@Override
//	public int getNbEncodedRegisters() {
//		return 1;
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
        test8BitsLimit(registerA);
        // The format is AA|op BBBB.
        int firstShort = ((registerA & 0xff) << 8) + opcodeByte;
        int secondShort = (label.getOffset() - instructionOffset) / 2; // / 2 because offsets are word-based.
        out.putShort(firstShort);
        out.putShort(secondShort);
    }

}
