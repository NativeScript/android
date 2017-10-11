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
import java.util.PriorityQueue;

import org.ow2.asmdex.Constants;
import org.ow2.asmdex.Opcodes;

/**
 * An Annotation Item (from the annotation_item structure).
 *
 * Consists in Encoded Values (annotation_elements actually), sorted by string_id, as well as
 * the visibility and the Type of the Annotation.
 *
 * An annotation_item is used to represent the various fields of an Annotation :
 * {@literal @}MyAnnotation(a=5, b=true)
 * myItem.
 *
 * Equals and HashCode are overridden so that they can be rightly stored into a HashSet.
 * Also overrides CompareTo in order to be sorted by type_id, as requested by annotation_set_item.
 *
 * CompareTo uses cached lists, as the PriorityQueue doesn't guarantee a traversal order, we have to sort them
 * and build an array, which is only build when and if necessary.
 *
 * @author Julien Névo
 *
 */
public class AnnotationItem implements Comparable<AnnotationItem> {

    /**
     * Visibility of the Annotation Item (see {@link Opcodes#VISIBILITY_BUILD},
     * {@link Opcodes#VISIBILITY_RUNTIME}, {@link Opcodes#VISIBILITY_SYSTEM}).
     */
    final private int visibility; // from annotation_item.

    /**
     * Type of the Annotation.
     */
    final private String annotationType; // from encoded_annotation.

    /**
     * Set of the Annotation Elements, sorted by string_id.
     */
    private PriorityQueue<AnnotationElement> annotationElements = new PriorityQueue<AnnotationElement>();

    /**
     * Indicates if the list of elements has been modified.
     */
    private boolean isListDirty = true;

    /**
     * Array of the Annotation Elements. This is useful for compareTo. This list is private and
     * is build when compareTo needs it, and only if it has changed to prevent overhead.
     */
    private AnnotationElement[] annotationElementsArray;

    /**
     * The hashcode is cached.
     */
    private int hashcode = Constants.HASHCODE_NOT_CALCULATED;

    /**
     * @param visibility visibility of the Annotation Item (see {@link Opcodes#VISIBILITY_BUILD},
     * {@link Opcodes#VISIBILITY_RUNTIME}, {@link Opcodes#VISIBILITY_SYSTEM}).
     * @param annotationType type of the Annotation.
     */
    public AnnotationItem(int visibility, String annotationType) {
        this.visibility = visibility;
        this.annotationType = annotationType;
    }

    /**
     * @param visible indicates whether the Annotation Item is visible at Run Time.
     * @param annotationType type of the Annotation.
     */
    public AnnotationItem(boolean visible, String annotationType) {
        this(visible ? Opcodes.VISIBILITY_RUNTIME : Opcodes.VISIBILITY_BUILD, annotationType);
    }

    // -----------------------------------------------------
    // Public methods.
    // -----------------------------------------------------

    /**
     * Adds an Annotation Element to the sorted list.
     * @param annotationElement the Annotation Element to add.
     */
    public void addAnnotationElement(AnnotationElement annotationElement) {
        annotationElements.add(annotationElement);
        hashcode = Constants.HASHCODE_NOT_CALCULATED; // Resets the hashcode.
        isListDirty = true;
    }

    // -----------------------------------------------------
    // Getters and Setters.
    // -----------------------------------------------------

    /**
     * Returns the item visibility (see {@link Opcodes#VISIBILITY_BUILD},
     * {@link Opcodes#VISIBILITY_RUNTIME}, {@link Opcodes#VISIBILITY_SYSTEM}).
     * @return the item visibility.
     */
    public int getVisibility() {
        return visibility;
    }

    /**
     * Returns the Annotation Type.
     * @return the Annotation Type.
     */
    public String getAnnotationType() {
        return annotationType;
    }

    /**
     * Returns the Annotation Element, sorted by string_id.
     * @return the Annotation Element, sorted by string_id.
     */
    public PriorityQueue<AnnotationElement> getAnnotationElements() {
        return annotationElements;
    }

    /**
     * Returns the number of Annotation elements currently hold by the Item.
     * @return the number of Annotation elements currently hold by the Item.
     */
    public int getNbAnnotationElements() {
        return annotationElements.size();
    }


    // ----------------------------------------------
    // Private methods.
    // ----------------------------------------------

    /**
     * Returns an array of Annotation Elements. It may be build if the list has changed.
     * @return an array of Annotation Elements.
     */
    private AnnotationElement[] getAnnotationElementsArray() {
        if (!isListDirty) {
            return annotationElementsArray;
        } else {
            Object[] array = annotationElements.toArray();
            Arrays.sort(array);
            int size = array.length;
            annotationElementsArray = new AnnotationElement[size];
            for (int i = 0; i < size; i++) {
                annotationElementsArray[i] = (AnnotationElement)array[i];
            }
            return annotationElementsArray;
        }
    }


    // ----------------------------------------------
    // Overridden methods.
    // ----------------------------------------------

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        boolean result = false;

        if (o instanceof AnnotationItem) {
            AnnotationItem annotationItem = (AnnotationItem)o;
            result = ((visibility == annotationItem.visibility) && (annotationType.equals(annotationItem.annotationType)));
            if (result) {
                int size = annotationElements.size();
                if (size == annotationItem.annotationElements.size()) {
                    for (AnnotationElement element : annotationElements) {
                        result = annotationItem.annotationElements.contains(element);
                        if (!result) {
                            break;
                        }
                    }

                }
            }
        }

        return result;
    }

    @Override
    public int hashCode() {
        if (hashcode != Constants.HASHCODE_NOT_CALCULATED) {
            return hashcode;
        }

        int res = visibility + annotationType.hashCode();
        for (AnnotationElement element : annotationElements) {
            res += element.hashCode();
        }

        hashcode = res;
        return res;
    }

    @Override
    public int compareTo(AnnotationItem ai) {
        if (this == ai) {
            return 0;
        }

        // Tests the type.
        int result = annotationType.compareTo(ai.annotationType);
        if (result == 0) {
            // Tests each element. The lists have been cached.
            AnnotationElement[] ae1 = getAnnotationElementsArray();
            AnnotationElement[] ae2 = ai.getAnnotationElementsArray();
            int size1 = ae1.length;
            int size2 = ae2.length;

            int i = 0;
            int size = (size1 < size2 ? size1 : size2);
            while ((result == 0) && (i < size)) {
                result = ae1[i].compareTo(ae2[i]);
                i++;
            }

            if (result == 0) {
                result = size1 == size2 ? 0 : (size1 < size2 ? -1 : 1);
            }
        }

        return result;
    }


}
