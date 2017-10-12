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

package org.ow2.asmdex;

import org.ow2.asmdex.encodedValue.EncodedValue;
import org.ow2.asmdex.encodedValue.EncodedValueArray;
import org.ow2.asmdex.encodedValue.EncodedValueEnum;
import org.ow2.asmdex.encodedValue.EncodedValueFactory;
import org.ow2.asmdex.encodedValue.EncodedValueType;
import org.ow2.asmdex.structureWriter.AnnotationElement;
import org.ow2.asmdex.structureWriter.AnnotationItem;
import org.ow2.asmdex.structureWriter.ClassDefinitionItem;
import org.ow2.asmdex.structureWriter.ConstantPool;
import org.ow2.asmdex.structureWriter.Field;

/**
 * The Annotation Writer. Visits an Annotation and stores its content.
 * <br /><br />
 * A visitor can have children when it needs to visit an Array or sub-Annotations. It has no link with them,
 * but a child knows its father, and once the visit of all its elements are over, the child builds a single
 * element (array, sub-annotation) and adds it to its parent Annotation Item list.
 * <br /><br />
 * Once the father has visited everything, it registers its list to the Constant Pool (unless its a Default
 * Annotation, which will be registered by the Class, in another constructed AnnotationItem consisting all the
 * DefaultAnnotation of the Class).
 * <br /><br />
 * There are exceptions though : the Signature Array, String, Enum and Class array must NOT be sorted.
 * So in the case of a visit(), if we're encoding an Array, we don't add the Elements to the current
 * AnnotationItem like we do for the other cases, but add the Elements directly to the Array, which in turn
 * will be added to the AnnotationItem at visitEnd().
 *
 * @author Julien Névo
 */
public class AnnotationWriter extends AnnotationVisitor {

    /**
     * The Constant Pool of the Application.
     */
    private ConstantPool constantPool;

    /**
     * The Annotation Item currently being visited.
     */
    protected AnnotationItem annotationItem;

    /**
     * Current name of the Annotation. This is only used for Enumeration and Class Arrays.
     * According to ASM's behavior, Enumerations inside an array have a Null name, as the visitArray
     * in which they are declared already has one. This "current name" is only used if the
     * name of the Enumeration or Class Array name is Null.
     */
    protected String currentName;

    /**
     * Type of the Annotation. Only used for sub-annotations, given by the "father".
     */
    protected String annotationType;

    /**
     * "Father" of the current AnnotationWriter. This is useful to give him the result of the parsing of the
     * sub-level of the annotation (Array, sub-annotation...).
     */
    protected AnnotationWriter father;

    /**
     * This is an Array, only used for Signature Array, String Array, Enum and Class Arrays,
     * as these ones are unsorted.
     * So instead of adding elements to the Annotation Item like we do in any other cases, we fill this Array,
     * which will be added to the current Annotation Item at visitEnd().
     * It is only created when needed, that is if the method setMustSortArray is called, with a false value.
     */
    protected EncodedValueArray unsortedArray;

    /**
     * Indicates if the Array elements must be sorted. Always true, except for Signature array and String
     * array, Enum and Class Arrays. It must NOT be set directly ; use the setMustSortArray method.
     */
    protected boolean mustSortArray = true;

    /**
     * Constructor of the Annotation Writer.
     * @param constantPool the Constant Pool of the Application.
     * @param annotationItem the Annotation Item to visit.
     */
    public AnnotationWriter(ConstantPool constantPool, AnnotationItem annotationItem) {
        super(Opcodes.ASM4);
        this.constantPool = constantPool;
        this.annotationItem = annotationItem;
    }


    // -----------------------------------------------------
    // Getters and Setters.
    // ----------------------------------------------------

    /**
     * Returns the Annotation Item being visited.
     * @return the Annotation Item being visited.
     */
    public AnnotationItem getAnnotationItem() {
        return annotationItem;
    }

    /**
     * Indicates if the array must be sorted. False only for Signature array and String array.
     * @param mustSortArray true to sort the Array.
     */
    public void setMustSortArray(boolean mustSortArray) {
        this.mustSortArray = mustSortArray;
        if (!mustSortArray && (unsortedArray == null)) {
            unsortedArray = new EncodedValueArray();
        }
    }


    // -----------------------------------------------------
    // Visitor methods.
    // -----------------------------------------------------

    @Override
    public void visit(String name, Object value) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("        AnnotationWriter : visit. Name = " + name +
                               ", value = " + value);
        }

        visitManageNonDefaultAnnotation(name, value, false);
    }

    /**
     * Method called only by Visit, in the case our annotation is not a DefaultAnnotation.
     * @param name the value name.
     * @param value the actual value.
     * @param isArrayAnnotation indicates if the current Annotation is an Array Annotation.
     */
    protected void visitManageNonDefaultAnnotation(String name, Object value, boolean isArrayAnnotation) {
        // If the name is null, it means the visit is contained in a visitArray, in which case its name
        // is Null, and we must use the name of its "parent", which was given to us.
        if (name == null) {
            name = currentName;
        }

        // Registers the name to the Constant Pool.
        constantPool.addStringToConstantPool(name);

        EncodedValue encodedValue = EncodedValueFactory.getEncodedValue(value);

        // Tests if the Object is a String, in which case we register it.
        // Arrays of String can't appear, as they are treated with visitArray() and String are one by one
        // sent to this method.
        if (encodedValue != null && encodedValue.getType() == Opcodes.VALUE_STRING) {
            constantPool.addStringToConstantPool((String)value);

            // If we are encoding an array, and as we've found a String, it means we are encoding
            // an array of Strings. They must not be sorted.
            if (mustSortArray && isArrayAnnotation) {
                setMustSortArray(false);
            }
        }

        // By default, all the elements are sorted, so we simply add them to the current Annotation Item.
        // However, the Signature Array is not sorted. So in that case ONLY, we add the element to an Array,
        // which holds them unsorted, and then we will add this Array to the Annotation Item at visitEnd().
        if (!mustSortArray && isArrayAnnotation) {
            unsortedArray.addEncodedValue(encodedValue);
        } else {
            AnnotationElement annotationElement = new AnnotationElement(name, encodedValue);
            // Adds the given Annotation information to the current Annotation Item.
            annotationItem.addAnnotationElement(annotationElement);
        }

    }

    @Override
    public void visitEnd() {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("        AnnotationWriter : visitEnd.");
        }

        // Now that the annotation_item is fully known, we can register it to the Constant Pool.
        // Only a Father registers it. If we're treating a Default Annotation, we don't register it now, as it won't
        // be used as-is, but will be converted into a new AnnotationItem consisting of all the DefaultAnnotation
        // of the Class.
        visitEndRegisterAnnotationItemNotDefaultAnnotation();
    }

    /**
     * Registers the annotation_item to the Constant Pool. Must only be called from a VisitEnd, and the
     * Annotation must NOT be a DefaultAnnotation, because it won't be used as-is, but will be converted
     * into a new AnnotationItem consisting of all the DefaultAnnotation of the Class.
     */
    protected void visitEndRegisterAnnotationItemNotDefaultAnnotation() {
        if (father == null) {
            constantPool.addAnnotationItemToConstantPool(annotationItem);
        }
    }

    @Override
    public void visitEnum(String name, String desc, String value) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("        AnnotationWriter : visitEnum. Name = " + name +
                               ", desc = " + desc +
                               ", value = " + value
                              );
        }

        // If the name is null, it means the visitEnum is contained in a visitArray, in which case its name
        // is Null, and we must use the name of its "parent", the visitArray.
        if (name == null) {
            name = currentName;

            // An Enum in an Array must NOT be sorted.
            setMustSortArray(false);
        }

        constantPool.addStringToConstantPool(name);

        // Registers the Field to the Constant pool.
        Field field = constantPool.addFieldToConstantPool(value, desc, desc, Opcodes.ACC_UNKNOWN, null, null);

        EncodedValue encodedValue = new EncodedValueEnum(field);

        if (mustSortArray) {
            // Case of a simple Enum outside an Array.
            AnnotationElement annotationElement = new AnnotationElement(name, encodedValue);
            // Adds the given Annotation information to the current Annotation Item.
            annotationItem.addAnnotationElement(annotationElement);
        } else {
            // Case of a Enum(s) inside an Array. We must not sort them.
            unsortedArray.addEncodedValue(encodedValue);
        }
    }

    @Override
    public AnnotationVisitor visitArray(String name) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("        AnnotationWriter : visitArray. Name = " + name);
        }

        constantPool.addStringToConstantPool(name);

        // Creates an AnnotationWriter child, so that it can parse the Array. It will automatically adds
        // its result to "this", its father, when it reaches visitEnd().
        AnnotationItem childAnnotationItem = new AnnotationItem(annotationItem.getVisibility(),
                annotationItem.getAnnotationType());
        AnnotationWriter childAnnotationWriter = new AnnotationWriterArray(constantPool, childAnnotationItem);
        childAnnotationWriter.father = this;
        childAnnotationWriter.currentName = name;
        childAnnotationWriter.setMustSortArray(mustSortArray);

        return childAnnotationWriter;
    }

    @Override
    public AnnotationVisitor visitAnnotation(String name, String desc) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("        AnnotationWriter : visitAnnotation. Name = " + name +
                               ", desc = " + desc);
        }

        // If the name is null, it means the visitAnnotation is contained in a visitArray, in which case its
        // name is Null, and we must use the name of its "parent", the visitArray.
        if (name == null) {
            name = currentName;
        }

        constantPool.addStringToConstantPool(name);
        constantPool.addTypeToConstantPool(desc);

        // Creates an AnnotationWriter child, so that it can parse the Sub-Annotation. It will automatically
        // adds its result to "this", its father, when it reaches visitEnd().
        AnnotationItem childAnnotationItem = new AnnotationItem(annotationItem.getVisibility(),
                annotationItem.getAnnotationType());
        AnnotationWriter childAnnotationWriter = new AnnotationWriterSubAnnotation(constantPool, childAnnotationItem);
        childAnnotationWriter.father = this;
        childAnnotationWriter.currentName = name;
        childAnnotationWriter.annotationType = desc;

        return childAnnotationWriter;
    }


    @Override
    public void visitClass(String annotationName, String className) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("        AnnotationWriter : visitClass. Name = " + annotationName +
                               ", className = " + className);
        }

        // If the name is null, it means the visitClass is contained in a visitArray, in which case its name
        // is Null, and we must use the name of its "parent", the visitArray.
        if (annotationName == null) {
            annotationName = currentName;
            // A Class in an Array must NOT be sorted.
            setMustSortArray(false);
        } else {
            // Bug 316471 : do not forget to add the string to constant pool.
            constantPool.addStringToConstantPool(annotationName);
        }

        // Registers the Type to the Constant Pool.
        constantPool.addTypeToConstantPool(className);

        EncodedValue encodedValue = new EncodedValueType(className);

        if (mustSortArray) {
            // Case of a simple Class outside an Array.
            AnnotationElement annotationElement = new AnnotationElement(annotationName, encodedValue);
            // Adds the given Annotation information to the current Annotation Item.
            annotationItem.addAnnotationElement(annotationElement);
        } else {
            // Case of a Class(es) inside an Array. We must not sort it.
            unsortedArray.addEncodedValue(encodedValue);
        }
    }


    // ------------------------------------------
    // Static Methods.
    // ------------------------------------------

    /**
     * Creates an Annotation Writer for it to visit a following Annotation. Also registers the
     * descriptor to the Constant pool. The Annotation Item created inside the Annotation Writer
     * is NOT registered to anything, so the calling Method may want to do it.
     * @param desc the class descriptor of the annotation class.
     * @param visible true if the annotation is visible at runtime.
     * @param constantPool the Constant Pool of the Application.
     * @param classDefinitionItem the Class the Annotation is linked to. This is ONLY useful for
     * 		  Default Annotation, and must be Null otherwise.
     * @return an Annotation Writer for it to visit a following Annotation.
     */
    public static AnnotationWriter createAnnotationWriter(String desc, boolean visible,
            ConstantPool constantPool, ClassDefinitionItem classDefinitionItem) {

        // Adds the desc to the Constant Pool.
        constantPool.addTypeToConstantPool(desc);

        // Creates an AnnotationItem, incomplete for now (no Elements), and registers it to the Class.
        // We do not add it to the Constant Pool now because of this incompleteness.
        // One test before : if the Annotation is of Signature type, we have to make sure we won't
        // sort the coming array. Also, System annotation requires a System visibility.
        int visibility = Opcodes.VISIBILITY_SYSTEM;
        boolean isSignature = false;
        if (desc.equals(Constants.SIGNATURE_ANNOTATION_INTERNAL_NAME)) {
            isSignature = true;
        } else {
            if (!desc.equals(Constants.ANNOTATION_DEFAULT_INTERNAL_NAME)) {
                visibility = visible ? Opcodes.VISIBILITY_RUNTIME : Opcodes.VISIBILITY_BUILD;
            }
        }

        AnnotationItem annotationItem = new AnnotationItem(visibility, desc);

        AnnotationWriter annotationWriter;

        // Default Annotation ?
        if (classDefinitionItem != null) {
            // While instantiating the AnnotationWriter, sets the Class related to this Annotation.
            // It is only useful to Default Annotation.
            annotationWriter = new AnnotationWriterDefaultAnnotation(constantPool, annotationItem, classDefinitionItem);
            annotationWriter.currentName = classDefinitionItem.getClassName();
        } else {
            annotationWriter = new AnnotationWriter(constantPool, annotationItem);
        }

        // If the annotation is about a Signature, it must NOT be sorted, as it consists of a list
        // of Strings.
        if (isSignature) {
            annotationWriter.setMustSortArray(false);
        }
        return annotationWriter;
    }
}
