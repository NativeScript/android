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
import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.structureWriter.ConstantPool;

import java.util.ArrayList;

/**
 * Class representing an Encoded Array.
 *
 * @author Julien Névo
 */
public class EncodedValueArray extends EncodedValue {

    /**
     * The Type of the Encoded Value.
     */
    private int type = Opcodes.VALUE_ARRAY;

    /**
     * The Encoded Values.
     */
    private ArrayList<EncodedValue> encodedValues = new ArrayList<EncodedValue>();

    // -----------------------------------------------------
    // Getters and Setters.
    // -----------------------------------------------------

    @Override
    public int getType() {
        return type;
    }

    /**
     * Returns the Encoded Values.
     * @return the Encoded Values.
     */
    public ArrayList<EncodedValue> getEncodedValues() {
        return encodedValues;
    }

    /**
     * Returns the number of Encoded Values.
     * @return the number of Encoded Values.
     */
    public int getNbEncodedValues() {
        return encodedValues.size();
    }


    // -----------------------------------------------------
    // Public methods.
    // -----------------------------------------------------

    /**
     * Adds an Encoded Value to the list.
     * @param encodedValue the Encoded Value to add.
     */
    public void addEncodedValue(EncodedValue encodedValue) {
        encodedValues.add(encodedValue);
    }

    @Override
    public byte[] encode(ConstantPool constantPool) {
        ByteVector out = new ByteVector();

        // Encodes first the value_array.
        out.putByte(Opcodes.VALUE_ARRAY);

        // Encodes then the encoded_array format.
        out.putUleb128(getNbEncodedValues());

        // Encodes each Encoded Value of the array.
        for (EncodedValue encodedValue : encodedValues) {
            out.putByteArray(encodedValue.encode(constantPool));
        }

        return out.getData();
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
        if (o instanceof EncodedValueArray) {
            EncodedValueArray encodedValue = (EncodedValueArray)o;
            result = encodedValues.equals(encodedValue.encodedValues);
        }
        return result;
    }

    @Override
    public int hashCode() {
        int result = type;
        for (EncodedValue encodedValue : encodedValues) {
            result += encodedValue.hashCode();
        }
        return result;
    }

    @Override
    int compareValue(EncodedValue encodedValue) {
        EncodedValueArray ea = (EncodedValueArray)encodedValue;

        int size1 = encodedValues.size();
        int size2 = ea.encodedValues.size();
        int size = size1 < size2 ? size1 : size2;
        int result = 0;
        int i = 0;
        while ((result == 0) && (i < size)) {
            EncodedValue v1 = encodedValues.get(i);
            EncodedValue v2 = ea.encodedValues.get(i);
            int v1Type = v1.getType();
            int v2Type = v2.getType();
            result = v1Type == v2Type ? 0 : (v1Type < v2Type ? -1 : 1);
            // Only compare the actual value if their type is the same, else it
            // doesn't mean anything.
            if (result == 0) {
                result = encodedValues.get(i).compareTo(ea.encodedValues.get(i));
                i++;
            }
        }

        if (result == 0) {
            result = size1 == size2 ? 0 : (size1 < size2 ? -1 : 1);
        }
        return result;
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        boolean isFirst = true;
        for (EncodedValue ev: encodedValues) {
            if (isFirst) {
                result.append("{");
                isFirst = false;
            } else {
                result.append(",");
            }
            result.append(ev);
        }
        result.append("}");
        return result.toString();
    }
}
