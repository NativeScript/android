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
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Encodes and decodes an instruction built with the Dalvik format 11X.
 *
 * @author Julien Névo
 */
public class InstructionFormat11X extends Instruction implements IOneRegisterInstruction {

    private static final int INSTRUCTION_SIZE = 2; // Instruction size in bytes.

    private int registerA;

    @Override
    public int getRegisterA() {
        return registerA;
    }

    /**
     * Constructor of the Instruction, using a Reader to parse its bytecode.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode 16-bit opcode.
     */
//	public InstructionFormat11X(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//		// The format is AA|op.
//		registerA = opcodeHighOrderByte & 0xff;
//	}

    /**
     * Returns the RegisterA encoded in the given 16-bit opcode.
     * @return the RegisterA.
     */
    public static int getRegisterA(int opcode) {
        return (opcode >> 8) & 0xff;
    }

    /**
     * Constructor of the Instruction by providing all the elements it's composed of.
     * @param opcode 8 or 16 bits opcode.
     * @param register the register.
     */
    public InstructionFormat11X(int opcode, int register) {
        super(opcode);
        registerA = register;
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

//	@Override
//	public int[] getUsedRegisters() {
//		int[] usedRegisters;
//
//		// Pair registers or not ?
//		if ((opcodeByte == 0xb) || (opcodeByte == 0x10)) {
//			usedRegisters = new int[] { registerA, registerA + 1};
//		} else {
//			usedRegisters = new int[] { registerA };
//		}
//
//		return usedRegisters;
//	}

//	@Override
//	public int getNbEncodedRegisters() {
//		return 1;
//	}

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        test8BitsLimit(registerA);
        out.putShort(((registerA & 0xff) << 8) + opcodeByte);
    }

}
