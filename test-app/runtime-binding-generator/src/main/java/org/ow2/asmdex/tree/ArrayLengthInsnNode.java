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

package org.ow2.asmdex.tree;

import java.util.Map;

import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.Opcodes;

/**
 * A node that represents an ARRAY-LENGTH instruction.
 *
 * @author Julien Névo, based on the ASM framework.
 */
public class ArrayLengthInsnNode extends AbstractInsnNode {

    /**
     * The destination register.
     */
    public int destinationRegister;

    /**
     * The register referencing the array.
     */
    public int arrayReferenceBearing;

    /**
     * Constructs a new {@link ArrayLengthInsnNode}.
     *
     * @param destinationRegister the destination register.
     * @param arrayReferenceBearing register referencing the array.
     */
    public ArrayLengthInsnNode(final int destinationRegister, final int arrayReferenceBearing) {
        super(Opcodes.INSN_ARRAY_LENGTH);
        this.destinationRegister = destinationRegister;
        this.arrayReferenceBearing = arrayReferenceBearing;
    }

    /**
     * Sets the opcode of this instruction.
     *
     * @param opcode the new instruction opcode.
     * 		  This opcode can only be ARRAY-LENGTH.
     */
    public void setOpcode(final int opcode) {
        this.opcode = opcode;
    }

    @Override
    public int getType() {
        return ARRAY_LENGTH_INSN;
    }

    @Override
    public void accept(MethodVisitor mv) {
        mv.visitArrayLengthInsn(destinationRegister, arrayReferenceBearing);
    }

    @Override
    public AbstractInsnNode clone(Map<LabelNode, LabelNode> labels) {
        return new ArrayLengthInsnNode(destinationRegister, arrayReferenceBearing);
    }



}
