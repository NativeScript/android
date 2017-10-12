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

/**
 * A node that represents a type instruction. A type instruction is an
 * instruction that takes a type descriptor as parameter.
 *
 * @author Eric Bruneton
 * @author Julien Névo (adaptation to AsmDex).
 */
public class TypeInsnNode extends AbstractInsnNode {

    /**
     * The operand of this instruction.
     */
    public String desc;

    /**
     * The destination register. Used only by CONST-CLASS, INSTANCE-OF, NEW-INSTANCE, NEW-ARRAY.
     */
    public int destinationRegister;

    /**
     * The reference bearing register. Only used for CHECK-CAST, INSTANCE-OF instructions.
     */
    public int referenceBearingRegister;

    /**
     * The size register. Used only for NEW-ARRAY.
     */
    public int sizeRegister;

    /**
     * Constructs a new {@link TypeInsnNode}.
     *
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is either CONST-CLASS, NEW-ARRAY, NEW-INSTANCE, CHECK-CAST
     *        or INSTANCE-OF.
     * @param destinationRegister the destination register. Used only by CONST-CLASS,
     * 		  INSTANCE-OF, NEW-INSTANCE, NEW-ARRAY.
     * @param referenceBearingRegister reference bearing register. Only used for
     * 		  CHECK-CAST, INSTANCE-OF instructions.
     * @param sizeRegister size register. Used only for NEW-ARRAY.
     * @param type the operand of the instruction to be constructed.
     */
    public TypeInsnNode(final int opcode, final int destinationRegister,
                        final int referenceBearingRegister, final int sizeRegister, final String type) {
        super(opcode);
        this.destinationRegister = destinationRegister;
        this.referenceBearingRegister = referenceBearingRegister;
        this.sizeRegister = sizeRegister;
        desc = type;
    }

    /**
     * Sets the opcode of this instruction.
     *
     * @param opcode the new instruction opcode. This opcode is either CONST-CLASS, NEW-ARRAY,
     * 		  NEW-INSTANCE, CHECK-CAST or INSTANCE-OF.
     */
    public void setOpcode(final int opcode) {
        this.opcode = opcode;
    }


    @Override
    public int getType() {
        return TYPE_INSN;
    }


    @Override
    public void accept(final MethodVisitor mv) {
        mv.visitTypeInsn(opcode, destinationRegister, referenceBearingRegister, sizeRegister, desc);
    }


    @Override
    public AbstractInsnNode clone(final Map<LabelNode, LabelNode> labels) {
        return new TypeInsnNode(opcode, destinationRegister, referenceBearingRegister, sizeRegister, desc);
    }
}