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
import org.ow2.asmdex.structureWriter.Field;

/**
 * @author Julien Névo
 * Represents a static value encoded in the file.
 */
public class EncodedValueEnum extends EncodedValue {

    /**
     * The Type of the Encoded Value.
     */
    private int type = Opcodes.VALUE_ENUM;

    /**
     * The value of the Encoded Value.
     */
    private Field value;

    /**
     * Constructs the Encoded Value from its value.
     * @param value the value.
     */
    public EncodedValueEnum(Field value) {
        this.value = value;
    }

    /**
     * Constructs the Encoded Value from an Object containing a value.
     * @param value the Object containing a value.
     */
    public EncodedValueEnum(Object value) {
        this.value = (Field)value;
    }

    @Override
    public byte[] encode(ConstantPool constantPool) {
        int index = constantPool.getFieldIndex(value);
        return EncodedValueUtil.encodeUnsignedValue(index, type);
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

        if (o instanceof EncodedValueEnum) {
            EncodedValueEnum encodedValue = (EncodedValueEnum)o;
            result = ((type == encodedValue.type) && (value.equals(encodedValue.value)));
        }

        return result;
    }

    @Override
    public int hashCode() {
        return type + value.hashCode() * 9664;
    }

    @Override
    int compareValue(EncodedValue encodedValue) {
        if (this == encodedValue) {
            return 0;
        }

        EncodedValueEnum ee = (EncodedValueEnum)encodedValue;
        return value.compareTo(ee.value);
    }

    @Override
    public String toString() {
        return String.valueOf(value);
    }

}
