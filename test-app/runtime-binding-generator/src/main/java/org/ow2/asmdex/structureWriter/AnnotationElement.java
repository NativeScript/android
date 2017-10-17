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

import org.ow2.asmdex.encodedValue.EncodedValue;
import org.ow2.asmdex.lowLevelUtils.ByteVector;

/**
 * An Annotation Element (from annotation_element structure).
 *
 * Simply consists of the name of the element and its value.
 *
 * CompareTo has been overridden in order to sort these Items by the string_id, like requested by the documentation.
 *
 * @author Julien Névo.
 */
public class AnnotationElement implements Comparable<AnnotationElement> {

    /**
     * Name of the Element.
     */
    final private String elementName;

    /**
     * Encoded Value of the Element.
     */
    final private EncodedValue encodedValue;

    /**
     * Constructor of an AnnotationElement.
     * @param elementName name of the Element.
     * @param encodedValue value of the Element.
     */
    public AnnotationElement(String elementName, EncodedValue encodedValue) {
        this.elementName = elementName;
        this.encodedValue = encodedValue;
    }


    // -----------------------------------------------------
    // Public Methods.
    // -----------------------------------------------------

    /**
     * Encode the annotation_element according to the format of the same name.
     */
    public byte[] encode(ConstantPool constantPool) {
        ByteVector out = new ByteVector();

        out.putUleb128(constantPool.getStringIndex(elementName)); // name_idx.
        out.putByteArray(encodedValue.encode(constantPool)); // encoded_value.

        return out.getData();
    }


    // -----------------------------------------------------
    // Getters and Setters.
    // -----------------------------------------------------

    /**
     * Gets the Element name.
     * @return the Element name.
     */
    public String getElementName() {
        return elementName;
    }

    /**
     * Gets the Value of the Element.
     * @return the Value of the Element.
     */
    public EncodedValue getEncodedValue() {
        return encodedValue;
    }


    // ----------------------------------------------
    // Overridden methods.
    // ----------------------------------------------

    @Override
    public int hashCode() {
        // Caching the hashcode is useless here (not enough calls).
        return (elementName != null ? elementName.hashCode() : 0) + (encodedValue != null ? encodedValue.hashCode() : 0);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }

        if (obj instanceof AnnotationElement) {
            AnnotationElement ae = (AnnotationElement)obj;
            return (elementName.equals(ae.elementName) && (encodedValue.equals(ae.encodedValue)));
        }

        return false;
    }

    @Override
    public int compareTo(AnnotationElement ae) {
        if (this == ae) {
            return 0;
        }

        int result = elementName.compareTo(ae.elementName);
        if (result == 0) {
            result = encodedValue.compareTo(ae.encodedValue);
        }

        return result;
    }

    @Override
    public String toString() {
        return elementName + ": " + encodedValue;
    }
}
