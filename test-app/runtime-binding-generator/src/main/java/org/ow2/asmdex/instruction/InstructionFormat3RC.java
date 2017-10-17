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
import org.ow2.asmdex.structureWriter.Method;

/**
 * Encodes and decodes an instruction built with the Dalvik format 3RC.
 *
 * This Instruction can refer to a Type, or a Method. Use the constructor according to what
 * this Instruction is used for.
 *
 * Note that the writing of this Instruction doesn't check the validity of the Registers.
 * They have to be consecutive.
 *
 * @author Julien Névo
 */
public class InstructionFormat3RC extends Instruction
    implements IRegisterArrayInstruction, IIndexInstruction {

    private static final int INSTRUCTION_SIZE = 6; // Instruction size in bytes.

    private int[] registers;
    private int index;

    /**
     * Type held by this Instruction.
     */
    private String type;

    /**
     * Method held by this Instruction.
     */
    private Method method;

    @Override
    public int[] getRegisters() {
        return registers;
    }

    @Override
    public int getIndex() {
        return index;
    }


    /**
     * Returns the Index encoded where the Reader points. <i>It must be performed only once after
     * having read the opcode and getRegisters must be called too.</i>
     * @param reader reader pointing after the 16-bit opcode.
     * @return the Index.
     */
    public static int getIndex(IDalvikValueReader reader) {
        return reader.ushort();
    }

    /**
     * Returns the Registers encoded where the Reader points. <i>It must be performed only once after
     * having read the opcode and called getIndex.</i>
     * @param reader reader pointing after the 16-bit opcode.
     * @param opcode the 16-bit opcode.
     * @return the Registers.
     */
    public static int[] getRegisters(IDalvikValueReader reader, int opcode) {
        // The format is AA|op BBBB CCCC.
        int opcodeHighOrderByte = (opcode >> 8) & 0xff;
        int nbRegisters = opcodeHighOrderByte;
        int[] registers = new int[nbRegisters];
        int currentRegister = reader.ushort();
        for (int registerIndex = 0; registerIndex < nbRegisters; registerIndex++) {
            registers[registerIndex] = currentRegister;
            currentRegister++;
        }
        return registers;
    }

    /**
     * Skips the data of this Instruction.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     */
    public static void skip(IDalvikValueReader reader) {
        reader.relativeSeek(INSTRUCTION_SIZE - 2); // The first word has already been read.
    }

    /**
     * Constructor of the Instruction, if it holds a Method.
     * @param opcode 8 or 16 bits opcode.
     * @param method Method this Instruction is holding.
     * @param registers the registers. <i>They must be consecutive</i>.
     */
    public InstructionFormat3RC(int opcode, Method method, int[] registers) {
        super(opcode);
        this.method = method;
        this.registers = registers;
    }

    /**
     * Constructor of the Instruction, if it holds a Type.
     * @param opcode 8 or 16 bits opcode.
     * @param type the Type this Instruction is holding.
     * @param registers the registers. <i>They must be consecutive</i>.
     */
    public InstructionFormat3RC(int opcode, String type, int[] registers) {
        super(opcode);
        this.type = type;
        this.registers = registers;
        Instruction.testRange(registers);
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        // The format is AA|op BBBB CCCC.
        out.putShort(((registers.length & 0xff) << 8) + opcodeByte);
        if (opcodeByte == 0x25) {
            out.putShort(constantPool.getTypeIndex(type));
        } else {
            out.putShort(constantPool.getMethodIndex(method));
        }
        // The registers should be consecutive, but this is not tested here.
        // We consider it is true and checked before.
        int regArg = (registers.length > 0) ? registers[0] : 0;
        out.putShort(regArg);
    }

}
