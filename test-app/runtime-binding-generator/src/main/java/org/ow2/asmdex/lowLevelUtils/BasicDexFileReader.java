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
import java.util.Arrays;

/**
 * Reads the basic structure of a DEX file and builds a representation of this file.
 *
 * @author Pierre Crégut
 * @author Julien Névo (little modifications)
 */
public class BasicDexFileReader {

    /**
     * Constant representing the absence of an index in a table especially for super class.
     */
    public final static int NO_INDEX = 0xFFFFFFFF;

    /**
     * The stream of code.
     */
    protected DalvikValueReader reader;

    private byte[] magic = new byte[8];

    private static final byte[] MAGIC_TEMPLATE =
    { 0x64,  0x65,  0x78,  0x0a,  0x30,  0x33,  0x35,  0x00 };

    // Offset of the format version number in the sequence above.
    private static final int OFFSET_FORMAT_VERSION_NUMBER_IN_MAGIC_SEQUENCE = 4;
    private static final int LENGTH_FORMAT_VERSION_NUMBER_IN_MAGIC_SEQUENCE = 3;

    /**
     * Offset to the file size field
     */
    protected final static int FILE_SIZE_OFFSET = 32;
    /**
     * Header size
     */
    protected final static int HEADER_NOMINAL_SIZE = 0x70;
    /**
     * Standard endianess marker
     */
    protected final static int STANDARD_ENDIAN_VALUE = 0x12345678;
    /**
     * Reverse endian marker
     */
    protected final static int REVERSE_ENDIAN_VALUE = 0x78563412;

    /**
     * ADLER 32 checksum of the DEX file
     */
    public long adler32Checksum;

    /**
     * Signature of the DEX file
     */
    public byte[] signature = new byte[20];

    /**
     * Standard or reverse endianess
     */
    protected boolean isStandardEndian;

    /**
     * Format version number
     */
    public byte[] formatVersionNumber = new byte[LENGTH_FORMAT_VERSION_NUMBER_IN_MAGIC_SEQUENCE]; // Set from the Magic array.

    /**
     * Size of the link table (but undefined otherwise)
     */
    public int linkTableSize;

    /**
     * Size of string ids table
     */
    protected int stringIdsSize;
    /**
     * Offset of string ids table
     */
    protected int stringIdsOffset;

    /**
     * Size of type ids table
     */
    protected int typeIdsSize;
    /**
     * Offset of type ids table
     */
    protected int typeIdsOffset;

    /**
     * Size of prototypes ids table
     */
    protected int protoIdsSize;
    /**
     * Offset of prototype ids table
     */
    protected int protoIdsOffset;

    /**
     * Size of field ids table
     */
    protected int fieldIdsSize;
    /**
     * Offset of field ids table
     */
    protected int fieldIdsOffset;

    /**
     * Size of method ids table
     */
    protected int methodIdsSize;
    /**
     * Offset of method ids table
     */
    protected int methodIdsOffset;

    /**
     * Size of class definition table
     */
    protected int classDefinitionsSize; // Class_defs_size in the documentation.
    /**
     * Offset of class definition table
     */
    protected int classDefinitionsOffset;

    /**
     * The Strings Constant Pool.
     */
    protected String[] constantPoolStrings;

    /**
     * The Types Constant Pool.
     */
    protected String[] constantPoolTypes;

    /**
     * Constructor.
     */
    public BasicDexFileReader() {
    }

    /**
     * Takes a byte array corresponding to a Dex file and populate the structure.
     * @param dexBytes the code
     * @throws IOException
     */
    public void parse(byte[] dexBytes) throws IllegalArgumentException, IOException { /*, RefNotFoundException */
        // Get a DalvikValueReader on the input stream.
        reader = new DalvikValueReader(dexBytes, FILE_SIZE_OFFSET);
        readHeader();
        readStrings();
        readTypes();
    }

    /**
     * Reads the Header of the previously loaded Dex file.
     * @throws IllegalArgumentException
     */
    protected void readHeader() throws IllegalArgumentException {
        // Checks the Magic sequence at offset 0.
        reader.bytes(magic);
        if (!Arrays.equals(magic, MAGIC_TEMPLATE)) {
            throw new IllegalArgumentException("Unknown bytecode version");
        }

        // Copy the format version number from the Magic sequence.
        for (int i = 0; i < formatVersionNumber.length; i++) {
            formatVersionNumber[i] = magic[i + OFFSET_FORMAT_VERSION_NUMBER_IN_MAGIC_SEQUENCE];
        }

        adler32Checksum = reader.uint(); // Reads the Adler32 checksum.
        reader.bytes(signature); // Reads the 20-byte signature.

        reader.skipInt(); // Skips the file size, not needed.

        // Compare the Header size with its nominal value (according to documentation).
        int header_size = reader.sint();
        if (header_size != HEADER_NOMINAL_SIZE) {
            throw new IllegalArgumentException("Bad header size: " + Integer.toHexString(header_size));
        }

        // Check the validity of the Endian value.
        int endianTag = reader.sint();
        if (endianTag == STANDARD_ENDIAN_VALUE) {
            isStandardEndian = true;
        } else if (endianTag == REVERSE_ENDIAN_VALUE) {
            isStandardEndian = false;
        } else {
            throw new IllegalArgumentException("Unknown Endian Value.");
        }

        // Read the following sizes/offsets for various structures.
        linkTableSize = reader.sint();
        reader.skipInt(); /* link_off = */
        reader.skipInt(); /* map_off = */

        stringIdsSize = reader.sint();
        stringIdsOffset = reader.sint();

        typeIdsSize = reader.sint();
        typeIdsOffset = reader.sint();

        protoIdsSize = reader.sint();
        protoIdsOffset = reader.sint();

        fieldIdsSize = reader.sint();
        fieldIdsOffset = reader.sint();

        methodIdsSize = reader.sint();
        methodIdsOffset = reader.sint();

        classDefinitionsSize = reader.sint();
        classDefinitionsOffset = reader.sint();

        reader.skipInt(); /* data_size = */
        reader.skipInt(); /* data_off = */
    }

    /**
     * Reads the string index table, and populates constantPoolStrings.
     * The table must have its offset set before (by readHeader).
     */
    protected void readStrings() {
        reader.seek(stringIdsOffset); // Move to the table offset.
        int[] stringOffsets = new int[stringIdsSize];

        // Gets the offset of each Strings.
        for (int i = 0; i < stringIdsSize; i++) {
            stringOffsets[i] = reader.sint();
        }

        // Now gets each Strings and fills the ConstantPoolStrings.
        // This is done by parsing the "string_data_item" structure.
        // The size of the string is followed by the string itself.
        constantPoolStrings = new String[stringIdsSize];
        for (int i = 0; i < stringIdsSize; i++) {
            reader.seek(stringOffsets[i]);
            int size = reader.uleb128();
            String string = reader.utf8String();
            constantPoolStrings[i] = string;

            // Checks the String length validity.
            if (string.length() != size) {
                StringBuilder b = new StringBuilder();
                for (int k = stringOffsets[i]; k < reader.getPos(); k++) {
                    b.append(" ").append(Integer.toHexString(reader.peek(k)));
                }
                System.out.println("Mismatched size for string " + string + " " +
                                   size + " / " + string.length() + b);
            }
        }
    }

    /**
     * Reads the types index table, and populates constantPoolTypes.
     * The table must have its offset set before (by readHeader), and readStrings must have
     * been called.
     */
    protected void readTypes() {
        constantPoolTypes = new String[typeIdsSize];
        for (int i = 0; i < typeIdsSize; i++) {
            reader.seek(getOffsetTypeIdItem(i));
            int index = reader.uint();
            constantPoolTypes[i] = constantPoolStrings[index];
        }
    }


    // ------------------------------------
    // Some utility methods
    // ------------------------------------
    /**
     * Size of a class definition in the pool in bytes
     */
    public final static int CLASS_DEF_ITEM_SIZE = 8 * 4;
    /**
     * Size of a field definition in the pool in bytes
     */
    public final static int FIELD_ID_ITEM_SIZE = 8;
    /**
     * Size of a string decl in the pool in bytes
     */
    public final static int STRING_ID_ITEM_SIZE = 4;
    /**
     * Size of a type declaration in the pool in bytes
     */
    public final static int TYPE_ID_ITEM_SIZE = 4;
    /**
     * Size of a method declaration in the pool in bytes
     */
    public final static int METHOD_ID_ITEM_SIZE = 8;
    /**
     * Size of a prototype definition in the pool in bytes
     */
    public final static int PROTO_ID_ITEM_SIZE = 12;

    /**
     * Returns the offset (from the beginning of the file) of a Field Id Item
     * (field_id_item structure) whose index is given.
     * The Dex file header <i>must</i> have been parsed.
     * @param itemIndex index of the Field Id Item.
     * @return the offset of the Field Id Item.
     */
    public int getOffsetFieldIdItem(int itemIndex) {
        return fieldIdsOffset + itemIndex * FIELD_ID_ITEM_SIZE;
    }

    /**
     * Returns the offset (from the beginning of the file) of a String Id Item
     * (string_id_item structure) whose index is given.
     * The Dex file header <i>must</i> have been parsed.
     * @param itemIndex index of the String Id Item.
     * @return the offset of the String Id Item.
     */
    public int getOffsetStringIdItem(int itemIndex) {
        return stringIdsOffset + itemIndex * STRING_ID_ITEM_SIZE;
    }

    /**
     * Returns the offset (from the beginning of the file) of a Type Id Item
     * (type_id_item structure) whose index is given.
     * The Dex file header <i>must</i> have been parsed.
     * @param itemIndex index of the Type Id Item.
     * @return the offset of the Type Id Item.
     */
    public int getOffsetTypeIdItem(int itemIndex) {
        return typeIdsOffset + itemIndex * TYPE_ID_ITEM_SIZE;
    }

    /**
     * Returns the offset (from the beginning of the file) of a Method Id Item
     * (method_id_item structure) whose index is given.
     * The Dex file header <i>must</i> have been parsed.
     * @param itemIndex index of the Method Id Item.
     * @return the offset of the Method Id Item.
     */
    public int getOffsetMethodIdItem(int itemIndex) {
        return methodIdsOffset + itemIndex * METHOD_ID_ITEM_SIZE;
    }

    /**
     * Returns the offset (from the beginning of the file) of a Prototype Id Item
     * (proto_id_item structure) whose index is given.
     * The Dex file header <i>must</i> have been parsed.
     * @param itemIndex index of the Method Id Item.
     * @return the offset of the Method Id Item.
     */
    public int getOffsetProtoIdItem(int itemIndex) {
        return protoIdsOffset + itemIndex * PROTO_ID_ITEM_SIZE;
    }

    /**
     * Returns the offset (from the beginning of the file) of a Class Id Item
     * (class_def_item structure) whose index is given.
     * The Dex file header <i>must</i> have been parsed.
     * @param itemIndex index of the Method Id Item.
     * @return the offset of the Method Id Item.
     */
    public int getClassDefinitionOffset(int itemIndex) {
        return classDefinitionsOffset + itemIndex * CLASS_DEF_ITEM_SIZE;
    }

    // ------------------------------------
    // Getters.
    // ------------------------------------
    /**
     * Endianess of the file
     * @return true if standard endian
     */
    public boolean isStandardEndian() {
        return isStandardEndian;
    }

    /**
     * Number of string ids in the pool
     * @return number
     */
    public int getStringIdsSize() {
        return stringIdsSize;
    }

    /**
     * Position of string table
     * @return position
     */
    public int getStringIdsOffset() {
        return stringIdsOffset;
    }

    /**
     * Number of type ids.
     * @return positive or null number
     */
    public int getTypeIdsSize() {
        return typeIdsSize;
    }

    /**
     * Position of type id table
     * @return positive or null number
     */
    public int getTypeIdsOffset() {
        return typeIdsOffset;
    }

    /**
     * Number of prototype ids
     * @return positive or null number
     */
    public int getProtoIdsSize() {
        return protoIdsSize;
    }

    /**
     * Position of prototype ids table
     * @return offset
     */
    public int getProtoIdsOffset() {
        return protoIdsOffset;
    }

    /**
     * Number of field declaration
     * @return positive or null number
     */
    public int getFieldIdsSize() {
        return fieldIdsSize;
    }

    /**
     * Position of the field table
     * @return offset
     */
    public int getFieldIdsOffset() {
        return fieldIdsOffset;
    }

    /**
     * Number of methods declared
     * @return positive or null number
     */
    public int getMethodIdsSize() {
        return methodIdsSize;
    }

    /**
     * Position of the method table
     * @return offset
     */
    public int getMethodIdsOffset() {
        return methodIdsOffset;
    }

    /**
     * Number of class declaration
     * @return positive or null number
     */
    public int getClassDefinitionsSize() {
        return classDefinitionsSize;
    }

    /**
     * Position of the class table
     * @return offset
     */
    public int getClassDefinitionsOffset() {
        return classDefinitionsOffset;
    }

    /**
     * Version of the format used by the file
     * @return enumeration
     */
    public byte[] getFormatVersionNumber() {
        return formatVersionNumber;
    }
}
