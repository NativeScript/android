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

import org.ow2.asmdex.encodedValue.EncodedValueArray;
import org.ow2.asmdex.structureWriter.AnnotationElement;
import org.ow2.asmdex.structureWriter.AnnotationItem;
import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * AnnotationWriter specialized for Arrays.
 *
 * @author Julien Névo
 */
public class AnnotationWriterArray extends AnnotationWriter {

    /**
     * Constructor
     * @param constantPool constant pool
     * @param annotationItem annotation to write
     */
    public AnnotationWriterArray(ConstantPool constantPool, AnnotationItem annotationItem) {
        super(constantPool, annotationItem);
    }

    @Override
    public void visit(String name, Object value) {
        visitManageNonDefaultAnnotation(name, value, true);
    }

    @Override
    public void visitEnd() {

        AnnotationElement annotationElement;
        if (mustSortArray) {
            EncodedValueArray encodedArray = new EncodedValueArray();
            for (AnnotationElement element : annotationItem.getAnnotationElements()) {
                encodedArray.addEncodedValue(element.getEncodedValue());
            }
            annotationElement = new AnnotationElement(currentName, encodedArray);
        } else {
            // Signature Array or String case. We already have a filled and unsorted array,
            // we simply add it to the current Annotation Item.
            annotationElement = new AnnotationElement(currentName, unsortedArray);
        }
        father.annotationItem.addAnnotationElement(annotationElement);

        visitEndRegisterAnnotationItemNotDefaultAnnotation();
    }
}
