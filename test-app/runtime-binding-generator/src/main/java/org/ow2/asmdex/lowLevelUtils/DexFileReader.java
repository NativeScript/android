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

package org.ow2.asmdex.lowLevelUtils;

import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.HashMap;

import org.ow2.asmdex.structureReader.ClassDefinitionItem;
import org.ow2.asmdex.structureReader.FieldIdItem;
import org.ow2.asmdex.structureReader.MethodIdItem;

/**
 * Reads a DEX file, builds a representation of this file, and implements
 * primitive Dalvik methods in order to conveniently parse the file structure.
 *
 * FIXME In order to optimize, the low level code is duplicated from DalvikValueReader
 * to avoid indirection.
 *
 * @author Julien Névo, based on the work by Pierre Crégut.
 */
public class DexFileReader extends BasicDexFileReader implements IDalvikValueReader {

    /**
     * Content of the Reader.
     */
    private byte[] contents;

    /**
     * position
     */
    protected int pos = 0;

    /**
     * Takes an input stream corresponding to a Dex file and populate the structure.
     * @param dexBytes bytes of the Dex file to read.
     * @throws IOException
     */
    @Override
    public void parse(byte[] dexBytes) throws IOException {
        super.parse(dexBytes);
        contents = dexBytes;
    }

    // --------------------------------------
    // Structural helper methods
    // --------------------------------------

    /**
     * Fill the returned HashMap with the index of the fields found in one of the structures
     * which offset is given. These structures can be a field_annotation, a
     * method_annotation, or a parameter_annotation. This method works because they
     * all have the exact same structure.
     * The dex file reader must point on the beginning of the list of annotations
     * (field_annotation, method_annotation, or a parameter_annotation).
     * @param nbFields count of fields.
     * @return a map filled with the index linked to its offset.
     */
    public HashMap<Integer, Integer> fillOffsetHashMap(int nbFields) {
        HashMap<Integer, Integer> map = new HashMap<Integer, Integer>(nbFields);
        for (int i = 0; i < nbFields; i++) {
            map.put(uint(), uint());
        }

        return map;
    }

    /**
     * Gets the String related to string_id_item table, according to the string index given.
     * @param index index of the string to find.
     * @return the string linked to the string_id_item table, at the index given.
     */
    public String getStringItemFromStringIndex(int index) {
        return constantPoolStrings[index];
    }

    /**
     * Gets the String related to the type_id_item table, linked to the string_id_item
     * table, according to the type index given.
     * The dex file reader previous position <i>is</i> saved.
     * @param index index of the string to find.
     * @return the string linked to the type_id_item table, at the index given.
     */
    public String getStringItemFromTypeIndex(int index) {
        return constantPoolTypes[index];
    }

    /**
     * Gets the Name, third field of the method_id_item table,
     * according to the method index given.
     * The dex file reader previous position <i>is</i> saved.
     * @param index index of the name to find.
     * @return the name of the <i>index</i> element of the method_id_item table.
     */
    public String getNameFromMethodIndex(int index) {
        int saveReaderPosition = getPos();
        seek(getOffsetMethodIdItem(index) + 4); // + 4 to skip class_idx and type_idx.
        String result = getStringItemFromStringIndex(uint());
        seek(saveReaderPosition);
        return result;
    }

    /**
     * Gets the String related to the shorty_idx item in the proto_id_item table,
     * according to the proto index given.
     * The dex file reader previous position <i>is</i> saved.
     * @param index index of the Prototype to find.
     * @return the string linked to the shorty_idx item in the proto_id_item table, at the index given.
     */
    public String getShortyStringFromProtoIndex(int index) {
        int saveReaderPosition = getPos();
        seek(getOffsetProtoIdItem(index));
        String result = getStringItemFromStringIndex(uint());
        seek(saveReaderPosition);
        return result;
    }

    /**
     * Gets the Name, third field of the field_it_item table,
     * according to the field index given.
     * The dex file reader previous position <i>is</i> saved.
     * @param index index of the name to find.
     * @return the name of the <i>index</i> element of the field_id_item table.
     */
    public String getNameFromFieldIndex(int index) {
        int saveReaderPosition = getPos();
        seek(getOffsetFieldIdItem(index) + 4); // + 4 to skip class_idx and type_idx.
        String result = getStringItemFromStringIndex(uint());
        seek(saveReaderPosition);
        return result;
    }

    /**
     * Gets the Type Name, second field of the field_it_item table,
     * linked to the type_id list, and according to the field index given.
     * The dex file reader previous position <i>is</i> saved.
     * @param index index of the name to find.
     * @return the type name of the <i>index</i> element of the field_id_item table.
     */
    public String getTypeNameFromFieldIndex(int index) {
        int saveReaderPosition = getPos();
        seek(getOffsetFieldIdItem(index));
        skipShort();  // Skip class_idx.
        String result = getStringItemFromTypeIndex(ushort());
        seek(saveReaderPosition); // Also saved by the subroutine...
        return result;
    }

    /**
     * Returns all the type_ids indexes of a type_list structure.
     * The dex file reader <i>must</i> point at the beginning of a type_list structure.
     * The reader current position <i>will</i> be changed.
     * @return an array containing all the indexes to the type_id of the type_list structure.
     */
    public int[] getTypeIdIndexesFromTypeList() {
        int nbEntries = uint();
        int[] indexes = new int[nbEntries];
        for (int i = 0; i < nbEntries; i++) {
            indexes[i] = ushort();
        }

        return indexes;
    }

    /**
     * Returns all the annotation item offsets of an annotation_set_item structure.
     * The dex file reader <i>must</i> point at the beginning of an annotation_set_item
     * structure. The reader current position <i>will</i> be changed.
     * @return all the annotation item offsets of an annotation_set_item structure.
     */
    public int[] getAnnotationItemOffsetsFromAnnotationSetItem() {
        int nbEntries = uint();
        int[] offsets = new int[nbEntries];
        for (int i = 0; i < nbEntries; i++) {
            offsets[i] = uint();
        }

        return offsets;
    }

    /**
     * Returns the descriptor of a prototype which id is given. The descriptor
     * is constructed from the return type and parameters of the structure.
     * The dex file reader position <i>is</i> saved.
     * @param prototypeIndex index of the prototype.
     * @return a String containing the descriptor of the prototype.
     */
    public String getDescriptorFromPrototypeIndex(int prototypeIndex) {
        int savedPosition = getPos();
        // Get to the proto_id_item structure.
        seek(getOffsetProtoIdItem(prototypeIndex));
        skipInt();	// Skip shorty_idx.
        // First, the return type.
        StringBuilder methodDescriptor = new StringBuilder();
        methodDescriptor.append(getStringItemFromTypeIndex(uint()));
        // Then, the parameters, if any.
        int parametersListOffset = uint();
        if (parametersListOffset != 0) {
            seek(parametersListOffset); // The offset points to the type_list structure.
            int[] parameterIndexes = getTypeIdIndexesFromTypeList();
            for (int paramIndex = 0, parameterIndexesSize = parameterIndexes.length; paramIndex < parameterIndexesSize; paramIndex++) {
                methodDescriptor.append(getStringItemFromTypeIndex(parameterIndexes[paramIndex]));
            }
        }

        seek(savedPosition);
        return methodDescriptor.toString();
    }

    /**
     * Returns a MethodIdItem structure containing the information related to
     * the method_id_item whose index is given.
     * The dex file reader position is <i>not</i> saved.
     * @param methodId index of the Method.
     * @return a MethodIdItem structure containing the information related to
     * the method_id_item whose index is given.
     */
    public MethodIdItem getMethodIdItem(int methodId) {
        seek(getOffsetMethodIdItem(methodId));
        return new MethodIdItem(ushort(), ushort(), uint());
    }

    /**
     * Gets a FieldIdItem structure containing the class_idx, type_idx and name_idx of the
     * Field which index is given.
     * The dex file reader previous position is <i>not</i> saved.
     * @param index index of the Field to find.
     * @return a FieldIdItem structure.
     */
    public FieldIdItem getFieldIdItem(int index) {
        seek(getOffsetFieldIdItem(index));
        return new FieldIdItem(ushort(), ushort(), uint());
    }

    /**
     * Returns a structure containing all the information of a class_def_item
     * structure, which Class index is given.
     * The dex file reader position <i>is</i> saved.
     * @param classIndex index of the Class.
     * @return a structure containing all the information of a class_def_item
     * 		   structure.
     */
    public ClassDefinitionItem getClassDefinitionItem(int classIndex) {
        return new ClassDefinitionItem(this, classIndex, getClassDefinitionOffset(classIndex));
    }


    // ----------------------------------------
    // Dalvik primitives
    // ----------------------------------------

    /*
    @Override
    public byte[] getContents() {
    	return reader.getContents();
    }

    @Override
    final public int getPos() {
    	return reader.pos;
    }

    @Override
    final public boolean hasMore() {
    	return reader.hasMore();
    }

    @Override
    final public byte sbyte() {
    	return reader.sbyte();
    }

    @Override
    final public void seek(int pos) {
    	reader.pos = pos;
    }

    @Override
    final public int sint() {
    	return reader.sint();
    }

    @Override
    final public long sizedLong(int sz) {
    	return reader.sizedLong(sz);
    }

    @Override
    final public long completeSignSizedLong(long l, int sz) {
    	return reader.completeSignSizedLong(l, sz);
    }

    @Override
    final public int sleb128() {
    	return reader.sleb128();
    }

    @Override
    final public short sshort() {
    	return reader.sshort();
    }

    @Override
    final public short ubyte() {
    	return reader.ubyte();
    }

    @Override
    final public int uint() {
    	return reader.uint();
    }

    @Override
    final public int uleb128() {
    	return reader.uleb128();
    }

    @Override
    final public long uleb128_16() {
    	return reader.uleb128_16();
    }

    @Override
    final public int uleb128_p1() {
    	return reader.uleb128_p1();
    }

    @Override
    public String unicodeString(int strSize) {
    	return reader.unicodeString(strSize);
    }

    @Override
    final public int ushort() {
    	return reader.ushort();
    }

    @Override
    public String utf8String() {
    	return reader.utf8String();
    }

    @Override
    final public void relativeSeek(int offset) {
    	reader.pos += offset;
    }

    @Override
    final public void skipInt() {
    	reader.pos += 4;
    }

    @Override
    final public void skipShort() {
    	reader.pos += 2;
    }

    @Override
    final public void skipByte() {
    	reader.pos += 1;
    }
    */


    // What follows is the duplicate code from DalvikValueReader.
    // We may want to comment it and uncomment the code above to get a cleaner code,
    // but slightly slower.

    /**
     * Reads an integer directly from an input stream. Usually used
     * while builder the value reader.
     * @param stream
     * @return an integer signed
     * @throws IOException
     */
    final public static int sint(InputStream stream) throws IOException {
        byte [] contents = new byte [4];
        if (stream.read(contents) != 4) {
            throw new RuntimeException("Cannot read integer");
        }
        return ((contents [0] & 0xFF) | ((contents[1] & 0xFF) << 8) | ((contents[2] & 0xFF) << 16) | ((contents[3] & 0xFF) << 24));
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sbyte()
     */
    final public byte sbyte()  {
        return contents[pos++];
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#ubyte()
     */
    final public short ubyte()  {
        return (short) (contents[pos++] & 0xff);
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sshort()
     */
    final public short sshort() {
        short v = (short) ((contents [pos] & 0xff) | ((contents[pos + 1] & 0xff) << 8));
        pos += 2;
        return v;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#ushort()
     */
    final public int ushort() {
        return ((contents [pos++] & 0xff) | ((contents[pos++] & 0xff) << 8));
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sint()
     */
    final public int sint() {
        int v = ((contents [pos] & 0xff)| ((contents[pos + 1] & 0xff) << 8) | ((contents[pos + 2] & 0xff) << 16) | ((contents[pos + 3] & 0xff) << 24));
        pos += 4;
        return v;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uint()
     */
    //final public long uint() {
    final public int uint() {
        // WARNING : int return whereas a long *should* be used (but slower !). For now, no problem...
        int v = ((contents [pos] & 0xff) | ((contents[pos + 1] & 0xff) << 8) | ((contents[pos + 2] & 0xff) << 16)) | ((contents[pos + 3] & 0xff) << 24);
        pos += 4;
        return v;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sleb128()
     */
    final public int sleb128() {
        int r, v;
        v = contents[pos++] & 0xff;
        r = v & 0x7f;
        if (v >= 0x80) {
            v = contents[pos++] & 0xff;
            r |= (v & 0x7f) << 7;
            if (v >= 0x80) {
                v = contents[pos++] & 0xff;
                r |= (v & 0x7f) << 14;
                if (v >= 0x80) {
                    v = contents[pos++] & 0xff;
                    r |= (v & 0x7f) << 21;
                    if (v >= 0x80) {
                        v = contents[pos++] & 0xff;
                        r |= (v & 0x7f) << 28;
                        if (v >= 0x80) {
                            throw new RuntimeException("Bad sleb128");
                        }
                    } else if ((v & 0x40) != 0) {
                        r |= 0xf0000000;
                    }
                } else if ((v & 0x40) != 0) {
                    r |= 0xffe00000;
                }
            } else if ((v & 0x40) != 0) {
                r |= 0xffffc000;
            }
        } else if ((v & 0x40) != 0) {
            r |= 0xffffff80;
        }
        return r;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uleb128()
     */
    //final public long uleb128(){
    final public int uleb128() {
        // WARNING : int returned whereas a long *should* be used (but slower !). For now, no problem...
        //long r;
        int r;
        int v;
        v = contents[pos++] & 0xff;
        r = v & 0x7f;
        if (v >= 0x80) {
            v = contents[pos++] & 0xff;
            r |= (v & 0x7f) << 7;
            if (v >= 0x80) {
                v = contents[pos++] & 0xff;
                r |= (v & 0x7f) << 14;
                if (v >= 0x80) {
                    v = contents[pos++] & 0xff;
                    r |= (v & 0x7f) << 21;
                    if (v >= 0x80) {
                        v = contents[pos++] & 0xff;
                        r |= (v & 0x7f) << 28;
                        if (v >= 0x80) {
                            throw new RuntimeException("Bad uleb128");
                        }
                    }
                }
            }
        }
        return r;

    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uleb128_p1()
     */
    //final public long uleb128_p1() {
    final public int uleb128_p1() {
        return uleb128() - 1;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uleb128_16()
     */
    final public long uleb128_16() {
        long r;
        int v;
        v = ushort();
        r = v & 0x7fff;
        if (v > 0x8000) {
            v = ushort();
            r |= (v & 0x7fff) << 15;
        }
        return r;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sizedLong(int)
     */
    final public long sizedLong(int sz) {
        long result = 0;
        int length = sz + 1;
        for (int i=0; i < length; i++) {
            short v = ubyte();
            result = result | (long)v << (8*i); // v must be cast, else we may lose data !
        }

        return result;
    }

    /**
     * Extends a long read with SizedLong of length sz according to its sign.
     * @param l
     * @param sz size-1 of the encoded number.
     * @return a long
     */
    final public long completeSignSizedLong(long l, int sz) {
        sz++;
        int shift = (8 - sz) * 8;
        return l << shift >> shift; // Generates the sign.
    }

    /**
     * Reads a given number of bytes.
     * @param b
     */
    public void bytes(byte [] b) {
        for (int i=0; i < b.length; i++) {
            b[i] = contents[pos+i];
        }
        pos += b.length;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#utf8String()
     */
    public String utf8String() {
        StringBuilder buf = new StringBuilder();
        int c;
        int v;
        while ( (c = (contents[pos++] & 0xff)) != 0) {
            if ((c & 0x80) == 0x80) {
                if ((c & 0xe0) == 0xc0) {
                    c &= 0x1f;
                    v = contents[pos++] & 0x3f;
                    c =  c << 6 | v;
                } else if ((c & 0xf0) == 0xe0) {
                    v = contents[pos++] & 0x3f;
                    c = c << 6 | v;
                    v = contents[pos++] & 0x3f;
                    c = c << 6 | v;
                } else {
                    System.out.println("Bad (point 4) UTF 8 " + Integer.toBinaryString(c));
                }
            }
            buf.append((char) c);
        }
        return buf.toString();
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#seek(int)
     */
    final public void seek(int pos) {
        this.pos = pos;
    }

    public void relativeSeek(int offset) {
        pos += offset;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#getPos()
     */
    final public int getPos() {
        return pos;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#unicodeString(int)
     */
    public String unicodeString(int strSize) {
        char [] content = new char [strSize];
        for (int i=0; i < strSize; i++) {
            content[i] = (char) ushort();
        }
        int c;
        if ((c = ushort()) != 0) {
            System.out.println("Did not find the ending character\n " + Arrays.toString(content) + " " + c );
        }
        return new String(content);
    }


    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#hasMore()
     */
    final public boolean hasMore() {
        return pos < contents.length;
    }

    // For debug only - not exposed.
    int peek(int i) {
        return ((int) contents[i]) & 0xff;
    }

    @Override
    final public void skipInt() {
        pos += 4;
    }

    @Override
    final public void skipShort() {
        pos += 2;
    }

    @Override
    final public void skipByte() {
        pos += 1;
    }

    @Override
    public byte[] getContents() {
        return contents;
    }

}
