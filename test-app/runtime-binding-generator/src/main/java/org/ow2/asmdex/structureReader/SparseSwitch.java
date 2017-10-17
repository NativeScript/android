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
 * Contains the data of a Spare Switch structure.
 *
 * @author Julien Névo
 *
 */
public class SparseSwitch implements ISwitchCase {

    /**
     * The keys of each "case".
     */
    private int[] keys;

    /**
     * The labels related to each "case".
     */
    private Label[] switchLabels;

    /**
     * The default label of the Switch structure.
     */
    private Label defaultLabel;


    /**
     * Set of values handled.
     * @return the values filtered as an array of integer
     */
    public int[] getKeys() {
        return keys;
    }
    /**
     * Setter for values handled by the sparse switch
     * @param keys
     */
    public void setKeys(int[] keys) {
        this.keys = keys;
    }

    @Override
    public Label[] getSwitchLabels() {
        return switchLabels;
    }
    @Override
    public void setSwitchLabels(Label[] switchLabels) {
        this.switchLabels = switchLabels;
    }

    @Override
    public Label getDefaultLabel() {
        return defaultLabel;
    }
    @Override
    public void setDefaultLabel(Label defaultLabel) {
        this.defaultLabel = defaultLabel;
    }
}
