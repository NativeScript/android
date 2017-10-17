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

/**
 * Prototype of a method, like described in the proto_id_item structure.
 *
 * Implements Comparable in order to easily sort the Prototypes. Like requested by the Dex
 * documentation, two prototypes are compared by their return types, and then their arguments.
 *
 * @author Julien Névo.
 */
public class Prototype implements Comparable<Prototype> {

    /**
     * Full descriptor of the Prototype, in the "TypeDescriptor" format.
     */
    final private String descriptor;

    /**
     * String representing the whole prototype (return type and parameters) in the "ShortyDescriptor"
     * format.
     */
    private String shortyDescriptor;

    /**
     * String representing the return type, in the "TypeDescriptor" format.
     */
    private String returnType;

    /**
     * Strings of the parameter types, in the "TypeDescriptor" format.
     */
    private TypeList parameterTypes;

    /**
     * The hashcode of the Prototype, calculated in the Constructor.
     */
    final private int hashcode;

    /**
     * Constructor of a Prototype. Builds the structure from a descriptor in the TypeDescriptor
     * format.
     * @param descriptor full descriptor in the TypeDescriptor format.
     */
    public Prototype(String descriptor) {
        this.descriptor = descriptor;
        hashcode = calculateHashCode(descriptor);
    }

    /**
     * Fix internal structure if this is the unique witness kept in the constantpool
     */
    public void initialize() {
        shortyDescriptor = getShortyDescriptorFromTypeDescriptor(descriptor);
        returnType = getFirstDescriptor(descriptor);
        parameterTypes = getDescriptors(descriptor, true);
    }

    /**
     * Calculates the hashcode of the Prototype from its descriptor in the TypeDescriptor format.
     * @return the hashcode of the Prototype.
     */
    public static int calculateHashCode(String descriptor) {
        return descriptor.hashCode();
    }


    // -----------------------------------------
    // Getters.
    // -----------------------------------------

    /**
     * Returns a String representing the whole prototype (return type and parameters) in the
     * "TypeDescriptor" format.
     * @return a String representing the whole prototype (return type and parameters) in the
     * 		   "TypeDescriptor" format.
     */
    public String getDescriptor() {
        return descriptor;
    }

    /**
     * Returns a String representing the whole prototype (return type and parameters) in the
     * "ShortyDescriptor" format.
     * @return a String representing the whole prototype (return type and parameters) in the
     * 		   "ShortyDescriptor" format.
     */
    public String getShortyDescriptor() {
        return shortyDescriptor;
    }

    /**
     * Returns a String representing the return type, in the "TypeDescriptor" format.
     * @return a String representing the return type, in the "TypeDescriptor" format.
     */
    public String getReturnType() {
        return returnType;
    }

    /**
     * Returns a TypeList of the parameter types, in the "TypeDescriptor" format. It may be an empty
     * array.
     * @return a TypeList of the parameter types, in the "TypeDescriptor" format.
     */
    public TypeList getParameterTypes() {
        return parameterTypes;
    }

    /**
     * Returns the number of parameters this Prototype contains (doesn't count the return type). May be 0.
     * @return the number of parameters this Prototype contains.
     */
    public int getNbParameters() {
        return parameterTypes.size();
    }


    // -----------------------------------------
    // Static utility methods
    // -----------------------------------------

    /**
     * Returns the first Type found in the descriptor given. Returns Null if the String is empty, Null or
     * invalid.
     * @param descriptor descriptor.
     * @return the first Type found in the descriptor given, or Null if the String is empty, Null or
     * 		   invalid.
     */
    public static String getFirstDescriptor(String descriptor) {
        StringBuilder result = new StringBuilder(10);
        if ((descriptor != null) && (descriptor.length() > 0)) {
            char c = descriptor.charAt(0);
            switch (c) {
            case 'V':
            case 'Z':
            case 'B':
            case 'S':
            case 'C':
            case 'I':
            case 'J':
            case 'F':
            case 'D':
                result.append(c);
                break;
            case 'L':
                int i = descriptor.indexOf(';');
                if (i > 0) {
                    result.append(descriptor.substring(0, i + 1));
                }
                break;
            case  '[':
                result.append('[');
                result.append(getFirstDescriptor(descriptor.substring(1)));
            }
        }

        return result.toString();
    }

    /**
     * Returns the size in bytes of a given type, or 0 for a Void or unknown type.
     * @param type type.
     * @return the size in bytes of a given type, or 0 for a Void or unknown type.
     */
    public static int getSizeOfType(String type) {
        int result = 0;
        switch (type.charAt(0)) {
        case 'Z':
        case 'B':
        case 'S':
        case 'C':
        case 'I':
        case 'F':
        case 'L':
        case '[':
            result = 2;
            break;
        case 'J':
        case 'D':
            result = 4;
            break;
        }

        return result;
    }

    /**
     * Returns the sum in bytes of the size of every type of the given descriptor, skipping or not
     * the return type (the first information of the descriptor).
     * @param descriptor the descriptor.
     * @param skipReturnType true to skip the first type found.
     * @return the sum in bytes of the size of every type of the given descriptor.
     */
    public static int getSizeOfDescriptor(String descriptor, boolean skipReturnType) {
        int result = 0;
        String foundDescriptor;
        while ((descriptor != null) && (descriptor.length() > 0)) {
            foundDescriptor = getFirstDescriptor(descriptor);
            if (foundDescriptor == null) {
                descriptor = null;
            } else {
                int foundResult = getSizeOfType(foundDescriptor);
                if (skipReturnType) {
                    skipReturnType = false;
                } else {
                    result += foundResult;
                }

                descriptor = descriptor.substring(foundDescriptor.length());
            }
        }
        return result;
    }

    /**
     * Returns a ShortyDescriptor from a given TypeDescriptor.
     * @param desc the descriptor.
     * @return a ShortyDescriptor.
     */
    public static String getShortyDescriptorFromTypeDescriptor(String desc) {
        StringBuilder result = new StringBuilder(5);
        while (!desc.equals("")) {
            String foundDesc = getFirstDescriptor(desc);
            desc = desc.substring(foundDesc.length());
            // Any reference type would be "Lxxx;" or "[xx", which means the returned String would
            // have a length > 1.
            if (foundDesc.length() > 1) {
                result.append('L');
            } else {
                result.append(foundDesc);
            }

        }
        return result.toString();
    }

    /**
     * Returns the number of parameters (return parameter not included) in the given descriptor.
     * @param desc the descriptor.
     * @return the number of parameters (return parameter not included).
     */
    public static int getNbParametersFromTypeDescriptor(String desc) {
        return getShortyDescriptorFromTypeDescriptor(desc).length() - 1;
    }

    /**
     * Returns a TypeList containing descriptors in the TypeDescriptor format from a descriptor also in the
     * TypeDescriptor format, or an empty array if nothing was found.
     * @param descriptor descriptors in the TypeDescriptor format.
     * @param skipFirst true to skip the first descriptor, usually the return type.
     * @return a TypeList of descriptors in the TypeDescriptor format.
     */
    public static TypeList getDescriptors(String descriptor, boolean skipFirst) {
        ArrayList<String> types = new ArrayList<String>();

        String foundDesc;
        while (!descriptor.equals("")) {
            foundDesc = getFirstDescriptor(descriptor);
            descriptor = descriptor.substring(foundDesc.length());
            if (skipFirst) {
                skipFirst = false;
            } else {
                types.add(foundDesc);
            }
        }

        return new TypeList(types.toArray(new String[types.size()]));
    }

    // -----------------------------------------
    // Overridden methods
    // -----------------------------------------

    @Override
    public int compareTo(Prototype prototype) {
        if (this == prototype) {
            return 0;
        }

        // Tests Return type first, as Prototype are sorted by Return type.
        int compare = returnType.compareTo(prototype.returnType);
        if (compare != 0) {
            return compare;
        }

        // Tests the arguments.
        return parameterTypes.compareTo(prototype.parameterTypes);
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o instanceof Prototype) {
            Prototype secondPrototype = (Prototype)o;
            return descriptor.equals(secondPrototype.descriptor);
        }

        return false;
    }

    @Override
    public int hashCode() {
        return hashcode;
    }
}
