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


/**
 *  Provides a small and fast bytecode manipulation framework for Android DEX format.
 *  <p>
 *  As the ASM framework, the ASMDEX framework is organized around  the {@link org.ow2.asmdex.ApplicationVisitor ApplicationVisitor}, the {@link org.ow2.asmdex.ClassVisitor ClassVisitor},
 *  {@link org.ow2.asmdex.FieldVisitor FieldVisitor}, {@link org.ow2.asmdex.MethodVisitor MethodVisitor} and
 *  {@link org.ow2.asmdex.AnnotationVisitor AnnotationVisitor} abstract classes,
 *  which allow one to visit the fields, methods and annotations of a class,
 *  including the bytecode instructions of each method.
 *  <p>
 *  In addition to these main abstract classes, ASMDEX provides a
 *   {@link org.ow2.asmdex.ApplicationReader applicationReader} class, that can parse an existing classes.dex bundle and make a given visitor visit it. ASMDEX also provides a {@link org.ow2.asmdex.ApplicationWriter ApplicationWriter} class, which is a visitor that generates Java class files.
 */
package org.ow2.asmdex;
