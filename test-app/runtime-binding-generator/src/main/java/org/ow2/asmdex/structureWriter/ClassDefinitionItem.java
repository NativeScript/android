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

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.encodedValue.EncodedValue;
import org.ow2.asmdex.encodedValue.EncodedValueArray;
import org.ow2.asmdex.encodedValue.EncodedValueFactory;

/**
 * Simple class representing one class_def_item, for the writer.
 *
 * The equals and hashCode methods have been overridden in order to detect easily
 * duplicates in Sets. ONLY className is used to differentiate the classes.
 *
 * Implements Comparable in order to easily sort the Classes. The test is minimalist is is only useful as
 * a first-pass sort. A second pass, after the Dex file is parsed, will be done to ensure SuperClass are
 * encoded before their children.
 *
 * It is IMPORTANT to note that the encoded Fields/Methods are NOT sorted according to their ids, but in
 * the order of appearance of the fields/methods.
 *
 * @author Julien Névo
 */
public class ClassDefinitionItem implements Comparable<ClassDefinitionItem>, IAnnotationsHolder {

    /**
     * Name of the Class.
     */
    final private String className;

    /**
     * Access flags of the Class.
     */
    final private int accessFlags;

    /**
     * Name of the super class if any, or Null.
     */
    final private String superClassName;

    /**
     * TypeList containing the names of the interface if any, or Null.
     */
    final private TypeList interfaceNames;

    /**
     * The Constant Pool of the Application.
     */
    final private ConstantPool constantPool;

    /**
     * The source file that defines the Class.
     */
    private String sourceFileName;

    /**
     * The Signature of this Class, or Null if there is none.
     */
    final private String[] signature;

    /**
     * The hashcode of the Class.
     */
    final private int hashCode;

    /**
     * Annotation_set_item, representing all the annotation_items for this Class.
     * It concerns Class Annotations only, not Field, Method or Parameters Annotations.
     */
    private AnnotationSetItem annotationSetItem = new AnnotationSetItem();

    // Fields of class_data_item.
    private ArrayList<Field> staticFields = new ArrayList<Field>();
    private ArrayList<Field> instanceFields = new ArrayList<Field>();

    // Methods of class_data_item.
    private ArrayList<Method> directMethods = new ArrayList<Method>();
    private ArrayList<Method> virtualMethods = new ArrayList<Method>();

    /**
     * This may, if needed, contain an EncodedArray which will contain Encoded Values to Member Classes. It will be
     * filled through the visitInnerClass method in ClassWriter. All this will allow the encoding of the
     * dalvik.annotation.MemberClasses Annotation.
     */
    private EncodedValueArray memberClassArray;

    /**
     * The annotation_directory_item of the Class. It is only filled after the Class has been fully
     * parsed, or stays to Null if no Annotation is present in the Class, or the Methods, Fields,
     * Parameters it possesses.
     */
    private AnnotationDirectoryItem annotationDirectoryItem;

    /**
     * List of AnnotationItems which is <i>ONLY</i> used for Default Annotations. We may encounter
     * several Default Annotation while parsing the <i>Methods</i>, but they must be encoded in the Class,
     * and as <i>one</i> AnnotationElement which contains one Encoded Value of VALUE_ANNOTATION type,
     * which contains all the AnnotationElement.
     * The list is Null if not used.
     */
    private List<AnnotationItem> annotationItemsForDefaultAnnotation;

    /**
     * Offset in bytes of the access_flags field in the class_def_item structure.
     */
    final public static int ACCESS_FLAGS_OFFSET = 4;

    /**
     * Offset in bytes of the superclass_idx field in the class_def_item structure.
     */
    final public static int SUPERCLASS_IDX_OFFSET = 4 * 2;

    /**
     * Offset in bytes of the interfaces_off field in the class_def_item structure.
     */
    final public static int INTERFACES_OFFSET = 4 * 3;

    /**
     * Offset in bytes of the source_file_idx field in the class_def_item structure.
     */
    final public static int SOURCE_FILE_IDX_OFFSET = 4 * 4;

    /**
     * Offset in bytes of the annotations_off field in the class_def_item structure.
     */
    final public static int ANNOTATIONS_OFF_OFFSET = 4 * 5;

    /**
     * Offset in bytes of the class_data_off field in the class_def_item structure.
     */
    final public static int CLASS_DATA_OFF_OFFSET = 4 * 6;

    /**
     * Constructor of a ClassDefinitionItem.
     * @param className the name of the Class, fully qualified.
     * @param superClassName the possible name of the super Class, or Null.
     * @param accessFlags the access flags.
     * @param interfaceNames names of the interface, or Null.
     * @param signature the Signature of this Class, or Null if there is none.
     * @param constantPool the Constant Pool of this Application.
     */
    public ClassDefinitionItem(String className, String superClassName, int accessFlags,
                               TypeList interfaceNames, String[] signature, ConstantPool constantPool) {
        this.className = className;
        this.accessFlags = accessFlags;
        this.superClassName = superClassName;
        this.interfaceNames = interfaceNames;
        this.signature = signature;
        this.constantPool = constantPool;

        hashCode = className.hashCode();
    }


    // ----------------------------------------------
    // Getters and Setters.
    // ----------------------------------------------

    /**
     * Returns the name of the Class.
     * @return the name of the Class.
     */
    public String getClassName() {
        return className;
    }

    /**
     * Returns the Super Class of this Class. May be Null is the Class has no Super Class.
     * @return the Super Class of this Class, or Null.
     */
    public String getSuperClassName() {
        return superClassName;
    }

    /**
     * Returns the Interfaces used by the Class. The list it contains may be empty.
     * @return the Interfaces used by the Class.
     */
    public TypeList getInterfaces() {
        return interfaceNames;
    }

    /**
     * Returns the access flags of the Class.
     * @return the access flags of the Class.
     */
    public int getAccessFlags() {
        return accessFlags;
    }

    /**
     * Returns the source file that defines the Class. May be Null.
     * @return the source file that defines the Class. May be Null.
     */
    public String getSourceFileName() {
        return sourceFileName;
    }

    /**
     * Sets the source file that defines the Class. May be Null.
     *
     */
    public void setSourceFileName(String sourceFileName) {
        this.sourceFileName = sourceFileName;
    }

    /**
     * Returns the Signature of the Class. May be Null.
     * @return the Signature of the Class. May be Null.
     */
    public String[] getSignature() {
        return signature;
    }

    /**
     * Returns the direct methods this class contains.
     * @return the direct methods this class contains.
     */
    public ArrayList<Method> getDirectMethods() {
        return directMethods;
    }

    /**
     * Returns the virtual methods this class contains.
     * @return the virtual methods this class contains.
     */
    public ArrayList<Method> getVirtualMethods() {
        return virtualMethods;
    }

    /**
     * Returns the static fields of this class.
     * @return the static fields of this class.
     */
    public ArrayList<Field> getStaticFields() {
        return staticFields;
    }

    /**
     * Returns the instance fields of this class.
     * @return the instance fields of this class.
     */
    public ArrayList<Field> getInstanceFields() {
        return instanceFields;
    }

    /**
     * Returns the number of direct methods this class currently contains.
     * @return the number of direct methods this class currently contains.
     */
    public int getNbDirectMethods() {
        return directMethods.size();
    }

    /**
     * Returns the number of virtual methods this class currently contains.
     * @return the number of virtual methods this class currently contains.
     */
    public int getNbVirtualMethods() {
        return virtualMethods.size();
    }

    /**
     * Returns the number of static fields this class currently contains.
     * @return the number of static fields this class currently contains.
     */
    public int getNbStaticFields() {
        return staticFields.size();
    }

    /**
     * Returns the number of instance fields this class currently contains.
     * @return the number of instance fields this class currently contains.
     */
    public int getNbInstanceFields() {
        return instanceFields.size();
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

    /**
     * Returns the count of annotated Methods, whether they are direct or virtual. This number is only set when
     * parsing the Class is fully done.
     * @return the count of annotated Methods.
     */
    public int getNbAnnotatedMethods() {
        return annotationDirectoryItem == null ? 0 : annotationDirectoryItem.getNbAnnotatedMethods();
    }

    /**
     * Returns the count of Methods that have annotated Parameters. This number is only set when
     * parsing the Class is fully done.
     * @return the count of Methods that have annotated Parameters.
     */
    public int getNbMethodWithAnnotatedParameters() {
        return annotationDirectoryItem == null ? 0 : annotationDirectoryItem.getNbAnnotatedParameters();
    }

    /**
     * Returns the count of annotated Fields, whether they are static or instance Fields.
     * This number is only set when parsing the Class is fully done.
     * @return the count of annotated Fields.
     */
    public int getNbAnnotatedFields() {
        return annotationDirectoryItem == null ? 0 : annotationDirectoryItem.getNbAnnotatedFields();
    }

    /**
     * Returns the Encoded Array containing the Member Classes, or null if none were added.
     * @return the Encoded Array containing the Member Classes, or null if none were added.
     */
    public EncodedValueArray getMemberClassArray() {
        return memberClassArray;
    }

    /**
     * Returns the annotation_directory_item, filled after the whole Class has been parsed.
     * It may be Null if no Annotation is present in the Class, or the Methods, Fields,
     * Parameters it possesses.
     * @return the annotation_directory_item, or Null.
     */
    public AnnotationDirectoryItem getAnnotationDirectoryItem() {
        return annotationDirectoryItem;
    }

    /**
     * Return the list of AnnotationItems containing each Default Annotation for this Class. It may be Null
     * if none were found.
     * @return the list of AnnotationItems, or Null.
     */
    public List<AnnotationItem> getAnnotationItemsForDefaultAnnotation() {
        return annotationItemsForDefaultAnnotation;
    }


    // ----------------------------------------------
    // Public methods.
    // ----------------------------------------------

    /**
     * Indicates if the Class is an Interface.
     * @return true if the Class is an Interface.
     */
    public boolean isInterface() {
        return (accessFlags & Opcodes.ACC_INTERFACE) != 0;
    }

    /**
     * Adds a Method (direct or virtual) to the method list of the Class.
     * @param method method to add.
     */
    public void addMethod(Method method) {
        if (method.isVirtual()) {
            virtualMethods.add(method);
        } else {
            directMethods.add(method);
        }
    }

    /**
     * Adds a Field to the Field list of the Class, whether is it a Static or Instance Field.
     * @param field the field to add.
     */
    public void addField(Field field) {
        if (field.isStatic()) {
            staticFields.add(field);
        } else {
            instanceFields.add(field);
        }
    }
    /**
     * Adds an annotation_item to the annotations_set_items.
     * @param annotationItem the Annotation Item to add.
     */
    public void addAnnotationItem(AnnotationItem annotationItem) {
        annotationSetItem.addAnnotationItem(annotationItem);
    }

    /**
     * Adds a Member Class Encoded Value to the Member Classes. They will be encoded as a
     * dalvik.annotation.MemberClasses Annotation.
     * @param memberClassType the value to add.
     * @param constantPool the Constant Pool of the Application.
     */
    public void addMemberClassValue(String memberClassType, ConstantPool constantPool) {
        if (memberClassArray == null) {
            memberClassArray = new EncodedValueArray();
        }

        constantPool.addTypeToConstantPool(memberClassType);

        // Creates an IElementValue and adds it to the Array.
        EncodedValue encodedValue = EncodedValueFactory.getEncodedValue(memberClassType, Opcodes.VALUE_TYPE);
        memberClassArray.addEncodedValue(encodedValue);
    }

    /**
     * Adds an AnnotationItem to the list of AnnotationItem of Default Annotation. It must only be
     * used in that case.
     * @param annotationItem the AnnotationItem to add.
     */
    public void addAnnotationItemForDefaultAnnotation(AnnotationItem annotationItem) {
        if (annotationItemsForDefaultAnnotation == null) {
            annotationItemsForDefaultAnnotation = new ArrayList<AnnotationItem>(1);
        }
        annotationItemsForDefaultAnnotation.add(annotationItem);
    }

    /**
     * Builds the annotation_directory_item. This must be done only after the Class has been fully parsed.
     * The structure is kept only if it possesses at least one annotation.
     */
    public void buildAnnotationDirectoryItem() {
        AnnotationDirectoryItem adi = new AnnotationDirectoryItem();
        adi.setClassAnnotationSetItem(annotationSetItem);

        boolean foundAnnotation = (annotationSetItem.getNbAnnotationItems() > 0);

        foundAnnotation |= buildAnnotationDirectoryItemForMethods(directMethods, adi);
        foundAnnotation |= buildAnnotationDirectoryItemForMethods(virtualMethods, adi);

        for (Field field : staticFields) {
            if (field.getNbAnnotations() > 0) {
                adi.addAnnotatedField(field);
                foundAnnotation = true;
            }
        }
        // Tiny redundant code, not very important.
        for (Field field : instanceFields) {
            if (field.getNbAnnotations() > 0) {
                adi.addAnnotatedField(field);
                foundAnnotation = true;
            }
        }

        if (foundAnnotation) {
            annotationDirectoryItem = adi;
            constantPool.addAnnotationDirectoryItem(annotationDirectoryItem);
        }
    }

    /**
     * Adds the Methods as annotated Methods if they have annotations. Should only been called
     * by buildAnnotationDirectoryItem.
     * @param methods a List of Methods, whether they are direct or virtual.
     * @param adi the AnnotationDirectoryItem to which to add the Methods.
     * @return true if at least an annotation was found.
     */
    private boolean buildAnnotationDirectoryItemForMethods(List<Method> methods, AnnotationDirectoryItem adi) {
        boolean foundAnnotation = false;
        for (Method method : methods) {
            if (method.getNbAnnotations() > 0) {
                adi.addAnnotatedMethods(method);
                foundAnnotation = true;
            }
            if (method.getNbParameterAnnotations() > 0) {
                adi.addAnnotatedParameter(method.getAnnotatedParameterSetRefList());
                foundAnnotation = true;
            }
        }

        return foundAnnotation;
    }

    // ----------------------------------------------
    // Overridden methods.
    // ----------------------------------------------

    @Override
    public int hashCode() {
        return hashCode;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }

        if (obj instanceof ClassDefinitionItem) {
            ClassDefinitionItem cdi = (ClassDefinitionItem)obj;
            return className.equals(cdi.className);
        }

        return false;
    }

    @Override
    public int compareTo(ClassDefinitionItem cdi) {
        if (this == cdi) {
            return 0;
        }

        // Tests the class name. This test is minimalist is is only useful as a first-pass sort.
        // A second pass, after the Dex file is parsed, will be done to ensure SuperClass are encoded
        // before their children.
        return className.compareTo(cdi.className);
    }






}
