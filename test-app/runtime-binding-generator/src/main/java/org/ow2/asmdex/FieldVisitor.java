/* Software Name : AsmDex
 * Version : 1.0
 *
 * Copyright © 2000,2012 INRIA, France Télécom
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

/**
 * A visitor to visit a Dalvik field. The methods of this interface must be called
 * in the following order: ( <tt>visitAnnotation</tt> |
 * <tt>visitAttribute</tt> )* <tt>visitEnd</tt>.
 * <br /><br />
 * Changes with ASM :
 * <ul><li> visitAttribute has <i>no effect</i> and is only here for compatibility with ASM,
 * as Attributes are not supported by AsmDex.</li>
 * </ul>
 *
 * @author Julien Névo, based on the ASM Framework.
 *
 */
public abstract class FieldVisitor {

    /**
     * The field visitor to which this visitor must delegate method calls. May
     * be null.
     */
    protected FieldVisitor fv;

    /**
     * The ASM API version implemented by this visitor.
     */
    final protected int api;

    /**
     * Constructs a new {@link FieldVisitor}.
     *
     */
    public FieldVisitor(final int api) {
        this.api = api;
    }

    /**
     * Constructs a new {@link FieldVisitor}.
     *
     * @param fv the field visitor to which this visitor must delegate method
     *        calls. May be null.
     */
    public FieldVisitor(final int api, final FieldVisitor fv) {
        this.api  = api;
        this.fv = fv;
    }

    /**
     * Visits an annotation of the field.
     *
     * @param desc the class descriptor of the annotation class.
     * @param visible <tt>true</tt> if the annotation is visible at runtime.
     * @return a visitor to visit the annotation values, or <tt>null</tt> if
     *         this visitor is not interested in visiting this annotation.
     */
    public AnnotationVisitor visitAnnotation(String desc, boolean visible) {
        if (fv != null) {
            return fv.visitAnnotation(desc, visible);
        }
        return null;
    }

    /**
     * Visits a non standard attribute of the field. This method is <i>ignored</i>
     * by AsmDex, and is only here for compatibility with ASM, as Attributes are
     * not supported by AsmDex.
     *
     * @param attr an attribute.
     */
    public void visitAttribute(Object attr) {
        if (fv != null) {
            fv.visitAttribute(attr);
        }
    }

    /**
     * Visits the end of the field. This method, which is the last one to be
     * called, is used to inform the visitor that all the annotations and
     * attributes of the field have been visited.
     */
    public void visitEnd() {
        if (fv != null) {
            fv.visitEnd();
        }
    }
}
