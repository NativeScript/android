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
 * Some constants that aren't Opcodes.
 *
 * @author Julien Névo
 */
public interface Constants {

    /*
     * Internal names of some annotations.
     */
    /**
     * class name for annotation 'enclosing class'
     */
    public final static String ENCLOSING_CLASS_ANNOTATION_INTERNAL_NAME = "Ldalvik/annotation/EnclosingClass;";
    /**
     * class name for annotation 'enclosing method'
     */
    public final static String ENCLOSING_METHOD_ANNOTATION_INTERNAL_NAME = "Ldalvik/annotation/EnclosingMethod;";
    /**
     * class name for annotation 'inner class'
     */
    public final static String INNER_CLASS_ANNOTATION_INTERNAL_NAME = "Ldalvik/annotation/InnerClass;";
    /**
     * class name for annotation 'member class'
     */
    public final static String MEMBER_CLASSES_ANNOTATION_INTERNAL_NAME = "Ldalvik/annotation/MemberClasses;";
    /**
     * class name for annotation 'throws (exception)'
     */
    public final static String EXCEPTION_ANNOTATION_INTERNAL_NAME = "Ldalvik/annotation/Throws;";
    /**
     * class name for annotation 'signature'
     */
    public final static String SIGNATURE_ANNOTATION_INTERNAL_NAME = "Ldalvik/annotation/Signature;";
    /**
     * class name for annotation 'default'
     */
    public final static String ANNOTATION_DEFAULT_INTERNAL_NAME = "Ldalvik/annotation/AnnotationDefault;";

    /*
     * Name of some fields of the Annotations.
     */
    /**
     * annotation field 'value'
     */
    public final static String VALUE_STRING = "value";
    /**
     * Annotation field 'name'
     */
    public final static String NAME_STRING = "name";
    /**
     * Annotation field 'access flags'
     */
    public final static String ACCESS_FLAGS_STRING = "accessFlags";

    /*
     * Miscellaneous strings.
     */
    /**
     * This as a string
     */
    public final static String THIS_STRING = "this";
    /**
     * class name of strings
     */
    public final static String STRING_TYPE = "Ljava/lang/String;";
    /**
     * class name of objects
     */
    public final static String OBJECT_STRING = "Ljava/lang/Object;";

    /**
     * Value of what is considered a not calculated hashcode.
     */
    public final static int HASHCODE_NOT_CALCULATED = -1;

    /**
     * Offset of the offset of the map, in the header of a dex file.
     */
    public final static int MAP_OFFSET_IN_HEADER = 0x34;

}
