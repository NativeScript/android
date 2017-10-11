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


/**
 * Tiny class to hold the name and value of an Default Annotation conveniently.
 * The description (class of the Annotation it belongs to) isn't stored here.
 *
 * @author Julien Névo
 */
public class DefaultAnnotationInformation {

    /**
     * The name of the field.
     */
    private String name;

    /**
     * The default value of the field.
     */
    private Object value;


    /**
     * Description of an enumeration item. Can be used a value if this is not a primitive object.
     * @author pierre
     *
     */
    static public class EnumInfo {

        /**
         * Description of the enum type
         */
        final public String enumDesc;
        /**
         * enumeration element.
         */
        final public String enumValue;

        /**
         * Constructor representing a statically encoded value from an enum type.
         * @param enumDesc the type description
         * @param enumValue the value
         */
        public EnumInfo(String enumDesc, String enumValue) {
            this.enumDesc = enumDesc;
            this.enumValue = enumValue;
        }
    }

    /**
     *
     * Representation of a value in an annotation that is class.
     */
    static public class ClassInfo {
        /**
         * The name of the class
         */
        final public String className;

        /**
         * Constructor for the value
         * @param className static name of the class.
         */
        public ClassInfo(String className) {
            this.className = className;
        }
    }

    /**
     * Constructor (record)
     * @param name
     * @param value
     */
    public DefaultAnnotationInformation(String name, Object value) {
        this.name = name;
        this.value = value;
    }

    /**
     * Returns the name of the field.
     * @return the name of the field.
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the default value of the field.
     * @return the default value of the field.
     */
    public Object getValue() {
        return value;
    }
}
