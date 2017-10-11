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
 * Encodes and decodes an instruction built with the Dalvik format 23X.
 *
 * @author Julien Névo
 */
public class InstructionFormat23X extends Instruction
    implements IThreeRegistersInstruction {

    private static final int INSTRUCTION_SIZE = 4; // Instruction size in bytes.

    private int registerA;
    private int registerB;
    private int registerC;

    @Override
    public int getRegisterA() {
        return registerA;
    }

    @Override
    public int getRegisterB() {
        return registerB;
    }

    @Override
    public int getRegisterC() {
        return registerC;
    }

    /**
     * Constructor of the Instruction, using a Reader to parse its bytecode.
     * @param reader reader on the Instruction to parse, pointing after the 16-bit opcode.
     * @param opcode 16-bit opcode.
     */
//	public InstructionFormat23X(IDalvikValueReader reader, int opcode) {
//		super(opcode);
//
//		// The format is AA|op CC|BB.
//		registerA = opcodeHighOrderByte;
//		int secondWord = (int)reader.ushort();
//		registerB = secondWord & 0xff;
//		registerC = (secondWord >> 8) & 0xff;
//	}

    /**
     * Returns the RegisterA encoded in the given 16-bit opcode.
     * @return the RegisterA.
     */
    public static int getRegisterA(int opcode) {
        return (opcode >> 8) & 0xff;
    }

    /**
     * Returns the RegisterB and C encoded as an int, from where the Reader points.
     * <i>It must be performed only once after having read the opcode.</i>. To decode the returned
     * int, use getRegisterBFromEncodedRegisterBAndC and getRegisterCFromEncodedRegisterBAndC.
     * @param reader reader pointing after the 16-bit opcode.
     * @return the RegisterB and C, encoded as one single int.
     */
    public static int getEncodedRegisterBAndC(IDalvikValueReader reader) {
        return reader.ushort();
    }

    /**
     * Returns the RegisterB encoded inside the given value, that should contained both RegisterB and C
     * from the getEncodedRegisterBAndC method.
     * @param encodedRegisterBAndC the RegisterB and C encoded as an int.
     * @return the RegisterB.
     */
    public static int getRegisterBFromEncodedRegisterBAndC(int encodedRegisterBAndC) {
        return encodedRegisterBAndC & 0xff;
    }

    /**
     * Returns the RegisterC encoded inside the given value, that should contained both RegisterB and C
     * from the getEncodedRegisterBAndC method.
     * @param encodedRegisterBAndC the RegisterB and C encoded as an int.
     * @return the RegisterC.
     */
    public static int getRegisterCFromEncodedRegisterBAndC(int encodedRegisterBAndC) {
        return (encodedRegisterBAndC >> 8) & 0xff;
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
     * @param registerC the third register to store.
     */
    public InstructionFormat23X(int opcode, int registerA, int registerB, int registerC) {
        super(opcode);

        this.registerA = registerA;
        this.registerB = registerB;
        this.registerC = registerC;
    }

    @Override
    public int getSize() {
        return INSTRUCTION_SIZE;
    }


//	@Override
//	public int[] getUsedRegisters() {
//		int[] usedRegisters;
//		// Pair registers or not ?
//		switch (opcodeByte) {
//		case 0x2f:
//		case 0x30:
//		case 0x31:
//			usedRegisters = new int[] { registerA, registerB, registerB + 1, registerC, registerC + 1 };
//			break;
//
//		case 0x45:
//		case 0x4c:
//			usedRegisters = new int[] { registerA, registerA + 1, registerB, registerC, };
//			break;
//
//		case 0x9b:
//		case 0x9c:
//		case 0x9d:
//		case 0x9e:
//		case 0x9f:
//		case 0xa0:
//		case 0xa1:
//		case 0xa2:
//		case 0xa3:
//		case 0xa4:
//		case 0xa5:
//		case 0xab:
//		case 0xac:
//		case 0xad:
//		case 0xae:
//		case 0xaf:
//			usedRegisters = new int[] { registerA, registerA + 1, registerB, registerB + 1,
//					registerC, registerC + 1 };
//			break;
//		default:
//			usedRegisters = new int[] { registerA, registerB, registerC };
//		}
//
//		return usedRegisters;
//	}

//	@Override
//	public int getNbEncodedRegisters() {
//		return 3;
//	}

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        test8BitsLimit(registerA | registerB | registerC);
        out.putShort(((registerA & 0xff) << 8) + opcodeByte);
        out.putShort(((registerC & 0xff) << 8) + (registerB & 0xff));
    }

}
