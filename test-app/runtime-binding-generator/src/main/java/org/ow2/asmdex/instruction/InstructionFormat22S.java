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
 * Encodes and decodes an instruction built with the Dalvik format 22S.
 *
 * @author Julien Névo
 */
public class InstructionFormat22S extends Instruction
    implements ITwoRegistersInstruction, ILiteralInstruction {

    private static final int INSTRUCTION_SIZE = 4; // Instruction size in bytes.

    private int registerA;
    private int registerB;
    private int literalC;

    @Override
    public int getRegisterA() {
        return registerA;
    }

    @Override
    public int getRegisterB() {
        return registerB;
    }

    @Override
    public int getLiteral() {
        return literalC;
    }

    /**
     * Constructor of the Instruction, using a Reader to parse its bytecode.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode 16-bit opcode.
     */
//	public InstructionFormat22S(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//
//		// The format is B|A|op CCCC.
//		registerA = (opcodeHighOrderByte & 0xf);
//		registerB = (opcodeHighOrderByte >> 4) & 0xf;
//		literalC = (int)reader.sshort();
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
     * Returns the LiteralC encoded where the Reader points. <i>It must be performed only once after
     * having read the opcode.</i>
     * @param reader reader pointing after the 16-bit opcode.
     * @return the LiteralC.
     */
    public static int getLiteralC(IDalvikValueReader reader) {
        return reader.sshort();
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
     * @param value the literal to store.
     */
    public InstructionFormat22S(int opcode, int registerA, int registerB, int value) {
        super(opcode);

        this.registerA = registerA;
        this.registerB = registerB;
        this.literalC = value;

        Instruction.test4BitsLimit(registerA);
        Instruction.test4BitsLimit(registerB);
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

//	@Override
//	public int[] getUsedRegisters() {
//		return new int[] { registerA, registerB };
//	}

//	@Override
//	public int getNbEncodedRegisters() {
//		return 2;
//	}

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        test4BitsLimit(registerA | registerB);
        out.putShort((((registerA & 0xf) << 8) + ((registerB & 0xf) << 12)) + opcodeByte);
        out.putShort(literalC & 0xffff);
    }


}
