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
 * Represents an instruction of the Dalvik bytecode.
 * 
 * An instruction also has a Line Number, superior to 0 if set.
 * 
 * @author Julien Névo
 */
public abstract class Instruction {
	
	/**
	 * Represents the data of the opcode (8 bits).
	 */
	protected int opcodeHighOrderByte;
	
	/**
	 * Represents the opcode itself (8 bits).
	 */
	protected int opcodeByte;
	
	/**
	 * Line Number of this Instruction. It must be superior to 0, unless the Line Number has not been
	 * set.
	 */
	protected int lineNumber;
	
	/**
	 * Builds an instruction from its 16-bit opcode.
	 * @param opcode the 16-bit opcode of the instruction.
	 */
	public Instruction(int opcode) {
		opcodeHighOrderByte = (opcode >> 8) & 0xff;
		opcodeByte = opcode & 0xff;
	}
	
	public void recycle()
	{
		
	}
	
	// -----------------------------------------------------
	// Public Methods.
	// -----------------------------------------------------
	
	/**
	 * Encodes the instruction, in the Dalvik format, to an output buffer.
	 * @param out output buffer.
	 * @param constantPool the constantPool.
	 */
	public abstract void write(ByteVector out, ConstantPool constantPool);
	

	// -----------------------------------------------------
	// Getters and Setters.
	// -----------------------------------------------------
	
	/**
	 * Returns the size of the instruction in bytes.
	 * @return the size of the instruction in bytes.
	 */
	public abstract int getSize();
	
	/**
	 * Returns the high order byte of the 16-bit opcode. 
	 * @return the high order byte of the 16-bit opcode.
	 */
	public int getOpcodeHighOrderByte() {
		return opcodeHighOrderByte;
	}
	
	/**
	 * Returns the low order byte of the 16-bit opcode (i.e. the opcode itself).
	 * @return the low order byte of the 16-bit opcode (i.e. the opcode itself).
	 */
	public int getOpcodeByte() {
		return opcodeByte;
	}
	
	/**
	 * Returns a list of the registers used by this instruction. May be null. According to the opcode,
	 * the registers may be given as pairs.
	 * @return the list of the registers used by this instruction. May be null.
	 */
	//public abstract int[] getUsedRegisters();
	
	/**
	 * Returns the number of registers encoded by this Instruction. Pair registers do NOT count as two.
	 * @return the number of registers encoded by this Instruction.
	 */
	//public abstract int getNbEncodedRegisters();
	
	/**
	 * Returns the line number of the Instruction. A line number equals to 0 means that no line number has
	 * been set.
	 * @return the line number of the Instruction.
	 */
	public int getLineNumber() {
		return lineNumber;
	}
	
	/**
	 * Sets the line number of the Instruction. It must be superior to 0 in order to be valid.
	 * @param lineNumber the line number.
	 */
	public void setLineNumber(int lineNumber) {
		this.lineNumber = lineNumber;
	}
	
	
	// -----------------------------------------------------
	// Static methods and values.
	// -----------------------------------------------------

	/**
	 * Tests if the given number can be held in 4 bits, throw an IllegalArgumentException otherwise.
	 */
	public static void test4BitsLimit(int number) {
		if (number > 0xf) {
			throw new IllegalArgumentException("The number " + number +" can't be held in 4 bits !");
		}
	}
	
    /**
     * Tests if the given number can be held in 8 bits, throw an IllegalArgumentException otherwise.
     */
    public static void test8BitsLimit(int number) {
        if ((number & 0xffffff00) != 0) {
            throw new IllegalArgumentException("The number " + number +" can't be held in 8 bits !");
        }
    }

    /**
     * Tests if the given number can be held in 16 bits, throw an IllegalArgumentException otherwise.
     */
    public static void test16BitsLimit(int number) {
        if ((number & 0xffff0000) != 0) {
            throw new IllegalArgumentException("The number " + number +" can't be held in 16 bits !");
        }
    }
    /**
	 * Tests if the given numbers can be held in 4 bits, throw an IllegalArgumentException otherwise.
	 */
	public static void test4BitsLimit(int[] numbers) {
		for (int number : numbers) {
			test4BitsLimit(number);
		}
	}
	
    /**
     * Tests if the given number can be held in 4 bits, throw an IllegalArgumentException otherwise.
     */
    public static void testRange(int[] numbers) {
        int length = numbers.length;
        if (length == 0) throw new IllegalArgumentException("Too short for a range");
        int expected = numbers[0];
        for(int i=1; i<length; i++) {
            if (numbers[i] != ++expected)
                throw new IllegalArgumentException("The register at position " + i +" is not consecutive.");
        }
    }
    /**
	 * Size in bytes of every instructions. Includes 256 opcodes.
	 */
	final private static byte[] instructionSizeInBytes = new byte[] {
		2,2,4,6,2,4,6,2,4,6,2,2,2,2,2,2,		// 0x00
		2,2,2,4,6,4,4,6,10,4,4,6,4,2,2,4,		// 0x10
		4,2,4,4,6,6,6,2,2,4,6,6,6,4,4,4,		// 0x20
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,2,2,		// 0x30
		2,2,2,2,4,4,4,4,4,4,4,4,4,4,4,4,		// 0x40
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,		// 0x50
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,6,6,		// 0x60
		6,6,6,2,6,6,6,6,6,2,2,2,2,2,2,2,		// 0x70
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,		// 0x80
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,		// 0x90
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,		// 0xa0
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,		// 0xb0
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,		// 0xc0
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,		// 0xd0
		4,4,4,2,2,2,2,2,2,2,2,2,2,2,2,2,		// 0xe0
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2			// 0xf0
		};
	
	/**
	 * Returns the size in bytes of the given opcode.
	 * @param opcode from 0 to 255.
	 * @return the size in bytes of the given opcode.
	 */
	public static byte getInstructionSizeInByte(int opcode) {
		return instructionSizeInBytes[opcode];
	}
	
}
