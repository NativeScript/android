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

package org.ow2.asmdex.specificAnnotationVisitors;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.Constants;

/**
 * Visitor for the InnerClass annotation (Ldalvik/annotation/InnerClass;).
 *
 * @author Julien Névo
 */
public class InnerClassAnnotationVisitor extends AnnotationVisitor {

    private int access;
    private String simpleName;

    /**
     * Constructor.
     * @param api the API level
     */
    public InnerClassAnnotationVisitor(int api) {
        super(api);
    }

    /**
     * Get the access flags as a bitset
     * @return integer coding the bitset
     */
    public int getAccess() {
        return access;
    }
    /**
     * Get the simple name
     * @return name
     */
    public String getSimpleName() {
        return simpleName;
    }

    @Override
    public void visit(String name, Object value) {
        if (name==null) {
            return;
        } else if (name.equals(Constants.NAME_STRING)) {
            simpleName = (String)value;
        } else if (name.equals(Constants.ACCESS_FLAGS_STRING) && value != null) {
            access = (Integer)value;
        }
    }

    @Override
    public AnnotationVisitor visitAnnotation(String name, String desc) {
        return null;
    }
    @Override
    public AnnotationVisitor visitArray(String name) {
        return null;
    }
    @Override
    public void visitEnd() { }
    @Override
    public void visitEnum(String name, String desc, String value) { }
    @Override
    public void visitClass(String annotationName, String className) { }
}
