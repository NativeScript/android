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
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Encodes and decodes an instruction built with the Dalvik format 32X.
 *
 * @author Julien Névo
 */
public class InstructionFormat32X extends Instruction
    implements ITwoRegistersInstruction {

    private static final int INSTRUCTION_SIZE = 6; // Instruction size in bytes.

    private int registerA;
    private int registerB;

    @Override
    public int getRegisterA() {
        return registerA;
    }

    @Override
    public int getRegisterB() {
        return registerB;
    }

    /**
     * Constructor of the Instruction, using a Reader to parse its bytecode.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode 16-bit opcode.
     */
//	public InstructionFormat32X(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//
//		// The format is 00|op AAAA BBBB.
//		registerA = (int)reader.ushort();
//		registerB = (int)reader.ushort();
//	}

    /**
     * Returns the RegisterA encoded where the Reader points. <i>It must be performed only once after
     * having read the opcode and getRegisterB must be called too.</i>
     * @param reader reader pointing after the 16-bit opcode.
     * @return the RegisterA.
     */
    public static int getRegisterA(IDalvikValueReader reader) {
        return reader.ushort();
    }

    /**
     * Returns the RegisterB encoded where the Reader points. <i>It must be performed only once after
     * having read the opcode.</i>
     * @param reader reader pointing after the 16-bit opcode.
     * @return the RegisterB.
     */
    public static int getRegisterB(IDalvikValueReader reader) {
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
     * Constructor of the Instruction by providing all the elements it's composed of.
     * @param opcode 8 or 16 bits opcode.
     * @param registerA the first register to store.
     * @param registerB the second register to store.
     */
    public InstructionFormat32X(int opcode, int registerA, int registerB) {
        super(opcode);
        this.registerA = registerA;
        this.registerB = registerB;
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

//	@Override
//	public int[] getUsedRegisters() {
//		int[] usedRegisters;
//		// Pair registers or not ?
//		if (opcodeByte == 0x06) {
//			usedRegisters = new int[] { registerA, registerA + 1, registerB, registerB + 1 };
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
        test16BitsLimit(registerA | registerB);
        out.putShort(opcodeByte);
        out.putShort(registerA & 0xffff);
        out.putShort(registerB & 0xffff);
    }

}
