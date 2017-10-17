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

import java.util.List;

import org.ow2.asmdex.encodedValue.EncodedValue;
import org.ow2.asmdex.encodedValue.EncodedValueAnnotation;
import org.ow2.asmdex.encodedValue.EncodedValueArray;
import org.ow2.asmdex.encodedValue.EncodedValueFactory;
import org.ow2.asmdex.encodedValue.EncodedValueType;
import org.ow2.asmdex.structureWriter.AnnotationElement;
import org.ow2.asmdex.structureWriter.AnnotationItem;
import org.ow2.asmdex.structureWriter.ClassDefinitionItem;
import org.ow2.asmdex.structureWriter.ConstantPool;
import org.ow2.asmdex.structureWriter.Field;
import org.ow2.asmdex.structureWriter.Method;

/**
 * Class that takes care of visiting a Class for the Application Writer to write it.
 *
 * @author Julien Névo
 */
public class ClassWriter extends ClassVisitor {

    /**
     * The Application Writer of this Class Writer. It is only useful for the optimization that consists
     * in copying the Constant Pool and skips the visit of Methods that aren't transformed by any Adapter.
     */
    private ApplicationWriter applicationWriter;

    /**
     * The Constant Pool of the Application.
     */
    private ConstantPool constantPool;

    /**
     * Contains all the information of the Class.
     */
    private ClassDefinitionItem classDefinitionItem;

    /**
     * Stores the Method this is linked to, if any, which means the current Class is defined in this Method
     * body. This Method is set by visitOuterClass and is never reset after as, according to the documentation,
     * a Class may have either of an EnclosingClass or EnclosingMethod annotation.
     */
    private Method enclosingMethod = null;

    /**
     * Constructor.
     * @param applicationWriter the Application Writer.
     * @param constantPool the constantPool of the Application.
     * @param access the access flags of the class.
     * @param name the name of the class.
     * @param signature the signature of the class. Used only for generics. May be null.
     * @param superName the name of the super class.
     * @param interfaces the name of the interfaces of the class.
     */
    public ClassWriter(ApplicationWriter applicationWriter, ConstantPool constantPool, int access,
                       String name, String[] signature, String superName, String[] interfaces) {
        super(Opcodes.ASM4);
        this.applicationWriter = applicationWriter;
        this.constantPool = constantPool;
        classDefinitionItem = constantPool.addClassToConstantPool(name, superName, access,
                              interfaces, signature);
    }


    // -----------------------------------------------------
    // Getters and Setters.
    // -----------------------------------------------------

    /**
     * Returns the ApplicationWriter of this Class Writer.
     */
    public ApplicationWriter getApplicationWriter() {
        return applicationWriter;
    }

    /**
     * Returns the name of the Class.
     * @return the name of the Class.
     */
    public String getName() {
        return classDefinitionItem.getClassName();
    }

    /**
     * Returns the current class definition item.
     * @return the current class definition item.
     */
    public ClassDefinitionItem getClassDefinitionItem() {
        return classDefinitionItem;
    }

    /**
     * Returns the Constant Pool related to this Application.
     * @return the Constant Pool related to this Application.
     */
    public ConstantPool getConstantPool() {
        return constantPool;
    }


    // -----------------------------------------------------
    // Visitor methods.
    // -----------------------------------------------------

    @Override
    public void visit(int version, int access, String name, String[] signature,
                      String superName, String[] interfaces) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("    ClassWriter : visit. Name = " + name
                             + ", SuperClassName = " + superName
                            );

            if (interfaces != null) {
                System.out.print(". Interfaces = ");
                for (int i = 0; i < interfaces.length; i++) {
                    System.out.print(interfaces[i] + ", ");
                }
            }
            if (signature != null) {
                System.out.print(". Signature = ");
                for (int i = 0; i < signature.length; i++) {
                    System.out.print(signature[i] + ", ");
                }
            }
            System.out.println();
        }

        // Creates the Signature Annotation if it exists.
        // Note that we use the signature that was *already* set by the ApplicationWriter, not the one
        // that is just given, even though they should be the same.
        String[] savedSignature = classDefinitionItem.getSignature();
        if (savedSignature != null) {
            int size = savedSignature.length;
            if (size > 0) {
                AnnotationVisitor av = visitAnnotation(Constants.SIGNATURE_ANNOTATION_INTERNAL_NAME, false);
                AnnotationVisitor subAv = av.visitArray(Constants.VALUE_STRING);
                for (String string : savedSignature) {
                    subAv.visit(null, string);
                }
                subAv.visitEnd();
                av.visitEnd();
            }
        }
    }

    @Override
    public void visitSource(String source, String debug) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("    ClassWriter : visitSource. Source = " + source);
        }
        classDefinitionItem.setSourceFileName(source);
        constantPool.addStringToConstantPool(source);
    }

    @Override
    public void visitEnd() {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("    ClassWriter : visitEnd.");
        }

        // Checks if the MemberClass array is Null. If not, we create a dalvik.annotation.MemberClasses around it
        // and adds it as an Class Annotation.
        EncodedValueArray array = classDefinitionItem.getMemberClassArray();
        if (array != null) {
            String val = Constants.VALUE_STRING;
            constantPool.addStringToConstantPool(val);
            constantPool.addTypeToConstantPool(Constants.MEMBER_CLASSES_ANNOTATION_INTERNAL_NAME);
            AnnotationItem annotationItem = new AnnotationItem(Opcodes.VISIBILITY_SYSTEM,
                    Constants.MEMBER_CLASSES_ANNOTATION_INTERNAL_NAME);
            AnnotationElement annotationElement = new AnnotationElement(val, array);
            annotationItem.addAnnotationElement(annotationElement);
            classDefinitionItem.addAnnotationItem(annotationItem);

            constantPool.addAnnotationItemToConstantPool(annotationItem);
        }

        // Checks if the Class contains Default Annotations. If yes, we have to create <i>ONE</i>
        // AnnotationItem regrouping them.
        List<AnnotationItem> defaultAnnotationItems = classDefinitionItem.getAnnotationItemsForDefaultAnnotation();
        if ((defaultAnnotationItems != null) && (defaultAnnotationItems.size() > 0)) {

            EncodedValueAnnotation createdEncodedValueAnnotation = null;

            String encodedAnnotationType = null;
            for (AnnotationItem annotationItems : defaultAnnotationItems) {
                // There should only one AnnotationElement, but we make a loop for security.
                for (AnnotationElement currentElement : annotationItems.getAnnotationElements()) {
                    EncodedValueAnnotation currentEncodedValue = (EncodedValueAnnotation)currentElement.getEncodedValue();
                    encodedAnnotationType = currentEncodedValue.getAnnotationType();

                    if (createdEncodedValueAnnotation == null) {
                        createdEncodedValueAnnotation = new EncodedValueAnnotation(encodedAnnotationType);
                    }

                    // There should be one AnnotationElement in the Annotation, but we make a loop for security.
                    for (AnnotationElement currentAnnotationElement : currentEncodedValue.getAnnotationElements()) {
                        createdEncodedValueAnnotation.addAnnotationElement(currentAnnotationElement);
                    }
                }
            }

            // We set up the DefaultAnnotation we need to create.
            AnnotationItem createdAnnotationItem = new AnnotationItem(Opcodes.VISIBILITY_SYSTEM,
                    Constants.ANNOTATION_DEFAULT_INTERNAL_NAME);
            // It consists of only one EncodedAnnotation, which has one AnnotationElement, of type "value"
            // and of one EncodedValue of VALUE_ANNOTATION type.
            AnnotationElement createdAnnotationElement = new AnnotationElement(Constants.VALUE_STRING, createdEncodedValueAnnotation);

            createdAnnotationItem.addAnnotationElement(createdAnnotationElement);

            // Adds the newly created AnnotationItem to the Constant Pool and to the Class.
            classDefinitionItem.addAnnotationItem(createdAnnotationItem);
            constantPool.addAnnotationItemToConstantPool(createdAnnotationItem);
        }

        // Now that the annotation_set_item are fully known, we can register it to the Constant Pool.
        constantPool.addAnnotationSetItemToConstantPool(classDefinitionItem.getAnnotationSetItem());

        // We also build the annotation_directory_item.
        classDefinitionItem.buildAnnotationDirectoryItem();
    }

    @Override
    public MethodVisitor visitMethod(int access, String name, String desc,
                                     String[] signature, String[] exceptions) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("    ClassWriter : visitMethod. Access = " + access +
                             ", Name = " + name + ", Desc = " + desc);
            if (signature != null) {
                System.out.print(". Signature = ");
                for (String sig : signature) {
                    System.out.print(sig +", ");
                }
            }
            if (exceptions != null) {
                System.out.print(". Exceptions = ");
                for (String exception : exceptions) {
                    System.out.print(exception +", ");
                }
            }
            System.out.println();
        }

        MethodWriter methodWriter = new MethodWriter(this, access, name, desc, signature, exceptions);

        // Registers the method to the Class. It may be virtual or direct.
        Method method = methodWriter.getMethod();
        classDefinitionItem.addMethod(method);

        // If the Method is using This (non static and non constructor), the String "this" is added to the
        // Constant Pool.
        if (method.isUsingThis()) {
            constantPool.addStringToConstantPool(Constants.THIS_STRING);
        }

        // Manages the Signature Annotation if it exists.
        String[] savedSignature = method.getSignature();
        if (savedSignature != null) {
            int size = savedSignature.length;
            if (size > 0) {
                AnnotationVisitor av = methodWriter.visitAnnotation(Constants.SIGNATURE_ANNOTATION_INTERNAL_NAME, false);
                AnnotationVisitor subAv = av.visitArray(Constants.VALUE_STRING);
                for (String string : savedSignature) {
                    subAv.visit(null, string);
                }
                subAv.visitEnd();
                av.visitEnd();
            }
        }

        // If there are exceptions, we need to create a dalvik.annotation.Throws annotation.
        // It consists of an Array Annotation with the type of the Exceptions as elements.
        if ((exceptions != null) && (exceptions.length > 0)) {
            String valString = Constants.VALUE_STRING;
            constantPool.addStringToConstantPool(valString);
            constantPool.addTypeToConstantPool(Constants.EXCEPTION_ANNOTATION_INTERNAL_NAME);

            EncodedValueArray array = new EncodedValueArray();
            for (String exception : exceptions) {
                constantPool.addTypeToConstantPool(exception);
                EncodedValue value = new EncodedValueType(exception);
                array.addEncodedValue(value);
            }
            AnnotationElement annotationElement = new AnnotationElement(valString, array);
            AnnotationItem annotationItem = new AnnotationItem(Opcodes.VISIBILITY_SYSTEM,
                    Constants.EXCEPTION_ANNOTATION_INTERNAL_NAME);
            annotationItem.addAnnotationElement(annotationElement);
            method.addAnnotationItem(annotationItem);

            constantPool.addAnnotationItemToConstantPool(annotationItem);
        }

        return methodWriter;
    }

    @Override
    public FieldVisitor visitField(int access, String name, String desc,
                                   String[] signature, Object value) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.print("    ClassWriter : visitField. Access = " + access +
                             ", Name = " + name + ", Desc = " + desc + ", value = " + value
                            );
            if (signature != null) {
                System.out.print(". Signature = ");
                for (String sig : signature) {
                    System.out.print(sig +", ");
                }
            }
            System.out.println();
        }

        FieldWriter fieldWriter = new FieldWriter(constantPool, getName(), access, name, desc,
                signature, value);

        // Registers the Field to the Class. It may be a static or instance Field.
        Field field = fieldWriter.getField();
        classDefinitionItem.addField(field);

        // Manages the Signature Annotation if it exists.
        String[] savedSignature = field.getSignature();
        if (savedSignature != null) {
            int size = savedSignature.length;
            if (size > 0) {
                AnnotationVisitor av = fieldWriter.visitAnnotation(Constants.SIGNATURE_ANNOTATION_INTERNAL_NAME, false);
                AnnotationVisitor subAv = av.visitArray(Constants.VALUE_STRING);
                for (String string : savedSignature) {
                    subAv.visit(null, string);
                }
                subAv.visitEnd();
                av.visitEnd();
            }
        }

        return fieldWriter;
    }

    @Override
    public AnnotationVisitor visitAnnotation(String desc, boolean visible) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("    ClassWriter : visitAnnotation. Desc = " + desc +
                               ", visible = " + visible
                              );
        }

        // Creates an AnnotationItem, incomplete for now (no Elements), and registers it to the Class.
        // We do not add it to the Constant Pool now because of this incompleteness.
        AnnotationWriter annotationWriter = AnnotationWriter.createAnnotationWriter(desc, visible, constantPool, null);
        classDefinitionItem.addAnnotationItem(annotationWriter.getAnnotationItem());

        return annotationWriter;
    }







    @Override
    public void visitInnerClass(String name, String outerName,
                                String innerName, int access) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("    ClassWriter : visitInnerClass. Name = " + name
                               + ", outerName = " + outerName
                               + ", innerName = " + innerName
                               + ", access = " + access
                              );
        }

        // Register all the given data to the ConstantPool. If the name is Null, the OuterName
        // is not a Type, but just a String.
        if (name == null) {
            constantPool.addStringToConstantPool(outerName);
        } else {
            constantPool.addTypeToConstantPool(name);
            constantPool.addTypeToConstantPool(outerName);
            constantPool.addStringToConstantPool(innerName);
        }

        // We're inside an Inner Class, visiting it.
        // Two annotations need to be created. The Enclosing Class/Method, and the Inner Class.
        // If the enclosingMethod reference is null, we create an Enclosing Class, else an Enclosing
        // Method.

        // Encodes the first annotation : Enclosing Class or Enclosing Method.
        boolean isEnclosingClass = (enclosingMethod == null);
        String annotationType = (isEnclosingClass ? Constants.ENCLOSING_CLASS_ANNOTATION_INTERNAL_NAME :
                                 Constants.ENCLOSING_METHOD_ANNOTATION_INTERNAL_NAME);
        String val = Constants.VALUE_STRING;
        constantPool.addStringToConstantPool(val);
        constantPool.addTypeToConstantPool(annotationType);
        AnnotationItem annotationItem = new AnnotationItem(Opcodes.VISIBILITY_SYSTEM, annotationType);
        EncodedValue encodedValue;
        if (isEnclosingClass) {
            // Enclosing Class.
            encodedValue = EncodedValueFactory.getEncodedValue(outerName, Opcodes.VALUE_TYPE);
        } else {
            // Enclosing Method.
            encodedValue = EncodedValueFactory.getEncodedValue(enclosingMethod, Opcodes.VALUE_METHOD);
        }

        AnnotationElement annotationElement = new AnnotationElement(val, encodedValue);
        annotationItem.addAnnotationElement(annotationElement);
        constantPool.addAnnotationItemToConstantPool(annotationItem);
        classDefinitionItem.addAnnotationItem(annotationItem);

        // Encodes the second annotation : the Inner Class.
        annotationItem = new AnnotationItem(Opcodes.VISIBILITY_SYSTEM,
                                            Constants.INNER_CLASS_ANNOTATION_INTERNAL_NAME);
        // Encodes the "name".
        val = Constants.NAME_STRING;
        constantPool.addStringToConstantPool(val);

        constantPool.addTypeToConstantPool(Constants.INNER_CLASS_ANNOTATION_INTERNAL_NAME);
        // According to the documentation, if the class is anonymous, the name becomes a Null value.
        if (innerName == null) { // DOUBLE CHECK IF NAME OR INNER NAME.
            encodedValue = EncodedValueFactory.getEncodedNullValue();
        } else {
            encodedValue = EncodedValueFactory.getEncodedValue(innerName, Opcodes.VALUE_STRING);
        }
        annotationElement = new AnnotationElement(val, encodedValue);
        annotationItem.addAnnotationElement(annotationElement);

        // Encodes the "accessFlags".
        val = Constants.ACCESS_FLAGS_STRING;
        constantPool.addStringToConstantPool(val);
        encodedValue = EncodedValueFactory.getEncodedValue(access, Opcodes.VALUE_INT);
        annotationElement = new AnnotationElement(val, encodedValue);
        annotationItem.addAnnotationElement(annotationElement);

        constantPool.addAnnotationItemToConstantPool(annotationItem);
        classDefinitionItem.addAnnotationItem(annotationItem);
    }

    @Override
    public void visitMemberClass(String name, String outerName,
                                 String innerName) {
        if (name != null) {
            constantPool.addTypeToConstantPool(name);
        }
        classDefinitionItem.addMemberClassValue(name, constantPool);
    }

    @Override
    public void visitOuterClass(String owner, String name, String desc) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("    ClassWriter : visitOuterClass. Name = " + name
                               + ", owner = " + owner
                               + ", desc = " + desc
                              );
        }

        // We found a reference to a Method. This Method could have been found before, or
        // not, in which case Constant Pool adds it.
        // We store this Method, which means that the next visitInnerClass will take it in account, generating
        // a Enclosing Method annotation instead of a Enclosing Class annotation.
        enclosingMethod = constantPool.addMethodToConstantPool(name, owner, desc, Opcodes.ACC_UNKNOWN, null, null);
    }


    @Override
    public void visitAttribute(Object attr) {
        // This method is ignored, as Attributes are not supported by AsmDex.
    }


}
