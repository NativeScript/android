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
 * A visitor to visit a Java annotation. The methods of this interface must be
 * called in the following order: (<tt>visit<tt> | <tt>visitEnum<tt> |
 * <tt>visitAnnotation<tt> | <tt>visitArray<tt> | <tt>visitClass<tt>)* <tt>visitEnd<tt>.
 * <br />
 * Differences with ASM :
 * <ul><li> visitClass is added. Is it useful to declare classes in annotations such as :<br />
 *   class<Short> a() <br />
 *   class<Short>[] b() <br />
 *   ASM used the visit method with a Type Object possessing a String (the class name) and
 *   an "Object" as an internal type ("sort" field). Since we don't implement Type in AsmDex,
 *   and in order to be more consistent with the other methods (visitArray, visitEnum...), we
 *   decided to add this visitClass method.</li></ul>
 *
 * @author Eric Bruneton
 * @author Eugene Kuleshov
 * @author Julien Névo (adaptation to AsmDex)
 */
public abstract class AnnotationVisitor {

    /**
     * The ASM API version implemented by this visitor.
     */
    final protected int api;

    /**
     * The annotation visitor to which this visitor must delegate method calls.
     * May be null.
     */
    protected AnnotationVisitor av;

    /**
     * Constructs a new {@link AnnotationVisitor}.
     *
     */
    public AnnotationVisitor(final int api) {
        this.api = api;
    }

    /**
     * Constructs a new {@link AnnotationVisitor}.
     *
     * @param av the annotation visitor to which this visitor must delegate
     *        method calls. May be null.
     */
    public AnnotationVisitor(final int api, final AnnotationVisitor av) {
        this.api = api;
        this.av = av;
    }
    /**
     * Visits a primitive value of the annotation.
     *
     * @param name the value name.
     * @param value the actual value, whose type must be {@link Byte},
     *        {@link Boolean}, {@link Character}, {@link Short},
     *        {@link Integer}, {@link Long}, {@link Float}, {@link Double},
     *        {@link String} or {@link java.lang.reflect.Type}. This value can also be an array
     *        of byte, boolean, short, char, int, long, float or double values
     *        (this is equivalent to using {@link #visitArray visitArray} and
     *        visiting each array element in turn, but is more convenient).
     */
    public void visit(String name, Object value) {
        if (av != null) {
            av.visit(name, value);
        }
    }

    /**
     * Visits an enumeration value of the annotation.
     *
     * @param name the value name.
     * @param desc the class descriptor of the enumeration class.
     * @param value the actual enumeration value.
     */
    public void visitEnum(String name, String desc, String value) {
        if (av != null) {
            av.visitEnum(name, desc, value);
        }
    }

    /**
     * Visits a nested annotation value of the annotation.
     *
     * @param name the value name.
     * @param desc the class descriptor of the nested annotation class.
     * @return a visitor to visit the actual nested annotation value, or
     *         <tt>null</tt> if this visitor is not interested in visiting
     *         this nested annotation. <i>The nested annotation value must be
     *         fully visited before calling other methods on this annotation
     *         visitor</i>.
     */
    public AnnotationVisitor visitAnnotation(String name, String desc) {
        if (av != null) {
            return av.visitAnnotation(name, desc);
        }
        return null;
    }

    /**
     * Visits an array value of the annotation. Note that arrays of primitive
     * types (such as byte, boolean, short, char, int, long, float or double)
     * can be passed as value to {@link #visit visit}. This is what
     * {@link ClassVisitor} does.
     *
     * @param name the value name.
     * @return a visitor to visit the actual array value elements, or
     *         <tt>null</tt> if this visitor is not interested in visiting
     *         these values. The 'name' parameters passed to the methods of this
     *         visitor are ignored. <i>All the array values must be visited
     *         before calling other methods on this annotation visitor</i>.
     */
    public AnnotationVisitor visitArray(String name) {
        if (av != null) {
            return av.visitArray(name);
        }
        return null;
    }

    /**
     * Visits a Class declaration inside an annotation (example : class<Short> a()).
     * @param annotationName the name of the annotation. Null if the visit of the Class is
     * 		  inside an Array.
     * @param className the name of the class.
     */
    public void visitClass(String annotationName, String className) {
        if (av != null) {
            av.visitClass(annotationName,className);
        }
    }

    /**
     * Visits the end of the annotation.
     */
    public void visitEnd() {
        if (av != null) {
            av.visitEnd();
        }
    }
}
