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

/**
 * Factory for Encoded Values.
 *
 * @author Julien Névo
 */
public class EncodedValueFactory {

    /**
     * Gets the Encoded Value from an Object containing the value to encode, and the descriptor of the
     * value. It can only be a primitive element.
     * @param value the Value.
     * @param desc descriptor of the value.
     * @return the Encoded Value.
     */
    public static EncodedValue getEncodedValue(Object value, String desc) {
        int type = EncodedValueUtil.getTypeFromDescriptor(desc);
        return getEncodedValue(value, type);
    }

    /**
     * Returns the Encoded Value from an Object and the type of the value.
     * The value is ignored for Arrays. It has to be filled element by element.
     * @param value the Value.
     * @param type type of the Object (see {@link Opcodes#VALUE_INT}).
     * @return the Encoded Value.
     */
    public static EncodedValue getEncodedValue(Object value, int type) {
        EncodedValue encodedValue = null;

        switch (type) {
        case Opcodes.VALUE_BYTE:
            encodedValue = new EncodedValueByte(value);
            break;
        case Opcodes.VALUE_SHORT:
            encodedValue = new EncodedValueShort(value);
            break;
        case Opcodes.VALUE_CHAR:
            encodedValue = new EncodedValueCharacter(value);
            break;
        case Opcodes.VALUE_INT:
            encodedValue = new EncodedValueInteger(value);
            break;
        case Opcodes.VALUE_LONG:
            encodedValue = new EncodedValueLong(value);
            break;
        case Opcodes.VALUE_FLOAT:
            encodedValue = new EncodedValueFloat(value);
            break;
        case Opcodes.VALUE_DOUBLE:
            encodedValue = new EncodedValueDouble(value);
            break;
        case Opcodes.VALUE_STRING:
            encodedValue = new EncodedValueString((String) value);
            break;
        case Opcodes.VALUE_TYPE:
            encodedValue = new EncodedValueType(value);
            break;
        case Opcodes.VALUE_FIELD:
            encodedValue = new EncodedValueField(value);
            break;
        case Opcodes.VALUE_METHOD:
            encodedValue = new EncodedValueMethod(value);
            break;
        case Opcodes.VALUE_ENUM:
            encodedValue = new EncodedValueEnum(value);
            break;
        case Opcodes.VALUE_ARRAY:
            encodedValue = new EncodedValueArray();
            break;
        case Opcodes.VALUE_ANNOTATION:
            encodedValue = new EncodedValueAnnotation(value);
            break;
        case Opcodes.VALUE_NULL:
            encodedValue = new EncodedValueNull();
            break;
        case Opcodes.VALUE_BOOLEAN:
            encodedValue = new EncodedValueBoolean(value);
            break;

        default:
            try {
                throw new Exception("Unknown type for this value.");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return encodedValue;
    }

    /**
     * Returns an Encoded Value, with a value to 0 (if a primitive) or Null (if a reference).
     * @param desc the descriptor of the Encoded Value.
     * @return an Encoded Value, with a value to 0 (if a primitive) or Null (if a reference).
     */
    public static EncodedValue getEncodedEmptyValue(String desc) {
        EncodedValue encodedValue = null;

        if (EncodedValueUtil.isTypeAReference(desc)) {
            encodedValue = new EncodedValueNull();
        } else {
            int type = EncodedValueUtil.getTypeFromDescriptor(desc);
            encodedValue = getEncodedValue(0, type);
        }

        return encodedValue;
    }

    /**
     * Returns a Null Encoded Value.
     * @return a Null Encoded Value.
     */
    public static EncodedValue getEncodedNullValue() {
        return new EncodedValueNull();
    }



    /**
     * Gets an Encoded Value, only from an Object. It should only be used when creating an Annotation,
     * as we have the Object but not the type. Only primitives and array of primitive can be created.
     * @param value the Object containing the value to encode (primitive or Object[]).
     * @return the Encoded Value.
     */
    public static EncodedValue getEncodedValue(Object value) {

        // First, checks the primitive.
        if (value instanceof Integer) {
            return new EncodedValueInteger(value);
        } else if (value instanceof Boolean) {
            return new EncodedValueBoolean(value);
        } else if (value instanceof Byte) {
            return new EncodedValueByte(value);
        } else if (value instanceof Character) {
            return new EncodedValueCharacter(value);
        } else if (value instanceof Double) {
            return new EncodedValueDouble(value);
        } else if (value instanceof Float) {
            return new EncodedValueFloat(value);
        } else if (value instanceof Long) {
            return new EncodedValueLong(value);
        } else if (value instanceof Short) {
            return new EncodedValueShort(value);
        } else if (value instanceof String) {
            return new EncodedValueString((String) value);
        }

        // Test the Arrays.
        else if (value instanceof int[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            int[] array = (int[])value;
            for (int v : array) {
                encodedArray.addEncodedValue(new EncodedValueInteger(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof boolean[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            boolean[] array = (boolean[])value;
            for (boolean v : array) {
                encodedArray.addEncodedValue(new EncodedValueBoolean(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof byte[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            byte[] array = (byte[])value;
            for (byte v : array) {
                encodedArray.addEncodedValue(new EncodedValueByte(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof char[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            char[] array = (char[])value;
            for (char v : array) {
                encodedArray.addEncodedValue(new EncodedValueCharacter(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof double[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            double[] array = (double[])value;
            for (double v : array) {
                encodedArray.addEncodedValue(new EncodedValueDouble(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof float[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            float[] array = (float[])value;
            for (float v : array) {
                encodedArray.addEncodedValue(new EncodedValueFloat(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof long[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            long[] array = (long[])value;
            for (long v : array) {
                encodedArray.addEncodedValue(new EncodedValueLong(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof short[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            short[] array = (short[])value;
            for (short v : array) {
                encodedArray.addEncodedValue(new EncodedValueShort(v));
            }
            return (EncodedValue)encodedArray;
        } else if (value instanceof String[]) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            String[] array = (String[])value;
            for (String v : array) {
                encodedArray.addEncodedValue(new EncodedValueString(v));
            }
            return (EncodedValue)encodedArray;
        }

        else {
            try {
                throw new Exception("Unable to find the type of this Value.");
            } catch (Exception e) {
                e.printStackTrace();
            }

            return null;
        }
    }

}
