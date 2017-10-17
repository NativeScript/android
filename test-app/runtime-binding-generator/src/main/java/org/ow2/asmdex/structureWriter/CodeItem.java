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

package org.ow2.asmdex.structureWriter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.instruction.Instruction;
import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.lowLevelUtils.DalvikValueReader;
import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureCommon.LocalVariable;

/**
 * Class representing the code instructions and debug informations of one method.
 * The instructions can link to debug instructions (the ones that don't concern lines/PC management).
 * Debug line/PC advances are managed directly by this class.
 *
 * @author Julien Névo
 */
public class CodeItem {

    /**
     * The generated code_item code. Includes the bytecode as well as the header of the code_item,
     * minus the reference to the debug_info_item as we don't know yet its offset.
     * <i>CAN</i> include the padding at the end needed by the Try/catch.
     * It is generated through the generateBytecode method.
     * It uses <i>symbolic</i> references, so must be parsed again to link them to the "real"
     * elements.
     */
    private ByteVector codeItemCode;

    /**
     * The generated code_item try/catch section, just after the instructions, which should be stuck
     * right after the codeItemCode. <i>Does NOT</i> include the padding right at the beginning. This ByteVector
     * had to be separated from the code because it is composed of ULeb128 fields, which can change size.
     * Instead of copying all the code if that happens, it is better to work on this part only.
     */
    private ByteVector codeItemTryCatch;

    /**
     * Offset to the tries_size field.
     */
    public static final int TRIES_SIZE_FIELD_OFFSET = 6;

    /**
     * Size in bytes of a try_item structure.
     */
    private static final int TRY_ITEM_STRUCTURE_SIZE = 8;

    /**
     * Offset to the debug_info_offset field.
     */
    private static final int DEBUG_INFO_OFFSET_OFFSET = 4 * 2;

    /**
     * Offset in bytes of the handler in a try_item structure.
     */
    private static final int HANDLER_OFFSET_IN_TRY_ITEM_STRUCTURE = 6;

    /**
     * Size in bytes of the header of the code_item structure (till insns_size included).
     */
    public static final int HEADER_SIZE = 4 * 2 + 2 * 4;

    /**
     * Offset in bytes of the insns_size structure.
     */
    public static final int INSNS_SIZE_OFFSET = 12;

    /**
     * The offset of the Code Item from the beginning of the Dex file. It is only known when producing the
     * output file, after the input has been fully parsed.
     */
    private int offset;

    /**
     * Current size in bytes of the total instructions of this item (and by extension, the method).
     * Does NOT take in account the header of the Code Item.
     */
    private int size = 0;

    /**
     * List of the Labels for this method.
     */
    private List<Label> labels = new ArrayList<Label>();

    /**
     * The number of words of incoming arguments used by this code.
     */
    private int incomingArgumentsSizeInWord = 0;

    /**
     * The number of words of outgoing arguments used by this code.
     */
    private int outgoingArgumentsSizeInWord = 0;

    /**
     * The number of words used by the register of the frame of this code.
     * (= temporary registers + "this" (if non-static and non-constructor) + parameters).
     */
    private int registerSize = 0;

    /**
     * The first line number found. It may be 0 if none were found.
     */
    private int firstLineNumber = 0;

    /**
     * List of the Try/Catch of this Code Item.
     */
    private ArrayList<TryCatch> tryCatches = new ArrayList<TryCatch>();

    /**
     * List of the instructions of this code item.
     */
    private ArrayList<Instruction> instructions = new ArrayList<Instruction>();

    /**
     * The Method this Code Item belongs to.
     */
    private Method method;

    /**
     * The Debug Info Item of this Code Item.
     */
    private DebugInfoItem debugInfoItem;

    /**
     * The Constant Pool of the Application.
     */
    final private ConstantPool constantPool;

    /**
     * Sets of encoded_catch_handlers, so that they are unique.
     */
    private HashSet<EncodedCatchHandler> encodedCatchHandlers = new HashSet<EncodedCatchHandler>();

    /**
     * Map linking a TryCatch structure to a unique encoded_catch_handlers.
     */
    private HashMap<TryCatch, EncodedCatchHandler> tryCatchToEncodedCatchHandler = new HashMap<TryCatch, EncodedCatchHandler>();

    /**
     * Map linking a unique EncodedCatchHandler to its offset relative to the encoded_catch_handler_list,
     * that is from where the try_items need it.
     */
    private HashMap<EncodedCatchHandler, Integer> encodedCatchHandlerToRelativeOffset = new HashMap<EncodedCatchHandler, Integer>();

    /**
     * Constructor of the Code Item.
     * @param method the Method this Code Item belongs to.
     */
    public CodeItem(Method method, ConstantPool constantPool) {
        this.method = method;
        this.constantPool = constantPool;

        debugInfoItem = new DebugInfoItem(constantPool);
    }


    // ------------------------------------
    // Public methods.
    // ------------------------------------

    /**
     * Frees all the structures (list of instructions, try/catch...) of this element and the debug_info_item
     * so that they don't consume memory. This <i>MUST</i> be done after having generated the bytecode, once
     * the method has been parsed and its end visited.
     */
    public void free() {
        labels = null;
        instructions = null;
        encodedCatchHandlers = null;
        tryCatchToEncodedCatchHandler = null;
        encodedCatchHandlerToRelativeOffset = null;

        debugInfoItem.free();
    }

    /**
     * Adds a label to the set of used labels. If Null, nothing is done.
     * @param label the label to add.
     */
    public void addLabel(Label label) {
        if ((label != null) && label.isResolved()) {
            // We must be sure the Label isn't already in the list.
            // The Object must not be here, not is content (that's why "contains" isn't used).
            boolean found = false;
            Iterator<Label> it = labels.iterator();
            while (!found && it.hasNext()) {
                found = (it.next() == label);
            }

            if (!found) {
                labels.add(label);
            }
        }
    }

    /**
     * Adds an instruction to the code item.
     * @param instruction instruction to add to the code item.
     */
    public void addInstruction(Instruction instruction) {
        instructions.add(instruction);
        size += instruction.getSize();
    }

    /**
     * Adds a Try/Catch structure to the list of Try/Catch.
     * @param tryCatch Try/Catch structure to add.
     */
    public void addTryCatch(TryCatch tryCatch) {
        // First we need to know if a similar Try (same Start) already exists.
        // If yes, we add the Handler and Type to the Try.
        // Else, we add the structure directly.

        TryCatch foundTc = null;
        Iterator<TryCatch> iterator = tryCatches.iterator();
        while ((foundTc == null) && (iterator.hasNext())) {
            TryCatch tc = iterator.next();
            if (tc.getStart() == tryCatch.getStart()) {
                foundTc = tc;
            }
        }

        if (foundTc != null) {
            foundTc.addExceptionHandlers(tryCatch.getExceptionHandlers());
        } else {
            tryCatches.add(tryCatch);
        }
    }

    /**
     * Generates the Instructions code, as Dalvik bytecode, in the codeItemCode buffer, as well as its
     * header. It uses <i>symbolic</i> references, so must be parsed again later to link them to the "real"
     * elements.
     * Also fills the debug_code_item in this instance. Note however that the debug_info_offset field
     * in the code_item header is not set, because we don't know where the debug_info_item is encoded
     * for now. Note that the debug_info_item is not written here, only built.
     * The alignment is not managed here, but must be by the calling method.
     */
    public void generateCodeItemCode() {
        codeItemCode = new ByteVector();

        List<LocalVariable> localVariables = method.getLocalVariables();

        // If the method has no instruction (can it happen in non-generated code ?), we skip all the
        // coding of the instructions and the debug info.
        boolean mustEncodeMethod = (instructions != null) && (instructions.size() > 0);

        // Initializes the debug_info_item by writing its header.
        if (mustEncodeMethod) {
            debugInfoItem.initializeDebugInfoItem(method.getParameters(), this, localVariables);
        }

        // Generates the header of the Code Item.
        codeItemCode.putShort(getRegistersSize());
        codeItemCode.putShort(incomingArgumentsSizeInWord);
        codeItemCode.putShort(outgoingArgumentsSizeInWord);
        codeItemCode.putShort(getTriesSize());
        codeItemCode.putInt(0); // The Debug Info Offset is not defined for now, as we don't know its offset yet.
        int sizeWord = size / 2;
        codeItemCode.putInt(sizeWord);

        int offsetByteCode = codeItemCode.getLength();

        if (mustEncodeMethod) {
            for (Instruction instruction : instructions) {
                debugInfoItem.parseDebugInformation(instruction, codeItemCode.getLength() - offsetByteCode);
                instruction.write(codeItemCode, constantPool);
            }

            // Padding may be needed if Try/Catch structures are present.
            if ((getTriesSize() != 0) && ((sizeWord % 2) != 0)) {
                codeItemCode.putShort(0);
            }

            // Encodes the Try/Catch structures in the second ByteVector.
            writeTryCatches(constantPool);

            // Ends the debug_info_item.
            debugInfoItem.closeDebugInfoItem();
        }
    }

    /**
     * Replaces one Instruction with a new given one.
     * @param oldInsn the replaced Instruction.
     * @param newInsn the new Instruction.
     */
    public void replaceInstructions(Instruction oldInsn, Instruction newInsn) {
        int indexInsnToReplace = instructions.indexOf(oldInsn);
        if (indexInsnToReplace >= 0) {
            instructions.remove(indexInsnToReplace);
            instructions.add(indexInsnToReplace, newInsn);

            // Updates the size of the Instructions.
            size += newInsn.getSize() - oldInsn.getSize();
        }
    }

    /**
     * Parses the debug bytecode and maps the resolved indexes (Strings, Types) from the
     * symbolic ones. The mapping between the two, done through two tables in the Constant Pool,
     * must have been performed before.
     * @param in input buffer to parse.
     * @param offsetInputBuffer offset inside the input buffer from where to start the parsing.
     * @return the parsed debug bytecode.
     */
    public ByteVector mapResolvedIndexesForDebug(ByteVector in, int offsetInputBuffer) {
        return debugInfoItem.mapResolvedIndexes(in, offsetInputBuffer);
    }

    /**
     * Parses the bytecode and tryCatch and maps the resolved indexes (Strings, Fields, Types, Methods) from
     * the symbolic ones. The mapping between the two, done through four tables in the Constant Pool,
     * must have been performed before.
     * This method must be called when the method has its own bytecode i.e. when the "ConstantPool"
     * optimization isn't performed.
     */
    public void mapResolvedIndexes() {
        mapResolvedIndexesByteCode(codeItemCode, 0);	// Maps the bytecode indexes.
        codeItemTryCatch = mapResolvedIndexesTryCatch(codeItemTryCatch, 0, getTriesSize());		// Maps the try/catch indexes.
    }

    final static private byte INDEX_STRING = 1;
    final static private byte INDEX_FIELD = 2;
    final static private byte INDEX_TYPE = 3;
    final static private byte INDEX_METHOD = 4;

    /**
     * Look-up table containing the Types of index every instructions uses. Includes 256 opcodes.
     * 0 = no index.
     */
    final private static byte[] typeOfIndexInInstructions = new byte[] {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0x00
        0,0,0,0,0,0,0,0,0,0,INDEX_STRING,INDEX_STRING,INDEX_TYPE,0,0,INDEX_TYPE,		// 0x10
        INDEX_TYPE,0,INDEX_TYPE,INDEX_TYPE,INDEX_TYPE,INDEX_TYPE,0,0,0,0,0,0,0,0,0,0,		// 0x20
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0x30
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0x40
        0,0,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,
        INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,		// 0x50
        INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,
        INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_FIELD,INDEX_METHOD,INDEX_METHOD,		// 0x60
        INDEX_METHOD,INDEX_METHOD,INDEX_METHOD,0,INDEX_METHOD,INDEX_METHOD,INDEX_METHOD,INDEX_METHOD,INDEX_METHOD,0,0,0,0,0,0,0,		// 0x70
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0x80
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0x90
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0xa0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0xb0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0xc0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0xd0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 0xe0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0			// 0xf0
    };

    /**
     * Parses the bytecode and maps the resolved indexes (Strings, Fields, Types, Methods) from the
     * symbolic ones. The given bytecode is <i>modified</i>.
     * The mapping between the two, done through four tables in the Constant Pool,
     * must have been performed before.
     * @param out ByteVector to be read and modified.
     * @param offsetByteCode offset in bytes where the parsing must begin.
     */
    public void mapResolvedIndexesByteCode(ByteVector out, int offsetByteCode) {
        // The same byte array is shared by the reader and writer.
        IDalvikValueReader reader = new DalvikValueReader(out.getBuffer());

        // Looks for the size of the instructions. We don't want to parse the possible try/catch
        // structures after the code.
        reader.seek(INSNS_SIZE_OFFSET + offsetByteCode);
        int insnsSizeInBytes = reader.uint() * 2; // The read size is in words, hence the * 2.
        int endPos = insnsSizeInBytes + reader.getPos();
        int indexFoundType = 0;

        // Parses all the opcodes.
        while (reader.getPos() < endPos) {
            int opcode = reader.ubyte();
            int secondByte = reader.ubyte();
            int indexSize = 2;		// By default, we read short index, but it can raise to 4.

            if (opcode == Opcodes.INSN_NOP) {
                // If we find an array, we have to skip it.
                int bytesToSkip = 0;
                switch (secondByte) {
                case 0x1: { // Packed-switch format.
                    int size = reader.ushort();
                    reader.skipInt();  // Skip first_key.
                    bytesToSkip = size * 4;
                    break;
                }
                case 0x2: { // Sparse-switch format.
                    int size = reader.ushort();
                    bytesToSkip = size * 4 * 2;
                    break;
                }
                case 0x3: { // Fill-array-data format.
                    int width = reader.ushort();
                    int size = reader.uint();
                    bytesToSkip = (size * width + 1) / 2 * 2;
                    break;
                }
                }
                if (bytesToSkip > 0) {
                    reader.relativeSeek(bytesToSkip);
                }
            } else {
                // Searches the type of the index inside a look-up table.
                indexFoundType = typeOfIndexInInstructions[opcode];
                if (opcode == Opcodes.INSN_CONST_STRING_JUMBO) {
                    indexSize = 4;
                }

                // If we didn't find any index, then skip to the next Instruction.
                if (indexFoundType == 0) {
                    // Skips the instruction according to its size, - 2 because we already read the 16-bit opcode.
                    int instructionSize = Instruction.getInstructionSizeInByte(opcode) - 2;
                    if (instructionSize > 0) {
                        reader.relativeSeek(instructionSize);
                    }
                } else {
                    int savedReaderOffset = reader.getPos();
                    // We get to the encoded index.
                    int indexOffset = savedReaderOffset; // + 2 - 2 : +2 to reach the index, -2 because we already read the 16-bit opcode.
                    reader.seek(indexOffset);
                    // Reads the symbolic index according to its size.
                    int symbolicIndex = (indexSize == 2 ? reader.ushort() :
                                         reader.ushort() + (reader.ushort() << 16));

                    int resolvedIndex = -1;
                    switch (indexFoundType) {
                    case INDEX_STRING:
                        resolvedIndex = constantPool.getResolvedStringIndexFromSymbolicStringIndex(symbolicIndex);
                        break;
                    case INDEX_FIELD:
                        resolvedIndex = constantPool.getResolvedFieldIndexFromSymbolicFieldIndex(symbolicIndex);
                        break;
                    case INDEX_TYPE:
                        resolvedIndex = constantPool.getResolvedTypeIndexFromSymbolicTypeIndex(symbolicIndex);
                        break;
                    case INDEX_METHOD:
                        resolvedIndex = constantPool.getResolvedMethodIndexFromSymbolicMethodIndex(symbolicIndex);
                        break;
                    default:
                        try {
                            throw new Exception("Unknown Index type.");
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }

                    // Overwrites the symbolic index with the resolved index.
                    if (indexSize == 2) {
                        out.putShort(resolvedIndex, indexOffset);
                    } else {
                        out.putShort(resolvedIndex & 0xffff, indexOffset);
                        out.putShort((resolvedIndex >> 16) & 0xffff, indexOffset + 2);
                    }

                    // Skips the instruction according to its size, - 2 because we already read the 16-bit opcode.
                    int instructionSize = Instruction.getInstructionSizeInByte(opcode) - 2;
                    if (instructionSize > 0) {
                        reader.seek(savedReaderOffset + instructionSize);
                    }
                }
            }
        }
    }

    /**
     * Parses the tryCatch and maps the resolved indexes (Strings, Fields, Types, Methods) from the
     * symbolic ones. The input vector is not modified.
     * The mapping between the two, done through four tables in the Constant Pool,
     * must have been performed before.
     * @param in a ByteVector containing the try_catch bytecode to modify.
     * @param offsetInInputArray offset in bytes where the parsing must begin.
     * @param nbTries count of Try/Catch.
     * @return a ByteVector containing the try_catch bytecode.
     */
    public ByteVector mapResolvedIndexesTryCatch(ByteVector in, int offsetInInputArray, int nbTries) {
        ByteVector out = new ByteVector();

        if (nbTries == 0) {
            return out;
        }
        // The reader and writer are not shared, as ULeb128 encoded field may differ in size.
        IDalvikValueReader reader = new DalvikValueReader(in.getBuffer());

        reader.seek(offsetInInputArray);

        // Duplicates the try_item elements.
        for (int i = 0; i < nbTries; i++) {
            out.putInt(reader.uint());
            out.putShort(reader.ushort());
            out.putShort(reader.ushort()); // It may move will be patched later.
        }

        // Get reference position of the encoded_catch_handler
        int absoluteOldPos = reader.getPos();
        int absoluteNewPos = out.getLength();
        HashMap<Integer,Integer> oldToNewHandlerOffsets = new HashMap<Integer,Integer>();
        // Encodes the encoded_catch_handler elements. First the list.
        int nbEncodedCatchHandler = reader.uleb128();
        out.putUleb128(nbEncodedCatchHandler);
        // Encodes each of them.
        for (int i = 0; i < nbEncodedCatchHandler; i++) {
            // Detects shifts in handler position
            int oldOffset = reader.getPos() - absoluteOldPos;
            int newOffset = out.getLength() - absoluteNewPos;
            if (oldOffset != newOffset) {
                oldToNewHandlerOffsets.put(oldOffset, newOffset);
            }

            int readSize = (int)reader.sleb128(); // May be negative if catch_all present.
            int size = (readSize >= 0 ? readSize : -readSize);
            out.putSleb128(readSize);
            // Encodes each encoded_type_addr_pair.
            for (int j = 0; j < size; j++) {
                int symbolicIndex = reader.uleb128(); // Encodes the type_idx.
                int resolvedIndex = constantPool.getResolvedTypeIndexFromSymbolicTypeIndex(symbolicIndex);
                out.putUleb128(resolvedIndex);

                out.putUleb128(reader.uleb128());	// Encodes the addr.
            }

            if (readSize <= 0) {
                out.putUleb128(reader.uleb128());	// Encodes the catch_all_addr.
            }
        }

        if (oldToNewHandlerOffsets.size() != 0) {
            // Handlers were shifted
            int offset = HANDLER_OFFSET_IN_TRY_ITEM_STRUCTURE;
            for (int i = 0; i < nbTries; i++) {
                reader.seek(offsetInInputArray + offset);
                int oldHandlerOffset = reader.ushort();
                Integer newHandlerOffset = oldToNewHandlerOffsets.get(oldHandlerOffset);
                if (newHandlerOffset != null) {
                    out.putShort(newHandlerOffset, offset);
                }
                offset += TRY_ITEM_STRUCTURE_SIZE;
            }
        }
        return out;
    }


    // ------------------------------------
    // Getters and setters.
    // ------------------------------------

    /**
     * Returns the size in bytes of all the instructions of this item (and by extension, the method).
     * Does NOT take in account the header of the Code Item.
     * @return the size in bytes of all the instructions of this item, without the header.
     */
    public int getSize() {
        return size;
    }

    /**
     * Returns the number of Try structure in this Code Item.
     * @return the number of Try structure in this Code Item.
     */
    public int getTriesSize() {
        return tryCatches.size();
    }

    /**
     * Sets the Register Size of this code.
     * @param registerSize the Register Size of this code.
     */
    public void setRegisterSize(int registerSize) {
        this.registerSize = registerSize;
    }

    /**
     * Gets the number of registers used by this code.
     * @return the number of registers used by this code.
     */
    public int getRegistersSize() {
        return registerSize;
    }

    /**
     * Returns the list of the Labels of this code item.
     * @return the list of the Labels of this code item.
     */
    public List<Label> getLabels() {
        return labels;
    }

    /**
     * Gets the size in word of the Incoming Arguments.
     * @return the size in word of the Incoming Arguments.
     */
    public int getIncomingArgumentsSizeInWord() {
        return incomingArgumentsSizeInWord;
    }

    /**
     * Sets the size in word of the Incoming Arguments.
     * @param incomingArgumentsSizeInWord the size in word of the Incoming Arguments.
     */
    public void setIncomingArgumentsSizeInWord(int incomingArgumentsSizeInWord) {
        this.incomingArgumentsSizeInWord = incomingArgumentsSizeInWord;
    }

    /**
     * Gets the size in word of the Outgoing Arguments.
     * @return the size in word of the Outgoing Arguments.
     */
    public int getOutgoingArgumentsSizeInWord() {
        return outgoingArgumentsSizeInWord;
    }

    /**
     * Sets the size in word of the Outgoing Arguments.
     * @param outgoingArgumentsSizeInWord the size in word of the Outgoing Arguments.
     */
    public void setOutgoingArgumentsSizeInWord(int outgoingArgumentsSizeInWord) {
        this.outgoingArgumentsSizeInWord = outgoingArgumentsSizeInWord;
    }

    /**
     * Returns the offset of the Code Item from the beginning of the file. It is only known when
     * producing the output file, after the input has been fully parsed.
     * @return the offset of the Code Item from the beginning of the file.
     */
    public int getOffset() {
        return offset;
    }

    /**
     * Sets the offset of the Code Item from the beginning of the file.
     * @param offset the offset of the Code Item from the beginning of the file.
     */
    public void setOffset(int offset) {
        this.offset = offset;
    }

    /**
     * Sets the Method this Code Item is linked to.
     * @param method the Method this Code Item is linked to.
     */
    public void setMethod(Method method) {
        this.method = method;
    }

    /**
     * Sets the offset of the Debug Info Item inside the already encoded Code Item.
     * @param out Vector of the Dex file. It already contains the current Code Item.
     * @param debugInfoItemOffset the offset of the debug_info_item, encoded or soon to be.
     */
    public void setDebugInfoItemOffset(ByteVector out, int debugInfoItemOffset) {
        out.putInt(debugInfoItemOffset, offset + DEBUG_INFO_OFFSET_OFFSET); // Reaches the debug_info_off field.
    }

    /**
     * Sets the first line number of this code_item, but only if none has been found before.
     * @param firstLineNumber the first line number of this code_item.
     */
    public void setFirstLineNumber(int firstLineNumber) {
        if (this.firstLineNumber == 0) {
            this.firstLineNumber = firstLineNumber;
        }
    }

    /**
     * Returns the first line number found. It may be 0 if none were.
     * @return the first line number found, or 0.
     */
    public int getFirstLineNumber() {
        return firstLineNumber;
    }

    /**
     * Returns the code_item code (including the code_item header and the bytecode), <i>without</i> the
     * try/catch fields after the insns field, but with the padding if needed.
     * It uses <i>symbolic</i> references, so must be parsed again to link them to the "real"
     * elements.
     * @return the code_item code, without the try/catch fields.
     */
    public ByteVector getCodeItemCode() {
        if (codeItemCode == null) {
            generateCodeItemCode();
        }
        return codeItemCode;
    }

    /**
     * Returns the try/catch section of the code_item, beginning by the possible padding after the insns field,
     * or Null if no try/catch is present.
     * It uses <i>symbolic</i> references, so must be parsed again to link them to the "real"
     * elements.
     * @return the try/catch code, or Null.
     */
    public ByteVector getCodeItemTryCatch() {
        if (codeItemCode == null) { // Generating the code will generate the try/catch
            generateCodeItemCode(); // (or not, if there is none).
        }
        return codeItemTryCatch;
    }

    /**
     * Returns the debug_info_item code, still using symbolic indexes.
     * It is available only after this method bytecode has been generated.
     * @return the debug_info_item code still using symbolic indexes.
     */
    public ByteVector getDebugInfoItemCode() {
        return debugInfoItem.getDebugInfoItemCode();
    }

    /**
     * Indicates if at least one index is encoded in the debug code. We use it to optimize the mapping
     * with the resolved indexes : it doesn't need do be done if no index was found.
     * @return true if at least one index is encoded in the debug code.
     */
    public boolean areSymbolicIndexesUsedInDebugCodeItem() {
        return debugInfoItem.areSymbolicIndexesUsed();
    }


    // ------------------------------------
    // Private methods.
    // ------------------------------------

    /**
     * Encodes the Try/Catch/Handler structures of the code_item, inside a new codeItemTryCatch buffer.
     * @param constantPool the Constant Pool.
     */
    private void writeTryCatches(ConstantPool constantPool) {
        if (getTriesSize() == 0) {
            return;
        }
        codeItemTryCatch = new ByteVector();

        // Encodes the "tries" field. For now, the handler is not encoded.
        for (TryCatch tc : tryCatches) {
            int startAddr = tc.getStart().getOffset();
            int endAddr = tc.getEnd().getOffset();
            codeItemTryCatch.putInt(startAddr / 2); // / 2 because based on 16-bit units.
            codeItemTryCatch.putShort((endAddr - startAddr) / 2); // / 2 because based on 16-bit units.
            codeItemTryCatch.putShort(0); // Not encoded for now.

            // Builds an EncodedCatchHandler to provide uniqueness of these structures, and link it to
            // the TryCatch.
            EncodedCatchHandler ech = new EncodedCatchHandler(constantPool);
            for (ExceptionHandler eh : tc.getExceptionHandlers()) {
                ech.addTypeAddrPair(eh.getType(), eh.getHandler());
            }
            encodedCatchHandlers.add(ech);

            // Links the TryCatch to the EncodedCatchHandler.
            tryCatchToEncodedCatchHandler.put(tc, ech);
        }

        // Encodes the encoded_catch_handler_list.
        int absoluteOffsetEncodedCatchHandlerList = codeItemTryCatch.getLength();
        // Encodes total count of Catch Handlers to be encoded. They may be shared.
        codeItemTryCatch.putUleb128(encodedCatchHandlers.size());

        for (EncodedCatchHandler ech : encodedCatchHandlers) {

            int offsetEncodedCatchHandlerCurrentItem = codeItemTryCatch.getLength() - absoluteOffsetEncodedCatchHandlerList;
            encodedCatchHandlerToRelativeOffset.put(ech, offsetEncodedCatchHandlerCurrentItem);
            ech.write(codeItemTryCatch);
        }

        // Links the TryCatches with the EncodedCatchHandler offsets.
        int offset = HANDLER_OFFSET_IN_TRY_ITEM_STRUCTURE;
        for (TryCatch tc : tryCatches) {
            codeItemTryCatch.putShort(encodedCatchHandlerToRelativeOffset.get(tryCatchToEncodedCatchHandler.get(tc)), offset);
            offset += TRY_ITEM_STRUCTURE_SIZE;
        }
    }
}
