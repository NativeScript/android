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

package org.ow2.asmdex.structureReader;

import org.ow2.asmdex.structureCommon.Label;

/**
 * Contains the data of a Try/Catch structure <i>for the Reader</i>.
 *
 * @author Julien Névo
 */
public class TryCatch {

    /**
     * Label pointing on the Instruction starting the try/catch block.
     */
    private Label start;

    /**
     * Label pointing on the Instruction following the end of the try/catch block.
     */
    private Label end;

    /**
     * Label pointing on the Handler of the exception.
     */
    private Label handler;

    /**
     * Type of the exception. May be null (for catch all).
     */
    private String type;

    /**
     * Constructor of a Try/Catch structure.
     * @param start Label pointing on the Instruction starting the try/catch block.
     * @param end Label pointing on the Instruction following the end of the try/catch block.
     * @param handler Label pointing on the Handler of the exception.
     * @param type Type of the exception. May be null (for catch all).
     */
    public TryCatch(Label start, Label end, Label handler, String type) {
        this.start = start;
        this.end = end;
        this.handler = handler;
        this.type = type;
    }


    // --------------------------------------------
    // Getters.
    // --------------------------------------------

    /**
     * Label of first instruction protected
     * @return label of first instruction
     */
    public Label getStart() {
        return start;
    }

    /**
     * Label of last instruction protected
     * @return label of last instruction
     */
    public Label getEnd() {
        return end;
    }

    /**
     * Label of exception handler
     * @return label of handler
     */
    public Label getHandler() {
        return handler;
    }

    /**
     * Type of the exception caught
     * @return type name
     */
    public String getType() {
        return type;
    }
}
