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

package org.ow2.asmdex.structureCommon;

import java.util.ArrayList;
import java.util.List;

/**
 * A Local Variable. It is identified by its Register Number. It can hold none or one Starts,
 * and none, one or or several Ends and/or Restart Labels.
 * The Start may be unknown. In that case, the name, type and signature are also unknown, as they
 * are given along with the Start in the debug_info_item.
 *
 * @author Julien Névo
 */
public class LocalVariable {

    /**
     * The register index of the variable.
     */
    private int register;

    /**
     * The name of the variable.
     */
    private String name;

    /**
     * The type (descriptor) of the variable.
     */
    private String type;

    /**
     * The signature. Only used by generics, Null otherwise.
     */
    private String signature;

    /**
     * The Label where the variable starts.
     */
    private Label start;

    /**
     * The Labels where the variable ends. May be null.
     */
    private List<Label> ends;

    /**
     * The Labels where the variable restarts. May be null.
     */
    private List<Label> restarts;

    /**
     * Constructor of the Local Variable.
     * @param register the register index of the variable.
     * @param name the name of the variable.
     * @param type the type (descriptor) of the variable.
     * @param signature the signature. Only used by generics, Null otherwise.
     * @param start the Label where the variable starts.
     * @param ends the Labels where the variable ends. May be null.
     * @param restarts the Labels where the variable restarts. May be null.
     */
    public LocalVariable(int register, String name, String type, String signature,
                         Label start, List<Label> ends, List<Label> restarts) {
        this.register = register;
        this.name = name;
        this.type = type;
        this.signature = signature;
        this.start = start;
        this.ends = ends;
        this.restarts = restarts;
    }

    /**
     * Constructor of the Local Variable.
     * @param register the register index of the variable.
     */
    public LocalVariable(int register) {
        this.register = register;
    }


    // ------------------------------------
    // Getters and Setters.
    // ------------------------------------

    /**
     * Returns the register index of the variable.
     * @return the register index of the variable.
     */
    public int getRegister() {
        return register;
    }

    /**
     * Returns the name of the variable.
     * @return the name of the variable.
     */
    public String getName() {
        return name;
    }

    /**
     * Sets the name of the variable.
     * @param name the name of the variable.
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Returns the type (descriptor) of the variable.
     * @return the type (descriptor) of the variable.
     */
    public String getType() {
        return type;
    }

    /**
     * Sets the type of the variable.
     * @param type the type of the variable.
     */
    public void setType(String type) {
        this.type = type;
    }

    /**
     * Returns the signature. Only used by generics, Null otherwise.
     * @return the signature. Only used by generics, Null otherwise.
     */
    public String getSignature() {
        return signature;
    }

    /**
     * Sets the signature of the variable.
     * @param signature the signature of the variable.
     */
    public void setSignature(String signature) {
        this.signature = signature;
    }

    /**
     * Returns the Label where the variable starts.
     * @return the Label where the variable starts.
     */
    public Label getStart() {
        return start;
    }

    /**
     * Sets the Start of the variable.
     * @param start the Start of the variable.
     */
    public void setStart(Label start) {
        this.start = start;
    }

    /**
     * Returns the Labels where the variable ends. May be null.
     * @return the Labels where the variable ends. May be null.
     */
    public List<Label> getEnds() {
        return ends;
    }

    /**
     * Returns the Labels where the variable restarts. May be null.
     * @return the Labels where the variable restarts. May be null.
     */
    public List<Label> getRestarts() {
        return restarts;
    }

    /**
     * Adds the Label where the variable ends.
     * @param end the Label where the variable ends.
     */
    public void addEnd(Label end) {
        ends = addLabel(ends, end);
    }

    /**
     * Adds the Label where the variable restarts.
     * @param restart the Label where the variable restarts.
     */
    public void addRestart(Label restart) {
        restarts = addLabel(restarts, restart);
    }

    /**
     * Adds a Label to the given list, creating it if it was null.
     * @param labelList the list.
     * @param label the Label to add.
     * @param the modified list.
     */
    private List<Label> addLabel(List<Label> labelList, Label label) {
        List<Label> result = labelList;
        if (result == null) {
            result = new ArrayList<Label>();
        }
        result.add(label);

        return result;
    }
}
