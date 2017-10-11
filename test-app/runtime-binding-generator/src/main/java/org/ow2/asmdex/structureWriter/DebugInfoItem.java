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
import java.util.List;
import java.util.TreeSet;

import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.instruction.DebugInstruction;
import org.ow2.asmdex.instruction.DebugInstructionAdvanceLine;
import org.ow2.asmdex.instruction.DebugInstructionAdvancePC;
import org.ow2.asmdex.instruction.DebugInstructionEndLocal;
import org.ow2.asmdex.instruction.DebugInstructionRestartLocal;
import org.ow2.asmdex.instruction.DebugInstructionSetPrologueEnd;
import org.ow2.asmdex.instruction.DebugInstructionSpecialOpcode;
import org.ow2.asmdex.instruction.DebugInstructionStartLocal;
import org.ow2.asmdex.instruction.DebugInstructionStartLocalExtended;
import org.ow2.asmdex.instruction.Instruction;
import org.ow2.asmdex.lowLevelUtils.ByteVector;
import org.ow2.asmdex.lowLevelUtils.DalvikValueReader;
import org.ow2.asmdex.lowLevelUtils.IDalvikValueReader;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureCommon.LocalVariable;

/**
 * This Class represents the debug_info_item structure of one code_item.
 *
 * It also holds the method to write to a byte array the debug bytecode.
 *
 * NOTE :
 * - Epilogue : Not encoded, as I don't know how it is handled (I never saw it encoded).
 * - Source file : Not encoded. It would require a new event from AsmDex, not very important.
 *
 * @author Julien Névo
 */
public class DebugInfoItem {

    /**
     * Vector that contains the opcodes of the debug_info_item, including the header.
     */
    private ByteVector debugCode;

    /**
     * Indicates if at least one index is encoded in the debug code. We use it to optimize the mapping
     * with the resolved indexes : it doesn't need do be done if no index was found.
     */
    private boolean areSymbolicIndexesUsed;

    /**
     * Current Debug Address. One of the debug_info_item internal variables.
     */
    private int debugAddress = 0;

    /**
     * Current Debug Line. One of the debug_info_item internal variables.
     */
    private int debugLine;

    /**
     * Indicates if the next Instruction is going to be the first. Used to write the DBG_PROLOGUE_END
     * Debug Instruction.
     */
    private boolean isFirstInstruction = true;

    /**
     * The Constant Pool of the Application.
     */
    final private ConstantPool constantPool;

    /**
     * Map linking an offset to one or more Debug Instruction.
     */
    private HashMap<Integer, List<DebugInstruction>> offsetToDebugInstructions = new HashMap<Integer, List<DebugInstruction>>();

    /**
     * Constructor of the Debug Info Item.
     * @param constantPool the Constant Pool of the Application.
     */
    public DebugInfoItem(ConstantPool constantPool) {
        this.constantPool = constantPool;
    }


    // ------------------------------------
    // Public methods.
    // ------------------------------------

    /**
     * Frees all the structures (list of debug instructions...) so that they don't
     * consume memory. This should be done <i>after</i> having generated the bytecode, once the
     * method has been parsed and its end visited.
     */
    public void free() {
        offsetToDebugInstructions = null;
        // Parameters not Nulled, as we need them when generating the final Dex file.
    }

    private enum LocalVariableType { end(0), local(1), restart(2);
                                     private final int id;
    LocalVariableType(int id) {
        this.id = id;
    }
                                   };

    /**
     * Simple intermediary class in order to help sorting the local variable.
     *
     * @author Julien Névo
     */
    private static class LocalVariableItem implements Comparable<LocalVariableItem> {
        public LocalVariableType localVariableType;
        public int register;
        public String name;
        public Label label;
        public String signature; // Only used by "starting" local variables.
        public String type; // Descriptor.

        public LocalVariableItem(LocalVariableType localVariableType, int register, String name,
                                 Label label, String signature, String type) {
            this.localVariableType = localVariableType;
            this.register = register;
            this.name = name;
            this.label = label;
            this.signature = signature;
            this.type = type;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }

            if (obj instanceof LocalVariableItem) {
                LocalVariableItem lvi = (LocalVariableItem)obj;
                boolean result = (name.equals(lvi.name))
                                 && (register == lvi.register)
                                 && (localVariableType == lvi.localVariableType)
                                 && (label.equals(lvi.label))
                                 && (type.equals(lvi.type));
                if (result) {
                    result = (signature != null ? signature.equals(lvi.signature) :
                              lvi.signature == null);
                }
                return result;
            }

            return false;
        }

        @Override
        public int hashCode() {
            int result = (signature != null ? signature.hashCode() : 0);
            result += localVariableType.hashCode() + register * 33 + name.hashCode()
                      + label.hashCode() + type.hashCode();

            return result;
        }


        @Override
        public int compareTo(LocalVariableItem var) {
            if (this == var) {
                return 0;
            }

            int result;

            if (localVariableType == var.localVariableType) {
                result = (register == var.register ? 0 : (register > var.register ? 1 : -1));
                if (result == 0) {
                    int offset1 = label.getOffset();
                    int offset2 = var.label.getOffset();
                    result = (offset1 == offset2 ? 0 : (offset1 > offset2 ? 1 : -1));
                }
            } else {
                int type1 = localVariableType.id;
                int type2 = var.localVariableType.id;
                result = (type1 == type2 ? 0 : (type1 > type2 ? 1 : -1));
            }

            return result;
        }
    }


    /**
     * Initializes the debug_info_item vector by creating its header. It is <i>not</i> useable as-is though,
     * but it can only be when the whole application has been parsed, as it encodes the parameters index
     * from its symbolic index. It will have to be parsed again later.
     *
     * @param parameters the parameters of the Method, or null.
     * @param codeItem the Code Item the Debug Info item is related to.
     * @param localVariables the Local Variables of the current Method.
     */
    public void initializeDebugInfoItem(String[] parameters, CodeItem codeItem, List<LocalVariable> localVariables) {
        debugCode = new ByteVector();

        // Initializes the Debug Instructions. Links offsets to Debug Instructions.
        offsetToDebugInstructions.clear();

        areSymbolicIndexesUsed = localVariables.size() > 0;

        // First of all, we need to sort the Local Variables. This is not mandatory in the dex format,
        // but dx does it and it allows a better output with baksmali (no diffs) when comparing files
        // when debugging.
        // To do that, we create intermediary objects that we sort.
        if (localVariables.size() > 0) {
            TreeSet<LocalVariableItem> items = new TreeSet<LocalVariableItem>();

            for (LocalVariable lv : localVariables) {
                // Is there any variable that starts ?
                Label start = lv.getStart();
                if (start != null) {
                    LocalVariableItem item = new LocalVariableItem(LocalVariableType.local,
                            lv.getRegister(), lv.getName(), start, lv.getSignature(), lv.getType()
                                                                  );
                    items.add(item);
                }


                // Is there any variable that ends ?
                if (lv.getEnds() != null) {
                    for (Label end : lv.getEnds()) {
                        LocalVariableItem item = new LocalVariableItem(LocalVariableType.end,
                                lv.getRegister(), lv.getName(), end, lv.getSignature(), lv.getType()
                                                                      );
                        items.add(item);
                    }
                }

                // Is there any variable that restarts ?
                if (lv.getRestarts() != null) {
                    for (Label restart : lv.getRestarts()) {
                        LocalVariableItem item = new LocalVariableItem(LocalVariableType.restart,
                                lv.getRegister(), lv.getName(), restart, lv.getSignature(), lv.getType()
                                                                      );
                        items.add(item);
                    }
                }
            }

            // Nows creates the Debug Instructions from the sorted list.
            for (LocalVariableItem item : items) {
                switch (item.localVariableType) {
                case end: {
                    DebugInstruction insn = new DebugInstructionEndLocal(item.register, item.label);
                    addDebugInstruction(item.label.getOffset(), insn);
                    break;
                }
                case local: {
                    String signature = item.signature;
                    DebugInstruction insn;
                    if (signature == null) {
                        insn = new DebugInstructionStartLocal(item.register,
                                                              item.name, item.type, item.label);
                    } else {
                        insn = new DebugInstructionStartLocalExtended(item.register,
                                item.name, item.type, signature, item.label);
                    }
                    addDebugInstruction(item.label.getOffset(), insn);
                    break;
                }
                case restart: {
                    DebugInstruction insn = new DebugInstructionRestartLocal(item.register, item.label);
                    addDebugInstruction(item.label.getOffset(), insn);
                    break;
                }
                default:
                    try {
                        throw new IllegalArgumentException("Unknown Local Variable type.");
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        // Gets the line_start, which we may have found while parsing the code.
        // If not, we set it to 1, as a line_number should be > 0.
        int firstLineFound = codeItem.getFirstLineNumber();
        debugLine = (firstLineFound == 0) ? 1 : firstLineFound;
        debugCode.putUleb128(debugLine);

        // Parameters_size.
        int parameterCount = (parameters == null) ? 0 : parameters.length;
        debugCode.putUleb128(parameterCount);

        // Parameter_names.
        if (parameterCount > 0) {
            areSymbolicIndexesUsed = true;
            for (int i = 0; i < parameterCount; i++) {
                int parameterIndex = parameters[i].equals("") ? Opcodes.NO_INDEX_SIGNED :
                                     constantPool.getStringIndex(parameters[i]);
                debugCode.putUleb128p1(parameterIndex);
            }
        }
    }

    /**
     * Parses the Debug Information that an Instruction to be encoded contains (or not), and fills
     * the debug_code_item accordingly. This one is however not encoded for now in the Dex file.
     * @param instruction the Instruction to be encoded.
     * @param address address in bytes of the Instruction from the beginning of the byte code itself.
     */
    public void parseDebugInformation(Instruction instruction, int address) {

        // If this instruction is the first one, marks the end of the Prologue.
        if (isFirstInstruction) {
            encodeDebugSetPrologueEnd();
            isFirstInstruction = false;
        }

        // If the Instruction has a line number, we encode it.
        int newLineNumber = instruction.getLineNumber();
        if (newLineNumber > 0) {
            int deltaLine = newLineNumber - debugLine;
            int deltaOffset = address - debugAddress;
            int deltaOffsetInWord = deltaOffset / 2;
            // In order to be encoded as a Special Opcodes, the delta line must be from (inclusive)
            // -4 and 10 (10 = DBG_LINE_RANGE + DBG_LINE_BASE - 1), and the delta offset from (inclusive)
            // 0 to 16 (16 because 16 * DBG_LINE_RANGE + DBG_FIRST_SPECIAL is the maximum number to fit in
            // a byte).
            boolean isDeltaLineWithinLimit =
                ((deltaLine <= (Opcodes.DBG_LINE_RANGE + Opcodes.DBG_LINE_BASE - 1))
                 && (deltaLine >= Opcodes.DBG_LINE_BASE));
            boolean isDeltaOffsetWithinLimit = ((deltaOffsetInWord >= 0) && (deltaOffsetInWord <= Opcodes.DBG_LINE_RANGE));

            // If one of the value is out of limit, we encode the change now (BEFORE the Special Opcode).
            // It will be taken in account for the next Instruction, when the Special Opcode is encountered.
            if (!isDeltaOffsetWithinLimit) {
                encodeDebugAdvancePC(deltaOffsetInWord);
            }
            if (!isDeltaLineWithinLimit) {
                encodeDebugAdvanceLine(deltaLine);
            }

            // We must encode a Special Opcode anyway, because it emits the new position. The out of
            // limit values are set as 0.
            encodeDebugSpecialOpcode(isDeltaOffsetWithinLimit ? deltaOffsetInWord : 0,
                                     isDeltaLineWithinLimit ? deltaLine : 0);

            debugLine = newLineNumber;
            debugAddress = address;
        }

        // Checks if this offset matches a Debug Instruction.
        if (offsetToDebugInstructions.containsKey(address)) {
            for (DebugInstruction debugInstruction : offsetToDebugInstructions.get(address)) {
                if (address != debugAddress) {
                    encodeDebugAdvancePC((address - debugAddress) / 2);
                }
                debugAddress = address;
                debugInstruction.write(debugCode, constantPool);
            }
        }
    }

    /**
     * Closes the debug_info_item by adding a End_Sequence opcode.
     */
    public void closeDebugInfoItem() {
        if (debugCode.getLength() > 0) {
            debugCode.putByte(Opcodes.DBG_END_SEQUENCE);
        }
    }

    /**
     * Parses the debug bytecode and maps the resolved indexes (Strings, Types) from the
     * symbolic ones. The mapping between the two, done through two tables in the Constant Pool,
     * must have been performed before.
     * @param in input buffer to parse.
     * @param offsetInputBuffer offset inside the input buffer from where to start the parsing.
     * @return the parsed and modified debug bytecode.
     */
    public ByteVector mapResolvedIndexes(ByteVector in, int offsetInputBuffer) {

        // A different byte array is used in input and output, because odds are that different sized
        // index will be encoded.
        byte[] data = in.getBuffer();
        IDalvikValueReader reader = new DalvikValueReader(data);
        reader.seek(offsetInputBuffer);
        ByteVector out = new ByteVector();

        out.putUleb128(reader.uleb128()); 	// Encodes line_start.
        int nbParameters = reader.uleb128();
        out.putUleb128(nbParameters);			// Encodes parameters_size.
        // Maps symbolic parameters, if any, to resolved parameters.
        for (int i = 0; i < nbParameters; i++) {
            mapResolvedIndexFromSymbolicIndex(out, reader.uleb128_p1(), true);
        }

        boolean continueParsing = true;
        // Encodes the debug instructions proper.
        while (continueParsing) {
            int opcode = reader.ubyte();
            out.putByte(opcode); // Always encodes the opcode. Also takes care of the special opcodes.
            switch (opcode) {
            case Opcodes.DBG_ADVANCE_PC:
            case Opcodes.DBG_END_LOCAL:
            case Opcodes.DBG_RESTART_LOCAL:
                out.putUleb128(reader.uleb128());
                break;
            case Opcodes.DBG_ADVANCE_LINE:
                out.putSleb128(reader.sleb128());
                break;
            case Opcodes.DBG_SET_FILE:
                mapResolvedIndexFromSymbolicIndex(out, reader.uleb128_p1(), true);
                break;
            case Opcodes.DBG_START_LOCAL:
                out.putUleb128(reader.uleb128()); // register_num.
                mapResolvedIndexFromSymbolicIndex(out, reader.uleb128_p1(), true); // name_idx.
                mapResolvedIndexFromSymbolicIndex(out, reader.uleb128_p1(), false); // type_idx.
                break;
            case Opcodes.DBG_START_LOCAL_EXTENDED:
                out.putUleb128(reader.uleb128()); // register_num.
                mapResolvedIndexFromSymbolicIndex(out, reader.uleb128_p1(), true); // name_idx.
                mapResolvedIndexFromSymbolicIndex(out, reader.uleb128_p1(), false); // type_idx.
                mapResolvedIndexFromSymbolicIndex(out, reader.uleb128_p1(), true); // sig_idx.
                break;
            case Opcodes.DBG_END_SEQUENCE:
                continueParsing = false;
            }

        }

        return out;
    }

    /**
     * Encodes a resolved String or Type index from a symbolic index, in the uleb128p1 format.
     * If the symbolic index is NO_INDEX, then the same NO_INDEX is encoded.
     * @param out the ByteVector where to encode the index.
     * @param symbolicIndex the symbolic index, or NO_INDEX.
     * @param isStringIndex true for String index, false for Type index.
     */
    private void mapResolvedIndexFromSymbolicIndex(ByteVector out, int symbolicIndex, boolean isStringIndex) {
        int resolvedIndex;
        if (symbolicIndex != Opcodes.NO_INDEX_SIGNED) {
            if (isStringIndex) {
                resolvedIndex = constantPool.getResolvedStringIndexFromSymbolicStringIndex(symbolicIndex);
            } else {
                resolvedIndex = constantPool.getResolvedTypeIndexFromSymbolicTypeIndex(symbolicIndex);
            }
        } else {
            resolvedIndex = Opcodes.NO_INDEX_SIGNED;
        }
        out.putUleb128p1(resolvedIndex);
    }


    // ------------------------------------
    // Getters and setters.
    // ------------------------------------

    /**
     * Returns the bytecode of the Debug Info Item, still using symbolic indexes.
     * The method bytecode must have been generated first.
     * @return the bytecode of the Debug Info Item, still using symbolic indexes.
     */
    public ByteVector getDebugInfoItemCode() {
        return debugCode;
    }

    /**
     * Indicates if at least one index is encoded in the debug code. We use it to optimize the mapping
     * with the resolved indexes : it doesn't need do be done if no index was found.
     * @return true if at least one index is encoded in the debug code.
     */
    public boolean areSymbolicIndexesUsed() {
        return areSymbolicIndexesUsed;
    }


    // ------------------------------------
    // Private methods.
    // ------------------------------------

    /**
     * Adds a Debug Instruction to the list linked to the given offset.
     * @param offset the offset to which is linked the Debug Instruction.
     * @param insn the debug instruction to add.
     */
    private void addDebugInstruction(int offset, DebugInstruction insn) {
        List<DebugInstruction> list;
        if (!offsetToDebugInstructions.containsKey(offset)) {
            list = new ArrayList<DebugInstruction>(1);
            offsetToDebugInstructions.put(offset, list);
        } else {
            list = offsetToDebugInstructions.get(offset);
        }
        list.add(insn);
    }

    /**
     * Encodes a Debug_Advance_PC Instruction.
     * @param deltaOffsetInWord delta offset in word to encode.
     */
    private void encodeDebugAdvancePC(int deltaOffsetInWord) {
        DebugInstruction insn = new DebugInstructionAdvancePC(deltaOffsetInWord);
        insn.write(debugCode, constantPool);
    }

    /**
     * Encodes a Debug_Advance_Line Instruction.
     * @param deltaLine delta line to encode.
     */
    private void encodeDebugAdvanceLine(int deltaLine) {
        DebugInstruction insn = new DebugInstructionAdvanceLine(deltaLine);
        insn.write(debugCode, constantPool);
    }

    /**
     * Encodes a Debug_Set_Prologue_End Instruction.
     */
    private void encodeDebugSetPrologueEnd() {
        DebugInstruction insn = new DebugInstructionSetPrologueEnd();
        insn.write(debugCode, constantPool);
    }


    /**
     * Encodes a Special Opcode (>= 0xa) according to the given delta offset and delta line.
     * This code doesn't test if the deltas are inside the authorized limits.
     * @param deltaOffsetInWord the delta offset in Word.
     * @param deltaLine the delta line.
     */
    private void encodeDebugSpecialOpcode(int deltaOffsetInWord, int deltaLine) {
        DebugInstruction insn = new DebugInstructionSpecialOpcode(deltaOffsetInWord, deltaLine);
        insn.write(debugCode, constantPool);
    }


}
