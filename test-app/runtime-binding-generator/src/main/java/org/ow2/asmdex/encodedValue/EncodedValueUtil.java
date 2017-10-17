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

import org.ow2.asmdex.Constants;
import org.ow2.asmdex.Opcodes;

/**
 * Utility Class for the encoding of Encoded Value.
 *
 * @author Julien Névo
 */
public class EncodedValueUtil {

    /**
     * Encodes the Header of a Value (the first byte including the value_arg and value_type).
     * @param size actual size of the encoded value (NOT decremented of 1).
     * @param type type of the value.
     * @return a byte representing the header.
     */
    public static byte encodeHeader(int size, int type) {
        if ((type == 0x00) || (type >= 0x1c)) {
            return (byte)type;
        }
        return (byte)(type | ((size - 1) << 5));
    }

    /**
     * Returns an unsigned encoded_value, including the header.
     * @param val unsigned value to encode.
     * @param type type of the value. This is only needed for the header.
     * @return an array consisting of an encoded_value and its header.
     */
    public static byte[] encodeUnsignedValue(int val, int type) {
        byte[] result;

        int nb1 = 0;
        int nb2 = 0;
        int nb3 = 0;
        int nb4 = 0;

        int size = 1;
        if (val != 0) {
            nb1 = val & 0xff;
            val = val >>> 8;
            if (val != 0) {
                size++;
                nb2 = val & 0xff;
                val = val >>> 8;
                if (val != 0) {
                    size++;
                    nb3 = val & 0xff;
                    val = val >>> 8;
                    if (val != 0) {
                        size++;
                        nb4 = val & 0xff;
                    }
                }
            }
        }

        result = new byte[size + 1]; // We keep the first entry to code the "header".
        if (size > 3) {
            result[4] = (byte)nb4;
        }
        if (size > 2) {
            result[3] = (byte)nb3;
        }
        if (size > 1) {
            result[2] = (byte)nb2;
        }
        result[1] = (byte)nb1;

        result[0] = encodeHeader(size, type);
        return result;
    }

    /**
     * Returns an encoded_value, including the header.
     * @param val value to encode.
     * @param type type of the value. This is only needed for the header.
     * @return an array consisting of an encoded_value and its header.
     */
    public static byte[] encodeSignedValue(int val, int type) {
        int nbBytes = getNbBytesInSignedValue(val);
        byte[] bytes = new byte[nbBytes + 1]; // Leave one byte for the header.
        for (int i = 1; i < (nbBytes + 1); i++) {
            bytes[i] = (byte)val;
            val = val >> 8;
        }
        bytes[0] = encodeHeader(nbBytes, type);
        return bytes;
    }

    /**
     * Returns an encoded_value (long), including the header.
     * @param val value to encode.
     * @param type type of the value. This is only needed for the header.
     * @return an array consisting of an encoded_value and its header.
     */
    public static byte[] encodeSignedValue(long val, int type) {
        int nbBytes = getNbBytesInSignedValue(val);
        byte[] bytes = new byte[nbBytes + 1]; // Leave one byte for the header.
        for (int i = 1; i < (nbBytes + 1); i++) {
            bytes[i] = (byte)val;
            val = val >> 8;
        }
        bytes[0] = encodeHeader(nbBytes, type);
        return bytes;
    }

    /**
     * Returns an zero-extended-to-the-right encoded_value, including the header.
     * @param val value to encode.
     * @param type type of the value. This is only needed for the header.
     * @return an array consisting of an encoded_value and its header.
     */
    public static byte[] encodeZeroExtendedToRightValue(long val, int type) {
        int nbBytes = getNbBytesForRightZeroExtendedValue(val);
        val = val >> (64 - (nbBytes * 8));
        byte[] bytes = new byte[nbBytes + 1]; // Leave one byte for the header.
        for (int i = 1; i < (nbBytes + 1); i++) {
            bytes[i] = (byte)val;
            val = val >> 8;
        }
        bytes[0] = encodeHeader(nbBytes, type);
        return bytes;
    }


    /**
     * Returns the bytes count needed to encode the given signed value.
     * @param value the signed value.
     * @return the bytes count needed to encode the value.
     */
    public static int getNbBytesInSignedValue(long value) {
        int nbBits = 65 - Long.numberOfLeadingZeros(value ^ (value >> 63));
        return (nbBits + 0x07) >> 3;
    }

    /**
     * Returns the bytes count needed to encode the given signed value.
     * @param value the signed value.
     * @return the bytes count needed to encode the value.
     */
    public static int getNbBytesInSignedValue(int value) {
        int nbBits = 33 - Integer.numberOfLeadingZeros(value ^ (value >> 31));
        return (nbBits + 0x07) >> 3;
    }

    /**
     * Returns the bytes count needed to encoded the given value, zero-extended
     * to the right.
     * @param value the value.
     * @return the bytes count needed to encode the value.
     */
    public static int getNbBytesForRightZeroExtendedValue(long value) {
        int requiredBits = 64 - Long.numberOfTrailingZeros(value);
        if (requiredBits == 0) {
            requiredBits = 1;
        }
        return (requiredBits + 0x07) >> 3;
    }

    /**
     * Converts a Descriptor into a Value Type. Works only for primitives.
     * @param desc descriptor. Should contain only one element, only the first is taken in account.
     * @return the value type, see {@link Opcodes}.
     */
    public static int getTypeFromDescriptor(String desc) {
        int result = 0;

        switch (desc.charAt(0)) {
        case 'Z':
            result = Opcodes.VALUE_BOOLEAN;
            break;
        case 'B':
            result = Opcodes.VALUE_BYTE;
            break;
        case 'S':
            result = Opcodes.VALUE_SHORT;
            break;
        case 'C':
            result = Opcodes.VALUE_CHAR;
            break;
        case 'I':
            result = Opcodes.VALUE_INT;
            break;
        case 'J':
            result = Opcodes.VALUE_LONG;
            break;
        case 'F':
            result = Opcodes.VALUE_FLOAT;
            break;
        case 'D':
            result = Opcodes.VALUE_DOUBLE;
            break;
        default:
            try {
                throw new Exception("Unknown descriptor to convert: " + desc);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return result;
    }

    /**
     * Indicates whether the given descriptor is a reference or a primitive.
     * @param desc descriptor. Should contain only one element, only the first is taken in account.
     * @return true if the given descriptor is a reference.
     */
    public static boolean isTypeAReference(String desc) {
        boolean result = false;

        switch (desc.charAt(0)) {
        case 'L':
        case '[':
            result = true;
            break;
        }

        return result;
    }

    /**
     * Indicates whether the given descriptor is a String.
     * @param desc descriptor. Should contain only one element.
     * @return true if the given descriptor is a String.
     */
    public static boolean isString(String desc) {
        return desc.equals(Constants.STRING_TYPE);
    }

}
