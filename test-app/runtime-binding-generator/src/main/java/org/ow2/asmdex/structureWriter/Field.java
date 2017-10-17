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

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.encodedValue.EncodedValue;
import org.ow2.asmdex.encodedValue.EncodedValueFactory;
import org.ow2.asmdex.encodedValue.EncodedValueString;
import org.ow2.asmdex.encodedValue.EncodedValueUtil;

/**
 * Class representing a Field. A Field, as described by the field_id_item format, consists in its name,
 * the name of the class owning it, and its type. However, contrary to the field_id_item, it also own
 * its access flags, and its value.
 *
 * The value is only used for Final Static fields, else it's Null.
 *
 * Having two fields equals does NOT mean their value is equal. Just that the name, owner and type are the
 * same.
 *
 * Implements Comparable in order to easily sort the Fields. Like requested by the Dex documentation,
 * two Fields are sorted according to the class owning type, the field name and then their own type.
 *
 * @author Julien Névo
 */
public class Field implements Comparable<Field>, IAnnotationsHolder {

    /**
     * Name of the Class, owner of the Field.
     */
    final private String className;

    /**
     * Type of the Field, described in the TypeDescriptor format.
     */
    final private String typeName;

    /**
     * Name of the Field.
     */
    final private String fieldName;

    /**
     * Access flags of the Field.
     */
    private int access;

    /**
     * The Signature of the Field. May be Null.
     */
    private String[] signature;

    /**
     * Value of the Field. Null for non-final static fields.
     */
    private EncodedValue value = null;

    /**
     * Annotation_set_item, representing all the annotation_items for this Field.
     */
    private AnnotationSetItem annotationSetItem = new AnnotationSetItem();

    /**
     * The hashcode of the Field, calculated in the Constructor.
     */
    final private int hashcode;

    /**
     * Constructor of the Field.
     * @param fieldName name of the Field.
     * @param desc type of the Field, described in the TypeDescriptor format.
     * @param classOwningName name of the Class, owner of the Field.
     */
    public Field(String fieldName, String desc, String classOwningName) {
        this.fieldName = fieldName;
        typeName = desc;
        className = classOwningName;
        hashcode = calculateHashCode(fieldName, className, typeName);
    }

    /**
     * Init if this is the one kept.
     * @param access the access flags of the Field.
     * @param signature the Signature of the field. May be Null.
     * @param value the value of the Field, or Null for non-static Fields.
     * @param constantPool the Constant Pool.
     */
    public void init( int access, String[] signature, Object value, ConstantPool constantPool) {
        this.access = access;
        this.signature = signature;
        setEncodedValueFromObject(value, typeName, constantPool);
    }


    // ----------------------------------------------
    // Private methods.
    // ----------------------------------------------

    /**
     * Sets the value field from the given Object.
     * @param value the Object.
     * @param desc the descriptor of the Field, described in the TypeDescriptor format. It is useful to know
     *        if the Object contains a reference of not.
     * @param constantPool the Constant Pool of the Application.
     */
    private void setEncodedValueFromObject(Object value, String desc, ConstantPool constantPool) {
        if ((access & (Opcodes.ACC_STATIC)) > 0) {
            if (value != null) {
                // Reference values are always encoded as Null, even if the value is given.
                // Exception for the Strings, which are encoded.
                boolean isRef = EncodedValueUtil.isTypeAReference(desc);
                if (isRef) {
                    if (EncodedValueUtil.isString(desc)) {
                        String str = (String)value;
                        this.value = new EncodedValueString(str);
                        constantPool.addStringToConstantPool(str);
                    } else {
                        setNoValue();
                    }
                } else {
                    this.value = EncodedValueFactory.getEncodedValue(value, desc);
                }
            }
        }
    }


    // ----------------------------------------------
    // Public methods.
    // ----------------------------------------------

    /**
     * Calculates the hashcode of a Field according to some of its attributes.
     * @param fieldName the name of the Field.
     * @param classOwningName the Class owning the Field.
     * @param desc type of the Field, described in the TypeDescriptor format.
     * @return the hashcode of a Field.
     */
    public static int calculateHashCode(String fieldName, String classOwningName, String desc) {
        return classOwningName.hashCode() + desc.hashCode() + fieldName.hashCode();
    }

    /**
     * Adds information to the current Field. This is useful <i>only</i> if this Field has the ACC_UNKNOWN
     * flag, which means Instructions referred to it while it has not yet been visited,
     * declaring it but not giving all the information it should have.
     */
    public void completeInformation(int access, String[] signature, Object value, ConstantPool constantPool) {
        this.access = access;
        this.signature = signature;
        setEncodedValueFromObject(value, typeName, constantPool);
    }

    /**
     * Indicates whether the Field is Static.
     */
    public boolean isStatic() {
        return (access & Opcodes.ACC_STATIC) > 0;
    }

    /**
     * Indicates whether the Field is Final and Static.
     */
    public boolean isFinalStatic() {
        return ((access & Opcodes.ACC_STATIC) > 0) && ((access & Opcodes.ACC_FINAL) > 0);
    }

    /**
     * Indicates whether the Field is unknown (referred to, but not yet parsed).
     * @return true if the Field is unknown.
     */
    public boolean isUnknown() {
        return (access & Opcodes.ACC_UNKNOWN) != 0;
    }

    /**
     * Adds an annotation_item to the annotations_set_items.
     * @param annotationItem the Annotation Item to add.
     */
    public void addAnnotationItem(AnnotationItem annotationItem) {
        annotationSetItem.addAnnotationItem(annotationItem);
    }

    // ----------------------------------------------
    // Getters.
    // ----------------------------------------------

    /**
     * Returns the Class owning the Field.
     * @return the Class owning the Field.
     */
    public String getClassName() {
        return className;
    }

    /**
     * Returns the Type of the Field, described in the TypeDescriptor format.
     * @return the Type of the Field, described in the TypeDescriptor format.
     */
    public String getTypeName() {
        return typeName;
    }

    /**
     * Returns the Name of the Field.
     * @return the Name of the Field.
     */
    public String getFieldName() {
        return fieldName;
    }

    /**
     * Returns the access flags of the Field.
     * @return the access flags of the Field.
     */
    public int getAccess() {
        return access;
    }

    /**
     * Returns the Signature of the Field. May be Null.
     * @return the Signature of the Field. May be Null.
     */
    public String[] getSignature() {
        return signature;
    }

    /**
     * Returns the encoded value of the field. Null for non-static fields.
     * @return the encoded value of the field, or Null.
     */
    public EncodedValue getValue() {
        return value;
    }

    /**
     * Encode the value into an array of Bytes.
     * @return an array of bytes.
     */
    public byte[] encodeValue(ConstantPool constantPool) {
        return (value == null) ? null : value.encode(constantPool);
    }

    /**
     * Sets the value to 0 or Null according to its type.
     */
    public void setNoValue() {
        value = EncodedValueFactory.getEncodedEmptyValue(typeName);
    }

    /**
     * Returns the annotation_set_item this structure currently contains.
     * @return the annotation_set_item this structure currently contains.
     */
    @Override
    public AnnotationSetItem getAnnotationSetItem() {
        return annotationSetItem;
    }

    /**
     * Returns the number of annotation_items this structure currently contains.
     * @return the number of annotation_items this structure currently contains.
     */
    @Override
    public int getNbAnnotations() {
        return annotationSetItem.getNbAnnotationItems();
    }


    // ----------------------------------------------
    // Overridden methods.
    // ----------------------------------------------

    @Override
    public int hashCode() {
        return hashcode;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }

        if (obj instanceof Field) {
            Field field = (Field)obj;
            return fieldName.equals(field.fieldName) &&
                   (className.equals(field.className) && (typeName.equals(field.typeName)));
        }

        return false;
    }

    @Override
    public int compareTo(Field field) {
        if (this == field) {
            return 0;
        }

        // Tests class owning name first.
        int compare = className.compareTo(field.className);
        if (compare != 0) {
            return compare;
        }

        // Tests the names.
        compare = fieldName.compareTo(field.fieldName);
        if (compare != 0) {
            return compare;
        }

        // Tests the type.
        return typeName.compareTo(field.typeName);
    }

}