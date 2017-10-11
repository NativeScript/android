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

package org.ow2.asmdex.tree;

import java.util.ArrayList;
import java.util.List;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.ApplicationVisitor;
import org.ow2.asmdex.ClassVisitor;

/**
 * A node that represents an Application.
 *
 * @author Julien Névo, based on the ASM framework.
 */
public class ApplicationNode extends ApplicationVisitor {

    /**
     * The Classes of this Application.
     */
    public List<ClassNode> classes = new ArrayList<ClassNode>();

    /**
     * The runtime visible annotations of this class, field or method.
     */
    public List<AnnotationNode> visibleAnnotations;

    /**
     * The runtime invisible annotations of this class, field or method.
     */
    public List<AnnotationNode> invisibleAnnotations;

    /**
     * Constructs a new {@link ApplicationNode}.
     * @param api the API level
     */
    public ApplicationNode(int api) {
        super(api);
    }
    // ------------------------------------------------------
    // Visitor Methods.
    // ------------------------------------------------------

    @Override
    public void visit() {
    }

    @Override
    public ClassVisitor visitClass(int access, String name, String[] signature,
                                   String superName, String[] interfaces) {

        ClassNode cn = new ClassNode(access, name, signature, superName, interfaces);
        classes.add(cn);

        return cn;
    }


    // ------------------------------------------------------------------------
    // Accept method.
    // ------------------------------------------------------------------------

    /**
     * Makes the given Application visitor visit this Application.
     *
     * @param av an Application visitor.
     */
    public void accept(final ApplicationVisitor av) {

        av.visit();

        for (ClassNode n : classes) {
            ClassVisitor cv = av.visitClass(n.access, n.name, getStringArray(n.signature), n.superName, getStringArray(n.interfaces));
            n.accept(cv);
        }

        // visits end
        av.visitEnd();
    }


    // ------------------------------------------------------------------------
    // Static method.
    // ------------------------------------------------------------------------

    /**
     * Returns an array of String from a list of Strings, or Null if the list if Null.
     * @param list the list to convert.
     * @return an array of String from a list of Strings, or Null if the list if Null.
     */
    public static String[] getStringArray(List<String> list) {
        String[] array = null;

        if (list != null) {
            int size = list.size();
            array = new String[size];
            for (int i = 0; i < size; i++) {
                array[i] = list.get(i);
            }
        }

        return array;
    }

    /**
     * Visits an annotation of this class, field or method.
     *
     * @param desc the class descriptor of the annotation class.
     * @param visible <tt>true</tt> if the annotation is visible at runtime.
     * @return a visitor to visit the annotation values.
     */
    public AnnotationVisitor visitAnnotation(final String desc, final boolean visible) {
        AnnotationNode an = new AnnotationNode(desc);
        if (visible) {
            if (visibleAnnotations == null) {
                visibleAnnotations = new ArrayList<AnnotationNode>(1);
            }
            visibleAnnotations.add(an);
        } else {
            if (invisibleAnnotations == null) {
                invisibleAnnotations = new ArrayList<AnnotationNode>(1);
            }
            invisibleAnnotations.add(an);
        }
        return an;
    }

}
