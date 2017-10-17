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
 * Encodes and decodes and encodes an instruction built with the Dalvik format 35C.
 *
 * This Instruction can refer to a Type, or a Method. Use the constructor according to what
 * this Instruction is used for.
 *
 * @author Julien Névo
 */
public class InstructionFormat35C extends Instruction
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
        // The format is B|A|op CCCC G|F|E|D.
        int nbRegisters = (opcode >> 12) & 0xf;
        int[] registers = new int[nbRegisters];
        int thirdShort = reader.ushort();
        if (nbRegisters > 0) {
            registers[0] = thirdShort & 0xf;
            if (nbRegisters > 1) {
                registers[1] = (thirdShort >> 4) & 0xf;
            }
            if (nbRegisters > 2) {
                registers[2] = (thirdShort >> 8) & 0xf;
            }
            if (nbRegisters > 3) {
                registers[3] = (thirdShort >> 12) & 0xf;
            }
            if (nbRegisters > 4) {
                registers[4] = (opcode >> 8) & 0xf;
            }
            if (nbRegisters > 5) {
                try {
                    throw new Exception("Abnormal arguments number : " + nbRegisters);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
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
     * @param method the Method this Instruction is holding.
     * @param registers the registers this Instruction holds.
     */
    public InstructionFormat35C(int opcode, Method method, int[] registers) {
        super(opcode);
        this.method = method;
        this.registers = registers;
        Instruction.test4BitsLimit(registers);
    }

    /**
     * Constructor of the Instruction, if it holds a Type.
     * @param opcode 8 or 16 bits opcode.
     * @param type the Type this Instruction is holding.
     * @param registers the registers this Instruction holds.
     */
    public InstructionFormat35C(int opcode, String type, int[] registers) {
        super(opcode);
        this.type = type;
        this.registers = registers;
        Instruction.test4BitsLimit(registers);
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        int nbRegisters = registers.length;

        int firstShort = (nbRegisters << 12) + opcodeByte +
                         (nbRegisters > 4 ? ((registers[4] & 0xf) << 8) : 0);
        int thirdShort = 0;
        if (nbRegisters > 0) {
            thirdShort = registers[0] & 0xf;
        }
        if (nbRegisters > 1) {
            thirdShort += ((registers[1] & 0xf) << 4);
        }
        if (nbRegisters > 2) {
            thirdShort += ((registers[2] & 0xf) << 8);
        }
        if (nbRegisters > 3) {
            thirdShort += ((registers[3] & 0xf) << 12);
        }
        if (nbRegisters > 5) {
            try {
                throw new Exception("Abnormal arguments number : " + nbRegisters);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        int mask=0;
        for (int i=0; i < nbRegisters; i++) {
            mask |= registers[i];
        }
        test4BitsLimit(mask);
        out.putShort(firstShort);
        if (opcodeByte == 0x24) {
            out.putShort(constantPool.getTypeIndex(type));
        } else {
            out.putShort(constantPool.getMethodIndex(method));
        }
        out.putShort(thirdShort);
    }
}
