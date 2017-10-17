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

import java.util.Iterator;
import java.util.TreeSet;

import org.ow2.asmdex.Constants;

/**
 * An Annotation_set_item. It is only useful when the Application is fully parsed, and is being written as a
 * Dex file.
 *
 * The purpose of this Class is to have a collection of annotation_item that is sorted by increasing
 * type_idx. The uniqueness of this Class is also important when encoding it.
 *
 * An annotation_set_item is used to assign one or several Annotations to an Item :
 * <code>
 * {@literal @}MyAnnotation(...)
 * {@literal @}MyAnnotation2(...)
 * </code>
 * myItem.
 *
 * The fact that the Annotations are composed of several fields (@ myAnnotation(int a, boolean b, float c...)
 * is handled by annotation_item, not annotation_set_item.
 *
 * @author Julien Névo
 *
 */
public class AnnotationSetItem implements Comparable<AnnotationSetItem> {

    /**
     * Set of annotation_items. They are all unique and sorted by type_idx.
     */
    private TreeSet<AnnotationItem> annotationItems = new TreeSet<AnnotationItem>();

    /**
     * The hashcode is cached.
     */
    private int hashcode = Constants.HASHCODE_NOT_CALCULATED;

    // -----------------------------------------------------
    // Public Methods.
    // -----------------------------------------------------

    /**
     * Adds an annotation_item to the annotation_set_item, ordered by type_idx.
     */
    public void addAnnotationItem(AnnotationItem annotationItem) {
        annotationItems.add(annotationItem);
        hashcode = Constants.HASHCODE_NOT_CALCULATED; // Resets the hashcode.
    }


    // -----------------------------------------------------
    // Getters and Setters.
    // -----------------------------------------------------

    /**
     * Returns the sorted Set of annotation_items.
     * @return annotaion items
     */
    public TreeSet<AnnotationItem> getAnnotationItems() {
        return annotationItems;
    }

    /**
     * Return the count of annotation_items present in this set.
     * @return positive or 0
     */
    public int getNbAnnotationItems() {
        return annotationItems.size();
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

        if (o instanceof AnnotationSetItem) {
            AnnotationSetItem secondAnnotationSetItem = (AnnotationSetItem)o;

            // If the size is the same, checks that all the elements of the first set are contained in the
            // second.
            if (annotationItems.size() == secondAnnotationSetItem.annotationItems.size()) {
                result = annotationItems.containsAll(secondAnnotationSetItem.annotationItems);
            }
        }
        return result;
    }

    @Override
    public int hashCode() {
        if (hashcode != Constants.HASHCODE_NOT_CALCULATED) {
            return hashcode;
        }

        int result = 0;
        for (AnnotationItem annotationItem : annotationItems) {
            result += annotationItem.hashCode();
        }
        hashcode = result;
        return result;
    }


    @Override
    public int compareTo(AnnotationSetItem asi) {
        if (this == asi) {
            return 0;
        }

        Iterator<AnnotationItem> it1 = annotationItems.iterator();
        Iterator<AnnotationItem> it2 = asi.annotationItems.iterator();
        int result = 0;
        while ((result == 0) && it1.hasNext() && it2.hasNext()) {
            result = it1.next().compareTo(it2.next());
        }

        if (result == 0) {
            if ((it1.hasNext()) && (!it2.hasNext())) {
                result = -1;
            } else if ((!it1.hasNext()) && (it2.hasNext())) {
                result = 1;
            }
        }

        return result;
    }

}
