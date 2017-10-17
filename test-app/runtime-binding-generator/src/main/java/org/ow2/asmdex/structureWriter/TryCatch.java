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

import org.ow2.asmdex.structureCommon.Label;

/**
 * Contains the data of a Try/Catch structure <i>for the Writer</i>.
 *
 * Two structures are considered equals if the Start Label is the same. A structure can't hold more than one
 * CatchAll (ExceptionHandler with a Null Type), and raises an Exception if one try to add more than one.
 *
 * @author Julien Névo
 */
public class TryCatch {

    /**
     * Label pointing on the Instruction starting the try/catch block.
     */
    final private Label start;

    /**
     * Label pointing on the Instruction following the end of the try/catch block.
     */
    final private Label end;

    /**
     * List of structures holding the Handler Label, and the Type of the Exception.
     */
    private ArrayList<ExceptionHandler> exceptionHandlers = new ArrayList<ExceptionHandler>();

    /**
     * Handler of the CatchAll, if any.
     */
    private Label catchAllHandler;

    /**
     * Constructor of a Try/Catch structure.
     * @param start Label pointing on the Instruction starting the try/catch block.
     * @param end Label pointing on the Instruction following the end of the try/catch block.
     * @param exceptionHandler Label pointing on the Handler of the exception.
     */
    public TryCatch(Label start, Label end, ExceptionHandler exceptionHandler) {
        this.start = start;
        this.end = end;
        addExceptionHandler(exceptionHandler);
    }

    /**
     * Adds an ExceptionHandler to the list.
     * @param exceptionHandler the ExceptionHandler to add to the list.
     */
    public void addExceptionHandler(ExceptionHandler exceptionHandler) {
        this.exceptionHandlers.add(exceptionHandler);
        if (exceptionHandler.getType() == null) {
            if (hasCatchAll()) {
                try {
                    throw new Exception("A Try/Catch can't support more than one CatchAll.");
                } catch (Exception e) {
                    e.printStackTrace();
                }
            } else {
                catchAllHandler = exceptionHandler.getHandler();
            }
        }
    }

    /**
     * Adds Exception Handlers to the list of the current structure.
     * @param exceptionHandlers
     */
    public void addExceptionHandlers(ArrayList<ExceptionHandler> exceptionHandlers) {
        for (ExceptionHandler exceptionHandler : exceptionHandlers) {
            addExceptionHandler(exceptionHandler);
        }
    }


    // --------------------------------------------
    // Getters.
    // --------------------------------------------

    /**
     * Returns the Start Label.
     * @return the Start Label.
     */
    public Label getStart() {
        return start;
    }

    /**
     * Returns the End Label (exclusive).
     * @return the End Label (exclusive).
     */
    public Label getEnd() {
        return end;
    }

    /**
     * Returns the List of ExceptionHandlers.
     * @return the List of ExceptionHandlers.
     */
    public ArrayList<ExceptionHandler> getExceptionHandlers() {
        return exceptionHandlers;
    }

    /**
     * Returns the count of Exception Handler, CatchAll included.
     * @return the count of Exception Handler, CatchAll included.
     */
    public int getNbExceptionHandler() {
        return exceptionHandlers.size();
    }

    /**
     * True if one of the Exception Handler has a Catch All.
     * @return true if one of the Exception Handler has a Catch All.
     */
    public boolean hasCatchAll() {
        return (catchAllHandler != null);
    }

    /**
     * Returns the CatchAll Handler, or Null if no Exception Handler uses one.
     * @return the CatchAll Handler, or Null if no Exception Handler uses one.
     */
    public Label getCatchAllHandler() {
        return catchAllHandler;
    }
}
