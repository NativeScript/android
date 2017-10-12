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

package org.ow2.asmdex.util;

import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.MethodVisitor;

/**
 * Class Adapter used to shift all the registers found in the bytecode.
 *
 * This is done by overriding the visitClass method of this ClassAdapter, then the visitMethod of the
 * MethodAdapter.
 *
 * So that we can sub-class RegisterShiftClassAdapter easily, instantiate it and give it to the
 * RegisterShiftApplicationAdapter, that will set it up through the setClassVisitor and setClassName
 * methods.
 *
 * @author Julien Névo
 */
public class RegisterShiftClassAdapter extends ClassVisitor {

    /**
     * Name of the current Class.
     */
    protected String className;

    /**
     * Empty constructor
     * @param api api level
     */

    public RegisterShiftClassAdapter(int api) {
        super(api, null);
    }

    /**
     * Sets the next Class Visitor.
     * @param cv the next Class Visitor.
     */
    public void setClassVisitor(ClassVisitor cv) {
        this.cv = cv;
    }

    /**
     * Sets the current Class name. It is useful to know if we need to shift the registers or not.
     * @param className the current Class name.
     */
    public void setClassName(String className) {
        this.className = className;
    }

    @Override
    public MethodVisitor visitMethod(int access, String name, String desc,
                                     String[] signature, String[] exceptions) {
        MethodVisitor mv = cv.visitMethod(access, name, desc, signature, exceptions);

        return new RegisterShiftMethodAdapter(api, mv, 0);
    }


}
