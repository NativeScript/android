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
import java.util.Arrays;
import java.util.List;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.FieldVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.Opcodes;

/**
 * A node that represents a class.
 *
 * @author Eric Bruneton
 * @author Julien Névo (adaptation to AsmDex)
 */
public class ClassNode extends ClassVisitor {

    /**
    * The class's access flags (see {@link org.ow2.asmdex.Opcodes}).
    */
    public int access;

    /**
     * The internal name of the class.
     */
    public String name;

    /**
     * The signature of the class. May be Null.
     */
    public List<String> signature;

    /**
     * The internal of name of the super class. For
     * interfaces, the super class is {@link Object}. May be <tt>null</tt>,
     * but only for the {@link Object} class.
     */
    public String superName;

    /**
     * The internal names of the class's interfaces. May be Null.
     */
    public List<String> interfaces;

    /**
     * The name of the source file from which this class was compiled. May be
     * <tt>null</tt>.
     */
    public String sourceFile;

    /**
     * The internal name of the enclosing class of the class. May be
     * <tt>null</tt>.
     */
    public String outerClass;

    /**
     * The name of the method that contains the class, or <tt>null</tt> if the
     * class is not enclosed in a method.
     */
    public String outerMethod;

    /**
     * The descriptor of the method that contains the class, or <tt>null</tt>
     * if the class is not enclosed in a method.
     */
    public String outerMethodDesc;

    /**
     * Informations about the inner classes of this class.
     */
    public List<InnerClassNode> innerClasses = new ArrayList<InnerClassNode>();

    /**
     * Informations about the member classes of this class.
     */
    public List<MemberClassNode> memberClasses = new ArrayList<MemberClassNode>();

    /**
     * The fields of this class.
     */
    public List<FieldNode> fields = new ArrayList<FieldNode>();

    /**
     * The methods of this class.
     */
    public List<MethodNode> methods = new ArrayList<MethodNode>();

    /**
     * The runtime visible annotations of this class, field or method.
     */
    public List<AnnotationNode> visibleAnnotations;

    /**
     * The runtime invisible annotations of this class, field or method.
     */
    public List<AnnotationNode> invisibleAnnotations;
    /**
     * Constructs a new {@link ClassNode}.
     */
    public ClassNode(int api) {
        super(api);
    }

    /**
     * Constructs a new {@link ClassNode}.
     */
    public ClassNode(int api, int access, String name, String[] signature, String superName, String[] interfaces) {
        super(api);
        this.access = access;
        this.name = name;
        if (signature != null) {
            this.signature = Arrays.asList(signature);
        }
        this.superName = superName;
        if (interfaces != null) {
            this.interfaces = Arrays.asList(interfaces);
        }
    }

    /**
     * Constructs a new {@link ClassNode}.
     */
    public ClassNode(int access, String name, String[] signature, String superName, String[] interfaces) {
        this(Opcodes.ASM4, access, name, signature, superName, interfaces);
    }

    // ------------------------------------------------------
    // Visitor Methods.
    // ------------------------------------------------------

    @Override
    public void visit(int version, int access, String name, String[] signature,
                      String superName, String[] interfaces) {
        // All the elements were already set when constructing this object. Nothing has to be
        // initialized once again.
    }

    @Override
    public FieldVisitor visitField(int access, String name, String desc,
                                   String[] signature, Object value) {
        FieldNode n = new FieldNode(access, name, desc, signature, value);
        fields.add(n);
        return n;
    }

    @Override
    public void visitInnerClass(String name, String outerName,
                                String innerName, int access) {
        InnerClassNode n = new InnerClassNode(name, outerName, innerName, access);
        innerClasses.add(n);
    }

    @Override
    public void visitMemberClass(String name, String outerName,
                                 String innerName) {
        MemberClassNode n = new MemberClassNode(name, outerName, innerName);
        memberClasses.add(n);
    }

    @Override
    public MethodVisitor visitMethod(int access, String name, String desc,
                                     String[] signature, String[] exceptions) {
        MethodNode n = new MethodNode(api, access, name, desc, signature, exceptions);
        methods.add(n);
        return n;
    }

    @Override
    public void visitOuterClass(String owner, String name, String desc) {
        outerClass = owner;
        outerMethod = name;
        outerMethodDesc = desc;
    }

    @Override
    public void visitSource(String source, String debug) {
        // The debug string is ignored by AsmDex.
        sourceFile = source;
    }


    // ------------------------------------------------------------------------
    // Accept method
    // ------------------------------------------------------------------------

    /**
     * Makes the given Class visitor visit this Class.
     *
     * @param cv a Class visitor.
     */
    public void accept(ClassVisitor cv) {

        cv.visit(0, access, name, ApplicationNode.getStringArray(signature), superName,
                 ApplicationNode.getStringArray(interfaces));

        // Visits source file.
        if (sourceFile != null) {
            cv.visitSource(sourceFile, null);
        }

        // Visits outer class.
        if (outerClass != null) {
            cv.visitOuterClass(outerClass, outerMethod, outerMethodDesc);
        }

        // Visits Annotations.
        int n;
        n = visibleAnnotations == null ? 0 : visibleAnnotations.size();
        for (int i = 0; i < n; ++i) {
            AnnotationNode an = (AnnotationNode) visibleAnnotations.get(i);
            an.accept(cv.visitAnnotation(an.desc, true));
        }
        n = invisibleAnnotations == null ? 0 : invisibleAnnotations.size();
        for (int i = 0; i < n; ++i) {
            AnnotationNode an = (AnnotationNode) invisibleAnnotations.get(i);
            an.accept(cv.visitAnnotation(an.desc, false));
        }

        // Visits Inner Classes.
        for (InnerClassNode icn : innerClasses) {
            icn.accept(cv);
        }

        // Visits Inner Classes.
        for (MemberClassNode icn : memberClasses) {
            icn.accept(cv);
        }

        // Visits Fields.
        for (FieldNode fn : fields) {
            fn.accept(cv);
        }

        // Visits Methods.
        for (MethodNode mn : methods) {
            mn.accept(cv);
        }

        // Visits end.
        cv.visitEnd();
    }

    /**
     * Visits an annotation of this class, field or method.
     *
     * @param desc the class descriptor of the annotation class.
     * @param visible <tt>true</tt> if the annotation is visible at runtime.
     * @return a visitor to visit the annotation values.
     */
    @Override
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
