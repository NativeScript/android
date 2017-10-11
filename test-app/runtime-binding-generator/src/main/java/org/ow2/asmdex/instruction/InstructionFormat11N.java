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
 * Encodes and decodes an instruction built with the Dalvik format 11N.
 *
 * @author Julien Névo
 */
public class InstructionFormat11N extends Instruction
    implements IOneRegisterInstruction, ILiteralInstruction {

    /**
     * Instruction size in bytes.
     */
    private static final int INSTRUCTION_SIZE = 2;

    private int registerA;
    private int literalB;

    @Override
    public int getRegisterA() {
        return registerA;
    }

    @Override
    public int getLiteral() {
        return literalB;
    }

    /**
     * Returns the RegisterA encoded in the given 16-bit opcode.
     * @return the RegisterA .
     */
    public static int getRegisterA(int opcode) {
        return (opcode >> 8) & 0xf;
    }

    /**
     * Returns the LiteralB encoded in the given 16-bit opcode.
     * @return the LiteralB.
     */
    public static int getLiteralB(int opcode) {
        return ((byte)(opcode >> 8)) >> 4; // Needs to keep the sign.
    }


    /**
     * Constructor of the Instruction by providing all the elements it's composed of.
     * @param opcode 8 or 16 bits opcode.
     * @param destinationRegister the destination register.
     * @param var the literal to store.
     */
    public InstructionFormat11N(int opcode, int destinationRegister, int var) {
        super(opcode);
        registerA = destinationRegister;
        literalB = var;
        Instruction.test4BitsLimit(registerA);
        Instruction.test4BitsLimit(literalB);
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        test4BitsLimit(registerA);
        int firstShort = ((literalB & 0xf) << 12) + ((registerA & 0xf) << 8) + opcodeByte;
        out.putShort(firstShort);
    }


}
