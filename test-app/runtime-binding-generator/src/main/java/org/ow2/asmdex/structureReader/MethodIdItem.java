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

package org.ow2.asmdex.structureReader;

/**
 * Simple class representing a method_id_item structure. It is only used to conveniently returns all
 * the data when reading a method_id_item, but nothing else.
 *
 * Only the classIndex, prototypeIndex and nameIndex are used by the ApplicationReader.
 * The Writer needs all the fields, though.
 *
 * @author Julien Névo
 */
public class MethodIdItem {

    private int classIndex;
    private int prototypeIndex;
    private int nameIndex;

    /**
     * Constructor of record type
     * @param classNameIndex class name index
     * @param prototypeIndex prototype index
     * @param methodNameIndex method name index
     */
    public MethodIdItem(int classNameIndex, int prototypeIndex, int methodNameIndex) {
        this.classIndex = classNameIndex;
        this.prototypeIndex = prototypeIndex;
        this.nameIndex = methodNameIndex;
    }

    /**
     * Accessor of class index in class pool
     * @return index
     */
    public int getClassIndex() {
        return classIndex;
    }
    /**
     * Accessor of prototype index in prototype pool
     * @return index
     */
    public int getPrototypeIndex() {
        return prototypeIndex;
    }
    /**
     * Accessor of name index in string pool
     * @return index
     */
    public int getNameIndex() {
        return nameIndex;
    }

}
