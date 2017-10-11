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

package org.ow2.asmdex.encodedValue;

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Class representing an Encoded Value containing a char.
 *
 * @author Julien Névo
 */
public class EncodedValueCharacter extends EncodedValue {

    /**
     * The Type of the Encoded Value.
     */
    private int type = Opcodes.VALUE_CHAR;

    /**
     * The value of the Encoded Value.
     */
    private char value;

    /**
     * Constructs the Encoded Value from its value.
     * @param value the value.
     */
    public EncodedValueCharacter(char value) {
        this.value = value;
    }

    /**
     * Constructs the Encoded Value from an Object containing a value.
     * @param value the Object containing a value.
     */
    public EncodedValueCharacter(Object value) {
        this.value = (Character)value;
    }

    @Override
    public byte[] encode(ConstantPool constantPool) {
        byte[] result;
        int val = (Character)value;
        int nb1 = 0;
        int nb2 = 0;
        int size = 1;
        if (val != 0) {
            nb1 = val & 0xff;
            val = val >>> 8;
            if (val != 0) {
                size++;
                nb2 = val & 0xff;
            }
        }
        result = new byte[size + 1]; // We keep the first entry to code the "header".
        if (size > 1) {
            result[2] = (byte)nb2;
        }
        result[1] = (byte)nb1;
        result[0] = EncodedValueUtil.encodeHeader(size, type);
        return result;
    }

    @Override
    public int getType() {
        return type;
    }


    // -----------------------------------------------------
    // Overridden Methods.
    // -----------------------------------------------------

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        boolean result = false;

        if (o instanceof EncodedValueCharacter) {
            EncodedValueCharacter encodedValue = (EncodedValueCharacter)o;
            result = ((type == encodedValue.type) && (value == encodedValue.value));
        }

        return result;
    }

    @Override
    public int hashCode() {
        return type + value * 729;
    }

    @Override
    int compareValue(EncodedValue encodedValue) {
        if (this == encodedValue) {
            return 0;
        }

        EncodedValueCharacter ec = (EncodedValueCharacter)encodedValue;
        char value2 = ec.value;
        return (value == value2 ? 0 : (value > value2 ? 1 : -1));
    }

    @Override
    public String toString() {
        return String.valueOf(value);
    }

}
