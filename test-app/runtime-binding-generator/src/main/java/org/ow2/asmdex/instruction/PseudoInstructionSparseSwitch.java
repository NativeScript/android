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
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * A Sparse-Switch Pseudo-Instruction. It should use a 4-byte alignment but it is NOT
 * managed here.
 *
 * @author Julien Névo
 */
public class PseudoInstructionSparseSwitch extends Instruction implements IPseudoInstruction {

    /**
     * The opcode of this Pseudo Instruction.
     */
    private final static int SPARSE_SWITCH_OPCODE = 0x200;

    /**
     * The keys for each Case.
     */
    private int[] keys;

    /**
     * The targets for each Case.
     */
    private Label[] targets;

    /**
     * Reference to the Sparse Switch Instruction.
     */
    private IOffsetInstruction instructionSparseSwitch;

    @Override
    public IOffsetInstruction getSourceInstruction() {
        return instructionSparseSwitch;
    }

    /**
     * Constructor of the Sparse Switch Pseudo Instruction.
     * @param keys the keys for each Case.
     * @param targets the targets for each Case.
     * @param instructionSparseSwitch reference to the Sparse Switch Instruction.
     */
    public PseudoInstructionSparseSwitch(int[] keys, Label[] targets, IOffsetInstruction instructionSparseSwitch) {
        super(SPARSE_SWITCH_OPCODE);
        this.keys = keys;
        this.targets = targets;
        this.instructionSparseSwitch = instructionSparseSwitch;
    }

    @Override
    public int getSize() {
        return targets.length * 8 + 2 + 2; // Additions for the Opcode (2) and size (2) .
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
        out.putShort(SPARSE_SWITCH_OPCODE);
        int size = targets.length;
        out.putShort(size);
        for (int i = 0; i < size; i++) {
            out.putInt(keys[i]);
        }
        for (int i = 0; i < size; i++) {
            out.putInt((targets[i].getOffset() - instructionSparseSwitch.getInstructionOffset()) / 2);
        }
    }
}
