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

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;
import org.ow2.asmdex.structureWriter.ConstantPool;
import org.ow2.asmdex.structureWriter.Field;

/**
 * Encodes and decodes an instruction built with the Dalvik format 22C.
 *
 * This Instruction can refer to a Type, or a Field. Use the constructor according to what
 * this Instruction is used for.
 *
 * @author Julien Névo
 */
public class InstructionFormat22C extends Instruction
    implements ITwoRegistersInstruction, IIndexInstruction {

    private static final int INSTRUCTION_SIZE = 4; // Instruction size in bytes.

    private int registerA;
    private int registerB;
    private int indexC;

    /**
     * Type identifying the Index, to which the Index refers.
     */
    private String type;

    /**
     * Field identifying the Index, to which the Index refers.
     */
    private Field field;

    @Override
    public int getRegisterA() {
        return registerA;
    }

    @Override
    public int getRegisterB() {
        return registerB;
    }

    @Override
    public int getIndex() {
        return indexC;
    }

    /**
     * Constructor of the Instruction, using a Reader to parse its bytecode.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode 16-bit opcode.
     */
//	public InstructionFormat22C(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//
//		// The format is B|A|op CCCC.
//		registerA = opcodeHighOrderByte & 0xf;
//		registerB = (opcodeHighOrderByte >> 4) & 0xf;
//		indexC = reader.ushort();
//	}

    /**
     * Returns the RegisterA encoded in the given 16-bit opcode.
     * @return the RegisterA.
     */
    public static int getRegisterA(int opcode) {
        return (opcode >> 8) & 0xf;
    }

    /**
     * Returns the RegisterB encoded in the given 16-bit opcode.
     * @return the RegisterB.
     */
    public static int getRegisterB(int opcode) {
        return (opcode >> 12) & 0xf;
    }

    /**
     * Returns the IndexC encoded where the Reader points. <i>It must be performed only once after
     * having read the opcode.</i>
     * @param reader reader pointing after the 16-bit opcode.
     * @return the IndexC.
     */
    public static int getIndexC(IDalvikValueReader reader) {
        return reader.ushort();
    }

    /**
     * Skips the data of this Instruction.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     */
    public static void skip(IDalvikValueReader reader) {
        reader.relativeSeek(INSTRUCTION_SIZE - 2); // The first word has already been read.
    }

    /**
     * Constructor of the Instruction, if it holds a Type.
     * @param opcode 8 or 16 bits opcode.
     * @param type Type held by this Instruction.
     * @param registerA the register this Instruction holds.
     * @param registerB the register this Instruction holds.
     */
    public InstructionFormat22C(int opcode, String type, int registerA, int registerB) {
        super(opcode);

        this.registerA = registerA;
        this.registerB = registerB;
        this.type = type;

        Instruction.test4BitsLimit(registerA);
        Instruction.test4BitsLimit(registerB);
    }

    /**
     * Constructor of the Instruction, if it holds a Field.
     * @param opcode 8 or 16 bits opcode.
     * @param field Field related the index this Instruction is holding.
     * @param registerA the register this Instruction holds.
     * @param registerB the register this Instruction holds.
     */
    public InstructionFormat22C(int opcode, Field field, int registerA, int registerB) {
        super(opcode);

        this.registerA = registerA;
        this.registerB = registerB;
        this.field = field;

        Instruction.test4BitsLimit(registerA);
        Instruction.test4BitsLimit(registerB);
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

//	@Override
//	public int[] getUsedRegisters() {
//		int[] usedRegisters;
//		// Pair registers or not ?
//		if ((opcodeByte == 0x53) || (opcodeByte == 0x5a)) {
//			usedRegisters = new int[] { registerA, registerA + 1, registerB};
//		} else {
//			usedRegisters = new int[] { registerA, registerB };
//		}
//
//		return usedRegisters;
//	}

//	@Override
//	public int getNbEncodedRegisters() {
//		return 2;
//	}

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        // The format is B|A|op CCCC.
        test8BitsLimit(registerA | registerB);
        out.putShort((((registerA & 0xf) << 8) + ((registerB & 0xf) << 12)) + opcodeByte);
        // The index may be a Type, or a Field index.
        int index;
        if ((opcodeByte == Opcodes.INSN_INSTANCE_OF) || (opcodeByte == Opcodes.INSN_NEW_ARRAY)) {
            index = constantPool.getTypeIndex(type);
        } else { // 0x52..0x5f
            index = constantPool.getFieldIndex(field);
        }

        out.putShort(index);
    }



}
