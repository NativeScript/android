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

import org.ow2.asmdex.structureWriter.AnnotationItem;
import org.ow2.asmdex.structureWriter.ClassDefinitionItem;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * AnnotationWriter specialized for DefaultAnnotation.
 *
 * @author Julien Névo
 */
public class AnnotationWriterDefaultAnnotation extends AnnotationWriter {

    /**
     * The Class related to this Annotation. This is only useful for
     * Default Annotation, and must not be set in any other cases.
     */
    protected ClassDefinitionItem classDefinitionItem = null;

    AnnotationVisitor subAnnotationVisitor;

    /**
     * Constructor for the default annotations.
     * @param constantPool constant pool
     * @param annotationItem annotation item
     * @param classDefinitionItem
     */
    public AnnotationWriterDefaultAnnotation(ConstantPool constantPool,	AnnotationItem annotationItem,
            ClassDefinitionItem classDefinitionItem) {
        super(constantPool, annotationItem);
        this.classDefinitionItem = classDefinitionItem;
    }

    @Override
    public void visit(String name, Object value) {
        // If Default Annotation, we have to create a subAnnotation and visit it.
        subAnnotationVisitor = super.visitAnnotation(Constants.VALUE_STRING, currentName);
        subAnnotationVisitor.visit(name, value);
        // Registering "name" and "currentName" is not needed, as they were registered before.
    }

    @Override
    public void visitEnum(String name, String desc, String value) {
        subAnnotationVisitor = super.visitAnnotation(Constants.VALUE_STRING, currentName);
        subAnnotationVisitor.visitEnum(name, desc, value);
    }

    @Override
    public void visitClass(String annotationName, String className) {
        subAnnotationVisitor = super.visitAnnotation(Constants.VALUE_STRING, currentName);
        subAnnotationVisitor.visitClass(annotationName, className);
    }

    @Override
    public AnnotationVisitor visitArray(String name) {
        subAnnotationVisitor = super.visitAnnotation(Constants.VALUE_STRING, currentName);
        return subAnnotationVisitor.visitArray(name);
    }

    @Override
    public AnnotationVisitor visitAnnotation(String name,String desc) {
        subAnnotationVisitor = super.visitAnnotation(Constants.VALUE_STRING, currentName);
        return subAnnotationVisitor.visitAnnotation(name,desc);
    }

    @Override
    public void visitEnd() {
        subAnnotationVisitor.visitEnd();
        // If DefaultAnnotation, we must not add it directly to the Method, it is not encoded this way.
        // It must be given to the Class, that will encode it as ONE AnnotationElement of VALUE_ANNOTATION
        // type, which will consist of the one or several DefaultAnnotations.
        classDefinitionItem.addAnnotationItemForDefaultAnnotation(annotationItem);
    }

}
