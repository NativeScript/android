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
import java.util.List;

import org.ow2.asmdex.Constants;

/**
 * Represents an annotation_directory_item. There can be only one maximum per Class, but a Class
 * can have none, in case it, its methods, fields, parameters doesn't have annotations.
 *
 * Note that this structure is only built after all has been parsed, in order to build them easily, and
 * find duplicates quickly, which is actually its sole purpose, the Class having already all the elements
 * it contains.
 *
 * @author Julien Névo
 */
public class AnnotationDirectoryItem {

    /**
     * The annotation_set_item the annotation_directory_item is linked with.
     */
    private AnnotationSetItem classAnnotationSetItems;

    /**
     * The list of annotated Fields.
     */
    private List<Field> annotatedFields = new ArrayList<Field>();

    /**
     * The list of annotated Methods.
     */
    private List<Method> annotatedMethods = new ArrayList<Method>();

    /**
     * The list of annotated Parameters.
     */
    private List<AnnotationSetRefList> annotatedParameters = new ArrayList<AnnotationSetRefList>();

    /**
     * The hashcode is cached.
     */
    private int hashcode = Constants.HASHCODE_NOT_CALCULATED;

    /**
     * Offset in bytes of the class_annotations_off field in the annotations_directory_item structure.
     */
    final public static int CLASS_ANNOTATIONS_OFF_OFFSET = 0;

    /**
     * Offset in bytes of the fields_size field in the annotations_directory_item structure.
     */
    final public static int FIELDS_SIZE_OFFSET = 4 * 1;

    /**
     * Offset in bytes of the annotated_methods_size field in the annotations_directory_item structure.
     */
    final public static int ANNOTATED_METHODS_SIZE_OFFSET = 4 * 2;

    /**
     * Offset in bytes of the annotated_parameters_size field in the annotations_directory_item structure.
     */
    final public static int ANNOTATED_PARAMETERS_SIZE_OFFSET = 4 * 3;


    // -------------------------------------------
    // Getters and Setters.
    // -------------------------------------------

    /**
     * Sets the Class Annotation Set Item.
     */
    public void setClassAnnotationSetItem(AnnotationSetItem classAnnotations) {
        this.classAnnotationSetItems = classAnnotations;
    }

    /**
     * Returns the Class Annotation Set Item.
     * @return the Class Annotation Set Item.
     */
    public AnnotationSetItem getClassAnnotationSetItem() {
        return classAnnotationSetItems;
    }

    /**
     * Returns the Annotated Fields.
     * @return the Annotated Fields.
     */
    public List<Field> getAnnotatedFields() {
        return annotatedFields;
    }

    /**
     * Returns the Annotated Methods.
     * @return the Annotated Methods.
     */
    public List<Method> getAnnotatedMethods() {
        return annotatedMethods;
    }

    /**
     * Returns the Annotated Parameters.
     * @return the Annotated Parameters.
     */
    public List<AnnotationSetRefList> getAnnotatedParameters() {
        return annotatedParameters;
    }

    /**
     * Returns the number of Annotated Fields.
     * @return the number of Annotated Fields.
     */
    public int getNbAnnotatedFields() {
        return annotatedFields.size();
    }

    /**
     * Returns the number of Annotated Methods.
     * @return the number of Annotated Methods.
     */
    public int getNbAnnotatedMethods() {
        return annotatedMethods.size();
    }

    /**
     * Returns the number of Annotated Parameters.
     * @return the number of Annotated Parameters.
     */
    public int getNbAnnotatedParameters() {
        return annotatedParameters.size();
    }


    // -------------------------------------------
    // Public Methods.
    // -------------------------------------------

    /**
     * Adds an Annotated Field to the directory.
     */
    public void addAnnotatedField(Field field) {
        annotatedFields.add(field);
        hashcode = Constants.HASHCODE_NOT_CALCULATED; // Resets the hashcode.
    }

    /**
     * Adds an Annotated Method to the directory.
     */
    public void addAnnotatedMethods(Method method) {
        annotatedMethods.add(method);
        hashcode = Constants.HASHCODE_NOT_CALCULATED; // Resets the hashcode.
    }

    /**
     * Adds an Annotated Parameter to the directory.
     */
    public void addAnnotatedParameter(AnnotationSetRefList parameter) {
        annotatedParameters.add(parameter);
        hashcode = Constants.HASHCODE_NOT_CALCULATED; // Resets the hashcode.
    }


    // -------------------------------------------
    // Overriden Methods.
    // -------------------------------------------

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }

        boolean result = false;

        if (obj instanceof AnnotationDirectoryItem) {
            AnnotationDirectoryItem adi = (AnnotationDirectoryItem)obj;
            result = classAnnotationSetItems.equals(adi.classAnnotationSetItems)
                     && annotatedFields.equals(adi.annotatedFields)
                     && annotatedMethods.equals(adi.annotatedMethods)
                     && annotatedParameters.equals(adi.annotatedParameters);
        }

        return result;
    }

    @Override
    public int hashCode() {

        if (hashcode != Constants.HASHCODE_NOT_CALCULATED) {
            return hashcode;
        }

        int result = classAnnotationSetItems.hashCode();
        for (Field field : annotatedFields) {
            result += field.hashCode();
        }
        for (Method method : annotatedMethods) {
            result += method.hashCode();
        }
        for (AnnotationSetRefList parameter : annotatedParameters) {
            result += parameter.hashCode();
        }

        hashcode = result;
        return result;
    }
}
