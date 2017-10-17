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

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * A Fill-Array-Data Pseudo-Instruction. It should use a 4-byte alignment but it is NOT
 * managed here.
 *
 * @author Julien Névo
 */
public class PseudoInstructionFillArrayData extends Instruction implements IPseudoInstruction {

    /**
     * The opcode of this Pseudo Instruction.
     */
    private final static int FILL_ARRAY_DATA_OPCODE = 0x300;

    /**
     * The number of bytes each elements of the array is composed of.
     */
    private int elementWidth;

    /**
     * The data array.
     */
    private Object[] dataArray;

    /**
     * Type of the element. Found according to the first element of the array. The type is
     * for example Opcodes.VALUE_INT.
     */
    private int type = -1;

    /**
     * Reference to the Fill Array Data Instruction.
     */
    private IOffsetInstruction instructionFillArrayData;

    @Override
    public IOffsetInstruction getSourceInstruction() {
        return instructionFillArrayData;
    }


    /**
     * Constructor of this Instruction.
     * @param dataArray The data array. It must at least have one element, else there's no point in
     * 		  building an array.
     * @param instructionFillArrayData reference to the Fill Array Data Instruction.
     */
    public PseudoInstructionFillArrayData(Object[] dataArray, IOffsetInstruction instructionFillArrayData) {
        super(FILL_ARRAY_DATA_OPCODE);
        this.dataArray = dataArray;
        this.instructionFillArrayData = instructionFillArrayData;
        setElementWidthAndType();
    }

    @Override
    public int getSize() {
        return ((dataArray.length * elementWidth + 1) / 2 + 4) * 2; // Additions for the Opcode (2), elementWidth (2) and size (4).
    }

    //	@Override
    //	public int[] getUsedRegisters() {
    //		return null;
    //	}

    //	@Override
    //	public int getNbEncodedRegisters() {
    //		return 0;
    //	}

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        out.putShort(FILL_ARRAY_DATA_OPCODE);

        out.putShort(elementWidth);
        int size = dataArray.length;
        out.putInt(size);

        switch (type) {
        case Opcodes.VALUE_BYTE: {
            for (int i = 0; i < size; i++) {
                out.putByte((Byte)dataArray[i]);
            }
            if (size % 2 == 1) {
                out.putByte(0);
            }
            break;
        }
        case Opcodes.VALUE_SHORT: {
            for (int i = 0; i < size; i++) {
                out.putShort((Short)dataArray[i]);
            }
            break;
        }
        case Opcodes.VALUE_CHAR: {
            for (int i = 0; i < size; i++) {
                out.putShort((Character)dataArray[i]);
            }
            break;
        }
        case Opcodes.VALUE_INT: {
            for (int i = 0; i < size; i++) {
                out.putInt((Integer)dataArray[i]);
            }
            break;
        }
        case Opcodes.VALUE_FLOAT: {
            for (int i = 0; i < size; i++) {
                out.putInt(Float.floatToRawIntBits((Float)dataArray[i]));
            }
            break;
        }
        case Opcodes.VALUE_DOUBLE: {
            for (int i = 0; i < size; i++) {
                out.putLong(Double.doubleToRawLongBits((Double)dataArray[i]));
            }
            break;
        }
        case Opcodes.VALUE_LONG: {
            for (int i = 0; i < size; i++) {
                out.putLong((Long)dataArray[i]);
            }
            break;
        }
        case Opcodes.VALUE_BOOLEAN: {
            for (int i = 0; i < size; i++) {
                out.putByte(((Boolean)dataArray[i]) ? -1 : 0);
            }
            if (size % 2 == 1) {
                out.putByte(0);
            }
            break;
        }
        }
    }


    /**
     * Finds the width (in bytes) and type of the data array, found according to the first element
     * of the array. The type is for example Opcodes.VALUE_INT.
     */
    private void setElementWidthAndType() {
        Object value = dataArray[0];
        if (value instanceof Integer) {
            elementWidth = 4;
            type = Opcodes.VALUE_INT;
        } else if (value instanceof Boolean) {
            elementWidth = 1;
            type = Opcodes.VALUE_BOOLEAN;
        } else if (value instanceof Byte) {
            elementWidth = 1;
            type = Opcodes.VALUE_BYTE;
        } else if (value instanceof Character) {
            elementWidth = 2;
            type = Opcodes.VALUE_CHAR;
        } else if (value instanceof Double) {
            elementWidth = 8;
            type = Opcodes.VALUE_DOUBLE;
        } else if (value instanceof Float) {
            elementWidth = 4;
            type = Opcodes.VALUE_FLOAT;
        } else if (value instanceof Long) {
            elementWidth = 8;
            type = Opcodes.VALUE_LONG;
        } else if (value instanceof Short) {
            elementWidth = 2;
            type = Opcodes.VALUE_SHORT;
        } else {
            throw new IllegalArgumentException("Array of this type can't be encoded.");
        }
    }
}
