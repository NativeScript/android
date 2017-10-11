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

import org.ow2.asmdex.Opcodes;

/**
 * A visitor to visit a Dex Application. The methods of this interface must be
 * called in the following order: <tt>visit<tt> (<tt>visitClass<tt>)* <tt>visitEnd<tt>.
 *
 * @author Julien Névo
 */
public abstract class ApplicationVisitor {

    /**
     * The application visitor to which this visitor must delegate method calls.
     * May be null.
     */
    protected ApplicationVisitor av;

    /**
     * The ASM API version implemented by this visitor.
     */
    final protected int api;

    /**
     * Constructs a new {@link ApplicationVisitor}.
     *
     */
    public ApplicationVisitor(final int api) {
        this.api = api;
    }

    /**
     * Constructs a new {@link ApplicationVisitor}.
     *
     * @param av the application visitor to which this visitor must delegate
     *        method calls. May be null.
     */
    public ApplicationVisitor(final int api, final ApplicationVisitor av) {
        this.api = api;
        this.av = av;
    }

    /**
     * Visits the header of the application.
     */
    public void visit() {
        if (av!= null) {
            av.visit();
        }
    }

    /**
     * Visits a class of the application. This method <i>must</i> return a new
     * {@link ClassVisitor} instance (or <tt>null</tt>) each time it is
     * called, i.e., it should not return a previously returned visitor.
     *
     * @param access the access flags of the class (see {@link Opcodes}).
     * @param name the internal name of the class.
     * @param signature the signature of this class. May be <tt>null</tt> if
     *        the class is not a generic one, and does not extend or implement
     *        generic classes or interfaces.
     * @param superName the internal of name of the super class. For interfaces,
     *        the super class is {@link Object}. May be <tt>null</tt>, but
     *        only for the {@link Object} class.
     * @param interfaces the internal names of the class's interfaces. May be
     *        <tt>null</tt>.
     * @return an object to visit the byte code of the application, or <tt>null</tt>
     *         if this application visitor is not interested in visiting the
     *         code of this class.

     */
    public ClassVisitor visitClass(
        int access,
        String name,
        String[] signature,
        String superName,
        String[] interfaces) {
        if (av!=null) {
            return av.visitClass(access, name, signature, superName, interfaces);
        }
        return null;
    }

    /**
     * Visits the end of the application. This method, which is the last one to be
     * called, is used to inform the visitor that the whole Application has been
     * visited.
     */
    public void visitEnd() {
        if (av!=null) {
            av.visitEnd();
        }
    }
}
