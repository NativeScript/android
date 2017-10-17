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
 * A node that represents an instruction that performs an unary or binary operation on one or two
 * sources or one array, possibly a literal value, to a destination register.
 *
 * @author Julien Névo.
 */
public class OperationInsnNode extends AbstractInsnNode {

    /**
     * The destination register or pair. For BINOP/2ADDR, it should be the same as firstSourceRegister.
     */
    public int destinationRegister;

    /**
     * The first source register or pair. For BINOP/2ADDR, it should be the same as destinationRegister.
     */
    public int firstSourceRegister;

    /**
     * secondSourceRegister the second source register or pair. Ignored for UNOP, BINOP/LIT16, BINOP/LIT8.
     */
    public int secondSourceRegister;

    /**
     * value literal value. Only useful for BINOP/LIT16, BINOP/LIT8.
     */
    public int value;

    /**
     * Constructs a new {@link OperationInsnNode}.
     * @param opcode the opcode of the type instruction to be visited. This
     *        opcode is in either of these categories : UNOP, BINOP, BINOP/2ADDR,
     *        BINOP/LIT16, BINOP/LIT8.
     * @param destinationRegister the destination register or pair. For BINOP/2ADDR,
     * 		  it should be the same as firstSourceRegister.
     * @param firstSourceRegister the first source register or pair. For BINOP/2ADDR,
     * 		  it should be the same as destinationRegister.
     * @param secondSourceRegister the second source register or pair. Ignored for
     * 		  UNOP, BINOP/LIT16, BINOP/LIT8.
     * @param value literal value. Only useful for BINOP/LIT16, BINOP/LIT8.
     */
    public OperationInsnNode(int opcode, int destinationRegister,
                             int firstSourceRegister, int secondSourceRegister, int value) {
        super(opcode);
        this.destinationRegister = destinationRegister;
        this.firstSourceRegister = firstSourceRegister;
        this.secondSourceRegister = secondSourceRegister;
        this.value = value;
    }

    /**
     * Sets the opcode of this instruction.
     *
     * @param opcode the new instruction opcode.
     * 		  This opcode is either CONST-STRING or CONST-STRING/JUMBO.
     */
    public void setOpcode(final int opcode) {
        this.opcode = opcode;
    }

    @Override
    public int getType() {
        return OPERATION_INSN;
    }

    @Override
    public void accept(final MethodVisitor mv) {
        mv.visitOperationInsn(opcode, destinationRegister, firstSourceRegister, secondSourceRegister, value);
    }

    @Override
    public AbstractInsnNode clone(final Map<LabelNode, LabelNode> labels) {
        return new OperationInsnNode(opcode, destinationRegister, firstSourceRegister, secondSourceRegister, value);
    }

}
