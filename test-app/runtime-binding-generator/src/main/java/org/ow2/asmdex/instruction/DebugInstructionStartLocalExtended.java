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
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Class representing a Debug Instruction Start Local Extended Instruction.
 *
 * @author Julien Névo
 */
public class DebugInstructionStartLocalExtended extends DebugInstruction
    implements IDebugLocalVariableStartInstruction, IDebugRegisterInstruction {

    /**
     * The register of the local variable.
     */
    protected int register;

    /**
     * The name of the local variable.
     */
    protected String name;

    /**
     * The type of the local variable.
     */
    protected String type;

    /**
     * The signature of the local variable.
     */
    protected String signature;

    /**
     * The Label from which starts the Debug Instruction.
     */
    protected Label start;

    /**
     * Constructor of the Debug Instruction Start Local.
     * @param register the register of the local variable.
     * @param name the name of the local variable.
     * @param type the type of the local variable.
     * @param signature the signature of the local variable.
     * @param start the Label where the Local Variable starts.
     */
    public DebugInstructionStartLocalExtended(int register, String name,
            String type, String signature, Label start) {
        this.register = register;
        this.name = name;
        this.type = type;
        this.signature = signature;
        this.start = start;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public String getType() {
        return type;
    }

    @Override
    public String getSignature() {
        return signature;
    }

    @Override
    public int getRegister() {
        return register;
    }

    @Override
    public int getOpcode() {
        return Opcodes.DBG_START_LOCAL_EXTENDED;
    }

    @Override
    public Label getLabel() {
        return start;
    }

    @Override
    public void write(ByteVector out, ConstantPool constantPool) {
        out.putByte(Opcodes.DBG_START_LOCAL_EXTENDED);
        out.putUleb128(register);
        out.putUleb128p1(constantPool.getStringIndex(name));
        out.putUleb128p1(constantPool.getTypeIndex(type));
        out.putUleb128p1(constantPool.getStringIndex(signature));
    }



}
