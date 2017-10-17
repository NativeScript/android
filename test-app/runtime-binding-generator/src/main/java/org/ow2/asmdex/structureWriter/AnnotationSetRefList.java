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

/**
 * A Class representing an annotation_set_ref_list structure.
 *
 * It possesses the Annotation_set_item that EACH argument of a Method has. Dalvik handles them this way :
 * If AT LEAST one argument of a Method is annotated, then all the arguments are stored here.
 * The non-annotated arguments have an annotation_set_item that has a size of 0.
 *
 * Since all the arguments must be stored, the constructor requires the number of parameters of the Method
 * this annotation_set_ref_list belongs.
 *
 * When all the parameters have been filled (which can be done in several passes, if several annotations), the
 * annotation_set_ref_list HAS to be "closed" (through the close() method) so that empty parameters can be
 * filled with empty annotation_set_items.
 *
 * @author Julien Névo
 *
 */
public class AnnotationSetRefList {

    /**
     * Array of annotation_set_item, each item for one parameter.
     */
    private AnnotationSetItem[] annotationSetItems;

    /**
     * Number of parameters of the Methods it belongs (return type not included).
     */
    final private int nbParameters;

    /**
     * Indicates how many annotation_set_items are actually used (the rest being empty).
     */
    private int nbAnnotationSetItemsUsed = 0;

    /**
     * The Method this structure belongs to.
     */
    final private Method method;

    /**
     * Constructor of an annotation_set_ref_list.
     * @param nbParameters number of parameters of the Methods it belongs to (return type not included).
     * @param method the Method this annotation_set_ref_list belongs to.
     */
    public AnnotationSetRefList(int nbParameters, Method method) {
        this.nbParameters = nbParameters;
        this.method = method;
        annotationSetItems = new AnnotationSetItem[nbParameters];
    }

    /**
     * An empty annotation_set_item, used to fill unused parameters.
     */
    private static AnnotationSetItem emptyAnnotationSetItem = null;

    // ----------------------------------------------
    // Public methods.
    // ----------------------------------------------

    /**
     * Adds an annotation_item to the given parameter.
     * @param parameterIndex the zero-based index of the parameter.
     * @param annotationItem the annotation_item to add.
     */
    public void addAnnotationItem(int parameterIndex, AnnotationItem annotationItem) {

        if (parameterIndex >= nbParameters) {
            try {
                throw new Exception("Annotation Parameter index >= Parameter count of this method.");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        AnnotationSetItem annotationSetItem;
        // Does the annotation_set_item for this parameter exists ? If not, we must create it.
        if (annotationSetItems[parameterIndex] == null) {
            annotationSetItem = new AnnotationSetItem();
            nbAnnotationSetItemsUsed++;
        } else {
            annotationSetItem = annotationSetItems[parameterIndex];
        }

        annotationSetItem.addAnnotationItem(annotationItem);
        annotationSetItems[parameterIndex] = annotationSetItem;
    }

    /**
     * Fill the empty parameters with an empty annotation_set_item. This has to be done once before
     * encoding the Dex file.
     */
    public void close() {
        for (int i = 0, size = annotationSetItems.length; i < size; i++) {
            if (annotationSetItems[i] == null) {
                annotationSetItems[i] = getEmptyAnnotationSetItem();
            }
        }
    }

    // ----------------------------------------------
    // Private Methods.
    // ----------------------------------------------

    /**
     * Provides an empty annotation_set_item.
     */
    private static AnnotationSetItem getEmptyAnnotationSetItem() {
        if (emptyAnnotationSetItem == null) {
            emptyAnnotationSetItem = new AnnotationSetItem();
        }
        return emptyAnnotationSetItem;
    }

    // ----------------------------------------------
    // Getters.
    // ----------------------------------------------

    /**
     * Returns an array containing, for each parameters, an annotation_set_item. All the parameters have an
     * annotation_set_items. However, it can have an empty size of the parameter is not annotated.
     * @return a Map containing, for each parameters, an annotation_set_item.
     */
    public AnnotationSetItem[] getAnnotationSetItems() {
        return annotationSetItems;
    }

    /**
     * Returns the annotation_set_item of the given parameter index.
     * @param parameterIndex the zero-based parameter index.
     * @return the annotation_set_item of the given parameter index.
     */
    public AnnotationSetItem getAnnotationSetItem(int parameterIndex) {
        return annotationSetItems[parameterIndex];
    }

    /**
     * Returns the number of annotation_set_items in the structure. Represents ALL the arguments, even
     * those who were not annotated.
     * @return the number of annotation_set_items in the structure.
     */
    public int getNbAnnotationSetItem() {
        return annotationSetItems.length;
    }

    /**
     * Returns the number of annotation_set_items that are not empty or null.
     * @return the number of annotation_set_items that are not empty or null.
     */
    public int getNbAnnotationSetItemsUsed() {
        return nbAnnotationSetItemsUsed;
    }

    /**
     * Returns the Method this structure belongs to.
     * @return the Method this structure belongs to.
     */
    public Method getMethod() {
        return method;
    }


    // ----------------------------------------------
    // Overridden methods.
    // ----------------------------------------------

    @Override
    public int hashCode() {
        // Caching the hashcode is useless (not enough calls).
        return java.util.Arrays.hashCode(annotationSetItems);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }

        if (obj instanceof AnnotationSetRefList) {
            AnnotationSetRefList asr = (AnnotationSetRefList)obj;
            // Patch for part of bug316398 :
            return method.equals(asr.method) && java.util.Arrays.equals(annotationSetItems, asr.annotationSetItems);
        }

        return false;
    }
}
