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
import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.structureCommon.LocalVariable;

/**
 * Simple class representing a Method. It's a mix between the method_id_item, but also encoded_method
 * structures.
 *
 * The equals and hashCode methods have been overridden in order to detect easily
 * duplicates in HashMaps.
 *
 * A Method is identified by its methodName, className and prototype. Access and ExceptionIndexes
 * are NOT taken in account to differentiate the methods.
 *
 * Access may be ACC_UNKNOWN if the method created is referenced, but not yet parsed (like when an
 * instruction refers to a not yet parsed method).
 *
 * A Method has a code_item. However, it may not if the method is abstract or interface.
 *
 * @author Julien Névo
 */
public class Method implements Comparable<Method>, IAnnotationsHolder {

    /**
     * Name of the Class owning the Method.
     */
    final private String className;

    /**
     * Prototype of the Method, in TypeDescriptor format.
     */
    final private Prototype prototype;

    /**
     * The name of the Method.
     */
    final private String methodName;

    /**
     * The access flags of the Field.
     */
    private int access;

    /**
     * The names of the exceptions. May be Null.
     */
    private String[] exceptionNames;

    /**
     * Start in bytes from the beginning of the Dex file, of the bytecode to copy, in case the
     * "ConstantPool" optimization is used if the Writer is directly linked to the Reader.
     * It includes the code_item header.
     */
    private int startBytecodeToCopy;

    /**
     * Start in bytes from the beginning of the Dex file, of the debug_info_item to copy, in case the
     * "ConstantPool" optimization is used if the Writer is directly linked to the Reader.
     */
    private int startDebugInfoToCopy;

    /**
     * The code of the Method, its Instructions. May be Null if abstract/interface.
     */
    private CodeItem codeItem;

    /**
     * Annotation_set_item, representing all the annotation_items for this Method.
     */
    private AnnotationSetItem annotationSetItem = new AnnotationSetItem();

    /**
     * Annotation_set_ref_item, representing the annotation_set_item for the Annotated Parameters
     * of this Method.
     */
    private AnnotationSetRefList annotatedParameterSetRefList;

    /**
     * The name of the parameters (return parameter not included).
     */
    private String[] parameters;

    /**
     * The Local Variables of this Method.
     */
    private List<LocalVariable> localVariables = new ArrayList<LocalVariable>();

    /**
     * The Signature of the Field. May be Null.
     */
    private String[] signature;

    /**
     * The hashcode is cached.
     */
    final private int hashcode;

    /**
     *
     * @param methodName name of the Method.
     * @param className name of the Class owning the Method.
     * @param methodDescriptor Prototype of the Method, in TypeDescriptor format.
     */

    public Method(String methodName, String className, Prototype methodDescriptor) {
        this.methodName = methodName;
        this.className = className;
        prototype = methodDescriptor;
        hashcode = calculateHashCode(methodName, className, prototype);
    }


    /**
     * Full init if this is the witness kept.
     * @param access The access flags of the Field.
     * @param signature the signature of the method. May be Null.
     * @param exceptionNames the names of the exceptions. May be Null.
     * @param constantPool the Constant Pool of the Application.
     */
    public void init(int access, String[] signature, String[] exceptionNames, ConstantPool constantPool) {
        this.access = access;
        this.exceptionNames = exceptionNames;
        this.signature = signature;
        if (!isUnknown() && supportsCodeItem()) {
            codeItem = new CodeItem(this, constantPool);
        }

        annotatedParameterSetRefList = new AnnotationSetRefList(prototype.getNbParameters(), this);
    }



    // ----------------------------------------------
    // Public methods.
    // ----------------------------------------------

    /**
     * Adds a Local Variable to this Method.
     */
    public void addLocalVariable(LocalVariable localVariable) {
        localVariables.add(localVariable);
    }

    /**
     * Adds information to the current Method. This is useful <i>only</i> if this
     * Method has the ACC_UNKNOWN flag, which means Instructions referred to it while it has not yet been
     * visited, declaring it but not giving all the information it should have.
     */
    public void completeInformation(int access, String[] exceptionNames, String[] signature, ConstantPool constantPool) {
        this.access = access;
        this.exceptionNames = exceptionNames;
        this.signature = signature;

        if ((codeItem == null) && !isUnknown() && supportsCodeItem()) { // It SHOULD be Null in all cases.
            codeItem = new CodeItem(this, constantPool);
        }
    }

    /**
     * Calculates the hashcode of the Method according to its given identifiers.
     * @param methodName the name of the Method.
     * @param className the Class it belongs to.
     * @param prototype the Prototype of the Method.
     * @return the hashcode of the Method.
     */
    public static int calculateHashCode(String methodName, String className, Prototype prototype) {
        return methodName.hashCode() + className.hashCode() + prototype.hashCode();
    }

    /**
     * Calculates the hashcode of the Method according to its given identifiers.
     * @param methodName the name of the Method.
     * @param className the Class it belongs to.
     * @param prototypeHashCode the hashcode of the Prototype.
     * @return the hashcode of the Method.
     */
    public static int calculateHashCode(String methodName, String className, int prototypeHashCode) {
        return methodName.hashCode() + className.hashCode() + prototypeHashCode;
    }

    /**
     * Indicates whether the Method is virtual (not static, private, or constructor).
     * @return true if the Method is virtual.
     */
    public boolean isVirtual() {
        return ((access & Opcodes.ACC_STATIC) + (access & Opcodes.ACC_PRIVATE)
                + (access & Opcodes.ACC_CONSTRUCTOR)) == 0;
    }

    /**
     * Indicates whether the Method is direct (static and/or private and/or constructor).
     * @return true if the Method is direct.
     */
    public boolean isDirect() {
        return !isVirtual();
    }

    /**
     * Indicates whether the Method is static.
     * @return true if the Method is static.
     */
    public boolean isStatic() {
        return (access & Opcodes.ACC_STATIC) != 0;
    }

    /**
     * Indicates whether the Method is abstract or interface.
     * @return true if the Method is abstract or interface.
     */
    public boolean isAbstractOrInterface() {
        return ((access & Opcodes.ACC_ABSTRACT) | (access & Opcodes.ACC_INTERFACE)) != 0;
    }

    /**
     * Indicates whether the Method is native.
     * @return true if the Method is static.
     */
    public boolean isNative() {
        return (access & Opcodes.ACC_NATIVE) != 0;
    }

    /**
     * Indicates whether the Method is unknown (referred to, but not yet parsed).
     * @return true if the Method is unknown.
     */
    public boolean isUnknown() {
        return (access & Opcodes.ACC_UNKNOWN) != 0;
    }

    /**
     * Indicates whether a method contains java code (a CodeItem).
     * @return true if the Method has code.
     */
    public boolean supportsCodeItem() {
        return (access & (Opcodes.ACC_ABSTRACT | Opcodes.ACC_INTERFACE | Opcodes.ACC_NATIVE)) == 0;
    }

    /**
     * Indicates whether the Method uses the "this" parameter.
     * @return true if the Method uses the "this" parameter.
     */
    public boolean isUsingThis() {
        // "This" is used if the Method is not static, and is not a Constructor and not abstract.
        // Patch for "not abstract" by Panxiaobo #316341 - patch 16.
        return (!isStatic()) && ((access & Opcodes.ACC_CONSTRUCTOR) == 0) && supportsCodeItem();
    }

    /**
     * Adds an annotation_item to the annotations_set_items.
     * @param annotationItem the Annotation Item to add.
     */
    public void addAnnotationItem(AnnotationItem annotationItem) {
        annotationSetItem.addAnnotationItem(annotationItem);
    }

    /**
     * Adds an annotation_item to a parameter of this Method.
     * @param parameterIndex zero-based index of the argument.
     * @param annotationItem the Annotation Item to add.
     */
    public void addParameterAnnotationItem(int parameterIndex, AnnotationItem annotationItem) {
        annotatedParameterSetRefList.addAnnotationItem(parameterIndex, annotationItem);
    }

    /**
     * Closes and registers the annotation_set_items and annotation_set_ref_items of this Method.
     * This must only be done once when the Method has been fully visited.
     * @param constantPool the Constant Pool of the Application.
     */
    public void closeAnnotations(ConstantPool constantPool) {
        annotatedParameterSetRefList.close();
        // Now that the annotation_set_item are fully known, we can register it to the Constant Pool.
        constantPool.addAnnotationSetItemToConstantPool(annotationSetItem);

        // We do the same for the annotation_set_ref_list.
        constantPool.addAnnotationSetRefListToConstantPool(annotatedParameterSetRefList);
    }

    /**
     * Generates the Instructions code, as Dalvik bytecode, in the codeItem buffer, as well as its
     * header. It uses <i>symbolic</i> references, so must be parsed again to link them to the "real"
     * elements.
     * Also fills the debug_code_item in this instance. Note however that the debug_info_offset field
     * in the code_item header is not set, because we don't know where the debug_info_item is encoded
     * for now. Note that the debug_info_item is not written here, only built.
     * The alignment is not managed here, but must be by the calling method.
     */
    public void generateCodeItemCode() {
        codeItem.generateCodeItemCode();
    }

    /**
     * Frees all the structures (list of instructions...) of the code_item and the debug_info_item
     * so that they don't consume memory. This <i>MUST</i> be done after having generated the bytecode, once
     * the method has been parsed and its end visited.
     */
    public void free() {
        codeItem.free();
    }


    // ----------------------------------------------
    // Getters and setters.
    // ----------------------------------------------

    /**
     * Returns the code_item code (including the code_item header and the bytecode), <i>without</i> the
     * try/catch fields after the insns field.
     * It uses <i>symbolic</i> references, so must be parsed again to link them to the "real"
     * elements.
     * @return the code_item code, without the try/catch fields.
     */
    public ByteVector getCodeItemCode() {
        return codeItem.getCodeItemCode();
    }

    /**
     * Returns the try/catch section of the code_item, beginning by the possible padding after the insns field,
     * or Null if no try/catch is present.
     * It uses <i>symbolic</i> references, so must be parsed again to link them to the "real"
     * elements.
     * @return the try/catch code, or Null.
     */
    public ByteVector getCodeItemTryCatch() {
        return codeItem.getCodeItemTryCatch();
    }

    /**
     * Sets the start of the bytecode to copy from the input Dex file to the
     * output. This is only useful when using the optimization that consists in copying part of
     * the Constant Pool and the bytecode of methods that doesn't change, if the Reader is linked
     * to the Writer with no Adapter to modify the methods in between.
     * @param start start in bytes from the beginning of the Dex file where the bytecode is. This
     *        includes the code_item header.
     */
    public void setStartBytecodeToCopy(int start) {
        startBytecodeToCopy = start;
    }

    /**
     * Gets the start in byte of the bytecode to copy from the input Dex file.
     * This is only useful when using the optimization that consists in copying part of
     * the Constant Pool and the bytecode of methods that doesn't change, if the Reader is linked
     * to the Writer with no Adapter to modify the methods in between.
     * @return the start in byte of the bytecode, or 0 if the optimization is not used.
     */
    public int getStartBytecodeToCopy() {
        return startBytecodeToCopy;
    }

    /**
     * Sets the start of the debug_info_item to copy from the input Dex file to the
     * output. This is only useful when using the optimization that consists in copying part of
     * the Constant Pool and the bytecode of methods that doesn't change, if the Reader is linked
     * to the Writer with no Adapter to modify the methods in between.
     * @param start start in bytes from the beginning of the Dex file where the debug_info_item is.
     */
    public void setStartDebugInfoToCopy(int start) {
        startDebugInfoToCopy = start;
    }

    /**
     * Gets the start in byte of the debug_info_item to copy from the input Dex file.
     * This is only useful when using the optimization that consists in copying part of
     * the Constant Pool and the bytecode of methods that doesn't change, if the Reader is linked
     * to the Writer with no Adapter to modify the methods in between.
     * @return the start in byte of the debug_info_item, or 0 if the optimization is not used.
     */
    public int getStartDebugInfoToCopy() {
        return startDebugInfoToCopy;
    }


    // ----------------------------------------------
    // Getters and Setters.
    // ----------------------------------------------

    /**
     * Returns the name of the Class owning the Method.
     * @return the name of the Class owning the Method.
     */
    public String getClassName() {
        return className;
    }

    /**
     * Returns the prototype of the Method, in TypeDescriptor format.
     * @return the prototype of the Method, in TypeDescriptor format.
     */
    public Prototype getPrototype() {
        return prototype;
    }

    /**
     * Returns the name of the Method.
     * @return the name of the Method.
     */
    public String getMethodName() {
        return methodName;
    }

    /**
     * Returns the access flags.
     * @return the access flags.
     */
    public int getAccess() {
        return access;
    }

    /**
     * Returns the exception names. May be Null.
     * @return the exception names, or Null.
     */
    public String[] getExceptionNames() {
        return exceptionNames;
    }

    /**
     * Returns the Code Item linked to this Method. May be Null if it hasn't any (if abstract or interface).
     * @return the Code Item linked to this Method or Null.
     */
    public CodeItem getCodeItem() {
        return codeItem;
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
     * Returns the number of annotation_items this structure currently contains. Does not concern the
     * parameter annotations.
     * @return the number of annotation_items this structure currently contains.
     */
    @Override
    public int getNbAnnotations() {
        return annotationSetItem.getNbAnnotationItems();
    }

    /**
     * Return the annotation_set_ref_list of this Method, indicating what are the annotations on each
     * parameters.
     * @return the annotation_set_ref_list of this Method.
     */
    public AnnotationSetRefList getAnnotatedParameterSetRefList() {
        return annotatedParameterSetRefList;
    }

    /**
     * Returns the number of annotation_set_items the Method has. They are only used by the
     * parameter annotations (one annotation_set_item per argument, even if this one in particular is
     * not annotated).
     * @return the number of annotation_set_items the Method has.
     */
    public int getNbParameterAnnotations() {
        return annotatedParameterSetRefList.getNbAnnotationSetItemsUsed();
    }

    /**
     * Returns the number of parameters of this Method (doesn't count the Return parameter). May be 0.
     * @return the number of parameters of this Method.
     */
    public int getNbParameters() {
        return prototype.getNbParameters();
    }

    /**
     * Returns the name of the parameters of this Method (excluding the Return parameter). May be null if
     * the Debug Info didn't provide them or was not visited.
     * @return the name of the parameters of this Method, or null.
     */
    public String[] getParameters() {
        return parameters;
    }

    /**
     * Sets the name of the parameters used by this Method.
     * @param parameters name of the parameters.
     */
    public void setParameters(String[] parameters) {
        this.parameters = parameters;
    }

    /**
     * Sets the first line number of the Method, but only if none has been found before.
     * @param firstLineNumber the first line number of this Method.
     */
    public void setFirstLineNumberIfNeeded(int firstLineNumber) {
        codeItem.setFirstLineNumber(firstLineNumber);
    }

    /**
     * Returns the Signature of the Method. May be Null.
     * @return the Signature of the Method. May be Null.
     */
    public String[] getSignature() {
        return signature;
    }

    /**
     * Returns the Local Variable list. It may be empty.
     * @return the Local Variable list.
     */
    public List<LocalVariable> getLocalVariables() {
        return localVariables;
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

        if (obj instanceof Method) {
            Method mii = (Method)obj;
            return methodName.equals(mii.methodName) &&
                   (className.equals(mii.className) && prototype.equals(mii.prototype));
        }

        return false;
    }

    @Override
    public int compareTo(Method method) {
        if (this == method) {
            return 0;
        }

        // Tests class owning name first.
        int compare = className.compareTo(method.className);
        if (compare != 0) {
            return compare;
        }

        // Tests the names.
        compare = methodName.compareTo(method.methodName);
        if (compare != 0) {
            return compare;
        }

        // Tests the prototype.
        return prototype.compareTo(method.prototype);
    }


}
