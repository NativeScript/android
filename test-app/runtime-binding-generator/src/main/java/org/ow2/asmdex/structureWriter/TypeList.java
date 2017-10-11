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

import java.util.Arrays;

/**
 * Class representing the type_list structure of the Dex file. Consists in a list of type_ids, which are
 * here represented as simple Strings.
 *
 * @author Julien Névo.
 *
 */
public class TypeList implements Comparable<TypeList> {

    /**
     * Types of the TypeList.
     */
    final private String[] types;

    /**
     * The hashCode of the TypeList.
     */
    final private int hashCode;

    /**
     * Constructor of a TypeList.
     * @param types types to add to the TypeList. May be Null.
     */
    public TypeList(String[] types) {
        if (types != null) {
            this.types = types;

            // Calculates the HashCode.
            int result = 0;
            for (String string : types) {
                result += string.hashCode();
            }

            hashCode = result;
        } else {
            hashCode = 0;
            this.types = new String[0];
        }
    }


    // -------------------------------------
    // Getters.
    // -------------------------------------

    /**
     * Gives back the list of types
     * @return types formatted in Java format
     */
    public String[] getTypeList() {
        return types;
    }

    /**
     * Number of types in the list
     * @return number of types.
     */
    public int size() {
        return types.length;
    }

    /**
     * Get a type from the list
     * @param index position of the type
     * @return the type as a string in Java format
     */
    public String get(int index) {
        return types[index];
    }


    // ----------------------------------------------
    // Overridden methods.
    // ----------------------------------------------

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o instanceof TypeList) {
            TypeList secondTypeList = (TypeList)o;
            return Arrays.deepEquals(types, secondTypeList.types);
        }

        return false;
    }

    @Override
    public int hashCode() {
        return hashCode;
    }

    @Override
    public int compareTo(TypeList o) {
        if (this == o) {
            return 0;
        }

        String[] secondTypes = o.getTypeList();

        // We compare the Strings one by one.
        int sizeFirstArray = types.length;
        int sizeSecondArray = secondTypes.length;
        int minSize = sizeFirstArray < sizeSecondArray ? sizeFirstArray : sizeSecondArray;
        for (int i = 0; i < minSize; i++) {
            int compare = types[i].compareTo(secondTypes[i]);
            if (compare != 0) {
                return compare;
            }
        }

        // Compares the size.
        return (sizeFirstArray == sizeSecondArray ? 0 : (sizeFirstArray < sizeSecondArray ? -1 : 1));
    }
}
