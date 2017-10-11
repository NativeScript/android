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

import java.util.ArrayList;
import java.util.List;

import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.structureCommon.Label;

/**
 * Class representing an encoded_catch_handler. It also possibly contains encoded_type_addr_pair.
 * It is only useful when encoding the Method it is contained in, in order to allow uniqueness of this
 * structure.
 *
 * @author Julien Névo
 */
public class EncodedCatchHandler {

    final private ConstantPool constantPool;

    /**
     * Count of Catch Types in the list. Does NOT count the Catch All, and does NOT invert
     * the sign if a Catch All is present.
     */
    private int sizeCatchTypes = 0;

    /**
     * Catch All Handler. May be Null.
     */
    private Label catchAllHandler;

    /**
     * Types of the various Handlers inside this Catch Handler. For each must corresponds
     * an Exception Handler below.
     */
    private List<String> types = new ArrayList<String>();

    /**
     * The Excepton Handlers inside this Catch handler. For each must corresponds
     * a Type above.
     */
    private List<Label> exceptionHandlers = new ArrayList<Label>();


    /**
     * Creates the catch handler block
     * @param constantPool
     */
    public EncodedCatchHandler(ConstantPool constantPool) {
        this.constantPool = constantPool;
    }


    // --------------------------------------
    // Public Methods.
    // --------------------------------------

    /**
     * Add a new exception handler
     * @param type the type of exceptions caught
     * @param exceptionHandler label where the handler is
     */
    public void addTypeAddrPair(String type, Label exceptionHandler) {
        if (type == null) {
            catchAllHandler = exceptionHandler;
        } else {
            types.add(type);
            exceptionHandlers.add(exceptionHandler);
            sizeCatchTypes++;
        }
    }

    /**
     * Write to a byte vector
     * @param out where to write to
     */
    public void write(ByteVector out) {
        // Now encodes the encoded_catch_handler item itself.
        boolean hasCatchAll = (catchAllHandler != null);
        // Encodes the number of Exception Handlers. Negative size if a CatchAll is present.
        out.putSleb128(hasCatchAll ? -sizeCatchTypes : sizeCatchTypes); // Encode size.

        // Encodes the encoded_type_addr_pair.
        for (int i = 0, size = types.size(); i < size; i++) {
            int typeIndex = constantPool.getTypeIndex(types.get(i));
            out.putUleb128(typeIndex);
            out.putUleb128(exceptionHandlers.get(i).getOffset() / 2);
        }

        // Encodes the catch_all_addr, if any.
        if (hasCatchAll) {
            out.putUleb128(catchAllHandler.getOffset() / 2);
        }
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

        if (obj instanceof EncodedCatchHandler) {
            EncodedCatchHandler ech = (EncodedCatchHandler)obj;
            if (catchAllHandler == null) {
                result = (ech.catchAllHandler == null);
            } else {
                result = (ech.catchAllHandler == null ? false :
                          catchAllHandler.equals(ech.catchAllHandler));
            }

            if (result) {
                result = types.equals(ech.types) && exceptionHandlers.equals(ech.exceptionHandlers);
            }
        }

        return result;
    }

    @Override
    public int hashCode() {
        int result = catchAllHandler == null ? 0 : catchAllHandler.hashCode();
        for (String type : types) {
            if (type != null) {
                result += type.hashCode();
            }
        }
        for (Label label : exceptionHandlers) {
            result += label.hashCode();
        }
        return result;
    }



}
