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
 * A node that represents a jump instruction. A jump instruction is an
 * instruction that may jump to another instruction.
 *
 * @author Eric Bruneton
 * @author Julien Névo (adaptation to AsmDex).
 */
public class JumpInsnNode extends AbstractInsnNode {

    /**
     * The operand of this instruction. This operand is a label that designates
     * the instruction to which this instruction may jump.
     */
    public LabelNode label;

    /**
     * Register A. Ignored for unconditional jump.
     */
    public int registerA;

    /**
     * Register B. Ignored for unconditional jump or zero tests (IF-EQZ, IF-NEZ...).
     */
    public int registerB;

    /**
     * Constructs a new {@link JumpInsnNode}.
     *
     * @param opcode the opcode of the type instruction to be constructed. This
     *        opcode is either GOTO, IF-EQ, IF-NE, IF-LT, IF-GE, IF-GT, IF-LE,
     *        or IF-EQZ, IF-NEZ, IF-LTZ, IF-GEZ, IF-GTZ, IF-LEZ.
     * @param label the operand of the instruction to be constructed. This
     *        operand is a label that designates the instruction to which the
     *        jump instruction may jump.
     * @param registerA the register A. Ignored for unconditional jump.
     * @param registerB the register B. Ignored for unconditional jump or zero
     * 		  tests (IF-EQZ, IF-NEZ...).
     */
    public JumpInsnNode(final int opcode, final LabelNode label, final int registerA, final int registerB) {
        super(opcode);
        this.label = label;
        this.registerA = registerA;
        this.registerB = registerB;
    }

    /**
     * Sets the opcode of this instruction.
     *
     * @param opcode the new instruction opcode. This opcode is either GOTO,
     * 		  IF-EQ, IF-NE, IF-LT, IF-GE, IF-GT, IF-LE, IF-EQZ, IF-NEZ,
     * 		  IF-LTZ, IF-GEZ, IF-GTZ, IF-LEZ.
     */
    public void setOpcode(final int opcode) {
        this.opcode = opcode;
    }

    @Override
    public int getType() {
        return JUMP_INSN;
    }

    @Override
    public void accept(final MethodVisitor mv) {
        mv.visitJumpInsn(opcode, label.getLabel(), registerA, registerB);
    }

    @Override
    public AbstractInsnNode clone(final Map<LabelNode, LabelNode> labels) {
        return new JumpInsnNode(opcode, clone(label, labels), registerA, registerB);
    }
}
