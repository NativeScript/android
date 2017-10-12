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

import java.util.ArrayList;

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.structureWriter.AnnotationElement;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Class representing an Encoded Value containing an Annotation.
 *
 * Annotation Element can then be added to the Annotation.
 *
 * @author Julien Névo
 */
public class EncodedValueAnnotation extends EncodedValue {

    /**
     * The Type of the Encoded Value.
     */
    private int type = Opcodes.VALUE_ANNOTATION;

    /**
     * The elements of the Encoded Value.
     */
    private ArrayList<AnnotationElement> annotationElements = new ArrayList<AnnotationElement>();

    /**
     * Type of the Annotation.
     */
    private String annotationType;

    /**
     * Constructs the Encoded Annotation, empty for now. The Annotation Element must be entered
     * one by one.
     * @param annotationType the type of the Annotation.
     */
    public EncodedValueAnnotation(String annotationType) {
        this.annotationType = annotationType;
    }

    /**
     * Constructs the Encoded Annotation, empty for now. The Annotation Element must be entered
     * one by one.
     * @param annotationType the type of the Annotation.
     */
    public EncodedValueAnnotation(Object annotationType) {
        this.annotationType = (String)annotationType;
    }

    /**
     * Adds an Annotation Element to the Annotation.
     * @param annotationElement the Annotation Element to add.
     */
    public void addAnnotationElement(AnnotationElement annotationElement) {
        annotationElements.add(annotationElement);
    }

    @Override
    public byte[] encode(ConstantPool constantPool) {
        ByteVector bv = new ByteVector();

        // Encodes the value_format.
        bv.putByte(Opcodes.VALUE_ANNOTATION);
        // Encodes the encoded_annotation.
        bv.putUleb128(constantPool.getTypeIndex(annotationType));
        bv.putUleb128(annotationElements.size());

        // Encodes the annotation_elements.
        for (AnnotationElement annotationElement : annotationElements) {
            bv.putByteArray(annotationElement.encode(constantPool));
        }
        return bv.getData();
    }

    @Override
    public int getType() {
        return type;
    }

    /**
     * Returns the type of the Annotation.
     * @return the type of the Annotation.
     */
    public String getAnnotationType() {
        return annotationType;
    }

    /**
     * Returns the AnnotationElements this EncodedValue contains.
     * @return the AnnotationElements.
     */
    public ArrayList<AnnotationElement> getAnnotationElements() {
        return annotationElements;
    }

    // ---------------------------------------------
    // Overridden Methods.
    // ---------------------------------------------

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        boolean result = false;

        if (o instanceof EncodedValueAnnotation) {
            EncodedValueAnnotation encodedValue = (EncodedValueAnnotation)o;
            result = ((type == encodedValue.type) && (annotationType.equals(encodedValue.annotationType)));
            if (result) {
                result = annotationElements.equals(encodedValue.annotationElements);
            }
        }

        return result;
    }

    @Override
    public int hashCode() {
        int result = type + annotationType.hashCode();
        for (AnnotationElement annotationElement : annotationElements) {
            result += annotationElement.hashCode();
        }
        return result;
    }

    @Override
    int compareValue(EncodedValue encodedValue) {
        if (this == encodedValue) {
            return 0;
        }

        EncodedValueAnnotation ev = (EncodedValueAnnotation)encodedValue;

        int	result = annotationType.compareTo(ev.annotationType);
        if (result == 0) {
            int i = 0;
            int size1 = annotationElements.size();
            int size2 = ev.annotationElements.size();
            int size = size1 < size2 ? size1 : size2;
            while ((result == 0) && (i < size)) {
                result = annotationElements.get(i).compareTo(ev.annotationElements.get(i));
                i++;
            }

            if (result == 0) {
                result = (size1 == size2 ? 0 : (size1 < size2 ? -1 : 1));
            }
        }
        return result;
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder("@");
        result.append(annotationType);
        boolean isFirst = true;
        for (AnnotationElement ae: annotationElements) {
            if (isFirst) {
                result.append("(");
                isFirst = false;
            } else {
                result.append(",");
            }
            result.append(ae);
        }
        result.append(")");
        return result.toString();
    }
}
