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

import org.ow2.asmdex.structureWriter.ConstantPool;
import org.ow2.asmdex.structureWriter.Field;

/**
 * Class visiting a Field and registering it in order to write it.
 *
 * @author Julien Névo.
 */
public class FieldWriter extends FieldVisitor {

    private ConstantPool constantPool;

    /**
     * The Field managed by this Class.
     */
    private Field field;

    /**
     * Constructor of a Field Writer.
     * @param constantPool the ConstantPool of the Application.
     * @param classOwningName the name of the Class owning the Field.
     * @param access the access of the Field.
     * @param name the name of the Field.
     * @param desc the description of the Field.
     * @param signature the Signature of the Field. May be Null.
     * @param value the value of the Field. May be Null.
     */
    public FieldWriter(ConstantPool constantPool, String classOwningName, int access,
                       String name, String desc, String[] signature, Object value) {
        super(Opcodes.ASM4);
        this.constantPool = constantPool;

        // Registers the Field in the Field constant pool.
        field = constantPool.addFieldToConstantPool(name, desc, classOwningName, access,
                signature, value);
    }


    // -----------------------------------------------------
    // Getters and Setters.
    // -----------------------------------------------------

    /**
     * Returns the Field managed by the FieldWriter.
     * @return the Field managed by the FieldWriter.
     */
    public Field getField() {
        return field;
    }


    // -----------------------------------------------------
    // Visitor methods.
    // -----------------------------------------------------

    @Override
    public AnnotationVisitor visitAnnotation(String desc, boolean visible) {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          FieldWriter : visitAnnotation(). "
                               + "Desc = " + desc
                               + ", visible = " + visible
                              );
        }

        // Creates an AnnotationItem, incomplete for now (no Elements), and registers it to the Class.
        // We do not add it to the Constant Pool now because of this incompleteness.
        AnnotationWriter annotationWriter = AnnotationWriter.createAnnotationWriter(desc, visible, constantPool, null);
        field.addAnnotationItem(annotationWriter.getAnnotationItem());

        return annotationWriter;
    }

    @Override
    public void visitEnd() {
        if (ApplicationWriter.DISPLAY_WRITER_INFORMATION) {
            System.out.println("          FieldWriter : visitEnd().");
        }

        // Now that the annotation_set_item are fully known, we can register it to the Constant Pool.
        // Nothing happens if the Field doesn't contain any annotation_set_items.
        constantPool.addAnnotationSetItemToConstantPool(field.getAnnotationSetItem());
    }

    @Override
    public void visitAttribute(Object attr) {
        // This method has no effect, as Attributes are not supported by AsmDex.
    }

}
