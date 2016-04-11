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

package org.ow2.asmdex.lowLevelUtils;

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.instruction.DebugInstruction;
import org.ow2.asmdex.instruction.DebugInstructionStartLocal;
import org.ow2.asmdex.instruction.DebugInstructionStartLocalExtended;
import org.ow2.asmdex.instruction.Instruction;
import org.ow2.asmdex.instruction.InstructionFormat10T;
import org.ow2.asmdex.instruction.InstructionFormat10X;
import org.ow2.asmdex.instruction.InstructionFormat11N;
import org.ow2.asmdex.instruction.InstructionFormat11X;
import org.ow2.asmdex.instruction.InstructionFormat12X;
import org.ow2.asmdex.instruction.InstructionFormat20T;
import org.ow2.asmdex.instruction.InstructionFormat21C;
import org.ow2.asmdex.instruction.InstructionFormat21H;
import org.ow2.asmdex.instruction.InstructionFormat21S;
import org.ow2.asmdex.instruction.InstructionFormat21T;
import org.ow2.asmdex.instruction.InstructionFormat22B;
import org.ow2.asmdex.instruction.InstructionFormat22C;
import org.ow2.asmdex.instruction.InstructionFormat22S;
import org.ow2.asmdex.instruction.InstructionFormat22T;
import org.ow2.asmdex.instruction.InstructionFormat22X;
import org.ow2.asmdex.instruction.InstructionFormat23X;
import org.ow2.asmdex.instruction.InstructionFormat30T;
import org.ow2.asmdex.instruction.InstructionFormat31C;
import org.ow2.asmdex.instruction.InstructionFormat31I;
import org.ow2.asmdex.instruction.InstructionFormat31T;
import org.ow2.asmdex.instruction.InstructionFormat32X;
import org.ow2.asmdex.instruction.InstructionFormat35C;
import org.ow2.asmdex.instruction.InstructionFormat3RC;
import org.ow2.asmdex.instruction.InstructionFormat51L;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureWriter.Field;
import org.ow2.asmdex.structureWriter.Method;

/**
 * Utility class that encodes Instructions in the most optimized possible form.
 * 
 * Each static method builds an Instruction according to its given parameters,
 * and returns it.
 * 
 * @author Julien Névo
 */
public class InstructionEncoder
{

	private final static int MIN_VALUE_4BITS_SIGNED = -8;
	private final static int MAX_VALUE_4BITS_SIGNED = 7;
	private final static int MAX_VALUE_4BITS_UNSIGNED = 0xf;
	private final static int MAX_VALUE_8BITS_UNSIGNED = 0xff;
	private final static int MIN_VALUE_16BITS_SIGNED = -32768;
	private final static int MAX_VALUE_16BITS_SIGNED = 32767;

	/**
	 * Method invocation encoder
	 * 
	 * @param opcode
	 * @param method
	 * @param arguments
	 * @return abstract instruction
	 */
	public static Instruction encodeMethodInsn(int opcode, Method method, int[] arguments)
	{
		// The opcode could be about an invoke Instruction, or an invoke/range.
		if (opcode < Opcodes.INSN_INVOKE_VIRTUAL_RANGE)
		{
			return new InstructionFormat35C(opcode, method, arguments);
		}
		else
		{
			return new InstructionFormat3RC(opcode, method, arguments);
		}
	}

	/**
	 * Move variable encoder
	 * 
	 * @param opcode
	 * @param destinationRegister
	 * @param var
	 * @return instruction encoded
	 */
	public static Instruction encodeVarInsn(int opcode, int destinationRegister, int var)
	{
		Instruction insn = null;
		switch (opcode)
		{

		// Testing all the move instructions to know which one is the best
		// adapted according to
		// the needs (4, 8, 16 bits...).
		case 0x01: // Move.
		case 0x02:
		case 0x03:
		case 0x04: // Move Wide.
		case 0x05:
		case 0x06:
		case 0x07: // Move Object.
		case 0x08:
		case 0x09:
			insn = createBest12X22X32XInstruction(opcode, destinationRegister, var);
			break;
		case 0x12: // Const/4, Const/16, Const, Const/high 16.
		case 0x13:
		case 0x14:
		case 0x15:
		{
			if ((destinationRegister <= MAX_VALUE_4BITS_UNSIGNED) && ((var <= MAX_VALUE_4BITS_SIGNED) && (var >= MIN_VALUE_4BITS_SIGNED)))
			{ // 4 bits ?
				insn = new InstructionFormat11N(Opcodes.INSN_CONST_4, destinationRegister, var);
			}
			else if ((destinationRegister <= MAX_VALUE_8BITS_UNSIGNED) && ((var <= MAX_VALUE_16BITS_SIGNED) && (var >= MIN_VALUE_16BITS_SIGNED)))
			{ // dest = 8 bits, var = 16 bits ?
				insn = new InstructionFormat21S(Opcodes.INSN_CONST_16, destinationRegister, var);
			}
			else
			{ // dest = 8 bits, var = 32 bits, or var = 16 bits in the format
				// BBBB0000 ?
				// Is var full 16-bits, or in the format BBBB0000 ?
				insn = ((var & 0x0000ffff) == 0) ? new InstructionFormat21H(Opcodes.INSN_CONST_HIGH16, destinationRegister, var >> 16) : new InstructionFormat31I(Opcodes.INSN_CONST, destinationRegister, var);
			}
			break;
		}
		case 0x16: // Const-wide/16, Const-wide/32, Const-wide, const-wide/high
					// 16.
		case 0x17:
		{
			if ((var <= MAX_VALUE_16BITS_SIGNED) && (var >= MIN_VALUE_16BITS_SIGNED))
			{ // var = 16 bits ?
				insn = new InstructionFormat21S(Opcodes.INSN_CONST_WIDE_16, destinationRegister, var);
			}
			else
			{ // var = 32 bits.
				insn = new InstructionFormat31I(Opcodes.INSN_CONST_WIDE_32, destinationRegister, var);
			}
			break;
		}
		default:
			throw new RuntimeException("Unexpected opcode : 0x" + Integer.toHexString(opcode));
		}
		return insn;
	}

	/**
	 * Move variable encoder for long constant source
	 * 
	 * @param opcode
	 * @param destinationRegister
	 * @param var
	 * @return abstract instruction
	 */
	public static Instruction encodeVarInsn(int opcode, int destinationRegister, long var)
	{
		Instruction insn = null;
		switch (opcode)
		{
		case 0x18:
		case 0x19:
		{
			// Is var full 64-bits, or in the format BBBB000000000000 ?
			insn = ((var & 0xffffffffffffL) == 0) ? new InstructionFormat21H(Opcodes.INSN_CONST_WIDE_HIGH16, destinationRegister, (int) (var >> 48)) : new InstructionFormat51L(Opcodes.INSN_CONST_WIDE, destinationRegister, var);
			break;
		}
		default:
			throw new RuntimeException("Unexpected opcode : 0x" + Integer.toHexString(opcode));
		}

		return insn;
	}

	/**
	 * Creates the best (4, 8, 16 bits) Instruction (12X, 22X, 32X) according to
	 * the given parameters.
	 * 
	 * @param opcode
	 *            the opcode of the Instruction (1-3, 4-6, 7-9).
	 * @param destinationRegister
	 *            the destination register.
	 * @param sourceRegister
	 *            the source register.
	 * @return the most optimized Instruction.
	 */
	private static Instruction createBest12X22X32XInstruction(int opcode, int destinationRegister, int sourceRegister)
	{
		if (opcode >= Opcodes.INSN_MOVE_OBJECT)
		{
			opcode = Opcodes.INSN_MOVE_OBJECT;
		}
		else if (opcode >= Opcodes.INSN_MOVE_WIDE)
		{
			opcode = Opcodes.INSN_MOVE_WIDE;
		}
		else if (opcode >= Opcodes.INSN_MOVE)
		{
			opcode = Opcodes.INSN_MOVE;
		}

		Instruction insn;

		if ((destinationRegister <= MAX_VALUE_4BITS_UNSIGNED) && (sourceRegister <= MAX_VALUE_4BITS_UNSIGNED))
		{ // 4 bits ?
			insn = new InstructionFormat12X(opcode, destinationRegister, sourceRegister);
		}
		else if (destinationRegister <= MAX_VALUE_8BITS_UNSIGNED)
		{ // dest = 8 bits ?
			insn = new InstructionFormat22X(opcode + 1, destinationRegister, sourceRegister);
		}
		else
		{
			insn = new InstructionFormat32X(opcode + 2, destinationRegister, sourceRegister); // all
																								// is
																								// 16
																								// bits.
		}

		return insn;
	}

	/**
	 * Encode instructions without args
	 * 
	 * @param opcode
	 * @return abstract instruction
	 */
	public static Instruction encodeInsn(int opcode)
	{
		return new InstructionFormat10X(opcode);
	}

	/**
	 * Encode three arguments operations
	 * 
	 * @param opcode
	 * @param destinationRegister
	 * @param firstSourceRegister
	 * @param secondSourceRegister
	 * @param value
	 * @return abstract instruction
	 */
	public static Instruction encodeOperationInsn(int opcode, int destinationRegister, int firstSourceRegister, int secondSourceRegister, int value)
	{
		Instruction insn = null;

		// Creates an instruction according to the opcode.
		if ((opcode >= Opcodes.INSN_ADD_INT) && (opcode <= Opcodes.INSN_REM_DOUBLE))
		{
			insn = new InstructionFormat23X(opcode, destinationRegister, firstSourceRegister, secondSourceRegister);
		}
		else if ((opcode >= Opcodes.INSN_CMPL_FLOAT) && (opcode <= Opcodes.INSN_CMP_LONG))
		{
			insn = new InstructionFormat23X(opcode, destinationRegister, firstSourceRegister, secondSourceRegister);
		}
		else if ((opcode >= Opcodes.INSN_NEG_INT) && (opcode <= Opcodes.INSN_INT_TO_SHORT))
		{
			insn = new InstructionFormat12X(opcode, destinationRegister, firstSourceRegister);
		}
		else if ((opcode >= Opcodes.INSN_ADD_INT) && (opcode <= Opcodes.INSN_REM_DOUBLE))
		{
			insn = new InstructionFormat23X(opcode, destinationRegister, firstSourceRegister, secondSourceRegister);
		}
		else if ((opcode >= Opcodes.INSN_ADD_INT_2ADDR) && (opcode <= Opcodes.INSN_REM_DOUBLE_2ADDR))
		{
			insn = new InstructionFormat12X(opcode, destinationRegister, secondSourceRegister);
		}
		else if ((opcode >= Opcodes.INSN_ADD_INT_LIT16) && (opcode <= Opcodes.INSN_XOR_INT_LIT16))
		{
			insn = new InstructionFormat22S(opcode, destinationRegister, firstSourceRegister, value);
		}
		else if ((opcode >= Opcodes.INSN_ADD_INT_LIT8) && (opcode <= Opcodes.INSN_USHR_INT_LIT8))
		{
			insn = new InstructionFormat22B(opcode, destinationRegister, firstSourceRegister, value);
		}
		else
		{
			throw new RuntimeException("Unexpected opcode : 0x" + Integer.toHexString(opcode));
		}

		return insn;
	}

	/**
	 * Unary instruction
	 * 
	 * @param opcode
	 * @param operand
	 * @return abstract instruction
	 */
	public static Instruction encodeIntInsn(int opcode, int operand)
	{
		return new InstructionFormat11X(opcode, operand);
	}

	/**
	 * Encode jumps (conditional or not)
	 * 
	 * @param opcode
	 * @param label
	 * @param registerA
	 * @param registerB
	 * @param instructionOffset
	 * @return abstract instruction
	 */
	public static Instruction encodeJumpInsn(int opcode, Label label, int registerA, int registerB, int instructionOffset)
	{
		Instruction insn = null;

		// Creates an instruction according to the opcode. We also give the
		// current offset of the
		// instruction of the method, because Jump Instructions need it to
		// resolve Labels.
		if (opcode == Opcodes.INSN_GOTO)
		{
			//insn = InstructionFormat10T.obtain(opcode, label, instructionOffset);
			insn = new InstructionFormat10T(opcode, label, instructionOffset);
		}
		else if (opcode == Opcodes.INSN_GOTO_16)
		{
			insn = new InstructionFormat20T(opcode, label, instructionOffset);
		}
		else if (opcode == Opcodes.INSN_GOTO_32)
		{
			insn = new InstructionFormat30T(opcode, label, instructionOffset);
		}
		else if ((opcode >= Opcodes.INSN_IF_EQ) && (opcode <= Opcodes.INSN_IF_LE))
		{
			insn = new InstructionFormat22T(opcode, label, registerA, registerB, instructionOffset);
		}
		else if ((opcode >= Opcodes.INSN_IF_EQZ) && (opcode <= Opcodes.INSN_IF_LEZ))
		{
			insn = new InstructionFormat21T(opcode, label, registerA, instructionOffset);
		}
		else
		{
			throw new RuntimeException("Unexpected opcode : 0x" + Integer.toHexString(opcode));
		}

		return insn;
	}

	/**
	 * Fill array
	 * 
	 * @param opcode
	 * @param arrayReference
	 * @param arrayLabel
	 * @param instructionOffset
	 * @return abstract instruction
	 */
	public static Instruction encodeFillArrayDataInsn(int opcode, int arrayReference, Label arrayLabel, int instructionOffset)
	{
		return new InstructionFormat31T(opcode, arrayLabel, arrayReference, instructionOffset);
	}

	/**
	 * Instructions handling type representation
	 * 
	 * @param opcode
	 * @param destinationRegister
	 * @param referenceBearingRegister
	 * @param sizeRegister
	 * @param type
	 * @return abstract instruction
	 */
	public static Instruction encodeTypeInsn(int opcode, int destinationRegister, int referenceBearingRegister, int sizeRegister, String type)
	{
		Instruction insn = null;

		// Creates an instruction according to the opcode.
		if (opcode == Opcodes.INSN_CONST_CLASS)
		{
			insn = new InstructionFormat21C(opcode, type, destinationRegister);
		}
		else if (opcode == Opcodes.INSN_CHECK_CAST)
		{
			insn = new InstructionFormat21C(opcode, type, referenceBearingRegister);
		}
		else if (opcode == Opcodes.INSN_INSTANCE_OF)
		{
			insn = new InstructionFormat22C(opcode, type, destinationRegister, referenceBearingRegister);
		}
		else if (opcode == Opcodes.INSN_NEW_INSTANCE)
		{
			insn = new InstructionFormat21C(opcode, type, destinationRegister);
		}
		else if (opcode == Opcodes.INSN_NEW_ARRAY)
		{
			insn = new InstructionFormat22C(opcode, type, destinationRegister, sizeRegister);
		}
		else
		{
			throw new RuntimeException("Unexpected opcode : 0x" + Integer.toHexString(opcode));
		}

		return insn;
	}

	/**
	 * Multi new array
	 * 
	 * @param type
	 * @param registers
	 * @return abstract instruction
	 */
	public static Instruction encodeMultiANewArrayInsn(String type, int[] registers)
	{
		Instruction insn = null;
		int length = registers.length;
		// The instruction to create varies according to the number of
		// registers.
		// Bug316405: we have to use a range if register indexes are beyond 16.
		// In that case we assume it is a range and let
		// instruction generation catch errors.
		if (length <= 5 && length > 0 && registers[length - 1] < 16)
		{
			insn = new InstructionFormat35C(Opcodes.INSN_FILLED_NEW_ARRAY, type, registers);
		}
		else
		{
			insn = new InstructionFormat3RC(Opcodes.INSN_FILLED_NEW_ARRAY_RANGE, type, registers);
		}

		return insn;
	}

	/**
	 * Table switch
	 * 
	 * @param register
	 * @param switchTableLabel
	 * @param instructionOffset
	 * @return abstract instruction
	 */
	public static Instruction encodeTableSwitchInsn(int register, Label switchTableLabel, int instructionOffset)
	{

		return new InstructionFormat31T(Opcodes.INSN_PACKED_SWITCH_INSN, switchTableLabel, register, instructionOffset);
	}

	/**
	 * Sparse switch
	 * 
	 * @param register
	 * @param switchTableLabel
	 * @param instructionOffset
	 * @return abstract instruction
	 */
	public static Instruction encodeSparseSwitchInsn(int register, Label switchTableLabel, int instructionOffset)
	{

		return new InstructionFormat31T(Opcodes.INSN_SPARSE_SWITCH_INSN, switchTableLabel, register, instructionOffset);
	}

	/**
	 * @param destinationRegister
	 * @param arrayReferenceBearing
	 * @return abstract instruction
	 */
	public static Instruction encodeArrayLength(int destinationRegister, int arrayReferenceBearing)
	{

		return new InstructionFormat12X(Opcodes.INSN_ARRAY_LENGTH, destinationRegister, arrayReferenceBearing);
	}

	/**
	 * Operation on array
	 * 
	 * @param opcode
	 * @param valueRegister
	 * @param arrayRegister
	 * @param indexRegister
	 * @return abstract instruction
	 */
	public static Instruction encodeArrayOperation(int opcode, int valueRegister, int arrayRegister, int indexRegister)
	{

		return new InstructionFormat23X(opcode, valueRegister, arrayRegister, indexRegister);
	}

	/**
	 * Operation with strings
	 * 
	 * @param opcode
	 * @param destinationRegister
	 * @param string
	 * @return abstract instruction
	 */
	public static Instruction encodeStringOperation(int opcode, int destinationRegister, String string)
	{
		Instruction insn = null;
		if (opcode == Opcodes.INSN_CONST_STRING)
		{
			insn = new InstructionFormat21C(opcode, string, destinationRegister);
		}
		else if (opcode == Opcodes.INSN_CONST_STRING_JUMBO)
		{
			insn = new InstructionFormat31C(opcode, string, destinationRegister);
		}
		else
		{
			throw new RuntimeException("Unexpected opcode : 0x" + Integer.toHexString(opcode));
		}
		return insn;
	}

	/**
	 * Field getter or setter
	 * 
	 * @param opcode
	 * @param valueRegister
	 * @param objectRegister
	 * @param field
	 * @return abstract instruction
	 */
	public static Instruction encodeFieldInsn(int opcode, int valueRegister, int objectRegister, Field field)
	{
		Instruction insn = null;
		if ((opcode >= Opcodes.INSN_IGET) && (opcode <= Opcodes.INSN_IPUT_SHORT))
		{
			insn = new InstructionFormat22C(opcode, field, valueRegister, objectRegister);
		}
		else if ((opcode >= Opcodes.INSN_SGET) && (opcode <= Opcodes.INSN_SPUT_SHORT))
		{
			insn = new InstructionFormat21C(opcode, field, valueRegister);
		}
		return insn;
	}

	/**
	 * Pseudo debug instruction
	 * 
	 * @param name
	 * @param desc
	 * @param signature
	 * @param index
	 * @param start
	 * @return abstract instruction
	 */
	public static DebugInstruction encodeDebugStartLocal(String name, String desc, String signature, int index, Label start)
	{
		DebugInstruction insn = null;
		if (signature == null)
		{
			insn = new DebugInstructionStartLocal(index, name, desc, start);
		}
		else
		{
			insn = new DebugInstructionStartLocalExtended(index, name, desc, signature, start);
		}
		return insn;

	}
}
