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
 * A node that represents a method instruction. A method instruction is an
 * instruction that invokes a method.
 *
 * @author Eric Bruneton
 * @author Julien Névo (adaptation to AsmDex).
 */
public class MethodInsnNode extends AbstractInsnNode {

    /**
     * The internal name of the method's owner class.
     */
    public String owner;

    /**
     * The method's name.
     */
    public String name;

    /**
     * The method's descriptor.
     */
    public String desc;

    /**
     * Arguments to give to the method.
     */
    public int[] arguments;

    /**
     * Constructs a new {@link MethodInsnNode}.
     *
     * @param opcode the opcode of the type instruction to be constructed. This
     *        opcode is either INVOKE-VIRTUAL, INVOKE-SUPER, INVOKE-DIRECT,
     *        INVOKE-STATIC or INVOKE-INTERFACE.
     * @param owner the internal name of the method's owner class.
     * @param name the method's name.
     * @param desc the method's descriptor.
     * @param arguments the arguments to give to the method.
     */
    public MethodInsnNode(final int opcode, final String owner, final String name, final String desc,
                          final int[] arguments) {
        super(opcode);
        this.owner = owner;
        this.name = name;
        this.desc = desc;
        this.arguments = arguments;
    }

    /**
     * Sets the opcode of this instruction.
     *
     * @param opcode the new instruction opcode. This opcode must be
     *        INVOKE-VIRTUAL, INVOKE-SUPER, INVOKE-DIRECT, INVOKE-STATIC or INVOKE-INTERFACE.
     */
    public void setOpcode(final int opcode) {
        this.opcode = opcode;
    }

    @Override
    public int getType() {
        return METHOD_INSN;
    }

    @Override
    public void accept(final MethodVisitor mv) {
        mv.visitMethodInsn(opcode, owner, name, desc, arguments);
    }

    @Override
    public AbstractInsnNode clone(final Map<LabelNode, LabelNode> labels) {
        return new MethodInsnNode(opcode, owner, name, desc, arguments);
    }
}