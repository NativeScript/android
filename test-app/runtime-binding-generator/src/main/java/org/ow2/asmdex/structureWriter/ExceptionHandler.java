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

package org.ow2.asmdex.structureWriter;

import org.ow2.asmdex.structureCommon.Label;

/**
 * Class used to store an Exception Handler, that is a Label to the Handler, and the Type of the Exception.
 * The Type can be Null, which means the Handler is a CatchAll.
 *
 * @author Julien Névo.
 *
 */
public class ExceptionHandler {

    /**
     * Label pointing on the Handler of the exception.
     */
    final private Label handler;

    /**
     * Type of the exception. May be null (for catch all).
     */
    final private String type;

    /**
     * Constructor.
     * @param handler Label to the Handler of the Exception.
     * @param type Type of the Exception, or Null if the Handler is a CatchAll.
     */
    public ExceptionHandler(Label handler, String type) {
        this.handler = handler;
        this.type = type;
    }

    /**
     * Returns True if the ExceptionHandler concerns a CatchAll.
     * @return True if the ExceptionHandler concerns a CatchAll.
     */
    public boolean isCatchAll() {
        return type == null;
    }

    /**
     * Returns the Label pointing on the Handler of the exception.
     * @return the Label pointing on the Handler of the exception.
     */
    public Label getHandler() {
        return handler;
    }

    /**
     * Returns the Type of the exception. May be null (for catch all).
     * @return the Type of the exception. May be null (for catch all).
     */
    public String getType() {
        return type;
    }


    // --------------------------------------
    // Overridden Methods.
    // --------------------------------------

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }

        boolean result = false;

        if (obj instanceof ExceptionHandler) {
            ExceptionHandler eh = (ExceptionHandler)obj;
            result = (handler.equals(eh.handler));
            if (result) {
                if (type == null) {
                    result = (eh.type == null);
                } else {
                    result = (eh.type == null ? false : type.equals(eh.type));
                }
            }
        }

        return result;
    }

    @Override
    public int hashCode() {
        return handler.hashCode() + type.hashCode();
    }
}