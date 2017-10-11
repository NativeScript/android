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

/**
 * A visitor to visit a Dex class. The methods of this interface must be
 * called in the following order: <tt>visit</tt> [ <tt>visitSource</tt> ] [
 * <tt>visitOuterClass</tt> ] ( <tt>visitAnnotation</tt> |
 * <tt>visitAttribute</tt> )* (<tt>visitInnerClass</tt> |
 * <tt>visitField</tt> | <tt>visitMethod</tt> )* <tt>visitEnd</tt>.<br />
 * <br />
 * Changes with ASM :
 * <ul>
 * <li> visit : the parameters have no effect and are only given for compatibility with ASM,
 *   as the Class currently visited has its identity given by ApplicationVisitor.visitClass.
 *   Also, the signature is now an array of Strings, instead of just one String.</li>
 * <li> visitSource : the Debug information is <i>ignored</i> by AsmDex, and is only
 *   there for compatibility with ASM.</li>
 * <li> visitAttribute : this method has <i>no effect</i> in AsmDex, since Attributes are
 *   not supported.</li>
 * <li> The "deprecated" flag is not encoded in the access flags.</li>
 * </ul>
 *
 * Note :
 * <ul><li> In ASM, it seems visitOuterClass() is only called from an Inner class INSIDE a Method, not a Class.
 * </li></ul>
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public abstract class ClassVisitor {
    /**
     * he class visitor to which this visitor must delegate method calls. May
     * be null.
     */
    protected ClassVisitor cv;

    /**
     * The ASM API version implemented by this visitor.
     */
    final protected int api;

    /**
     * Constructs a new {@link ClassVisitor}.
     */
    public ClassVisitor(final int api) {
        this.api = api;
    }

    /**
     * Constructs a new {@link ClassVisitor}.
     *
     * @param cv the class visitor to which this visitor must delegate method
     *        calls. May be null.
     */
    public ClassVisitor(final int api, final ClassVisitor cv) {
        this.api = api;
        this.cv = cv;
    }

    /**
     * Visits the header of the class. <i>The information given here is only given
     * for compatibility with ASM. They are NOT used by AsmDex, as the Class
     * currently visited has its identity given by ApplicationVisitor.visitClass.</i>
     *
     * @param version the class version. <i>Ignored</i> by AsmDex, only here for
     * 		  compatibility with ASM.
     * @param access the class's access flags (see {@link Opcodes}). <i>Ignored</i>
     * 		  by AsmDex, only here for compatibility with ASM, as this parameter has
     *        already been filled by an {@link ApplicationVisitor}.
     * @param name the internal name of the class.
     *        <i>Ignored</i> by AsmDex, only here for compatibility with ASM, as
     *        this parameter has already been filled by an {@link ApplicationVisitor}.
     * @param signature the signature of this class. May be <tt>null</tt> if
     *        the class is not a generic one, and does not extend or implement
     *        generic classes or interfaces.
     *        <i>Ignored</i> by AsmDex, only here for compatibility with ASM, as
     *        this parameter has already been filled by an {@link ApplicationVisitor}.
     * @param superName the internal of name of the super class. For interfaces,
     *        the super class is {@link Object}. May be <tt>null</tt>, but
     *        only for the {@link Object} class.
     *        <i>Ignored</i> by AsmDex, only here for compatibility with ASM, as
     *        this parameter has already been filled by an {@link ApplicationVisitor}.
     * @param interfaces the internal names of the class's interfaces. May be
     *        <tt>null</tt>.
     *        <i>Ignored</i> by AsmDex, only here for compatibility with ASM, as
     *        this parameter has already been filled by an {@link ApplicationVisitor}.
     */
    public void visit(
        int version,
        int access,
        String name,
        String[] signature,
        String superName,
        String[] interfaces)     {
        if (cv != null) {
            cv.visit(version, access, name, signature, superName, interfaces);
        }
    }


    /**
     * Visits the source of the class.
     *
     * @param source the name of the source file from which the class was
     *        compiled. May be <tt>null</tt>.
     * @param debug additional debug information to compute the link
     *        between source and compiled elements of the class. May be
     *        <tt>null</tt>. This parameter is <i>ignored</i> by AsmDex, and is only
     *        there for compatibility with ASM.
     */
    public void visitSource(String source, String debug) {
        if (cv != null) {
            cv.visitSource(source, debug);
        }
    }

    /**
     * Visits the enclosing class of the class. This method must be called only
     * if the class has an enclosing class.
     *
     * @param owner internal name of the enclosing class of the class.
     * @param name the name of the method that contains the class, or
     *        <tt>null</tt> if the class is not enclosed in a method of its
     *        enclosing class.
     * @param desc the descriptor of the method that contains the class, or
     *        <tt>null</tt> if the class is not enclosed in a method of its
     *        enclosing class.
     */
    public void visitOuterClass(String owner, String name, String desc) {
        if (cv != null)  {
            cv.visitOuterClass(owner, name, desc);
        }
    }

    /**
     * Visits an annotation of the class.
     *
     * @param desc the class descriptor of the annotation class.
     * @param visible <tt>true</tt> if the annotation is visible at runtime.
     * @return a visitor to visit the annotation values, or <tt>null</tt> if
     *         this visitor is not interested in visiting this annotation.
     */
    public AnnotationVisitor visitAnnotation(
        String desc,
        boolean visible) {
        if (cv != null) {
            return cv.visitAnnotation(desc, visible);
        }
        return null;
    }

    /**
     * Visits a non standard attribute of the class. This method is
     * <i>ignored</i> by AsmDex, since attributes are not supported.
     *
     * @param attr an attribute.
     */
    public void visitAttribute(Object attr) {
        if (cv != null) {
            cv.visitAttribute(attr);
        }
    }

    /**
     * Visits information about an inner class. This inner class is not
     * necessarily a member of the class being visited.
     *
     * @param name the internal name of an inner class.
     * @param outerName the internal name of the class to which the inner class
     *        belongs. May be <tt>null</tt> for not member classes.
     * @param innerName the (simple) name of the inner class inside its
     *        enclosing class. May be <tt>null</tt> for anonymous inner
     *        classes.
     * @param access the access flags of the inner class as originally declared
     *        in the enclosing class.
     */
    public void visitInnerClass(
        String name,
        String outerName,
        String innerName,
        int access) {
        if (cv != null) {
            cv.visitInnerClass(name, outerName, innerName, access);
        }
    }

    /**
     * Visits information about a member class.
     *
     * @param outerName the internal name of the class to which the member class
     *        belongs.
     * @param innerName the (simple) name of the member class inside its
     *        enclosing class.
     */
    public void visitMemberClass(
        String name,
        String outerName,
        String innerName) {
        if (cv != null) {
            cv.visitMemberClass(name,outerName, innerName);
        }
    }
    /**
     * Visits a field of the class.
     *
     * @param access the field's access flags (see {@link Opcodes}). This
     *        parameter also indicates if the field is synthetic.
     * @param name the field's name.
     * @param desc the field's descriptor.
     * @param signature the field's signature. May be <tt>null</tt> if the
     *        field's type does not use generic types.
     * @param value the field's initial value. This parameter, which may be
     *        <tt>null</tt> if the field does not have an initial value, must
     *        be an {@link Integer}, a {@link Float}, a {@link Long}, a
     *        {@link Double} or a {@link String} (for <tt>int</tt>,
     *        <tt>float</tt>, <tt>long</tt> or <tt>String</tt> fields
     *        respectively). <i>This parameter is only used for static fields</i>.
     *        Its value is ignored for non static fields, which must be
     *        initialized through bytecode instructions in constructors or
     *        methods.
     * @return a visitor to visit field annotations and attributes, or
     *         <tt>null</tt> if this class visitor is not interested in
     *         visiting these annotations and attributes.
     */
    public FieldVisitor visitField(
        int access,
        String name,
        String desc,
        String[] signature,
        Object value) {
        if (cv != null) {
            return cv.visitField(access, name, desc, signature, value);
        }
        return null;
    }

    /**
     * Visits a method of the class. This method <i>must</i> return a new
     * {@link MethodVisitor} instance (or <tt>null</tt>) each time it is
     * called, i.e., it should not return a previously returned visitor.
     *
     * @param access the method's access flags (see {@link Opcodes}). This
     *        parameter also indicates if the method is synthetic.
     * @param name the method's name.
     * @param desc the method's descriptor.
     * @param signature the field's signature. May be <tt>null</tt> if the
     *        field's type does not use generic types.
     * @param exceptions the internal names of the method's exception classes.
     *        May be <tt>null</tt>.
     * @return an object to visit the byte code of the method, or <tt>null</tt>
     *         if this class visitor is not interested in visiting the code of
     *         this method.
     */
    public MethodVisitor visitMethod(
        int access,
        String name,
        String desc,
        String[] signature,
        String[] exceptions) {
        if (cv != null) {
            return cv.visitMethod(access, name, desc, signature, exceptions);
        }
        return null;
    }

    /**
     * Visits the end of the class. This method, which is the last one to be
     * called, is used to inform the visitor that all the fields and methods of
     * the class have been visited.
     */
    public void visitEnd() {
        if (cv != null) {
            cv.visitEnd();
        }
    }
}
