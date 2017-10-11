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

import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;

/**
 * Simple class representing one class_def_item which data are read from a Dex file.
 *
 * @author Julien Névo
 */
public class ClassDefinitionItem {

    private int classIndex;
    private int accessFlags;
    private int superclassIndex;
    private int interfacesOffset;
    private int sourceFileIndex;
    private int annotationsOffset;
    private int classDataOffset;
    private int staticValuesOffset;

    /**
     * Constructor of a ClassDefinitionItem.
     * @param reader reader on the Dex file. Its position <i>is</i> saved.
     * @param classIndex index of the class.
     * @param classDefinitionOffset the offset of the class_def_item in bytes.
     */
    public ClassDefinitionItem(IDalvikValueReader reader, int classIndex, int classDefinitionOffset) {
        int savedPosition = reader.getPos();

        reader.seek(classDefinitionOffset);

        this.classIndex = reader.uint();
        accessFlags = reader.uint();
        superclassIndex = reader.uint();
        interfacesOffset = reader.uint();
        sourceFileIndex = reader.uint();
        annotationsOffset = reader.uint();
        classDataOffset = reader.uint();
        staticValuesOffset = reader.uint();

        reader.seek(savedPosition);
    }

    /**
     * Index of the class in the class pool
     * @return index
     */
    public int getClassIndex() {
        return classIndex;
    }
    /**
     * Access flags coded as a bit field
     * @return access flags as in java/dalvik spec
     */
    public int getAccessFlags() {
        return accessFlags;
    }
    /**
     * Index of super class in the class pool
     * @return index
     */
    public int getSuperclassIndex() {
        return superclassIndex;
    }
    /**
     * Position of interface definitions as an offset
     * @return absolute offset
     */
    public int getInterfacesOffset() {
        return interfacesOffset;
    }
    /**
     * Index of source file name in string pool
     * @return index
     */
    public int getSourceFileIndex() {
        return sourceFileIndex;
    }
    /**
     * Offset of annotation definitions for the class
     * @return offset
     */
    public int getAnnotationsOffset() {
        return annotationsOffset;
    }
    /**
     * Offset of class content
     * @return offset
     */
    public int getClassDataOffset() {
        return classDataOffset;
    }
    /**
     * Offset for declarations of static values.
     * @return offset
     */
    public int getStaticValuesOffset() {
        return staticValuesOffset;
    }

}
