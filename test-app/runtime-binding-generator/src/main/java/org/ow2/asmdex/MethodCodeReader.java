package org.ow2.asmdex;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.ow2.asmdex.instruction.InstructionFormat10T;
import org.ow2.asmdex.instruction.InstructionFormat11N;
import org.ow2.asmdex.instruction.InstructionFormat11X;
import org.ow2.asmdex.instruction.InstructionFormat12X;
import org.ow2.asmdex.instruction.InstructionFormat20T;
import org.ow2.asmdex.instruction.InstructionFormat21C;
import org.ow2.asmdex.instruction.InstructionFormat21H;
import org.ow2.asmdex.instruction.InstructionFormat21S;
import org.ow2.asmdex.instruction.InstructionFormat21T;
import org.ow2.asmdex.instruction.InstructionFormat22B;
import org.ow2.asmdex.instruction.InstructionFormat22C;
import org.ow2.asmdex.instruction.InstructionFormat22S;
import org.ow2.asmdex.instruction.InstructionFormat22T;
import org.ow2.asmdex.instruction.InstructionFormat22X;
import org.ow2.asmdex.instruction.InstructionFormat23X;
import org.ow2.asmdex.instruction.InstructionFormat30T;
import org.ow2.asmdex.instruction.InstructionFormat31C;
import org.ow2.asmdex.instruction.InstructionFormat31I;
import org.ow2.asmdex.instruction.InstructionFormat31T;
import org.ow2.asmdex.instruction.InstructionFormat32X;
import org.ow2.asmdex.instruction.InstructionFormat35C;
import org.ow2.asmdex.instruction.InstructionFormat3RC;
import org.ow2.asmdex.instruction.InstructionFormat51L;
import org.ow2.asmdex.lowLevelUtils.DexFileReader;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureCommon.LocalVariable;
import org.ow2.asmdex.structureReader.ISwitchCase;
import org.ow2.asmdex.structureReader.PackedSwitch;
import org.ow2.asmdex.structureReader.SparseSwitch;
import org.ow2.asmdex.structureReader.TryCatch;

/**
 * Class made to read the code of one method of a Dex file, and make a visitor visits its instructions.
 * Also parses the debug_info structure to get complementary information (line numbers, local variables...).
 * <br/><br/>
 * Should only be used by ApplicationReader.
 * <br/><br/>
 * The NOP instructions are ignored if we reach any Pseudo Instruction such as Packed/Sparse Switch or
 * Fill Array Data, so that NOP don't stack up if we transform an already generated dex file. Meeting any
 * other instruction will stop this behavior.
 *
 * @author Julien Névo
 */
public class MethodCodeReader {

    /**
     * The Method Visitor.
     */
    protected MethodVisitor methodVisitor;

    /**
     * The reader of the Dex file.
     */
    protected DexFileReader dexFile;

    /**
     * Offset of the code of the method inside the Dex file. May be 0 if the method is abstract or native.
     */
    protected int codeOffset;

    /**
     * Indicates if the debug information must be skipped.
     */
    protected boolean skipDebug;

    /**
     * Offset in the Dex file of the first opcode of the code of the current method.
     */
    protected int instructionsStartOffset;

    /**
     * Offset in the Dex file of the byte next to the last opcode/data of the code
     * of the method.
     */
    protected int instructionsEndOffset;

    /**
     * Map containing the labels located at the given offset (from the beginning
     * of the code of the method).
     */
    protected HashMap<Integer, Label> labels;

    /**
     * Map containing the Packed Switch Structures located at the given offset
     * (from the beginning of the code of the method). The offset points to the
     * packed-switch opcode, NOT the packed-switch format itself.
     */
    protected HashMap<Integer, PackedSwitch> packedSwitchStructures;

    /**
     * Map containing the Sparse Switch Structures located at the given offset
     * (from the beginning of the code of the method). The offset points to the
     * sparse-switch opcode, NOT the sparse-switch format itself.
     */
    protected HashMap<Integer, SparseSwitch> sparseSwitchStructures;

    /**
     * Map containing lists of the TryCatch Structures located at the given offset
     * (from the beginning of the code of the method). The offset points to the
     * Try.
     */
    protected HashMap<Integer, ArrayList<TryCatch>> listTryCatchStructures;

    /**
     * Map containing, for each local variable identified by its number (as
     * described in the debug_info_item), a list of Local Variables.
     * Why a List ? Because a register number can share one or more variables
     * (though it doesn't happen often).
     * Note that when a Local Variable is started, it is added to the end of the
     * list, and all the Restarts/Ends found will be added to this Variable.
     */
    protected HashMap<Integer, List<LocalVariable>> localVariableLists;

    /**
     * Map linking a register to a type if it holds an array. It is useful for the
     * NewArray/FillNewArray instructions.
     */
    private HashMap<Integer, Integer> registerArrayType = new HashMap<Integer, Integer>();

    /*
     * The five "state machine" registers of the debug_info_item.
     */
    /**
     * register of debug state machine : the debug address
     */
    protected int debugAddress;
    /**
     * register of debug state machine : the debug line
     */
    protected int debugLine;
    /**
     * register of debug state machine : the source file
     */
    protected String debugSourceFile;
    /**
     * register of debug state machine : the prologue end
     */
    protected boolean debugPrologueEnd;
    /**
     * register of debug state machine : the epilogue end
     */
    protected boolean debugEpilogueBegin;

    /**
     * Copy of the original debugLine. This is useful, because we make two passes, and need
     * to recover the original value for the second.
     */
    protected int debugLineAtInitialization;

    /**
     * Copy of the original debugCurrentOpcodeOffset. This is useful, because we make two
     * passes, and need to recover the original value for the second.
     */
    protected int debugCurrentOpcodeOffsetAtInitialization;

    /**
     * Offset on opcodes of the debug_info_item.
     */
    protected int debugCurrentOpcodeOffset;

    /**
     * Indicates if a new Debug Line is emitted.
     */
    protected boolean newDebugLineEmitted;

    /**
     * Last emitted line (DBG_ADVANCE_LINE not counted).
     */
    protected int debugEmittedLine;

    /**
     * Size in bytes of a try_item structure.
     */
    private static final int TRY_ITEM_STRUCTURE_SIZE = 8;

    /**
     * Indicates if the Pseudo Instructions at the end of the method has been reached.
     * If true, the NOPs encountered aren't thrown so as not to interpret them and stack them
     * when reading once again a generated file. However, it returns to false if others instructions are
     * encountered, though it shouldn't happen as we encode try/catch/data array structures at the end
     * of the Method. But some compilers may do otherwise.
     */
    private boolean hasReachedPseudoInstructions = false;


    /**
     * Constructor of the MethodCodeReader.
     * @param dexFile  the Dex file.
     * @param methodVisitor visitor to visit the instructions of the code of the method.
     * @param codeOffset offset of the code of the method inside the Dex file. May be 0 if
     * 		  the method is abstract or native.
     * @param skipDebug indicates if the debug information must be skipped.
     */
    public MethodCodeReader(DexFileReader dexFile, MethodVisitor methodVisitor,
                            int codeOffset, boolean skipDebug) {
        this.dexFile = dexFile;
        this.methodVisitor = methodVisitor;
        this.codeOffset = codeOffset;
        this.skipDebug = skipDebug;
    }

    /**
     * Visits the code of a method. Also needs to parse the debug_info structure to get
     * complementary information (line number, local variables...).
     */
    public void visitMethodCode() {

        if (codeOffset != 0) {	// 0 if abstract or native.

            dexFile.seek(codeOffset);

            int registerSize = dexFile.ushort();

            dexFile.skipShort(); // Skips incomingArgumentsSize.
            dexFile.skipShort(); // Skips outgoingArgumentsSize.
            int triesSize = dexFile.ushort();
            int debugInfoOffset = dexFile.uint();
            int instructionsSizeInUShort = dexFile.uint();

            instructionsStartOffset = dexFile.getPos();
            instructionsEndOffset = instructionsStartOffset + instructionsSizeInUShort * 2;

            labels = new HashMap<Integer, Label>();
            packedSwitchStructures = new HashMap<Integer, PackedSwitch>();
            sparseSwitchStructures = new HashMap<Integer, SparseSwitch>();
            listTryCatchStructures = new HashMap<Integer, ArrayList<TryCatch>>(triesSize);
            localVariableLists = new HashMap<Integer, List<LocalVariable>>();

            // Checks the debug_info_item.
            if (!skipDebug && (debugInfoOffset != 0)) {
                // Reads the debug parameters. Even if not needed, we read them to get
                // past to reach the debug code just after.
                dexFile.seek(debugInfoOffset);

                debugLineAtInitialization = dexFile.uleb128();
                int debugParametersSize = dexFile.uleb128();

                // Parses the Parameters, and visits them.
                if (debugParametersSize > 0) {
                    String[] parameters = new String[debugParametersSize];
                    for (int indexParameter = 0 ; indexParameter < debugParametersSize; indexParameter++) {
                        int stringIndex = dexFile.uleb128_p1();
                        String paramName;
                        if (stringIndex == Opcodes.NO_INDEX_SIGNED) {
                            paramName = "";
                        } else {
                            paramName = dexFile.getStringItemFromStringIndex(stringIndex);
                        }
                        parameters[indexParameter] = paramName;
                    }
                    methodVisitor.visitParameters(parameters);
                }

                debugCurrentOpcodeOffsetAtInitialization = dexFile.getPos();
            }

            methodVisitor.visitCode();

            // Contrary to ASM, we visit the Maxs right at the beginning of the code.
            methodVisitor.visitMaxs(registerSize, 0);

            // Before parsing the code, we skip it in order to reach the
            // try/catch handler just after, if there is one.
            if (triesSize != 0) {
                dexFile.seek(instructionsEndOffset);
                // Padding ?
                if ((instructionsSizeInUShort % 2) != 0) {
                    dexFile.ushort();
                }
                parseTryItemsFormat(dexFile.getPos(), triesSize);
            }

            // Now we parse the code, in two passes.
            // First pass : we only parse the code in order to find the Labels
            // (from Goto, Switch...). Visitors are NOT called. So Adapters can't add/remove code either.
            parseCodeInstructions(methodVisitor, skipDebug, true);
            // Second pass : we parse the code and call all the visitors. Adapters may add/remove code.
            parseCodeInstructions(methodVisitor, skipDebug, false);

            // Visits the Local Variables.
            for (List<LocalVariable> localVariableList : localVariableLists.values()) {
                for (LocalVariable localVariable : localVariableList) {
                    methodVisitor.visitLocalVariable(localVariable.getName(), localVariable.getType(),
                                                     localVariable.getSignature(), localVariable.getStart(),
                                                     localVariable.getEnds(), localVariable.getRestarts(),
                                                     localVariable.getRegister());
                }
            }
        }

        // The visitEnd event is thrown by the calling method.
    }

    /**
     * Parses the instructions of the code of the current method, and makes the given visitor
     * visit it, unless the parsing is done only to get the Label offsets.
     * In the latter case, we build the Label table whenever the following instructions are
     * encountered : GOTO, GOTO/16, GOTO/32, PACKED-SWITCH, SPARSE-SWITCH, IF-test, IF-testz.
     * The dex file position <i>is</i> modified.
     * @param instructionsStartOffset offset in the Dex file of the first opcode of the
     * code of the method.
     * @param instructionsEndOffset offset in the Dex file of the byte next to the last
     * opcode/data of the code of the method.
     * @param methodVisitor visitor to call when parsing the code of the method.
     * @param skipDebug indicates if the debug information must be skipped.
     * @param findLabelsOnly indicates if the parsing is only about getting the Label offsets.
     * In this case, no method from the visitor must be called.
     */
    private void parseCodeInstructions(MethodVisitor methodVisitor, boolean skipDebug, boolean findLabelsOnly) {

        hasReachedPseudoInstructions = false;

        // Initializes the debug values.
        debugCurrentOpcodeOffset = debugCurrentOpcodeOffsetAtInitialization; // Offset on opcodes of the debug_info_item.

        debugAddress = 0;
        debugLine = debugLineAtInitialization;
        debugEmittedLine = 0;
        debugSourceFile = null;
        debugPrologueEnd = false;
        debugEpilogueBegin = false;
        newDebugLineEmitted = false;

        // Parses all the instructions.
        dexFile.seek(instructionsStartOffset);
        while (dexFile.getPos() < instructionsEndOffset) {
            int relativeOffset = dexFile.getPos() - instructionsStartOffset;
            if (!skipDebug && (debugCurrentOpcodeOffset >= 0)) {
                // Parses the Debug Information Item. This will work only if it has information
                // to display for the current offset. Note that we don't need to parse
                // this structure on the first pass (findLabelOnly).

                parseDebugInformationItem(methodVisitor, relativeOffset, findLabelsOnly);
            }
            // If a Label is present here, visit it. The label table must of course have
            // been built before (on the first pass).
            if (labels.containsKey(relativeOffset)) {
                Label label = labels.get(relativeOffset);
                // The line number is valid only if set in the last occurrence
                // of the Debug Informations parsing (it implies !skipDebug).
                if (newDebugLineEmitted) {
                    label.setLine(debugEmittedLine);
                }

                if (!findLabelsOnly) {
                    methodVisitor.visitLabel(label);
                }
            }

            // Visits the line number (previously parsed) AFTER we visited
            // the Label.
            if (newDebugLineEmitted) {
                if (!findLabelsOnly) {
                    methodVisitor.visitLineNumber(debugEmittedLine,
                                                  getLabel(relativeOffset));
                }
                newDebugLineEmitted = false;
            }

            // If a Try/Catch block is here, visit it. The TryCatch table has been
            // built before on first pass.
            if (listTryCatchStructures.containsKey(relativeOffset)) {
                ArrayList<TryCatch> listTryCatch = listTryCatchStructures.get(relativeOffset);
                for (TryCatch tcs : listTryCatch) {
                    if (!findLabelsOnly) {
                        methodVisitor.visitTryCatchBlock(tcs.getStart(),
                                                         tcs.getEnd(), tcs.getHandler(), tcs.getType());
                    }
                }
            }

            // Now reads the bytecode itself.
            int fullOpcode = dexFile.ushort();
            int shortOpcode = fullOpcode & 0xff;
            int highOrderByte = (fullOpcode >> 8) & 0xff;

            // Finding a NOP or "special" instructions such as the special instructions
            // such as Switch structures and Fill Array Data.
            if (shortOpcode == 0x00) {
                // NOP or "special" instructions.
                switch (highOrderByte) {
                case 0x00: // NOP.
                    if ((!findLabelsOnly) && (!hasReachedPseudoInstructions)) {
                        // NOPs are ignored when we have reached the Pseudo Instructions area (at the end
                        // of the methods. This prevents the stacking of NOPs when reading a file that we
                        // have previously generated.
                        methodVisitor.visitInsn(shortOpcode);
                    }
                    break;
                case 0x01: { // Packed-switch structure
                    // Since the structure was already parsed (because we parse it
                    // immediately when finding the packed-switch opcode), we skip it.
                    int packedSwitchSize = dexFile.ushort();
                    dexFile.seek(dexFile.getPos() + packedSwitchSize * 4 + 4);
                    hasReachedPseudoInstructions = true;
                    break;
                }
                case 0x02: { // Sparse-switch
                    // Since the structure was already parsed (because we parse it
                    // immediately when finding the packed-switch opcode), we skip it.
                    int sparseSwitchSize = dexFile.ushort();
                    dexFile.seek(dexFile.getPos() + sparseSwitchSize * 4 * 2);
                    hasReachedPseudoInstructions = true;
                    break;
                }
                case 0x03: { // Fill Array Data
                    // Since the structure was already parsed (because we parse it
                    // immediately when finding the fill-array-data opcode), we skip it.
                    int elementWidth = dexFile.ushort();
                    int elementSize = dexFile.uint();
                    int totalSizeInBytes = elementWidth * elementSize;
                    int codeUnitNumber = ((totalSizeInBytes + 1) / 2);
                    dexFile.seek(dexFile.getPos() + codeUnitNumber * 2);
                    hasReachedPseudoInstructions = true;
                    break;
                }
                default :
                    throw new RuntimeException("Unknown opcode after a 0x00 : 0x" + Integer.toHexString(highOrderByte) + " at " + Integer.toHexString(dexFile.getPos() - 2));
                }
            } else {
                // Other instructions than NOP are found. We thus consider we are not in a
                // "Pseudo Instructions" area, or have left one.
                hasReachedPseudoInstructions = false;
                switch (shortOpcode) {
                case 0x1: // 12x format.
                case 0x4:
                case 0x7: {
                    if (findLabelsOnly) {
                        // InstructionFormat12X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat12X.getRegisterA(fullOpcode);
                        int regB = InstructionFormat12X.getRegisterB(fullOpcode);
                        visitVarInsn(shortOpcode, methodVisitor, regA, regB);
                    }
                    break;
                }
                case 0x2: // 22x format.
                case 0x5:
                case 0x8: {
                    if (findLabelsOnly) {
                        InstructionFormat22X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat22X.getRegisterA(fullOpcode);
                        int regB = InstructionFormat22X.getRegisterB(dexFile);
                        visitVarInsn(shortOpcode, methodVisitor, regA, regB);
                    }
                    break;
                }
                case 0x3: // 32x format.
                case 0x6:
                case 0x9: {
                    if (findLabelsOnly) {
                        InstructionFormat32X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat32X.getRegisterA(dexFile);
                        int regB = InstructionFormat32X.getRegisterB(dexFile);
                        visitVarInsn(shortOpcode, methodVisitor, regA, regB);
                    }
                    break;
                }
                case 0xa: // 11x format.
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xf:
                case 0x10:
                case 0x11:
                case 0x1d:
                case 0x1e:
                case 0x27: {
                    if (findLabelsOnly) {
                        // InstructionFormat11X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat11X.getRegisterA(fullOpcode);
                        methodVisitor.visitIntInsn(shortOpcode, regA);
                    }
                    break;
                }
                case 0xe: { // 10x format.
                    if (!findLabelsOnly) {
                        methodVisitor.visitInsn(shortOpcode);
                    }
                    break;
                }
                case 0x12: { // 11n format.
                    if (findLabelsOnly) {
                        // InstructionFormat11N.skip(dexFile);
                    } else {
                        int regA = InstructionFormat11N.getRegisterA(fullOpcode);
                        int literalB = InstructionFormat11N.getLiteralB(fullOpcode);
                        visitVarInsn(shortOpcode, methodVisitor, regA, literalB);
                    }
                    break;
                }
                case 0x13: // 21s format.
                case 0x16: {
                    if (findLabelsOnly) {
                        InstructionFormat21S.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21S.getRegisterA(fullOpcode);
                        int literalB = InstructionFormat21S.getLiteralB(dexFile);
                        visitVarInsn(shortOpcode, methodVisitor, regA, literalB);
                    }
                    break;
                }
                case 0x14: // 31i format.
                case 0x17: {
                    if (findLabelsOnly) {
                        InstructionFormat31I.skip(dexFile);
                    } else {
                        int regA = InstructionFormat31I.getRegisterA(fullOpcode);
                        int literalB = InstructionFormat31I.getLiteralB(dexFile);
                        visitVarInsn(shortOpcode, methodVisitor, regA, literalB);
                    }
                    break;
                }
                case 0x15: { // 21h format.
                    if (findLabelsOnly) {
                        InstructionFormat21H.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21H.getRegisterA(fullOpcode);
                        int literalB = InstructionFormat21H.getLiteralB(dexFile);
                        visitVarInsn(shortOpcode, methodVisitor, regA, literalB << 16);
                    }
                    break;
                }
                case 0x19: { // 21h format.
                    if (findLabelsOnly) {
                        InstructionFormat21H.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21H.getRegisterA(fullOpcode);
                        int literalB = InstructionFormat21H.getLiteralB(dexFile);
                        visitVarInsn(shortOpcode, methodVisitor, regA, (long)literalB << 48);
                    }
                    break;
                }
                case 0x18: { // 51l format.
                    if (findLabelsOnly) {
                        InstructionFormat51L.skip(dexFile);
                    } else {
                        int regA = InstructionFormat51L.getRegisterA(fullOpcode);
                        long literalB = InstructionFormat51L.getLiteralB(dexFile);
                        methodVisitor.visitVarInsn(shortOpcode, regA, literalB);
                    }
                    break;
                }
                case 0x1a: { // 21c format.
                    if (findLabelsOnly) {
                        InstructionFormat21C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21C.getRegisterA(fullOpcode);
                        int indexB = InstructionFormat21C.getIndexB(dexFile);
                        String string = dexFile.getStringItemFromStringIndex(indexB);
                        methodVisitor.visitStringInsn(shortOpcode, regA, string);
                    }
                    break;
                }
                case 0x1b: { // 31c format.
                    if (findLabelsOnly) {
                        InstructionFormat31C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat31C.getRegisterA(fullOpcode);
                        int indexB = InstructionFormat31C.getIndexB(dexFile);
                        String string = dexFile.getStringItemFromStringIndex(indexB);
                        methodVisitor.visitStringInsn(shortOpcode, regA, string);
                    }
                    break;
                }
                case 0x1c: { // 21c format.
                    if (findLabelsOnly) {
                        InstructionFormat21C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21C.getRegisterA(fullOpcode);
                        int indexB = InstructionFormat21C.getIndexB(dexFile);
                        String typeName = dexFile.getStringItemFromTypeIndex(indexB);
                        methodVisitor.visitTypeInsn(shortOpcode, regA, 0, 0, typeName);
                    }
                    break;
                }
                case 0x1f: { // 21c format.
                    if (findLabelsOnly) {
                        InstructionFormat21C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21C.getRegisterA(fullOpcode);
                        int indexB = InstructionFormat21C.getIndexB(dexFile);
                        String typeName = dexFile.getStringItemFromTypeIndex(indexB);
                        methodVisitor.visitTypeInsn(shortOpcode, 0, regA, 0, typeName);
                    }
                    break;
                }
                case 0x20: { // 22c format.
                    if (findLabelsOnly) {
                        InstructionFormat22C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat22C.getRegisterA(fullOpcode);
                        int regB = InstructionFormat22C.getRegisterB(fullOpcode);
                        int indexC = InstructionFormat22C.getIndexC(dexFile);
                        String typeName = dexFile.getStringItemFromTypeIndex(indexC);
                        methodVisitor.visitTypeInsn(shortOpcode, regA, regB, 0, typeName);
                    }
                    break;
                }
                case 0x21: { // 12x format.
                    if (findLabelsOnly) {
                        // InstructionFormat12X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat12X.getRegisterA(fullOpcode);
                        int regB = InstructionFormat12X.getRegisterB(fullOpcode);
                        methodVisitor.visitArrayLengthInsn(regA, regB);
                    }
                    break;
                }
                case 0x22: { // 21c format.
                    if (findLabelsOnly) {
                        InstructionFormat21C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21C.getRegisterA(fullOpcode);
                        int indexB = InstructionFormat21C.getIndexB(dexFile);
                        String typeName = dexFile.getStringItemFromTypeIndex(indexB);
                        methodVisitor.visitTypeInsn(shortOpcode, regA, 0, 0, typeName);
                    }
                    break;
                }
                case 0x23: { // 22c format.
                    if (findLabelsOnly) {
                        InstructionFormat22C.skip(dexFile);
                    } else {
                        int registerArray = InstructionFormat22C.getRegisterA(fullOpcode);
                        int regB = InstructionFormat22C.getRegisterB(fullOpcode);
                        int indexC = InstructionFormat22C.getIndexC(dexFile);
                        String typeName = dexFile.getStringItemFromTypeIndex(indexC);
                        methodVisitor.visitTypeInsn(shortOpcode, registerArray, 0, regB, typeName);

                        // We have to store the type linked to the register used by the array declaration,
                        // in case it is filled with a Fill Array Data instruction.
                        int type = getTypeFromTypeArray(typeName);
                        registerArrayType.put(registerArray, type);
                    }
                    break;
                }
                case 0x24: { // 35c format.
                    if (findLabelsOnly) {
                        InstructionFormat35C.skip(dexFile);
                    } else {
                        int index = InstructionFormat35C.getIndex(dexFile);
                        int[] registers = InstructionFormat35C.getRegisters(dexFile, fullOpcode);
                        visitMultiANewArrayInsn(methodVisitor, index, registers);
                    }
                    break;
                }
                case 0x25: { // 3rc format.
                    if (findLabelsOnly) {
                        InstructionFormat3RC.skip(dexFile);
                    } else {
                        int index = InstructionFormat3RC.getIndex(dexFile);
                        int[] registers = InstructionFormat3RC.getRegisters(dexFile, fullOpcode);
                        visitMultiANewArrayInsn(methodVisitor, index, registers);
                    }
                    break;
                }
                case 0x26: { // 31t format.
                    if (findLabelsOnly) {
                        InstructionFormat31T.skip(dexFile);
                    } else {
                        int regA = InstructionFormat31T.getRegisterA(fullOpcode);
                        int offset = InstructionFormat31T.getOffset(dexFile, fullOpcode);
                        visitFillArrayData(methodVisitor, regA, offset);
                    }
                    break;
                }
                case 0x28: { // 10t format.
                    int offset = InstructionFormat10T.getOffset(dexFile, fullOpcode);
                    visitJumpInsn(shortOpcode, offset, 0, 0, methodVisitor, findLabelsOnly);
                    break;
                }
                case 0x29: { // 20t format.
                    int offset = InstructionFormat20T.getOffset(dexFile, fullOpcode);
                    visitJumpInsn(shortOpcode, offset, 0, 0, methodVisitor, findLabelsOnly);
                    break;
                }
                case 0x2a: { // 30t format.
                    int offset = InstructionFormat30T.getOffset(dexFile, fullOpcode);
                    visitJumpInsn(shortOpcode, offset, 0, 0, methodVisitor, findLabelsOnly);
                    break;
                }
                case 0x2b: // 31t format.
                case 0x2c: {
                    parseAndVisitSwitchCase(methodVisitor, fullOpcode, findLabelsOnly);
                    break;
                }
                case 0x2d: // 23x format.
                case 0x2e:
                case 0x2f:
                case 0x30:
                case 0x31: {
                    if (findLabelsOnly) {
                        InstructionFormat23X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat23X.getRegisterA(fullOpcode);
                        int regBAndC = InstructionFormat23X.getEncodedRegisterBAndC(dexFile);
                        int regB = InstructionFormat23X.getRegisterBFromEncodedRegisterBAndC(regBAndC);
                        int regC = InstructionFormat23X.getRegisterCFromEncodedRegisterBAndC(regBAndC);
                        methodVisitor.visitOperationInsn(shortOpcode, regA, regB, regC, 0);
                    }
                    break;
                }
                case 0x32: // 22t format.
                case 0x33:
                case 0x34:
                case 0x35:
                case 0x36:
                case 0x37: {
                    int regA = InstructionFormat22T.getRegisterA(fullOpcode);
                    int regB = InstructionFormat22T.getRegisterB(fullOpcode);
                    int offset = InstructionFormat22T.getOffset(dexFile, fullOpcode);
                    visitJumpInsn(shortOpcode, offset, regA, regB, methodVisitor, findLabelsOnly);
                    break;
                }
                case 0x38: // 21t format.
                case 0x39:
                case 0x3a:
                case 0x3b:
                case 0x3c:
                case 0x3d: {
                    int regA = InstructionFormat21T.getRegisterA(fullOpcode);
                    int offset = InstructionFormat21T.getOffset(dexFile, fullOpcode);
                    visitJumpInsn(shortOpcode, offset, regA, 0, methodVisitor, findLabelsOnly);
                    break;
                }
                case 0x44: // 23x format (Array Operations).
                case 0x45:
                case 0x46:
                case 0x47:
                case 0x48:
                case 0x49:
                case 0x4a:
                case 0x4b:
                case 0x4c:
                case 0x4d:
                case 0x4e:
                case 0x4f:
                case 0x50:
                case 0x51: {
                    if (findLabelsOnly) {
                        InstructionFormat23X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat23X.getRegisterA(fullOpcode);
                        int regBAndC = InstructionFormat23X.getEncodedRegisterBAndC(dexFile);
                        int regB = InstructionFormat23X.getRegisterBFromEncodedRegisterBAndC(regBAndC);
                        int regC = InstructionFormat23X.getRegisterCFromEncodedRegisterBAndC(regBAndC);
                        methodVisitor.visitArrayOperationInsn(shortOpcode, regA, regB, regC);
                    }
                    break;
                }
                case 0x52: // 22c format.
                case 0x53:
                case 0x54:
                case 0x55:
                case 0x56:
                case 0x57:
                case 0x58:
                case 0x59:
                case 0x5a:
                case 0x5b:
                case 0x5c:
                case 0x5d:
                case 0x5e:
                case 0x5f: {
                    if (findLabelsOnly) {
                        InstructionFormat22C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat22C.getRegisterA(fullOpcode);
                        int regB = InstructionFormat22C.getRegisterB(fullOpcode);
                        int indexC = InstructionFormat22C.getIndexC(dexFile);
                        visitFieldInsn(methodVisitor, shortOpcode, regA, regB, indexC);
                    }
                    break;
                }
                case 0x60: // 21c format.
                case 0x61:
                case 0x62:
                case 0x63:
                case 0x64:
                case 0x65:
                case 0x66:
                case 0x67:
                case 0x68:
                case 0x69:
                case 0x6a:
                case 0x6b:
                case 0x6c:
                case 0x6d: {
                    if (findLabelsOnly) {
                        InstructionFormat21C.skip(dexFile);
                    } else {
                        int regA = InstructionFormat21C.getRegisterA(fullOpcode);
                        int indexC = InstructionFormat21C.getIndexB(dexFile);
                        visitFieldInsn(methodVisitor, shortOpcode, regA, 0, indexC);
                    }
                    break;
                }
                case 0x6e:
                case 0x6f:
                case 0x70:
                case 0x71:
                case 0x72: {
                    if (findLabelsOnly) {
                        InstructionFormat35C.skip(dexFile);
                    } else {
                        int index = InstructionFormat35C.getIndex(dexFile);
                        int[] registers = InstructionFormat35C.getRegisters(dexFile, fullOpcode);
                        visitMethodInstruction(methodVisitor, shortOpcode, index, registers);
                    }
                    break;
                }
                case 0x74:
                case 0x75:
                case 0x76:
                case 0x77:
                case 0x78: {
                    if (findLabelsOnly) {
                        InstructionFormat3RC.skip(dexFile);
                    } else {
                        int index = InstructionFormat3RC.getIndex(dexFile);
                        int[] registers = InstructionFormat3RC.getRegisters(dexFile, fullOpcode);
                        visitMethodInstruction(methodVisitor, shortOpcode, index, registers);
                    }
                    break;
                }
                case 0x7b:
                case 0x7c:
                case 0x7d:
                case 0x7e:
                case 0x7f:
                case 0x80:
                case 0x81:
                case 0x82:
                case 0x83:
                case 0x84:
                case 0x85:
                case 0x86:
                case 0x87:
                case 0x88:
                case 0x89:
                case 0x8a:
                case 0x8b:
                case 0x8c:
                case 0x8d:
                case 0x8e:
                case 0x8f: {
                    if (findLabelsOnly) {
                        // InstructionFormat12X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat12X.getRegisterA(fullOpcode);
                        int regB = InstructionFormat12X.getRegisterB(fullOpcode);
                        methodVisitor.visitOperationInsn(shortOpcode, regA, regB, 0, 0);
                    }
                    break;
                }
                case 0x90:
                case 0x91:
                case 0x92:
                case 0x93:
                case 0x94:
                case 0x95:
                case 0x96:
                case 0x97:
                case 0x98:
                case 0x99:
                case 0x9a:
                case 0x9b:
                case 0x9c:
                case 0x9d:
                case 0x9e:
                case 0x9f:
                case 0xa0:
                case 0xa1:
                case 0xa2:
                case 0xa3:
                case 0xa4:
                case 0xa5:
                case 0xa6:
                case 0xa7:
                case 0xa8:
                case 0xa9:
                case 0xaa:
                case 0xab:
                case 0xac:
                case 0xad:
                case 0xae:
                case 0xaf: {
                    if (findLabelsOnly) {
                        InstructionFormat23X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat23X.getRegisterA(fullOpcode);
                        int regBAndC = InstructionFormat23X.getEncodedRegisterBAndC(dexFile);
                        int regB = InstructionFormat23X.getRegisterBFromEncodedRegisterBAndC(regBAndC);
                        int regC = InstructionFormat23X.getRegisterCFromEncodedRegisterBAndC(regBAndC);
                        methodVisitor.visitOperationInsn(shortOpcode, regA, regB, regC, 0);
                    }
                    break;
                }
                case 0xb0:
                case 0xb1:
                case 0xb2:
                case 0xb3:
                case 0xb4:
                case 0xb5:
                case 0xb6:
                case 0xb7:
                case 0xb8:
                case 0xb9:
                case 0xba:
                case 0xbb:
                case 0xbc:
                case 0xbd:
                case 0xbe:
                case 0xbf:
                case 0xc0:
                case 0xc1:
                case 0xc2:
                case 0xc3:
                case 0xc4:
                case 0xc5:
                case 0xc6:
                case 0xc7:
                case 0xc8:
                case 0xc9:
                case 0xca:
                case 0xcb:
                case 0xcc:
                case 0xcd:
                case 0xce:
                case 0xcf: {
                    if (findLabelsOnly) {
                        // InstructionFormat12X.skip(dexFile);
                    } else {
                        int regA = InstructionFormat12X.getRegisterA(fullOpcode);
                        int regB = InstructionFormat12X.getRegisterB(fullOpcode);
                        methodVisitor.visitOperationInsn(shortOpcode, regA, regA, regB, 0);
                    }
                    break;
                }
                case 0xd0: // 22s format.
                case 0xd1:
                case 0xd2:
                case 0xd3:
                case 0xd4:
                case 0xd5:
                case 0xd6:
                case 0xd7: {
                    if (findLabelsOnly) {
                        InstructionFormat22S.skip(dexFile);
                    } else {
                        int regA = InstructionFormat22S.getRegisterA(fullOpcode);
                        int regB = InstructionFormat22S.getRegisterB(fullOpcode);
                        int literalC = InstructionFormat22S.getLiteralC(dexFile);
                        methodVisitor.visitOperationInsn(shortOpcode, regA, regB, 0, literalC);
                    }
                    break;
                }
                case 0xd8: // 22b format.
                case 0xd9:
                case 0xda:
                case 0xdb:
                case 0xdc:
                case 0xdd:
                case 0xde:
                case 0xdf:
                case 0xe0:
                case 0xe1:
                case 0xe2: {
                    if (findLabelsOnly) {
                        InstructionFormat22B.skip(dexFile);
                    } else {
                        int regA = InstructionFormat22B.getRegisterA(fullOpcode);
                        int regBAndLitC = InstructionFormat22B.getEncodedRegisterBAndLiteralC(dexFile);
                        int regB = InstructionFormat22B.getRegisterBFromEncodedRegisterBAndC(regBAndLitC);
                        int literalC = InstructionFormat22B.getLiteralCFromEncodedRegisterBAndLiteralC(regBAndLitC);
                        methodVisitor.visitOperationInsn(shortOpcode, regA, regB, 0, literalC);
                    }
                    break;
                }
                default:
                    // All other opcodes are considered unused.
                    if (!findLabelsOnly) {
                        methodVisitor.visitInsn(shortOpcode);
                    }
                    break;

                }
            }
        }
    }




    /**
     * Converts an array descriptor to a value (such as Opcodes.VALUE_INT). Only primitive types
     * can be converted, other types such as reference will return -1. This is like that because
     * only primitive can be used for the Fill Array Data instruction.
     * @param typeName the type of the array. Should be in the form "[X]", where X is a primitive
     * 	      descriptor.
     * @return the value of elements contained in the array (such as {@link Opcodes#VALUE_INT}), or -1.
     */
    private static int getTypeFromTypeArray(String typeName) {
        int result = -1;

        if ((typeName.length() == 2) && (typeName.startsWith("["))) {
            char t = typeName.charAt(1);
            switch (t) {
            case 'Z' :
                result = Opcodes.VALUE_BOOLEAN;
                break;
            case 'B' :
                result = Opcodes.VALUE_BYTE;
                break;
            case 'S' :
                result = Opcodes.VALUE_SHORT;
                break;
            case 'C' :
                result = Opcodes.VALUE_CHAR;
                break;
            case 'I' :
                result = Opcodes.VALUE_INT;
                break;
            case 'J' :
                result = Opcodes.VALUE_LONG;
                break;
            case 'F' :
                result = Opcodes.VALUE_FLOAT;
                break;
            case 'D' :
                result = Opcodes.VALUE_DOUBLE;
                break;
            }
        }

        return result;
    }




    /**
     * Parses the debug_info_item information of the given debug opcode. We stop the
     * reading when the debugAddress doesn't coincide with the bytecode pointer.
     * Updates the debugAddress and debugCurrentOpcodeOffset. However, the visit
     * isn't done here, but by the calling method.
     * The dex file reader position <i>is</i> saved.
     * @param methodVisitor the visitor to visit the debug information item.
     * @param currentBytecodeOffset Offset <i>from the method opcodes</i>
     * currently being parsed.
     * @param findLabelsOnly true if no visitor must be called, because we only want to set up
     * the Labels by parsing this structure.
     */
    protected void parseDebugInformationItem(MethodVisitor methodVisitor,
            int currentBytecodeOffset, boolean findLabelsOnly) {
        int saveReaderPosition = dexFile.getPos();
        dexFile.seek(debugCurrentOpcodeOffset);
        newDebugLineEmitted = false;

        int nextDebugAddress;

        boolean debugParsingContinue = true;
        while (debugParsingContinue) {
            int debugOpcode = dexFile.ubyte();
            switch (debugOpcode) {
            case Opcodes.DBG_END_SEQUENCE:
                debugParsingContinue = false;
                break;
            case Opcodes.DBG_ADVANCE_PC:
                debugAddress += dexFile.uleb128();
                break;
            case Opcodes.DBG_ADVANCE_LINE:
                debugLine += dexFile.sleb128();
                break;
            case Opcodes.DBG_START_LOCAL: {
                // Makes sure the variable is read at the right address.
                if ((debugAddress * 2) != currentBytecodeOffset) {
                    debugParsingContinue = false;
                } else {
                    parseDebugStartLocalVariable(methodVisitor, currentBytecodeOffset, false,
                                                 findLabelsOnly);
                }
                break;
            }
            case Opcodes.DBG_START_LOCAL_EXTENDED: {
                // Makes sure the variable is read at the right address.
                if ((debugAddress * 2) != currentBytecodeOffset) {
                    debugParsingContinue = false;
                } else {
                    parseDebugStartLocalVariable(methodVisitor, currentBytecodeOffset, true,
                                                 findLabelsOnly);
                }
                break;
            }
            case Opcodes.DBG_END_LOCAL: {
                // Makes sure the variable is read at the right address.
                if ((debugAddress * 2) != currentBytecodeOffset) {
                    debugParsingContinue = false;
                } else {
                    // Adds the END_LOCAL information to the local variable structure. It should
                    // exist !
                    int registerNumber = dexFile.uleb128();
                    if (findLabelsOnly) {
                        Label endLabel = createAndAddLabel(debugAddress * 2);
                        addEndLabelToLocalVariableLabels(registerNumber, endLabel);
                    }
                }
                break;
            }
            case Opcodes.DBG_RESTART_LOCAL: {
                // Makes sure the variable is read at the right address.
                if ((debugAddress * 2) != currentBytecodeOffset) {
                    debugParsingContinue = false;
                } else {
                    // Adds the RESTART_LOCAL information to the local variable structure. It
                    // should exist !
                    int registerNumber = dexFile.uleb128();
                    if (findLabelsOnly) {
                        Label restartLabel = createAndAddLabel(debugAddress * 2);
                        addRestartLabelToLocalVariableLabels(registerNumber, restartLabel);
                    }
                }
                break;
            }
            case Opcodes.DBG_SET_PROLOGUE_END:
                debugPrologueEnd = true;
                break;
            case Opcodes.DBG_SET_EPILOGUE_BEGIN:
                debugEpilogueBegin = true;
                break;
            case Opcodes.DBG_SET_FILE: {
                int nameIndex = dexFile.uleb128_p1();
                if (nameIndex != Opcodes.NO_INDEX_SIGNED) {
                    debugSourceFile = dexFile.getStringItemFromStringIndex(nameIndex);
                }
                break;
            }
            default:
                // Special Opcode (>=0xa).
                // We only emit a New Debug Line if the address it points to is the current one.
                // Else, we do NOT make the reader advance to the next opcode, so that we
                // check, next time, if the line number we were on is now become
                // the current one.

                int adjustedOpcode = debugOpcode - Opcodes.DBG_FIRST_SPECIAL;
                nextDebugAddress = debugAddress + adjustedOpcode / Opcodes.DBG_LINE_RANGE;
                // Note : the debugAddress must be multiplied by 2 when comparing,
                // because it is an offset of 16-bit units.
                if (currentBytecodeOffset != (nextDebugAddress * 2)) {
                    debugParsingContinue = false;
                } else {
                    debugEpilogueBegin = false;
                    debugPrologueEnd = false;

                    debugLine += Opcodes.DBG_LINE_BASE + (adjustedOpcode % Opcodes.DBG_LINE_RANGE);
                    // We store this new emitted debug line so that a Label found can be linked
                    // to this line.
                    newDebugLineEmitted = true;
                    debugEmittedLine = debugLine;
                    debugAddress = nextDebugAddress;

                    // By default, if we find a line number, we add a Label, because in
                    // ASM line numbers always refer to a Label.
                    createAndAddLabel(debugAddress * 2); // * 2 because offset of 16-bit units.
                }
                break;
            }

            // If the parsing stops, we don't move the reader after the debug instruction
            // that made it stops. We will parse the same instruction on the next iteration.
            if (debugParsingContinue) {
                debugCurrentOpcodeOffset = dexFile.getPos();
            }
        }

        dexFile.seek(saveReaderPosition);
    }

    /**
     * Parses a DBG_START_LOCAL or DBG_START_LOCAL_EXTENDED instruction, and visits
     * the local variables.
     * @param methodVisitor the visitor to visit the debug information item.
     * @param currentBytecodeOffset Offset <i>from the method opcodes</i> currently being parsed.
     * @param isLocalExtended true if the instruction concerns a LOCAL_EXTENDED variable.
     * @param findLabelsOnly true if no visitor must be called, because we only want to set up
     * 		  the Labels by parsing this structure.
     */
    protected void parseDebugStartLocalVariable(MethodVisitor methodVisitor,
            int currentBytecodeOffset, boolean isLocalExtended, boolean findLabelsOnly) {
        int registerNumber = dexFile.uleb128();
        int variableNameIndex = dexFile.uleb128_p1();
        int variableTypeIndex = dexFile.uleb128_p1();
        int variableSignatureIndex = 0;
        if (isLocalExtended) {
            variableSignatureIndex = dexFile.uleb128_p1();
        }
        String variableName = (variableNameIndex == Opcodes.NO_INDEX_SIGNED) ?
                              null : dexFile.getStringItemFromStringIndex(variableNameIndex);
        String variableType = (variableTypeIndex == Opcodes.NO_INDEX_SIGNED) ?
                              null : dexFile.getStringItemFromTypeIndex(variableTypeIndex);
        String variableSignature = isLocalExtended ?
                                   ((variableSignatureIndex == Opcodes.NO_INDEX_SIGNED) ?
                                    null : dexFile.getStringItemFromStringIndex(variableSignatureIndex)):
                                   null;

        Label startLabel = createAndAddLabel(currentBytecodeOffset);

        // Creates the localVariableLabels structure for this variable, if first pass.
        // We always create a new Local Variable and adds it to the list. Some variable can share the
        // same register, but of different type, so they must be managed separately.
        if (findLabelsOnly) {
            LocalVariable localVariable = new LocalVariable(registerNumber, variableName,
                    variableType, variableSignature, startLabel, null, null);
            addNewLocalVariable(registerNumber, localVariable);
        }
    }

    /**
     * Returns the LAST Local Variable used according to the given register number.
     * If it doesn't exist, it creates it.
     * @param registerNumber
     * @return a Local Variable, new or existing.
     */
    protected LocalVariable getLocalVariable(int registerNumber) {
        LocalVariable localVariable;
        if (localVariableLists.containsKey(registerNumber)) {
            List<LocalVariable> list = localVariableLists.get(registerNumber);
            localVariable = list.get(list.size() - 1);
        } else {
            List<LocalVariable> list = new ArrayList<LocalVariable>(1);
            localVariable = new LocalVariable(registerNumber);

            list.add(localVariable);
            localVariableLists.put(registerNumber, list);
        }

        return localVariable;
    }

    /**
     * Adds the Local Variable at the end of the list designed by the register number.
     * @param registerNumber the register number of the Local Variable.
     * @param localVariable the Local Variable to add.
     */
    protected void addNewLocalVariable(int registerNumber, LocalVariable localVariable) {
        if (localVariableLists.containsKey(registerNumber)) {
            List<LocalVariable> list = localVariableLists.get(registerNumber);
            list.add(localVariable);
        } else {
            List<LocalVariable> list = new ArrayList<LocalVariable>(1);
            list.add(localVariable);
            localVariableLists.put(registerNumber, list);
        }
    }

    /**
     * Parses the "tries" structure. It may contains several "try_item" items.
     * In order to get all the information needed, it will also parse the
     * encoded_catch_handler_list (only reaching the information needed).
     * The dex file reader position <i>is</i> modified.
     * @param tryItemOffset offset to the "tries" structure (which offset is given in
     * the code_item structure), from the beginning of the file.
     * @param triesSize number of "try" structure.
     */
    protected void parseTryItemsFormat(int tryItemOffset, int triesSize) {
        dexFile.seek(tryItemOffset);

        for (int tryItemIndex = 0; tryItemIndex < triesSize ; tryItemIndex++) {
            int startAddress = dexFile.uint() * 2; // * 2 because encoded in 16-bit units.
            int instructionCount = dexFile.ushort() * 2;
            int handlerOffset = dexFile.ushort();

            // Seek the handler information into the catch_handler_item located in the
            // catch_handler_list structure. The offset inside this structure is given
            // by the field handlerOffset.
            int saveReaderPositionInTryItem = dexFile.getPos();

            dexFile.seek(tryItemOffset + triesSize * TRY_ITEM_STRUCTURE_SIZE
                         + handlerOffset);

            int nbCatchTypesRead = dexFile.sleb128();
            boolean isCatchAll = (nbCatchTypesRead <= 0);
            int nbCatchTypes = Math.abs(nbCatchTypesRead);

            // Read the encoded_type_addr_pair list.
            // For each exception handled type, we create a TryCatch item that will
            // be visited on the second pass.
            for (int typeAddrPairIndex = 0; typeAddrPairIndex < nbCatchTypes; typeAddrPairIndex++) {
                int typeIndex = dexFile.uleb128();
                int addr = dexFile.uleb128() * 2; // * 2 because mesured in 16-bit units.

                // Creates the Labels.
                Label startLabel = createAndAddLabel(startAddress);
                Label endLabel = createAndAddLabel(startAddress + instructionCount);
                Label handlerLabel = createAndAddLabel(addr);

                TryCatch tcs = new TryCatch(startLabel, endLabel, handlerLabel,
                                            dexFile.getStringItemFromTypeIndex(typeIndex));

                // Adds this TryCatch structure to the map, so that we can call the
                // visitTryCatchBlock when visiting the bytecode itself by comparing
                // the current offset to the offsets of the tryCatchStructures, and
                // see if one (or more) fits.
                addTryCatchStructure(tcs, startAddress);
            }

            // Manage Catch All, if any.
            if (isCatchAll) {
                int catchAllAddress = dexFile.uleb128() * 2; // * 2 because mesured in 16-bit units.

                Label startLabel = createAndAddLabel(startAddress);
                Label endLabel = createAndAddLabel(startAddress + instructionCount);
                Label handlerLabel = createAndAddLabel(catchAllAddress);

                TryCatch tcs = new TryCatch(startLabel, endLabel, handlerLabel, null); // In ASM, type is Null for catch all.

                addTryCatchStructure(tcs, startAddress);
            }

            dexFile.seek(saveReaderPositionInTryItem);
        }
    }


    /**
     * Adds a new Label to the Label list, mapped according to its offset from the first
     * byte of the code of the method. Doesn't add it if it already exists.
     * @param offset offset from the first byte of the code of the method.
     */
    protected void addLabel(int offset) {
        if (!labels.containsKey(offset)) {
            Label label = new Label();
            label.setOffset(offset);
            labels.put(offset, label);
        }
    }

    /**
     * Convenient method to add an (already resolved) label to the Label list. Returns
     * a reference to the actually labels here. This is useful is a label already exists
     * here.
     * @param label label to add.
     * @return the label actually put. This is useful not to have two labels at the same
     * location.
     */
    protected Label addLabel(Label label) {
        int offset = label.getOffset();
        Label returnedLabel = label;
        if (labels.containsKey(offset)) {
            returnedLabel = labels.get(offset);
        } else {
            labels.put(offset, label);
        }
        return returnedLabel;
    }

    /**
     * Convenient method to add (already resolved) labels to the Label list.
     * @param labelList list of the Labels to add.
     */
    protected void addLabels(Label[] labelList) {
        for (Label label : labelList) {
            addLabel(label);
        }
    }

    /**
     * Convenient method to add a TryCatchStructure to the listTryCatchStructures. As it
     * contains lists of TryCatchStructure, we have to make sure that a list already
     * exists before adding the structure, else we have to create one.
     * @param tcs TryCatchStructure to add.
     * @param relativeOffset Offset to where the Try is in the bytecode, relative
     * to the first byte of the code of the method.
     */
    protected void addTryCatchStructure(TryCatch tcs, int relativeOffset) {
        ArrayList<TryCatch> list = listTryCatchStructures.get(relativeOffset);
        if (list != null) {
            list.add(tcs);
        } else {
            list = new ArrayList<TryCatch>(1);
            list.add(tcs);
            listTryCatchStructures.put(relativeOffset, list);
        }
    }

    /**
     * Convenient method to get the Label related to the offset (relative to the first
     * byte of the code of the method).
     * @param offset offset of the Label, relative to the first byte of the code of the method.
     * @return the Label related to the offset given, or Null if the Label isn't found.
     */
    protected Label getLabel(int offset) {
        return labels.get(offset);
    }

    /**
     * Creates and adds to the Labels list a new Label which offset (relative to the first
     * byte of the code of the method) is given, but if a Label already exists here,
     * returns this existing label.
     * @param offset offset of the Label, relative to the first byte of the code of
     * the method).
     * @return A new Label, or the Label existing at this offset.
     */
    protected Label createAndAddLabel(int offset) {
        Label returnedLabel;
        if (labels.containsKey(offset)) {
            returnedLabel = labels.get(offset);
        } else {
            returnedLabel = new Label();
            returnedLabel.setOffset(offset);
            labels.put(offset, returnedLabel);
        }

        return returnedLabel;
    }


    /**
     * Visits a Var Instruction.
     * @param opcodeByte 8-bit opcode of the instruction.
     * @param methodVisitor visitor to call.
     * @param destinationRegister the destination register.
     * @param var the operand of the instruction to be visited. This operand is
     *        either a value or a source Register.
     */
    protected void visitVarInsn(int opcodeByte, MethodVisitor methodVisitor,
                                int destinationRegister, int var) {
        methodVisitor.visitVarInsn(opcodeByte, destinationRegister, var);
    }

    /**
     * Visits a Var Instruction.
     * @param opcodeByte 8-bit opcode of the instruction.
     * @param methodVisitor visitor to call.
     * @param destinationRegister the destination register.
     * @param var the operand of the instruction to be visited. This operand is
     *        either a value or a source Register.
     */
    protected void visitVarInsn(int opcodeByte, MethodVisitor methodVisitor,
                                int destinationRegister, long var) {
        methodVisitor.visitVarInsn(opcodeByte, destinationRegister, var);
    }





    /**
     * Method used by the methods that decodes the invoke-kind, in order to get the
     * invoked method information and call the visitor.
     * The dex file reader position <i>is</i> saved.
     * @param methodVisitor visitor to call.
     * @param opcodeByte 8-bit opcode of the instruction.
     * @param methodIndex method index of the invoked method.
     * @param registers list of the registers encoded to pass to the invoked method.
     */
    protected void visitMethodInstruction(MethodVisitor methodVisitor, int opcodeByte,
                                          int methodIndex, int[] registers) {
        int savePosReader = dexFile.getPos();

        dexFile.seek(dexFile.getOffsetMethodIdItem(methodIndex));
        int classIndex = dexFile.ushort(); // Get name from class_idx.
        int protoIndex = dexFile.ushort();
        int nameIndex = dexFile.uint();
        String methodOwner = dexFile.getStringItemFromTypeIndex(classIndex);
        String methodDescriptor = dexFile.getDescriptorFromPrototypeIndex(protoIndex);
        String methodName = dexFile.getStringItemFromStringIndex(nameIndex);

        methodVisitor.visitMethodInsn(opcodeByte, methodOwner, methodName, methodDescriptor, registers);
        dexFile.seek(savePosReader);
    }

    /**
     * Parses and visits a Jump Instruction (format 10X, 20X, 30X).
     * @param opcode the 8-bit opcode of the Instruction.
     * @param offset offset relative to the beginning of the Dex file, to go to.
     * @param firstRegister first register to test, if any.
     * @param secondRegister second register to test, if any.
     * @param methodVisitor MethodVisitor to visit the Jump.
     * @param findLabelsOnly indicates if the parsing is only about getting the Label offsets.
     * 		  In this case, no method from the visitor must be called.
     */
    protected void visitJumpInsn(int opcode, int offset, int firstRegister,
                                 int secondRegister, MethodVisitor methodVisitor, boolean findLabelsOnly) {
        int relativeOffset = offset - instructionsStartOffset;
        if (findLabelsOnly) {
            // Generate a Label, which offset is related to the current opcode.
            addLabel(relativeOffset);
        } else {
            methodVisitor.visitJumpInsn(opcode, getLabel(relativeOffset), firstRegister, secondRegister);
        }
    }

    /**
     * Visits a given Field Instruction, and parses it.
     * The dex file reader position <i>is</i> saved.
     * @param methodVisitor MethodVisitor to visit the instruction.
     * @param opcode Opcode of the instruction
     * @param registerA First register
     * @param registerB Second register
     * @param index index of the field definition
     */
    protected void visitFieldInsn(MethodVisitor methodVisitor, int opcode, int registerA, int registerB, int index) {

        int savePosReader = dexFile.getPos();

        dexFile.seek(dexFile.getOffsetFieldIdItem(index));
        int classIndex = dexFile.ushort(); // Get name from class_idx.
        int typeIndex = dexFile.ushort();
        int nameIndex = dexFile.uint();
        String fieldOwner = dexFile.getStringItemFromTypeIndex(classIndex);
        String fieldDescriptor = dexFile.getStringItemFromTypeIndex(typeIndex);
        String fieldName = dexFile.getStringItemFromStringIndex(nameIndex);

        methodVisitor.visitFieldInsn(opcode, fieldOwner, fieldName, fieldDescriptor, registerA, registerB);
        dexFile.seek(savePosReader);
    }

    /**
     * Visits a MultiANewArrayInsn Instruction.
     * The dex file reader position <i>is</i> saved.
     * @param methodVisitor MethodVisitor to visit the instruction.
     * @param index the index of the descriptor of the Array.
     * @param registers the registers containing the values of the Array.
     */
    protected void visitMultiANewArrayInsn(MethodVisitor methodVisitor,	int index, int[] registers) {
        String desc = dexFile.getStringItemFromTypeIndex(index);
        methodVisitor.visitMultiANewArrayInsn(desc, registers);
    }

    /**
     * Visits a FillArrayData instruction. Just like switch/case instruction, we directly
     * get to the array data (encoded later) in order to parse them for this visitor.
     * @param methodVisitor MethodVisitor to visit the instruction.
     * @param register register holding the array.
     * @param offset offset of the data structure of the Array.
     */
    protected void visitFillArrayData(MethodVisitor methodVisitor, int register, int offset) {

        int saveReaderPosition = dexFile.getPos();

        // Gets the type of the element held by the register. It should have been read before.
        int type = -1;
        if (registerArrayType.containsKey(register)) {
            type = registerArrayType.get(register);
        }

        // Gets to the Array Data and parse it.
        dexFile.seek(offset);

        dexFile.skipShort(); // Skips the 0x300 opcode.
        if (type == -1 ) {
            switch (dexFile.sshort()) {
            case 1:
                type = Opcodes.VALUE_BYTE;
                break;
            case 2:
                type = Opcodes.VALUE_SHORT;
                break;
            case 4:
                type = Opcodes.VALUE_INT;
                break;
            case 8:
                type = Opcodes.VALUE_LONG;
                break;
            default:
            }
        } else {
            dexFile.skipShort(); // Skips Element Width.
        }
        int elementSize = dexFile.uint();

        // Fills the array according to its type.
        Object[] elements;
        switch (type) {
        case Opcodes.VALUE_BYTE: {
            elements = new Byte[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Byte.valueOf(dexFile.sbyte());
            }
            break;
        }
        case Opcodes.VALUE_SHORT: {
            elements = new Short[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Short.valueOf(dexFile.sshort());
            }
            break;
        }
        case Opcodes.VALUE_CHAR: {
            elements = new Character[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Character.valueOf((char)dexFile.sshort());
            }
            break;
        }
        case Opcodes.VALUE_INT: {
            elements = new Integer[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Integer.valueOf(dexFile.sint());
            }
            break;
        }
        case Opcodes.VALUE_FLOAT: {
            elements = new Float[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Float.valueOf(Float.intBitsToFloat((int)dexFile.sizedLong(4 - 1)));
            }
            break;
        }
        case Opcodes.VALUE_DOUBLE: {
            elements = new Double[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Double.valueOf(Double.longBitsToDouble(dexFile.sizedLong(8 - 1)));
            }
            break;
        }
        case Opcodes.VALUE_LONG: {
            elements = new Long[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Long.valueOf(dexFile.sizedLong(8 - 1));
            }
            break;
        }
        case Opcodes.VALUE_BOOLEAN: {
            elements = new Boolean[elementSize];
            for (int i = 0; i < elementSize; i++) {
                elements[i] = Boolean.valueOf(dexFile.sbyte() != 0);
            }
            break;
        }
        default :
            throw new IllegalArgumentException("This type (" + type + ") can't be encoded in an Array of primitive.");
        }

        methodVisitor.visitFillArrayDataInsn(register, elements);

        dexFile.seek(saveReaderPosition);
    }


    /**
     * Parses and visits a Switch Case instruction, whether it be a Packed or Sparse switch.
     * Its nature is found according to the given opcode.
     * The dex file reader must point after the 16-bit opcode. On return, points after
     * the full instruction.
     * @param methodVisitor MethodVisitor to visit the switch.
     * @param fullOpcode the 16-bit opcode.
     * @param findLabelsOnly indicates if the parsing is only about getting the Label offsets.
     * 		  In this case, no method from the visitor must be called.
     */
    protected void parseAndVisitSwitchCase(MethodVisitor methodVisitor,
                                           int fullOpcode,	boolean findLabelsOnly) {

        boolean isPackedSwitch = ((fullOpcode & 0xff) == 0x2b);

        // Get the offset of the switch (just after the 16-bit opcode).
        int switchOffset = dexFile.getPos() - 2;
        int regA = InstructionFormat31T.getRegisterA(fullOpcode);
        int structureOffset = InstructionFormat31T.getOffset(dexFile, fullOpcode);

        if (findLabelsOnly) {
            int relativeStructureOffset = structureOffset - instructionsStartOffset;
            // Generates a Label to the structure, which offset is related
            // to the current opcode.
            addLabel(relativeStructureOffset);

            // Gets the Labels of the structure, and adds the structure for visiting it
            // later.
            int relativeSwitchOffset = switchOffset - instructionsStartOffset;
            ISwitchCase cs = (isPackedSwitch ?
                              parsePackedSwitchFormat(relativeSwitchOffset, structureOffset) :
                              parseSparseSwitchFormat(relativeSwitchOffset, structureOffset));

            Label[] switchLabels = cs.getSwitchLabels();
            addLabels(switchLabels);
            addLabel(cs.getDefaultLabel());
            if (isPackedSwitch) {
                packedSwitchStructures.put(switchOffset, (PackedSwitch)cs);
            } else {
                sparseSwitchStructures.put(switchOffset, (SparseSwitch)cs);
            }
        } else {
            // Visits the switch structure.
            // The structure SHOULD be here, as it has been put in the first pass.
            if (isPackedSwitch) {
                PackedSwitch pss = packedSwitchStructures.get(switchOffset);
                methodVisitor.visitTableSwitchInsn(regA, pss.getValueMin(), pss.getValueMax(),
                                                   pss.getDefaultLabel(), pss.getSwitchLabels());
            } else {
                SparseSwitch sss = sparseSwitchStructures.get(switchOffset);
                methodVisitor.visitLookupSwitchInsn(regA, sss.getDefaultLabel(),
                                                    sss.getKeys(), sss.getSwitchLabels());
            }
        }
    }


    /**
     * Reads a packed-switch structure, from the switch offset and the packed switch
     * offset and returns a structure containing all the information read.
     * The dex file reader position <i>is</i> saved.
     * @param relativeSwitchOffset offset of the first byte of the switch opcode, from
     * 		  the beginning of the code of the method.
     * @param packedSwitchOffset offset of the packed switch structure, from the
     * 		  beginning of the Dex file.
     * @return a PackedSwitchStructure containing all the information of the packed
     * 		  switch structure.
     */
    protected PackedSwitch parsePackedSwitchFormat(int relativeSwitchOffset, int packedSwitchOffset) {
        int savedReaderPosition = dexFile.getPos();

        PackedSwitch pss = new PackedSwitch();
        dexFile.seek(packedSwitchOffset);

        dexFile.skipShort(); // Skips identifying pseudo-code (0x100).
        int nbEntries = dexFile.ushort();
        int valueMin = dexFile.sint();

        // Reads the Label list.
        Label[] switchLabels = new Label[nbEntries];
        for (int entryIndex = 0; entryIndex < nbEntries; entryIndex++) {
            // * 2 because branch offsets are word based.
            int labelOffset = dexFile.sint() * 2 + relativeSwitchOffset;
            Label label = createAndAddLabel(labelOffset);
            switchLabels[entryIndex] = label;
        }

        pss.setSwitchLabels(switchLabels);
        // Default label is what follows the switch opcode.
        pss.setDefaultLabel(createAndAddLabel(relativeSwitchOffset + 6));
        pss.setValueMin(valueMin);
        pss.setValueMax(valueMin + nbEntries - 1);
        dexFile.seek(savedReaderPosition);
        return pss;
    }

    /**
     * Reads a sparse-switch structure, from the switch offset and the packed switch
     * offset and returns a structure containing all the information read.
     * The dex file reader position <i>is</i> saved.
     * @param relativeSwitchOffset offset of the first byte of the switch opcode, from
     * the beginning of the code of the method.
     * @param sparseSwitchOffset offset of the packed switch structure, from the
     * beginning of the Dex file.
     * @return a SparseSwitchStructure containing all the information of the sparse
     * switch structure.
     */
    protected SparseSwitch parseSparseSwitchFormat(int relativeSwitchOffset, int sparseSwitchOffset) {
        int savedReaderPosition = dexFile.getPos();

        SparseSwitch sss = new SparseSwitch();
        dexFile.seek(sparseSwitchOffset);

        dexFile.skipShort(); // Skips identifying pseudo-code (0x200).
        int nbEntries = dexFile.ushort();
        // Reads the Keys array.
        int[] keys = new int[nbEntries];
        for (int entryIndex = 0; entryIndex < nbEntries; entryIndex++) {
            keys[entryIndex] = dexFile.sint();
        }

        // Reads the Targets array.
        Label[] switchLabels = new Label[nbEntries];
        for (int entryIndex = 0; entryIndex < nbEntries; entryIndex++) {
            // * 2 because branch offsets are word based.
            int labelOffset = dexFile.sint() * 2 + relativeSwitchOffset;
            Label label = createAndAddLabel(labelOffset);
            switchLabels[entryIndex] = label;
        }

        sss.setSwitchLabels(switchLabels);
        sss.setKeys(keys);

        // Default label is what follows the switch opcode.
        sss.setDefaultLabel(createAndAddLabel(relativeSwitchOffset + 6));

        dexFile.seek(savedReaderPosition);
        return sss;
    }

    /**
     * Adds an End Label to a Local Variable labels structure.
     * @param registerNumber register number.
     * @param endLabel the Label to add.
     */
    protected void addEndLabelToLocalVariableLabels(int registerNumber, Label endLabel) {
        LocalVariable localVariable = getLocalVariable(registerNumber);
        localVariable.addEnd(endLabel);
    }

    /**
     * Adds an Restart Label to a Local Variable labels structure.
     * @param registerNumber register number.
     * @param restartLabel the Label to add.
     */
    protected void addRestartLabelToLocalVariableLabels(int registerNumber, Label restartLabel) {
        LocalVariable localVariable = getLocalVariable(registerNumber);
        localVariable.addRestart(restartLabel);
    }

}
