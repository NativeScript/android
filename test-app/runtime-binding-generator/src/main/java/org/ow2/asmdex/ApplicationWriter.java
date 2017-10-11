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

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.zip.Adler32;

import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;
import org.ow2.asmdex.structureWriter.AnnotationDirectoryItem;
import org.ow2.asmdex.structureWriter.AnnotationElement;
import org.ow2.asmdex.structureWriter.AnnotationItem;
import org.ow2.asmdex.structureWriter.AnnotationSetItem;
import org.ow2.asmdex.structureWriter.AnnotationSetRefList;
import org.ow2.asmdex.structureWriter.ClassDefinitionItem;
import org.ow2.asmdex.structureWriter.CodeItem;
import org.ow2.asmdex.structureWriter.ConstantPool;
import org.ow2.asmdex.structureWriter.Field;
import org.ow2.asmdex.structureWriter.IAnnotationsHolder;
import org.ow2.asmdex.structureWriter.Method;
import org.ow2.asmdex.structureWriter.Prototype;
import org.ow2.asmdex.structureWriter.TypeList;

/**
 * A {@link ApplicationVisitor} that generates an Android dex file in
 * bytecode form. More precisely this visitor generates a byte array conforming
 * to the dex file format. It can be used alone, to generate Android classes
 * "from scratch", or with one or more {@link ApplicationReader ApplicationReader}
 * and adapter Application visitor to generate a modified class from one or more existing
 * Java applications.<br /><br />
 *
 * KNOWN ISSUES
 * <ul><li> The line number management from the beginning to the "data" for parse/sparse switch/
 *   fillArray till the end is wrong. It can't be fixed properly. It shouldn't matter much, as it
 *   only happen if two or more switches/fillArray are encoded, and if there are line number for
 *   each. Only the last one will be encoded, at the beginning of the first structure.</li>
 * </ul>
 *
 * DIFFERENCES WITH THE DX BYTECODE :
 * <ul><li> TypeList are correctly sorted, but dx put the Annotations TypeList before the others.</li>
 *
 * <li> AnnotationItems are correctly sorted, but dx seems to put the Throws exception before the others.
 *   I don't do that, as my collected is ordered naturally.</li>
 *
 * <li> Annotation_directory_item are not sorted by increasing offset.</li>
 *
 * <li> First line number isn't perfectly accurate, because we don't have it through the ASM interface.
 *   So the first one found becomes the first one in the debug_info_item header. It is not perfectly
 *   accurate, because it doesn't represent the first actual line number of the method. However, it
 *   works fine and allows to get the same file size.</li>
 * </ul>
 *
 * NOTES:
 * <ul><li> The management of several Local Variable that uses the SAME register number isn't accurate, but won't
 *   be improved (is it useful ?). Each End/Restart is linked to the last Local Variable found (with the
 *   right register number), till a new one is added. Works fine for all the cases found.
 *
 * <li> The "type_id" doesn't exist as a Class, because it only contains a string_id. Structures
 *   referring to "type_id" directly refers to a String (added to the Constant Pool as
 *   both String and Type). So there is a Type structure, but only present in the Constant Pool.
 *
 * <li> The Debug Info.
 *   <ul><li> We only know the instruction addresses when actually writing the Dex file (except for the
 *     Jump Instruction, but they might change if an Instruction must be resized). So we can't
 *     write the DBG_ADVANCE_PC/Special Opcodes as we gather the Instructions. It is simpler to write the
 *     Debug Instructions along with writing effectively the Code Item.</li>
 *   <li> The Line Number are added to the Instructions directly.</li>
 *   <li> We can get the Line_start if the first instruction has a line number. Else, we consider it is 1.</li>
 *   </ul>
 * </ul>
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class ApplicationWriter extends ApplicationVisitor {

    /**
     * The Application Constant Pool.
     */
    private ConstantPool constantPool = new ConstantPool();

    /**
     * Buffer representing our output Dex file.
     */
    private ByteVector out;

    /**
     * The ApplicationReader that may be linked to this ApplicationWriter. This is only used for
     * the optimization that consists in copying the Constant Pool from the ApplicationReader and
     * also the methods if they aren't going to be modified by an Adapter.
     */
    private ApplicationReader applicationReader;

    /**
     * Indicates if the writer information must be displayed.
     */
    public static final boolean DISPLAY_WRITER_INFORMATION = false;

    /**
     * Indicates if the writer must encode the Debug information that it is given.
     * If the flag is true, the debug_info_items are not encoded at all, which is
     * different from the SKIP_DEBUG from ApplicationReader that just doesn't emit
     * {@link MethodVisitor#visitLocalVariable visitLocalVariable} and
     * {@link MethodVisitor#visitLineNumber visitLineNumber}, but has no power over the
     * creation of the debug_info_items.
     */
    public static final boolean SKIP_DEBUG_INFO_ITEMS = false;

    /*
     * Offsets of the structures. Only computed when the parsing of the Application has been done.
     */
    private int stringIdsOffset = HEADER_NOMINAL_SIZE; // The strings always come just after the header.
    private int typeIdsOffset;
    private int prototypeIdsOffset;
    private int fieldIdsOffset;
    private int methodIdsOffset;
    private int classDefinitionsOffset;
    private int dataOffset;

    private int annotationSetRefListOffset; // Offset of the first annotation_set_ref_list in the data section.
    private int annotationItemsOffset; // Offset of the first annotation_item in the data section.
    private int annotationSetItemOffset; // Offset of the first annotation_set_item in the data section.
    private int annotationDirectoryItemsOffset; // Offset of the first annotation_directory_item in the data section.
    private int encodedArrayItemsOffset; // Offset of the first array_item in the data section.
    private int debugInfoItemOffset; // Offset of the first debug_info_item in the data section.
    private int typeListOffset;
    private int stringDataOffset;
    private int classDataItemOffset; // Offset of the first class_data_item, in the data section.
    private int codeItemOffset; // Offset of the first code_item, in the data section.

    private int debugInfoItemCount = 0; // Global count of the debug_info_items in the data section.
    private int encodedArrayItemsCount = 0; // Global count of the array_items in the data section.
    private int classDataItemCount = 0; // Global count of the class_data_items in the data section.
    private int codeItemCount; // Count of the Code Items of this application.

    /**
     * The Dex_File_Magic number.
     */
    private static final byte[] dexFileMagic = new byte[] {0x64, 0x65, 0x78, 0xa, 0x30, 0x33, 0x35, 0x00 };

    private static final int ADLER_OFFSET = 8; // offset in bytes of the Adler32 Checksum.
    private static final int ADLER_SIZE = 4; // Size in bytes of the Adler32 Checksum.

    private static final int SHA1_SIGNATURE_OFFSET = 12; // offset in bytes of the SHA1 Signature.
    private static final int SHA1_SIGNATURE_SIZE = 20; // Size in bytes of the SHA1 Signature.

    private static final int HEADER_NOMINAL_SIZE = 0x70; // Fixed in documentation.
    private static final int STANDARD_ENDIAN_VALUE = 0x12345678;
    private static final int MAP_OFFSET_OFFSET = 0x34; // Offset of the offset of the Map, in header_item.
    private static final int FILE_SIZE_OFFSET = 0x20; // Offset of the file_size item, in header_item.


    // Sizes in bytes of the different atomic structures linking to the Data sections.
    private static final int STRING_ID_ITEM_SIZE = 4;
    private static final int TYPE_ID_ITEM_SIZE = 4;
    private static final int PROTO_ID_ITEM_SIZE = 4 * 3;
    private static final int FIELD_ID_ITEM_SIZE = 2 * 2 + 4;
    private static final int METHOD_ID_ITEM_SIZE = 2 * 2 + 4;
    private static final int CLASS_DEF_ITEM_SIZE = 8 * 4;


    /**
     * Constructs an new {@link ClassWriter}.
     */
    public ApplicationWriter() {
        super(Opcodes.ASM4);
    }

    /**
     * Constructs a new {@link ClassWriter} object and enables optimizations for
     * "mostly add" bytecode transformations. These optimizations are the
     * following:
     *
     * <ul> <li>The constant pool from the original application is copied as is in
     * the new application, which saves time. New constant pool entries will be added
     * if necessary, but unused constant pool entries <i>won't be
     * removed</i>.</li> <li>Methods that are not transformed are copied as
     * is in the new application, directly from the original application bytecode (i.e.
     * without emiting visit events for all the method instructions), which
     * saves a <i>lot</i> of time. Untransformed methods are detected by the
     * fact that the {@link ApplicationReader} receives {@link MethodVisitor} objects
     * that come from a {@link ApplicationWriter} (and not from any other {@link ApplicationVisitor} instance).</li>
     * </ul>
     *
     * @param applicationReader the {@link ApplicationReader} used to read the original
     *        application. It will be used to copy the entire constant pool from the
     *        original application and also to copy other fragments of original
     *        bytecode where applicable.
     */
    public ApplicationWriter(final ApplicationReader applicationReader) {
        super(Opcodes.ASM4);
        applicationReader.copyPool(this);
        this.applicationReader = applicationReader;
    }


    // ------------------------------------------------------------------------
    // Implementation of the ClassVisitor interface
    // ------------------------------------------------------------------------

    @Override
    public void visit() {
        if (DISPLAY_WRITER_INFORMATION) {
            System.out.println("ApplicationWriter : visit.");
        }
    }

    @Override
    public ClassVisitor visitClass(int access, String name, String[] signature,
                                   String superName, String[] interfaces) {
        if (DISPLAY_WRITER_INFORMATION) {
            System.out.print("ApplicationWriter : visitClass. Acces = " + access
                             + ", name = " + name
                             + ", SuperClassName = " + superName
                            );

            if (interfaces != null) {
                System.out.print(". Interfaces = ");
                for (int i = 0; i < interfaces.length; i++) {
                    System.out.print(interfaces[i] + ", ");
                }
            }
            if (signature != null) {
                System.out.print(". Signature = ");
                for (int i = 0; i < signature.length; i++) {
                    System.out.print(signature[i] + ", ");
                }
            }
            System.out.println();
        }

        return new ClassWriter(this, constantPool, access, name, signature, superName, interfaces);
    }

    @Override
    public void visitEnd() {
        if (DISPLAY_WRITER_INFORMATION) {
            System.out.println("ApplicationWriter : visitEnd.");
        }

        generateDexFile();
    }


    // ------------------------------------------------------------------------
    // Other public methods
    // ------------------------------------------------------------------------

    /**
     * Returns the bytecode of the class that was built with this
     * application writer. The application Dex file must have been parsed before.
     *
     * @return the bytecode of the class that was build with this application
     * writer, or Null if the application has not been parsed yet.
     */
    public byte[] toByteArray() {

        if (out == null) {
            return null;
        }

        return out.getData();

    }


    // ------------------------------------------------------------------------
    // Getters and Setters.
    // ------------------------------------------------------------------------

    /**
     * Gets the Constant Pool.
     * @return the Constant Pool.
     */
    public ConstantPool getConstantPool() {
        return constantPool;
    }


    // ------------------------------------------------------------------------
    // Private methods
    // ------------------------------------------------------------------------

    /**
     * Generate the Dex output file. The application files must have been parsed before.
     */
    private void generateDexFile() {

        out = new ByteVector(HEADER_NOMINAL_SIZE);

        // Sizes in bytes of the different structures linking to the Data sections.
        int stringIdsSize;
        int typeIdsSize;
        int protoIdsSize;
        int fieldIdsSize;
        int methodIdsSize;

        // Fills the Indexes maps, and sorts the collections.
        constantPool.prepareGeneration();

        // ------------------------------
        // Creates the header.
        // ------------------------------
        out.putByteArray(dexFileMagic, 0, dexFileMagic.length); // Put dex_file_magic.
        out.putInt(0); // Put a fake Checksum.
        for (int i = 0; i < 20; i++) {
            out.putByte(0);    // Put a fake Signature.
        }
        out.putInt(0); // Put a fake file_size.
        out.putInt(HEADER_NOMINAL_SIZE); // Put header_size.
        out.putInt(STANDARD_ENDIAN_VALUE); // Put the endian_tag.

        out.putInt(0); // No Link section.
        out.putInt(0);

        out.putInt(0); // No Map section. Filled later when the Map is written.

        // String_ids
        stringIdsOffset = HEADER_NOMINAL_SIZE; // The strings always come just after the header.
        int nbStrings = constantPool.getStringCount();
        out.putInt(nbStrings);
        out.putInt(nbStrings == 0 ? 0 : stringIdsOffset);
        stringIdsSize = nbStrings * STRING_ID_ITEM_SIZE;

        // Type_ids
        typeIdsOffset = stringIdsOffset + stringIdsSize;
        int nbTypes = constantPool.getTypeCount();
        out.putInt(nbTypes);
        out.putInt(nbTypes == 0 ? 0 : typeIdsOffset);
        typeIdsSize = nbTypes * TYPE_ID_ITEM_SIZE;

        // Proto_ids
        prototypeIdsOffset = typeIdsOffset + typeIdsSize;
        int nbProtos = constantPool.getPrototypeCount();
        out.putInt(nbProtos);
        out.putInt(nbProtos == 0 ? 0 : prototypeIdsOffset);
        protoIdsSize = nbProtos * PROTO_ID_ITEM_SIZE;

        // Field_ids
        fieldIdsOffset = prototypeIdsOffset + protoIdsSize;
        int nbFields = constantPool.getFieldCount();
        out.putInt(nbFields);
        out.putInt(nbFields == 0 ? 0 : fieldIdsOffset);
        fieldIdsSize = nbFields * FIELD_ID_ITEM_SIZE;

        // Method_ids
        methodIdsOffset = fieldIdsOffset + fieldIdsSize;
        int nbMethods = constantPool.getMethodCount();
        out.putInt(nbMethods);
        out.putInt(nbMethods == 0 ? 0 : methodIdsOffset);
        methodIdsSize = nbMethods * METHOD_ID_ITEM_SIZE;

        // Class_defs
        classDefinitionsOffset = methodIdsOffset + methodIdsSize;
        int nbClasses = constantPool.getClassDefinitionCount();
        out.putInt(nbClasses);
        out.putInt(nbClasses == 0 ? 0 : classDefinitionsOffset);

        // Data size is not known for now. We'll come back for it later.
        // Data offset could be found, but it is the same as the first code_item, which may need
        // padding. We will take care of it when writing the code_items.
        int dataSizeOffset = out.getLength(); // Store this offset to get back later.
        out.putInt(0); // data_size, unknown for now.
        out.putInt(0); // data_off, calculated later.


        // ------------------------------
        // Fills the string_id, type_id, proto_id, field_id, method_id, class_defs section.
        // Some fields can be correctly filled now, but not all. These ones are filled will 0, and
        // we will come back here later.
        // ------------------------------

        // String_ids can't be filled for now.
        for (int i = 0; i < nbStrings; i++) {
            out.putInt(0);
        }

        // Type_ids can be filled. They consists in the index of the String the type refers.
        for (String type : constantPool.getTypes()) {
            int index = constantPool.getStringIndex(type);
            out.putInt(index);
        }

        // Prototypes. Filled later.
        for (int i = 0; i < nbProtos; i++) {
            out.putInt(0);
            out.putInt(0);
            out.putInt(0);
        }

        // Field_ids.
        for (Field field : constantPool.getFields()) {
            String className = field.getClassName();
            String typeName = field.getTypeName();
            int classNameIndex = constantPool.getTypeIndex(className);
            int typeNameIndex = constantPool.getTypeIndex(typeName);
            int nameIndex = constantPool.getStringIndex(field.getFieldName());

            out.putShort(classNameIndex);
            out.putShort(typeNameIndex);
            out.putInt(nameIndex);
        }

        // Methods_ids.
        for (Method method : constantPool.getMethods()) {
            String className = method.getClassName();
            Prototype prototype = method.getPrototype();
            String name = method.getMethodName();
            int classNameIndex = constantPool.getTypeIndex(className);
            int nameIndex = constantPool.getStringIndex(name);
            int prototypeIndex = constantPool.getPrototypeIndex(prototype);

            out.putShort(classNameIndex);
            out.putShort(prototypeIndex);
            out.putInt(nameIndex);
        }

        // Classes. Filled later.
        for (int i = 0; i < nbClasses; i++) {
            out.putInt(0);
            out.putInt(0);
            out.putInt(0);
            out.putInt(0);
            out.putInt(0);
            out.putInt(0);
            out.putInt(0);
            out.putInt(0);
        }


        // ------------------------------
        // Now comes the Data section, and the filling of previous sections we couldn't do earlier.
        // ------------------------------
        // The elements are stored in the same order they are generated by the dx compiler, though it
        // is absolutely not mandatory.
        dataOffset = out.getLength();

        prepareAnnotationSetRefLists();

        prepareAnnotationSetItems();

        writeCodeItems();

        prepareAnnotationDirectoryItems();

        writeTypeList();

        writePrototypeIds();

        writeStringDataItemSection();

        writeDebugInfoItems();

        writeAnnotationItems();
        completeAnnotationSetItems();
        completeAnnotationSetRefLists();
        completeAnnotationDirectoryItems();

        writeEncodedArrayItems();

        writeClassDataItems();

        writeMap();

        // Writes the size and offset of the Data Block into the data_size and data_off fields of the header.
        int dataSize = out.getLength() - dataOffset;
        if ((dataSize % 4) != 0) {
            try {
                throw new Exception("Data Size isn't a multiple of (uint).");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        out.putInt(dataSize, dataSizeOffset);
        out.putInt(dataOffset, dataSizeOffset + 4);

        // Writes the file size, now that we know it.
        out.putInt(out.getLength(), FILE_SIZE_OFFSET);

        addSHA1Signature();
        addAdler32Checksum();
    }


    /**
     * Calculates and adds the Adler32 Checksum at the beginning of the file, in the header.
     */
    private void addAdler32Checksum() {
        Adler32 adler = new Adler32();
        adler.update(out.getBuffer(), ADLER_OFFSET + ADLER_SIZE, out.getLength() - (ADLER_OFFSET + ADLER_SIZE));
        out.putInt((int)adler.getValue(), ADLER_OFFSET);
    }

    /**
     * Calculates and adds the SHA1 signature at the beginning of the file, in the header.
     */
    private void addSHA1Signature() {
        MessageDigest sha = null;
        try {
            sha = MessageDigest.getInstance("SHA-1");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }

        if (sha != null) {
            int sha1PostSignatureOffset = SHA1_SIGNATURE_OFFSET + SHA1_SIGNATURE_SIZE;
            sha.update(out.getBuffer(), sha1PostSignatureOffset, out.getLength() - sha1PostSignatureOffset);
            byte[] digest = sha.digest();
            if (digest.length == SHA1_SIGNATURE_SIZE) {
                out.putByteArray(digest, SHA1_SIGNATURE_OFFSET);
            } else {
                try {
                    throw new Exception("SHA-1 digest has an unexpected size : " + digest.length);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * Prepares the annotation_set_ref_lists at the end of the file. We can resolve their offset, but not their
     * content, because we don't know the offsets of the annotation_set_items yet.
     */
    private void prepareAnnotationSetRefLists() {
        for (AnnotationSetRefList annotationSetRefList : constantPool.getAnnotationSetRefLists()) {
            out.addPadding();
            if (annotationSetRefListOffset == 0) {
                annotationSetRefListOffset = out.getLength();
            }
            // Sets the offset.
            constantPool.setAnnotationSetRefListOffset(annotationSetRefList, out.getLength());

            // Writes fake data.
            out.putInt(0); // Fake size.
            for (int i = 0, size = annotationSetRefList.getNbAnnotationSetItem(); i < size; i++) {
                out.putInt(0); // Fake annotations_off.
            }
        }
    }

    /**
     * Prepares the Annotations Set Items structure by filling the area with 0, at the end of the Dex file.
     * This is needed because this is the second section encoded, and we don't have the offset of the
     * Annotation_Items yet. However, we set the annotation_set_item offset inside the Classes, Methods, Fields.
     */
    private void prepareAnnotationSetItems() {

        for (AnnotationSetItem annotationSetItem : constantPool.getAnnotationSetItems()) {
            int size = annotationSetItem.getNbAnnotationItems(); //annotationHolder.getNbAnnotations();

            // We do not discard annotation_set_item with size of 0, because Annotation Parameters use them.
            out.addPadding();
            if (annotationSetItemOffset == 0) {
                annotationSetItemOffset = out.getLength();
            }
            constantPool.setAnnotationSetItemOffset(annotationSetItem, out.getLength());

            out.putInt(0); // Fake size.

            for (int i = 0; i < size; i++) {
                out.putInt(0); // Fake offsets.
            }
        }


    }

    /**
     * Writes the code items at the end of the buffer, and sets their offset. Each code must be parsed
     * once again to map the symbolic elements link to the resolved elements.
     */
    private void writeCodeItems() {

        // Scans all the methods inside all the classes.
        for (ClassDefinitionItem cdi : constantPool.getClasses()) {
            // And every direct or virtual methods in these classes.
            writeMethodsOfCodeItem(cdi.getDirectMethods());
            writeMethodsOfCodeItem(cdi.getVirtualMethods());

        }
    }

    /**
     * Write the given Methods. Should only be called by writeCodeItems.
     * @param methods List of methods, whether they are direct or virtual.
     */
    private void writeMethodsOfCodeItem(List<Method> methods) {
        for (Method method : methods) {
            if (method.supportsCodeItem()) {
                out.addPadding();
                if (codeItemOffset == 0) {
                    codeItemOffset = out.getLength();
                }

                // Checks if the ConstantPool optimization (Reader-->Writer) is used.
                int startByteCodeToCopy = method.getStartBytecodeToCopy();
                if (startByteCodeToCopy == 0) {
                    // No optimization. Parses the code normally.
                    ByteVector code = method.getCodeItemCode();
                    if (code.getLength() > 0) {
                        // First encodes the code itself.
                        CodeItem codeItem = method.getCodeItem();
                        codeItem.setOffset(out.getLength());
                        codeItem.mapResolvedIndexes();
                        out.putByteVector(code);

                        // Then encodes the try/catch following the code.
                        ByteVector tryCatchCode = codeItem.getCodeItemTryCatch();
                        if (tryCatchCode != null) {
                            out.putByteVector(tryCatchCode);
                        }
                    }
                } else {
                    // Optimization used.
                    // First, needs to parse the code from the Dex file and adds it to the output file.
                    CodeItem codeItem = method.getCodeItem();
                    int outputFileByteCodeOffset = out.getLength();
                    codeItem.setOffset(outputFileByteCodeOffset);

                    IDalvikValueReader dexFile = applicationReader.getDexFile();

                    // Calculates the size of code. We use the size of the source buffer.
                    // The size of the try/catch is not useful : we will parse it and copy it byte
                    // per byte.
                    int lengthByteCode = CodeItem.HEADER_SIZE;
                    dexFile.seek(startByteCodeToCopy + CodeItem.TRIES_SIZE_FIELD_OFFSET);
                    int nbTries = dexFile.ushort();
                    // Reads the debug_info_off. Useful later when copying the debug_info_item.
                    int startDebugInfoItem = dexFile.uint();
                    method.setStartDebugInfoToCopy(startDebugInfoItem);
                    int insnsSizeInWord = dexFile.uint();
                    lengthByteCode += insnsSizeInWord * 2;
                    if ((nbTries != 0) && ((insnsSizeInWord % 2) != 0)) { // Padding present ?
                        lengthByteCode += 2;
                    }

                    // Copies the bytecode from the input file to the output.
                    out.putByteArray(dexFile.getContents(), startByteCodeToCopy, lengthByteCode);

                    // Maps the resolved symbols in the newly copied bytecode. This doesn't include
                    // the try/catch that hasn't been copied yet.
                    codeItem.mapResolvedIndexesByteCode(out, outputFileByteCodeOffset);

                    // Now takes care of the try/catch. Contrary to the bytecode before, we can't copy
                    // it and parse it because it will surely not be the same size (as it encodes its
                    // fields as ULeb128).
                    if (nbTries != 0) {
                        ByteVector inputDexFileByteArray = new ByteVector(dexFile.getContents());
                        ByteVector tryCatch = codeItem.mapResolvedIndexesTryCatch(inputDexFileByteArray,
                                              startByteCodeToCopy + lengthByteCode, nbTries);
                        out.putByteVector(tryCatch);
                    }

                }
                codeItemCount++;
            }
        }
    }

    /**
     * Prepares the Annotations Directory Items structure by filling the area with mostly 0. This is needed
     * because we don't have the offset of the annotation_set_items yet.
     */
    private void prepareAnnotationDirectoryItems() {

        // Encodes the annotation_directory_items with fake data. This allows however to know the offset
        // of each one of them.
        for (AnnotationDirectoryItem adi : constantPool.getAnnotationDirectoryItems()) {
            out.addPadding();
            if (annotationDirectoryItemsOffset == 0) { // Sets the Offset of the first Item.
                annotationDirectoryItemsOffset = out.getLength();
            }

            constantPool.setAnnotationDirectoryItemOffset(adi, out.getLength());

            out.putInt(0); // Fake class_annotations_off, or right one if there is none !

            int nbAnnotatedFields = adi.getNbAnnotatedFields();
            int nbAnnotatedMethods = adi.getNbAnnotatedMethods();
            int nbAnnotatedParameters = adi.getNbAnnotatedParameters();

            out.putInt(0);
            out.putInt(0);
            out.putInt(0);

            int size = nbAnnotatedFields + nbAnnotatedMethods + nbAnnotatedParameters;
            for (int i = 0; i < size; i++) {
                out.putInt(0); // Fake index.
                out.putInt(0); // Fake annotation_off.
            }
        }
    }

    /**
     * Writes the type_list section at the end of the output buffer, and fills the typeListToOffset map of
     * the Constant Pool.
     */
    private void writeTypeList() {
        for (TypeList typeList : constantPool.getTypeList()) {
            out.addPadding();
            if (typeListOffset == 0) {
                typeListOffset = out.getLength();
            }
            // Encodes the offset of the structure in the corresponding map.
            constantPool.setTypeListOffset(typeList, out.getLength());

            // Encodes the type_list and type_item section.
            int size = typeList.size();
            if (size > 0) {
                out.putInt(size);
                for (String type : typeList.getTypeList()) {
                    out.putShort(constantPool.getTypeIndex(type));
                }
            }
        }
    }

    /**
     * Write the prototype_ids section in the beginning of the buffer. This has to be done only after the
     * type_list has been encoded and their offset found.
     */
    private void writePrototypeIds() {

        int prototypeIdsOffset = this.prototypeIdsOffset;

        for (Prototype prototype : constantPool.getPrototypes()) {
            String shortyDescriptor = prototype.getShortyDescriptor();
            out.putInt(constantPool.getStringIndex(shortyDescriptor), prototypeIdsOffset); // Shorty_idx.
            prototypeIdsOffset += 4;

            String returnType = prototype.getReturnType();
            out.putInt(constantPool.getTypeIndex(returnType), prototypeIdsOffset); // Return_type_idx.
            prototypeIdsOffset += 4;

            // Parameters_off. May be 0 if the Prototype has no parameter.
            TypeList typeList = prototype.getParameterTypes();
            int typeListOffset = (typeList.size() == 0) ? 0 : constantPool.getTypeListOffset(typeList);
            out.putInt(typeListOffset, prototypeIdsOffset);
            prototypeIdsOffset += 4;
        }
    }

    /**
     * Writes the string_data_item section at the end of the output buffer, and completes the string_id
     * section.
     */
    private void writeStringDataItemSection() {

        if (constantPool.getStringCount() > 0) {
            // The string_id_item section is always padded after the header.
            int stringIdsOffset = this.stringIdsOffset;

            stringDataOffset = out.getLength();

            for (String string : constantPool.getStrings()) {
                // Encodes the offset in the string_id section.
                out.putInt(out.getLength(), stringIdsOffset);
                stringIdsOffset += 4;

                // Encodes the size and data in the string_data_item.
                out.putUleb128(string.length());
                out.putMUTF8(string);
            }
        }
    }

    /**
     * Writes the debug_info_items at the end of the output buffer. Also sets their offset inside the
     * Methods previously encoded.
     */
    private void writeDebugInfoItems() {
        if (SKIP_DEBUG_INFO_ITEMS) {
            return;
        }

        for (Method method : constantPool.getMethods()) {
            // If the Method is "Unknown" (referred to but not encoded), or is Abstract or Interface
            // don't write its Debug Information.
            if (!method.isUnknown() && method.supportsCodeItem()) {
                CodeItem codeItem = method.getCodeItem();

                // "Constant Pool" optimization enabled ?
                int startDebugInfoToCopy = method.getStartDebugInfoToCopy();
                if (startDebugInfoToCopy == 0) {
                    // No optimization. Writes the Debug Info Item we created.
                    ByteVector debugInfoItemCode = codeItem.getDebugInfoItemCode();
                    if (debugInfoItemCode != null) {
                        //ByteVector debugInfoItemHeader = codeItem.getDebugInfoItemHeader();
                        int currentDebugInfoItemOffset = out.getLength();
                        // Sets the offset of the Debug Info Item inside the related Code Item.
                        int currentCodeItemOffset = codeItem.getOffset();
                        if (currentCodeItemOffset != 0) {
                            // Sets the offset of the first Debug Info item in the data section.
                            if (debugInfoItemOffset == 0) {
                                debugInfoItemOffset = currentDebugInfoItemOffset;
                            }

                            // Sets the offset of the debug_info_item to the current Code Item encoded bytes.
                            codeItem.setDebugInfoItemOffset(out, currentDebugInfoItemOffset);

                            // Encodes the debug_info_item code.
                            // But first we may have to resolve its indexes as it has been encoded with
                            // symbolic indexes. However, if no index was used in this debug_info_item, we
                            // can use it as-is.
                            if (codeItem.areSymbolicIndexesUsedInDebugCodeItem()) {
                                debugInfoItemCode = codeItem.mapResolvedIndexesForDebug(debugInfoItemCode, 0);
                            }
                            out.putByteVector(debugInfoItemCode);

                            debugInfoItemCount++;
                        }
                    }
                } else {
                    // "Constant Pool" optimization. We need to parse the input file to map the
                    // symbolic indexes to resolved indexes. We can't directly copy the debug_info_item
                    // to the destination and change the indexes from here because they are encoded
                    // as ULeb128, so the new indexes may have different size.
                    // First, get the input Dex file.
                    IDalvikValueReader dexFile = applicationReader.getDexFile();
                    ByteVector inputDexFileByteArray = new ByteVector(dexFile.getContents());

                    int currentDebugInfoItemOffset = out.getLength();
                    // Sets the offset of the first Debug Info item in the data section.
                    if (debugInfoItemOffset == 0) {
                        debugInfoItemOffset = currentDebugInfoItemOffset;
                    }

                    ByteVector debugInfoItem = codeItem.mapResolvedIndexesForDebug(inputDexFileByteArray,
                                               startDebugInfoToCopy);
                    out.putByteVector(debugInfoItem);

                    // Sets the offset of the debug_info_item to the current Code Item encoded bytes.
                    codeItem.setDebugInfoItemOffset(out, currentDebugInfoItemOffset);

                    debugInfoItemCount++;
                }
            }
        }
    }

    /**
     * Writes the annotation_item section at the end of the output buffer. Stores their offset in the
     * Constant Pool.
     */
    private void writeAnnotationItems() {

        annotationItemsOffset = out.getLength();

        // Encodes the Annotation Items found. They are all unique and ordered.
        for (AnnotationItem annotationItem : constantPool.getAnnotationItems()) {
            constantPool.setAnnotationItemOffset(annotationItem, out.getLength());

            out.putByte(annotationItem.getVisibility());
            // Encoding the encoded_annotation format.
            out.putUleb128(constantPool.getTypeIndex(annotationItem.getAnnotationType())); // type_idx.
            out.putUleb128(annotationItem.getNbAnnotationElements()); // size.
            // Encodes the elements (values) of the annotation.
            PriorityQueue<AnnotationElement> annotationElements = annotationItem.getAnnotationElements();
            //ArrayList<AnnotationElement> annotationElements = annotationItem.getAnnotationElements();
            for (AnnotationElement annotationElement : annotationElements) {
                out.putUleb128(constantPool.getStringIndex(annotationElement.getElementName())); // name_idx.
                out.putByteArray(annotationElement.getEncodedValue().encode(constantPool)); // element.
            }
        }
    }

    /**
     * Completes the annotation_set_items we filled with 0 before, now that we have the annotation_item offsets.
     * Only call this method after writeAnnotationItems().
     */
    private void completeAnnotationSetItems() {
        // Encodes all the annotation_set_items.
        if ((annotationSetItemOffset != 0) && (constantPool.getAnnotationSetItemCount() > 0)) {
            int offset = annotationSetItemOffset;

            for (AnnotationSetItem annotationSetItem : constantPool.getAnnotationSetItems()) {
                out.putInt(annotationSetItem.getNbAnnotationItems(), offset); // size.
                offset += 4;

                for (AnnotationItem annotationItem : annotationSetItem.getAnnotationItems()) {
                    out.putInt(constantPool.getAnnotationItemOffset(annotationItem), offset);
                    offset += 4;
                }

            }
        }
    }

    /**
     * Completes the annotation_set_ref_lists we filled with 0 before, now that was have the
     * annotation_set_items offsets. Only call this method after completeAnnotationSetItems().
     */
    private void completeAnnotationSetRefLists() {

        int offset = annotationSetRefListOffset;
        for (AnnotationSetRefList annotationSetRefList : constantPool.getAnnotationSetRefLists()) {
            int size = annotationSetRefList.getNbAnnotationSetItem();
            out.putInt(size, offset); // Encodes size of the list, in entries.
            offset += 4;

            // Encodes the offsets to the annotation_set_items.
            for (int i = 0; i < size;  i++) {
                AnnotationSetItem annotationSetItem = annotationSetRefList.getAnnotationSetItem(i);
                out.putInt(constantPool.getAnnotationSetItemOffset(annotationSetItem), offset);
                offset += 4;
            }
        }

    }

    /**
     * Completes the annotation_directory_item section previously prepared. The annotation_set_item
     * section must have been completed.
     */
    private void completeAnnotationDirectoryItems() {

        for (AnnotationDirectoryItem adi : constantPool.getAnnotationDirectoryItems()) {

            // Encodes only if there is actually Annotations in this Class.
            int directoryItemOffset = constantPool.getAnnotationDirectoryItemOffset(adi);
            if (directoryItemOffset != 0) {

                AnnotationSetItem annotationSetItem = adi.getClassAnnotationSetItem();
                // There may be no Class Annotations.
                if (annotationSetItem.getNbAnnotationItems() > 0) {
                    int annotationOffset = constantPool.getAnnotationSetItemOffset(annotationSetItem);
                    out.putInt(annotationOffset, directoryItemOffset); // class_annotation_off.
                }

                out.putInt(adi.getNbAnnotatedFields(), directoryItemOffset + AnnotationDirectoryItem.FIELDS_SIZE_OFFSET);
                out.putInt(adi.getNbAnnotatedMethods(), directoryItemOffset + AnnotationDirectoryItem.ANNOTATED_METHODS_SIZE_OFFSET);
                out.putInt(adi.getNbAnnotatedParameters(), directoryItemOffset + AnnotationDirectoryItem.ANNOTATED_PARAMETERS_SIZE_OFFSET);

                directoryItemOffset += 4 + 3 * 4;

                // Encodes the field_annotation format.
                if (adi.getNbAnnotatedFields() > 0) {
                    TreeMap<Integer,IAnnotationsHolder> orderedContent = new TreeMap<Integer,IAnnotationsHolder> ();
                    for (Field field : adi.getAnnotatedFields()) {
                        orderedContent.put(constantPool.getFieldIndex(field),(IAnnotationsHolder)field);
                    }
                    for (Map.Entry<Integer, IAnnotationsHolder> entry: orderedContent.entrySet()) {
                        directoryItemOffset = writeFieldMethodIndexAnnotation(entry.getValue(),
                                              entry.getKey(), directoryItemOffset);
                    }
                }

                // Encodes the method_annotation format.
                if (adi.getNbAnnotatedMethods() > 0) {
                    TreeMap<Integer,IAnnotationsHolder> orderedContent = new TreeMap<Integer,IAnnotationsHolder> ();
                    for (Method method : adi.getAnnotatedMethods()) {
                        orderedContent.put(constantPool.getMethodIndex(method),(IAnnotationsHolder) method);
                    }
                    for (Map.Entry<Integer, IAnnotationsHolder> entry: orderedContent.entrySet()) {
                        directoryItemOffset = writeFieldMethodIndexAnnotation(entry.getValue(),
                                              entry.getKey(), directoryItemOffset);
                    }
                }

                // Encodes the parameter_annotation format.
                if (adi.getNbAnnotatedParameters() > 0) {
                    TreeMap<Integer,AnnotationSetRefList> orderedContent = new TreeMap<Integer,AnnotationSetRefList> ();

                    for (AnnotationSetRefList annotatedParameters : adi.getAnnotatedParameters()) {
                        Method method = annotatedParameters.getMethod();
                        orderedContent.put(constantPool.getMethodIndex(method), method.getAnnotatedParameterSetRefList());
                    }
                    for (Map.Entry<Integer, AnnotationSetRefList> entry : orderedContent.entrySet()) {
                        out.putInt(entry.getKey(), directoryItemOffset);
                        out.putInt(constantPool.getAnnotationSetRefListOffset(entry.getValue()),
                                   directoryItemOffset + 4);
                        directoryItemOffset += 8;
                    }
                }
            }
        }

    }

    /**
     * Writes the field_annotation, method_annotation or parameter_annotation format at the given offset of the
     * Dex file. This works for all these structures because its format is the same.
     * On return, the new offset after what has been written is given.
     * If the Holder has no Annotations, nothing happens.
     * @param annotationsHolder holder of the Annotations.
     * @param index index of the Method or Field (for Parameter, the Index is from the Method it is from).
     * @param offset offset on the Dex file where to write.
     * @return the new offset after what has been written.
     */
    private int writeFieldMethodIndexAnnotation(IAnnotationsHolder annotationsHolder, int index, int offset) {
        if (annotationsHolder.getNbAnnotations() > 0) {
            out.putInt(index, offset);
            out.putInt(constantPool.getAnnotationSetItemOffset(annotationsHolder.getAnnotationSetItem())
                       , offset + 4);
            return (offset + 8);
        } else {
            return offset;
        }
    }

    /**
     * Writes the encoded_array_item section at the end of the output buffer. It must be done before
     * writing the class_def_items.
     */
    private void writeEncodedArrayItems() {

        /**
         * Tiny class to hold the offset and size of the previously encoded structure.
         *
         * @author Julien Névo
         */
        class Place {
            public int offset;
            public int size;
            public Place(int offset, int size) {
                this.offset = offset;
                this.size = size;
            }
        }

        encodedArrayItemsOffset = out.getLength();

        // Each encoded_array_item must be unique. So we store the offset and size of each one encoded
        // to be able to compare the new one with the encoded ones.
        List<Place> places = new ArrayList<Place>();

        // Encodes the array of static values of each Class.
        for (ClassDefinitionItem cdi : constantPool.getClasses()) {
            // According to the output of a compiled Class, only Final Static Fields have their value
            // encoded. The Static only Fields may be encoded, with their value set to 0 or null, ONLY
            // if a Final Static Field is after them. The last Final Static Field taken in account must have
            // its value set, i.e. it is not a reference type.
            // Primitive type may NOT have a value, in case they are set by a Static Constructor.

            if (cdi.getNbStaticFields() > 0) {
                ArrayList<Field> fields = cdi.getStaticFields();
                // Then, we look for the last Final Static Field inside.
                boolean found = false;
                int lastFinalStaticIndex = fields.size() - 1;
                while (!found && (lastFinalStaticIndex >= 0)) {
                    Field field = fields.get(lastFinalStaticIndex);
                    found = (field.isFinalStatic() && (field.getValue() != null) && (field.getValue().getType() != Opcodes.VALUE_NULL));
                    if (!found) {
                        lastFinalStaticIndex--;
                    }
                }

                if (found) {
                    // We store the offset where the item might be encoded. But we might discard it if it is
                    // a duplicate.
                    int possibleOffset = out.getLength();

                    // Used to store the complete encoded Array Item. We may discard it if it happens to be a duplicate.
                    ByteVector bv = new ByteVector();

                    // Encodes the encoded_array. First, the size.
                    bv.putUleb128(lastFinalStaticIndex + 1);

                    // Encodes the array. Iterates forward.
                    int index = 0;
                    while (index <= lastFinalStaticIndex) {
                        Field field = fields.get(index);
                        // To comply the output of a compiled Class, Fields with no value are set
                        // to 0 or Null, because they are "before" the last Final Static Field.
                        if (field.getValue() == null) {
                            field.setNoValue();
                        }
                        byte[] bytes = field.encodeValue(constantPool);

                        if (bytes != null) {
                            bv.putByteArray(bytes);
                        }
                        index++;
                    }

                    // Checks if the newly created item must be encoded. Is it a duplicate ?
                    boolean foundDuplicate = false;
                    int indexPlace = 0;
                    int size = places.size();
                    int scannedStructureOffset = 0;
                    // Scans all the encoded items.
                    while (!foundDuplicate && (indexPlace < size)) {
                        byte[] bytesPending = bv.getBuffer(); // We take the whole buffer, to avoid making copies.
                        int bytesPendingSize = bv.getLength();
                        Place currentPlace = places.get(indexPlace);
                        int placeSize = currentPlace.size;
                        if (placeSize == bytesPendingSize) {
                            // They have the same size. We have to check byte by byte.
                            int posPending = 0;
                            scannedStructureOffset = currentPlace.offset;
                            boolean difference = false;
                            while (!difference && posPending < placeSize) {
                                difference = bytesPending[posPending] != out.getBuffer()[scannedStructureOffset + posPending];
                                posPending++;
                            }

                            // Difference ? If no, we found a duplicate and we can stop here.
                            // Else, we continue to compare with the next elements.
                            foundDuplicate = !difference;
                        }

                        indexPlace++;
                    }

                    // Found no duplicate ? Then encodes the structure.
                    if (!foundDuplicate) {
                        encodedArrayItemsCount++;
                        places.add(new Place(possibleOffset, bv.getLength()));
                        // Saves the offset to the array we're going to encode.
                        out.putByteVector(bv);
                    } else {
                        // We found a duplicate. We don't encode the structure, but the Class must refer to
                        // the already encoded structure !
                        possibleOffset = scannedStructureOffset;
                    }
                    constantPool.addOffsetForStaticValuesEncodedArrayItemOfClass(cdi, possibleOffset);

                }
            }
        }
    }

    /**
     * Writes the class_data_item section at the end of the output buffer, and completes the class_defs
     * section.
     */
    private void writeClassDataItems() {

        int currentClassDefinitionOffset = classDefinitionsOffset;

        classDataItemOffset = out.getLength();

        for (ClassDefinitionItem cdi : constantPool.getClasses()) {
            // Writes the class_def_item.
            out.putInt(constantPool.getTypeIndex(cdi.getClassName()), currentClassDefinitionOffset); // Class_idx.

            out.putInt(cdi.getAccessFlags(), currentClassDefinitionOffset + ClassDefinitionItem.ACCESS_FLAGS_OFFSET); // Access_flags.
            // Superclass_idx. If no super class, encodes the NO_INDEX value.
            String superClassName = cdi.getSuperClassName();
            int superClassIndex = superClassName == null ? Opcodes.NO_INDEX_SIGNED :
                                  constantPool.getTypeIndex(superClassName);
            out.putInt(superClassIndex, currentClassDefinitionOffset + ClassDefinitionItem.SUPERCLASS_IDX_OFFSET);

            // Interfaces. 0 if none.
            TypeList interfaces = cdi.getInterfaces();
            int indexTypeListInterface = (interfaces.size() == 0) ? 0 :
                                         constantPool.getTypeListOffset(interfaces);
            out.putInt(indexTypeListInterface, currentClassDefinitionOffset + ClassDefinitionItem.INTERFACES_OFFSET);

            // Source_file_idx. NO_INDEX if none.
            String sourceFile = cdi.getSourceFileName();
            int indexSourceFile = (sourceFile == null) ? Opcodes.NO_INDEX_SIGNED :
                                  constantPool.getStringIndex(sourceFile);
            out.putInt(indexSourceFile, currentClassDefinitionOffset + ClassDefinitionItem.SOURCE_FILE_IDX_OFFSET);

            // Annotations_off.
            out.putInt(constantPool.getAnnotationDirectoryItemOffset(cdi.getAnnotationDirectoryItem()),
                       currentClassDefinitionOffset + ClassDefinitionItem.ANNOTATIONS_OFF_OFFSET);

            // Class_data_off. We point at the end of the file, as that's where we're going to add the
            // Class Data, just below.
            // If the class is an Interface, and has no fields or methods, then it is considered "marker
            // interface" and the class_data_item isn't encoded.
            int classDataOffset;
            if ((cdi.isInterface() && (cdi.getNbStaticFields() == 0)
                    && (cdi.getNbInstanceFields() == 0))
                    && ((cdi.getNbDirectMethods() == 0)
                        && (cdi.getNbVirtualMethods() == 0))) {
                classDataOffset = 0;
            } else {
                classDataOffset = out.getLength();
            }
            out.putInt(classDataOffset, currentClassDefinitionOffset + ClassDefinitionItem.CLASS_DATA_OFF_OFFSET);

            // Static_values_off.
            out.putInt(constantPool.getOffsetOfStaticValuesEncodedArrayItemOfClass(cdi), currentClassDefinitionOffset + 4 * 7);

            currentClassDefinitionOffset += CLASS_DEF_ITEM_SIZE;

            // Writes the class_data_item at the end of the file, only if the class isn't a "marker interface".
            if (classDataOffset != 0) {
                out.putUleb128(cdi.getNbStaticFields());
                out.putUleb128(cdi.getNbInstanceFields());
                out.putUleb128(cdi.getNbDirectMethods());
                out.putUleb128(cdi.getNbVirtualMethods());

                writeFieldsInClassDataItem(cdi.getStaticFields());
                writeFieldsInClassDataItem(cdi.getInstanceFields());

                writeMethodsInClassDataItem(cdi.getDirectMethods());
                writeMethodsInClassDataItem(cdi.getVirtualMethods());

                classDataItemCount++;
            }
        }

    }



    /**
     * Writes the given Methods according to the "direct/virtual_method" field format in the class_data_item
     * at the end of the output file. Requires code_items to have their offset resolved.
     * @param methods the Methods to encode.
     */
    private void writeMethodsInClassDataItem(ArrayList<Method> methods) {

        if (methods.size() == 0) {
            return;
        }

        // We must sort the Array in order to get a growing index number.
        TreeSet<Method> sortedMethods = new TreeSet<Method>(methods);

        boolean isFirstMethod = true;
        int previousIndex = 0;
        int indexToEncode = 0;

        for (Method method : sortedMethods) {
            // Index is coded as a difference from the previous Index, except for the first occurrence.
            int methodIndex = constantPool.getMethodIndex(method); //  constantPool.getMethodsToIndexesMap().get(method);

            if (isFirstMethod) {
                indexToEncode = methodIndex;
                isFirstMethod = false;
            } else {
                indexToEncode = methodIndex - previousIndex;
            }

            out.putUleb128(indexToEncode);
            out.putUleb128(method.getAccess());
            CodeItem codeItem = method.getCodeItem();
            out.putUleb128(codeItem == null ? 0 : codeItem.getOffset());

            previousIndex = methodIndex;
        }
    }

    /**
     * Writes the given Fields according to the "static/instance_method" field format in the class_data_item
     * at the end of the output file.
     * @param fields the Fields to encode.
     */
    private void writeFieldsInClassDataItem(ArrayList<Field> fields) {

        if (fields.size() == 0) {
            return;
        }

        // We must sort the Array in order to get a growing index number.
        TreeSet<Field> sortedFields = new TreeSet<Field>(fields);

        boolean isFirstField = true;
        int previousIndex = 0;
        int indexToEncode = 0;

        for (Field field : sortedFields) {
            // Index is coded as a difference from the previous Index, except for the first occurrence.
            int fieldIndex = constantPool.getFieldIndex(field);

            if (isFirstField) {
                indexToEncode = fieldIndex;
                isFirstField = false;
            } else {
                indexToEncode = fieldIndex - previousIndex;
            }

            out.putUleb128(indexToEncode);
            out.putUleb128(field.getAccess());

            previousIndex = fieldIndex;
        }
    }

    /**
     * Writes the Map section at the end of the output buffer. All the elements must have been parsed and
     * their offset resolved.
     */
    private void writeMap() {

        out.addPadding();

        // We don't know yet how many entries there are, so we save it to write it for later.
        int mapOffset = out.getLength();
        out.putInt(0);

        out.putInt(mapOffset, MAP_OFFSET_OFFSET); // Sets the Map Offset in the header.

        // The Map elements are stored in the same order they are generated in the file, though
        // this is absolutely not mandatory.
        // Header_item.
        int nbEntries = writeMapItem(Opcodes.TYPE_HEADER_ITEM, 1, 0, 0);
        nbEntries = writeMapItem(Opcodes.TYPE_STRING_ID_ITEM, constantPool.getStringCount(),
                                 stringIdsOffset, nbEntries); // String_id_item.
        nbEntries = writeMapItem(Opcodes.TYPE_TYPE_ID_ITEM, constantPool.getTypeCount(),
                                 typeIdsOffset, nbEntries); // Type_id_item.
        nbEntries = writeMapItem(Opcodes.TYPE_PROTO_ID_ITEM, constantPool.getPrototypeCount(),
                                 prototypeIdsOffset, nbEntries); // Proto_id_item.
        nbEntries = writeMapItem(Opcodes.TYPE_FIELD_ID_ITEM, constantPool.getFieldCount(),
                                 fieldIdsOffset, nbEntries); // Field_id_item.
        nbEntries = writeMapItem(Opcodes.TYPE_METHOD_ID_ITEM, constantPool.getMethodCount(),
                                 methodIdsOffset, nbEntries); // Method_id_item.
        nbEntries = writeMapItem(Opcodes.TYPE_CLASS_DEF_ITEM, constantPool.getClassDefinitionCount(),
                                 classDefinitionsOffset, nbEntries); // Class_def_item.

        // Data items.
        nbEntries = writeMapItem(Opcodes.TYPE_ANNOTATION_SET_REF_LIST, constantPool.getAnnotationSetRefListsCount(),
                                 annotationSetRefListOffset, nbEntries); // Annotation_set_ref_list.
        nbEntries = writeMapItem(Opcodes.TYPE_ANNOTATION_SET_ITEM, constantPool.getAnnotationSetItemCount(),
                                 annotationSetItemOffset, nbEntries); // Annotation_set_item.
        nbEntries = writeMapItem(Opcodes.TYPE_CODE_ITEM, codeItemCount,
                                 codeItemOffset, nbEntries); // Code_item.
        nbEntries = writeMapItem(Opcodes.TYPE_ANNOTATIONS_DIRECTORY_ITEM, constantPool.getAnnotationDirectoryItemCount(),
                                 annotationDirectoryItemsOffset, nbEntries); // Annotation_directory_item.
        nbEntries = writeMapItem(Opcodes.TYPE_TYPE_LIST, constantPool.getTypeListCount(),
                                 typeListOffset, nbEntries); // Type_list.
        nbEntries = writeMapItem(Opcodes.TYPE_STRING_DATA_ITEM, constantPool.getStringCount(),
                                 stringDataOffset, nbEntries); // String_data_item.
        nbEntries = writeMapItem(Opcodes.TYPE_DEBUG_INFO_ITEM, debugInfoItemCount,
                                 debugInfoItemOffset, nbEntries); // Debug_info_item.
        nbEntries = writeMapItem(Opcodes.TYPE_ANNOTATION_ITEM, constantPool.getAnnotationItemCount(),
                                 annotationItemsOffset, nbEntries); // Annotation_item.
        nbEntries = writeMapItem(Opcodes.TYPE_ENCODED_ARRAY_ITEM, encodedArrayItemsCount,
                                 encodedArrayItemsOffset, nbEntries); // Encoded_array_item.
        nbEntries = writeMapItem(Opcodes.TYPE_CLASS_DATA_ITEM, classDataItemCount,
                                 classDataItemOffset, nbEntries); // Class_data_item.
        nbEntries = writeMapItem(Opcodes.TYPE_MAP_LIST, 1, mapOffset, nbEntries); // map_list.

        // We can now set the right number of entries at the beginning of the structure.
        out.putInt(nbEntries, mapOffset);
    }

    /**
     * Convenient method to write, at the end of the file, a map_item item. It is not written if the size is 0.
     * @param type type of the item (see {@link Opcodes#TYPE_HEADER_ITEM}).
     * @param size count of the number of items.
     * @param offset offset from the start of the file to the items.
     * @param nbEntries the current number of entries in the Map.
     * @return the number of entries in the Map, plus the size given.
     */
    private int writeMapItem(int type, int size, int offset, int nbEntries) {
        if (size > 0) {
            out.putShort(type);
            out.putShort(0);
            out.putInt(size);
            out.putInt(offset);
            nbEntries++;
        }
        return nbEntries;
    }


    // ---------------------------------------------------------
    // Code and ConstantPool copy optimization methods.
    // ---------------------------------------------------------

    /**
     * Returns the ApplicationReader that may be linked to this ApplicationWriter. May be null.
     */
    public ApplicationReader getApplicationReader() {
        return applicationReader;
    }

    /**
     * When the optimization about the ApplicationReader/ApplicationWriter is enabled, the
     * ApplicationReader will want to register Strings into the Constant Pool of the Writer.
     * This method is used for that. Note that the String added should be considered by the Writer
     * as symbolic, as more of them can be added later.
     * @param string the String to add to the Constant Pool.
     */
    public void addStringFromApplicationReader(String string) {
        constantPool.addStringToConstantPool(string);
    }

    /**
     * When the optimization about the ApplicationReader/ApplicationWriter is enabled, the
     * ApplicationReader will want to register Types into the Constant Pool of the Writer.
     * This method is used for that. Note that the Type added should be considered by the Writer
     * as symbolic, as more of them can be added later.
     * @param type the Type to add to the Constant Pool.
     */
    public void addTypeNameFromApplicationReader(String type) {
        constantPool.addTypeToConstantPool(type);
    }

    /**
     * When the optimization about the ApplicationReader/ApplicationWriter is enabled, the
     * ApplicationReader will want to register Fields into the Constant Pool of the Writer.
     * This method is used for that. Note that the Field added should be considered by the Writer
     * as symbolic, as more of them can be added later.
     * @param className the Class owner name.
     * @param type the type of the Field.
     * @param fieldName the name of the Field.
     */
    public void addFieldFromApplicationReader(String className, String type, String fieldName) {
        constantPool.addFieldToConstantPool(fieldName, type, className, Opcodes.ACC_UNKNOWN, null, null);
    }

    /**
     * When the optimization about the ApplicationReader/ApplicationWriter is enabled, the
     * ApplicationReader will want to register Methods into the Constant Pool of the Writer.
     * This method is used for that. Note that the Method added should be considered by the Writer
     * as symbolic, as more of them can be added later.
     * @param className the Class owner name.
     * @param prototype the prototype of the method, in TypeDescriptor format.
     * @param methodName the name of the Method.
     */
    public void addMethodFromApplicationReader(String className, String prototype, String methodName) {
        constantPool.addMethodToConstantPool(methodName, className, prototype, Opcodes.ACC_UNKNOWN,
                                             null, null);
    }

}



