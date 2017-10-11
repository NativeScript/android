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
 * Encodes and decodes an instruction built with the Dalvik format 31C.
 *
 * @author Julien Névo
 */
public class InstructionFormat31C extends Instruction
    implements IOneRegisterInstruction, IIndexInstruction {

    private static final int INSTRUCTION_SIZE = 6; // Instruction size in bytes.

    private int registerA;
    private int indexB;

    /**
     * String held by this Instruction, to which the Index refers.
     */
    private String string;

    @Override
    public int getRegisterA() {
        return registerA;
    }

    @Override
    public int getIndex() {
        return indexB;
    }

    /**
     * Constructor of the Instruction, using a Reader to parse its bytecode.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode 16-bit opcode.
     */
//	public InstructionFormat31C(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//
//		// The format is AA|op BBBBlo BBBBhi.
//		registerA = opcodeHighOrderByte;
//		indexB = (int)reader.ushort() + ((int)reader.ushort() << 16);
//	}

    /**
     * Returns the RegisterA encoded in the given 16-bit opcode.
     * @return the RegisterA .
     */
    public static int getRegisterA(int opcode) {
        return (opcode >> 8) & 0xff;
    }

    /**
     * Returns the IndexB encoded where the Reader points. <i>It must be performed only once after
     * having read the opcode.</i>
     * @param reader reader pointing after the 16-bit opcode.
     * @return the IndexB.
     */
    public static int getIndexB(IDalvikValueReader reader) {
        return reader.ushort() + (reader.ushort() << 16);
    }

    /**
     * Skips the data of this Instruction.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     */
    public static void skip(IDalvikValueReader reader) {
        reader.relativeSeek(INSTRUCTION_SIZE - 2); // The first word has already been read.
    }

    /**
     * Constructor of the Instruction.
     * @param opcode 8 or 16 bits opcode.
     * @param string string related the index this Instruction is holding.
     * @param registerA the register this Instruction holds.
     */
    public InstructionFormat31C(int opcode, String string, int registerA) {
        super(opcode);

        this.registerA = registerA;
        this.string = string;
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
    public void write(ByteVector out, ConstantPool constantPool) {
        test8BitsLimit(registerA);
        // The format is AA|op BBBBlo BBBBhi.
        out.putShort(((registerA & 0xff) << 8) + opcodeByte);
        int index = constantPool.getStringIndex(string);
        out.putShort(index & 0xffff);
        out.putShort((index >> 16) & 0xffff);
    }



}
