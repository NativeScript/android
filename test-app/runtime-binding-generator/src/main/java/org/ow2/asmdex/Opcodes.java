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
 * Defines the Dalvik opcodes, access flags and array type codes.
 *
 * @author Julien Névo
 */
public interface Opcodes {

    /**
     * API level
     */
    public static final int ASM4 = 0x40000;

    /**
     * Spurious accessor used by the writer when it an instruction refers to a
     * class, Method or a Field not parsed yet.
     */
    public static final int ACC_UNKNOWN = 0x40000;

    /**
     * Public
     */
    public final static int ACC_PUBLIC = 0x1;
    /**
     * Private
     */
    public final static int ACC_PRIVATE = 0x2;
    /**
     * Protected
     */
    public final static int ACC_PROTECTED = 0x4;
    /**
     * Static
     */
    public final static int ACC_STATIC = 0x8;
    /**
     * Final
     */
    public final static int ACC_FINAL = 0x10;
    /**
     * Volatile
     */
    public final static int ACC_VOLATILE = 0x40;
    /**
     * Bridge
     */
    public final static int ACC_BRIDGE = 0x40;
    /**
     * Transient
     */
    public final static int ACC_TRANSIENT = 0x80;
    /**
     * Varargs
     */
    public final static int ACC_VARARGS = 0x80;
    /**
     * Native
     */
    public final static int ACC_NATIVE = 0x100;
    /**
     * Interface
     */
    public final static int ACC_INTERFACE = 0x200;
    /**
     * abstract
     */
    public final static int ACC_ABSTRACT = 0x400;
    /**
     * Strict
     */
    public final static int ACC_STRICT = 0x800;
    /**
     * Synthetic
     */
    public final static int ACC_SYNTHETIC = 0x1000;
    /**
     * Annotation
     */
    public final static int ACC_ANNOTATION = 0x2000;
    /**
     * Enum
     */
    public final static int ACC_ENUM = 0x4000;
    /**
     * Constructor
     */
    public final static int ACC_CONSTRUCTOR = 0x10000;
    /**
     * Synchronized
     */
    public final static int ACC_DECLARED_SYNCHRONIZED = 0x20000;

    /**
     * Annotation visibility build
     */
    public final static int VISIBILITY_BUILD = 0x00;
    /**
     * Annotation visibility runtime
     */
    public final static int VISIBILITY_RUNTIME = 0x01;
    /**
     * Annotation visibility system
     */
    public final static int VISIBILITY_SYSTEM = 0x02;

    /**
     *
     */
    public final static int NO_INDEX_SIGNED = -1;

    // Value formats in encoded_value encoding.
    /**
     * Type id for values : byte
     */
    public final static int VALUE_BYTE = 0x00;
    /**
     * Type id for values : short
     */
    public final static int VALUE_SHORT = 0x02;
    /**
     * Type id for values : char
     */
    public final static int VALUE_CHAR = 0x03;
    /**
     * Type id for values : int
     */
    public final static int VALUE_INT = 0x04;
    /**
     * Type id for values : long
     */
    public final static int VALUE_LONG = 0x06;
    /**
     * Type id for values : float
     */
    public final static int VALUE_FLOAT = 0x10;
    /**
     * Type id for values : double
     */
    public final static int VALUE_DOUBLE = 0x11;
    /**
     * Type id for values : string
     */
    public final static int VALUE_STRING = 0x17;
    /**
     * type
     */
    public final static int VALUE_TYPE = 0x18;
    /**
     * Type id for values : field
     */
    public final static int VALUE_FIELD = 0x19;
    /**
     * Type id for values : method
     */
    public final static int VALUE_METHOD = 0x1a;
    /**
     * Type id for values : enum
     */
    public final static int VALUE_ENUM = 0x1b;
    /**
     * Type id for values : array
     */
    public final static int VALUE_ARRAY = 0x1c;
    /**
     * Type id for values : annotation
     */
    public final static int VALUE_ANNOTATION = 0x1d;
    /**
     *Type id for values : null
     */
    public final static int VALUE_NULL = 0x1e;
    /**
     * Type id for values : boolean
     */
    public final static int VALUE_BOOLEAN = 0x1f;

    // Debug_info_item constants.
    /**
     * Debug instruction : end sequence
     */
    public final static int DBG_END_SEQUENCE = 0x00;
    /**
     * Debug instruction : advance pc
     */
    public final static int DBG_ADVANCE_PC = 0x01;
    /**
     * Debug instruction : advance linie
     */
    public final static int DBG_ADVANCE_LINE = 0x02;
    /**
     * debug instruction : start local
     */
    public final static int DBG_START_LOCAL = 0x03;
    /**
     * Debug instruction : start local (extended)
     */
    public final static int DBG_START_LOCAL_EXTENDED = 0x04;
    /**
     * Debug instruction : end local
     */
    public final static int DBG_END_LOCAL = 0x05;
    /**
     * debug instruction : restart local
     */
    public final static int DBG_RESTART_LOCAL = 0x06;
    /**
     * debug instruction : set prologue end
     */
    public final static int DBG_SET_PROLOGUE_END= 0x07;
    /**
     * debug instruction : set epilogue begin
     */
    public final static int DBG_SET_EPILOGUE_BEGIN = 0x08;
    /**
     * debug instruction : set file
     */
    public final static int DBG_SET_FILE = 0x09;

    /**
     * Debug offset first special
     */
    public final static int DBG_FIRST_SPECIAL = 0x0a;
    /**
     * Debug offset line base
     */
    public final static int DBG_LINE_BASE = -4;
    /**
     * Debug offset line range
     */
    public final static int DBG_LINE_RANGE = 15;

    /**
     * Header
     */
    public static final int TYPE_HEADER_ITEM = 0x0000;
    /**
     * String id
     */
    public static final int TYPE_STRING_ID_ITEM = 0x0001;
    /**
     * Type id
     */
    public static final int TYPE_TYPE_ID_ITEM = 0x0002;
    /**
     * Prototype Id
     */
    public static final int TYPE_PROTO_ID_ITEM = 0x0003;
    /**
     * Field id
     */
    public static final int TYPE_FIELD_ID_ITEM = 0x0004;
    /**
     * Method id
     */
    public static final int TYPE_METHOD_ID_ITEM = 0x0005;
    /**
     * Class definition
     */
    public static final int TYPE_CLASS_DEF_ITEM = 0x0006;
    /**
     * Map list
     */
    public static final int TYPE_MAP_LIST = 0x1000;
    /**
     * Type list
     */
    public static final int TYPE_TYPE_LIST = 0x1001;
    /**
     * List of ref to annotation sets
     */
    public static final int TYPE_ANNOTATION_SET_REF_LIST = 0x1002;
    /**
     * Annotation set
     */
    public static final int TYPE_ANNOTATION_SET_ITEM = 0x1003;
    /**
     * Class data
     */
    public static final int TYPE_CLASS_DATA_ITEM = 0x2000;
    /**
     * Code
     */
    public static final int TYPE_CODE_ITEM = 0x2001;
    /**
     * String data
     */
    public static final int TYPE_STRING_DATA_ITEM = 0x2002;
    /**
     * Debug info
     */
    public static final int TYPE_DEBUG_INFO_ITEM = 0x2003;
    /**
     * Annotation
     */
    public static final int TYPE_ANNOTATION_ITEM = 0x2004;
    /**
     * Encoded array
     */
    public static final int TYPE_ENCODED_ARRAY_ITEM = 0x2005;
    /**
     * Annotations directory
     */
    public static final int TYPE_ANNOTATIONS_DIRECTORY_ITEM = 0x2006;

    // Opcodes for the Dalvik Instructions.
    /**
     * NOP
     */
    public static final int INSN_NOP = 0x0;						// visitInsn
    /**
     * MOVE
     */
    public static final int INSN_MOVE = 0x1;					// visitVarInsn
    /**
     * Move from 16
     */
    public static final int INSN_MOVE_FROM16 = 0x2;
    /**
     * Move 16
     */
    public static final int INSN_MOVE_16 = 0x3;
    /**
     * Move wide
     */
    public static final int INSN_MOVE_WIDE = 0x4;
    /**
     * Move wide from 16
     */
    public static final int INSN_MOVE_WIDE_FROM16 = 0x5;
    /**
     * Move wide 16
     */
    public static final int INSN_MOVE_WIDE_16 = 0x6;
    /**
     * Move object
     */
    public static final int INSN_MOVE_OBJECT = 0x7;
    /**
     * Move object from 16
     */
    public static final int INSN_MOVE_OBJECT_FROM16 = 0x8;
    /**
     * Moe object 16
     */
    public static final int INSN_MOVE_OBJECT_16 = 0x9;
    /**
     * Move result
     */
    public static final int INSN_MOVE_RESULT = 0xa;				// visitIntInsn
    /**
     * Move result wide
     */
    public static final int INSN_MOVE_RESULT_WIDE = 0xb;
    /**
     * Move result object
     */
    public static final int INSN_MOVE_RESULT_OBJECT = 0xc;
    /**
     * Move exception
     */
    public static final int INSN_MOVE_EXCEPTION = 0xd;
    /**
     * Return void
     */
    public static final int INSN_RETURN_VOID = 0xe;				// visitInsn
    /**
     * Return
     */
    public static final int INSN_RETURN = 0xf;					// visitIntInsn
    /**
     * Return wide
     */
    public static final int INSN_RETURN_WIDE = 0x10;
    /**
     * Return object
     */
    public static final int INSN_RETURN_OBJECT = 0x11;
    /**
     * Constant 4
     */
    public static final int INSN_CONST_4 = 0x12;				// visitVarInsn
    /**
     * Constant 16
     */
    public static final int INSN_CONST_16 = 0x13;
    /**
     * Constant
     */
    public static final int INSN_CONST = 0x14;
    /**
     * Constant High 16
     */
    public static final int INSN_CONST_HIGH16 = 0x15;
    /**
     * Constant Wide 16
     */
    public static final int INSN_CONST_WIDE_16 = 0x16;
    /**
     * Constant Wide 32
     */
    public static final int INSN_CONST_WIDE_32 = 0x17;
    /**
     * Constant Wide
     */
    public static final int INSN_CONST_WIDE = 0x18;
    /**
     * Constant Wide High 16
     */
    public static final int INSN_CONST_WIDE_HIGH16 = 0x19;
    /**
     * Cosntant String
     */
    public static final int INSN_CONST_STRING = 0x1a;			// visitStringInsn
    /**
     *
     */
    public static final int INSN_CONST_STRING_JUMBO = 0x1b;
    /**
     * Constant Class
     */
    public static final int INSN_CONST_CLASS = 0x1c;			// visitTypeInsn
    /**
     * Monitor Enter
     */
    public static final int INSN_MONITOR_ENTER = 0x1d;			// visitIntInsn
    /**
     * Monitor Exit
     */
    public static final int INSN_MONITOR_EXIT = 0x1e;
    /**
     * Check Cast
     */
    public static final int INSN_CHECK_CAST = 0x1f;				// visitTypeInsn
    /**
     * Instance of
     */
    public static final int INSN_INSTANCE_OF = 0x20;
    /**
     * Array length
     */
    public static final int INSN_ARRAY_LENGTH = 0x21;			// visitArrayLengthInsn
    /**
     * New Instance
     */
    public static final int INSN_NEW_INSTANCE = 0x22;			// visitTypeInsn
    /**
     * New array
     */
    public static final int INSN_NEW_ARRAY = 0x23;
    /**
     * Filled new array
     */
    public static final int INSN_FILLED_NEW_ARRAY = 0x24;		// visitMultiANewArrayInsn
    /**
     * New array range
     */
    public static final int INSN_FILLED_NEW_ARRAY_RANGE = 0x25;
    /**
     * Fill array data
     */
    public static final int INSN_FILL_ARRAY_DATA = 0x26;		// visitFillArrayDataInsn
    /**
     * Throw
     */
    public static final int INSN_THROW = 0x27;					// visitIntInsn
    /**
     * Goto
     */
    public static final int INSN_GOTO = 0x28;					// visitJumpInsn
    /**
     * Goto 16
     */
    public static final int INSN_GOTO_16 = 0x29;
    /**
     * Goto 32
     */
    public static final int INSN_GOTO_32 = 0x2a;
    /**
     * Packed Switch
     */
    public static final int INSN_PACKED_SWITCH_INSN = 0x2b;		// visitTableSwitchInsn
    /**
     * Sparse switch
     */
    public static final int INSN_SPARSE_SWITCH_INSN = 0x2c;		// visitLookupSwitchInsn
    /**
     * Compare Lower Float
     */
    public static final int INSN_CMPL_FLOAT = 0x2d;				// visitOperationInsn
    /**
     * Compare Greater Float
     */
    public static final int INSN_CMPG_FLOAT = 0x2e;
    /**
     * Compare Lower Double
     */
    public static final int INSN_CMPL_DOUBLE = 0x2f;
    /**
     * Compare Greater Double
     */
    public static final int INSN_CMPG_DOUBLE = 0x30;
    /**
     * Compare Long
     */
    public static final int INSN_CMP_LONG = 0x31;
    /**
     * If eq
     */
    public static final int INSN_IF_EQ = 0x32;					// visitJumpInsn
    /**
     * If not eq
     */
    public static final int INSN_IF_NE = 0x33;
    /**
     * If less than
     */
    public static final int INSN_IF_LT = 0x34;
    /**
     * If greater or equal
     */
    public static final int INSN_IF_GE = 0x35;
    /**
     * If greater
     */
    public static final int INSN_IF_GT = 0x36;
    /**
     * If lower or equal
     */
    public static final int INSN_IF_LE = 0x37;
    /**
     * If eq zero
     */
    public static final int INSN_IF_EQZ = 0x38;
    /**
     * If not zero
     */
    public static final int INSN_IF_NEZ = 0x39;
    /**
     * If less than zero
     */
    public static final int INSN_IF_LTZ = 0x3a;
    /**
     * If greater or equal to zero
     */
    public static final int INSN_IF_GEZ = 0x3b;
    /**
     * If greater than zero
     */
    public static final int INSN_IF_GTZ = 0x3c;
    /**
     * If less or equal to zero
     */
    public static final int INSN_IF_LEZ = 0x3d;

    /**
     * Array get
     */
    public static final int INSN_AGET = 0x44;					// visitArrayOperationInsn
    /**
     * Array get wide
     */
    public static final int INSN_AGET_WIDE = 0x45;
    /**
     * Array get object
     */
    public static final int INSN_AGET_OBJECT = 0x46;
    /**
     * Array get boolean
     */
    public static final int INSN_AGET_BOOLEAN = 0x47;
    /**
     * Array get byte
     */
    public static final int INSN_AGET_BYTE = 0x48;
    /**
     * Array get char
     */
    public static final int INSN_AGET_CHAR = 0x49;
    /**
     * Array get short
     */
    public static final int INSN_AGET_SHORT = 0x4a;
    /**
     * Array put
     */
    public static final int INSN_APUT = 0x4b;
    /**
     * Array put wide
     */
    public static final int INSN_APUT_WIDE = 0x4c;
    /**
     * Array put object
     */
    public static final int INSN_APUT_OBJECT = 0x4d;
    /**
     * Array put boolean
     */
    public static final int INSN_APUT_BOOLEAN = 0x4e;
    /**
     * Array put byte
     */
    public static final int INSN_APUT_BYTE = 0x4f;
    /**
     * Array put char
     */
    public static final int INSN_APUT_CHAR = 0x50;
    /**
     * Arrray put short
     */
    public static final int INSN_APUT_SHORT = 0x51;

    /**
     * Instance field get
     */
    public static final int INSN_IGET = 0x52;					// visitFieldInsn
    /**
     * Instance field get wide
     */
    public static final int INSN_IGET_WIDE = 0x53;
    /**
     * Instance field get object
     */
    public static final int INSN_IGET_OBJECT = 0x54;
    /**
     * Instance field get boolean
     */
    public static final int INSN_IGET_BOOLEAN = 0x55;
    /**
     * Instance field get byte
     */
    public static final int INSN_IGET_BYTE = 0x56;
    /**
     * Instance field get char
     */
    public static final int INSN_IGET_CHAR = 0x57;
    /**
     * Instance field get short
     */
    public static final int INSN_IGET_SHORT = 0x58;
    /**
     * Instance field put
     */
    public static final int INSN_IPUT = 0x59;
    /**
     * Instance field put wide
     */
    public static final int INSN_IPUT_WIDE = 0x5a;
    /**
     * Instance field put object
     */
    public static final int INSN_IPUT_OBJECT = 0x5b;
    /**
     * Instance field put boolean
     */
    public static final int INSN_IPUT_BOOLEAN = 0x5c;
    /**
     * Instance field put byte
     */
    public static final int INSN_IPUT_BYTE = 0x5d;
    /**
     * Instance field put char
     */
    public static final int INSN_IPUT_CHAR = 0x5e;
    /**
     * Instance field put short
     */
    public static final int INSN_IPUT_SHORT = 0x5f;

    /**
     * Static get
     */
    public static final int INSN_SGET = 0x60;
    /**
     * Static get wide
     */
    public static final int INSN_SGET_WIDE = 0x61;
    /**
     * Static get object
     */
    public static final int INSN_SGET_OBJECT = 0x62;
    /**
     * Static get boolean
     */
    public static final int INSN_SGET_BOOLEAN = 0x63;
    /**
     * Static get byte
     */
    public static final int INSN_SGET_BYTE = 0x64;
    /**
     * Static get char
     */
    public static final int INSN_SGET_CHAR = 0x65;
    /**
     * Static get short
     */
    public static final int INSN_SGET_SHORT = 0x66;
    /**
     * Static put
     */
    public static final int INSN_SPUT = 0x67;
    /**
     * Static put wide
     */
    public static final int INSN_SPUT_WIDE = 0x68;
    /**
     * Static put object
     */
    public static final int INSN_SPUT_OBJECT = 0x69;
    /**
     * Static put boolean
     */
    public static final int INSN_SPUT_BOOLEAN = 0x6a;
    /**
     * Static put byte
     */
    public static final int INSN_SPUT_BYTE = 0x6b;
    /**
     * Static put char
     */
    public static final int INSN_SPUT_CHAR = 0x6c;
    /**
     * Static put short
     */
    public static final int INSN_SPUT_SHORT = 0x6d;

    /**
     * Invoke virtual
     */
    public static final int INSN_INVOKE_VIRTUAL = 0x6e;			// visitMethodInsn
    /**
     * Invoke super
     */
    public static final int INSN_INVOKE_SUPER = 0x6f;
    /**
     * Invoke direct
     */
    public static final int INSN_INVOKE_DIRECT = 0x70;
    /**
     * Invoke static
     */
    public static final int INSN_INVOKE_STATIC = 0x71;
    /**
     * Invoke interface
     */
    public static final int INSN_INVOKE_INTERFACE = 0x72;

    /**
     * Invoke virtual range
     */
    public static final int INSN_INVOKE_VIRTUAL_RANGE = 0x74;
    /**
     * Invoke super range
     */
    public static final int INSN_INVOKE_SUPER_RANGE = 0x75;
    /**
     * Invoke direct range
     */
    public static final int INSN_INVOKE_DIRECT_RANGE = 0x76;
    /**
     * Invoke static range
     */
    public static final int INSN_INVOKE_STATIC_RANGE = 0x77;
    /**
     * Invoke interface range
     */
    public static final int INSN_INVOKE_INTERFACE_RANGE = 0x78;

    /**
     * Invoke neg range
     */
    public static final int INSN_NEG_INT = 0x7b;				// visitOperationInsn
    /**
     * Not integer
     */
    public static final int INSN_NOT_INT = 0x7c;
    /**
     * Neg long
     */
    public static final int INSN_NEG_LONG = 0x7d;
    /**
     * Not long
     */
    public static final int INSN_NOT_LONG = 0x7e;
    /**
     * Neg float
     */
    public static final int INSN_NEG_FLOAT = 0x7f;
    /**
     * Neg double
     */
    public static final int INSN_NEG_DOUBLE = 0x80;
    /**
     * Int to long
     */
    public static final int INSN_INT_TO_LONG = 0x81;
    /**
     * Int to float
     */
    public static final int INSN_INT_TO_FLOAT = 0x82;
    /**
     * Int to double
     */
    public static final int INSN_INT_TO_DOUBLE = 0x83;
    /**
     * Long to int
     */
    public static final int INSN_LONG_TO_INT = 0x84;
    /**
     * Long to float
     */
    public static final int INSN_LONG_TO_FLOAT = 0x85;
    /**
     * Long to double
     */
    public static final int INSN_LONG_TO_DOUBLE = 0x86;
    /**
     * Float to int
     */
    public static final int INSN_FLOAT_TO_INT = 0x87;
    /**
     * Float to long
     */
    public static final int INSN_FLOAT_TO_LONG = 0x88;
    /**
     * Float to double
     */
    public static final int INSN_FLOAT_TO_DOUBLE = 0x89;
    /**
     * Double to int
     */
    public static final int INSN_DOUBLE_TO_INT = 0x8a;
    /**
     * Double to long
     */
    public static final int INSN_DOUBLE_TO_LONG = 0x8b;
    /**
     * Double to float
     */
    public static final int INSN_DOUBLE_TO_FLOAT = 0x8c;
    /**
     * Int to byte
     */
    public static final int INSN_INT_TO_BYTE = 0x8d;
    /**
     * Int to char
     */
    public static final int INSN_INT_TO_CHAR = 0x8e;
    /**
     * Int to short
     */
    public static final int INSN_INT_TO_SHORT = 0x8f;

    /**
     * Add int
     */
    public static final int INSN_ADD_INT = 0x90;
    /**
     * Substract int
     */
    public static final int INSN_SUB_INT = 0x91;
    /**
     * Multiply int
     */
    public static final int INSN_MUL_INT = 0x92;
    /**
     * Divide int
     */
    public static final int INSN_DIV_INT = 0x93;
    /**
     * Remainder int
     */
    public static final int INSN_REM_INT = 0x94;
    /**
     * And int
     */
    public static final int INSN_AND_INT = 0x95;
    /**
     * Or int
     */
    public static final int INSN_OR_INT = 0x96;
    /**
     * Xor int
     */
    public static final int INSN_XOR_INT = 0x97;
    /**
     * Shift left int
     */
    public static final int INSN_SHL_INT = 0x98;
    /**
     * Sifht right int
     */
    public static final int INSN_SHR_INT = 0x99;
    /**
     * Unsigned Shift right int
     */
    public static final int INSN_USHR_INT = 0x9a;
    /**
     * Add long
     */
    public static final int INSN_ADD_LONG = 0x9b;
    /**
     * Subtract long
     */
    public static final int INSN_SUB_LONG = 0x9c;
    /**
     * Multiply long
     */
    public static final int INSN_MUL_LONG = 0x9d;
    /**
     * Divide long
     */
    public static final int INSN_DIV_LONG = 0x9e;
    /**
     * Remainder long
     */
    public static final int INSN_REM_LONG = 0x9f;
    /**
     * And long
     */
    public static final int INSN_AND_LONG = 0xa0;
    /**
     * Or long
     */
    public static final int INSN_OR_LONG = 0xa1;
    /**
     * Xor long
     */
    public static final int INSN_XOR_LONG = 0xa2;
    /**
     * Shift left long
     */
    public static final int INSN_SHL_LONG = 0xa3;
    /**
     * Shift right long
     */
    public static final int INSN_SHR_LONG = 0xa4;
    /**
     * Unsigned shift right long
     */
    public static final int INSN_USHR_LONG = 0xa5;

    /**
     * Add float
     */
    public static final int INSN_ADD_FLOAT = 0xa6;
    /**
     * Substract float
     */
    public static final int INSN_SUB_FLOAT = 0xa7;
    /**
     * Multiply float
     */
    public static final int INSN_MUL_FLOAT = 0xa8;
    /**
     * Divide float
     */
    public static final int INSN_DIV_FLOAT = 0xa9;
    /**
     * Remainder float
     */
    public static final int INSN_REM_FLOAT = 0xaa;
    /**
     * Add double
     */
    public static final int INSN_ADD_DOUBLE = 0xab;
    /**
     * Substract double
     */
    public static final int INSN_SUB_DOUBLE = 0xac;
    /**
     * Multiply double
     */
    public static final int INSN_MUL_DOUBLE = 0xad;
    /**
     * Divide double
     */
    public static final int INSN_DIV_DOUBLE = 0xae;
    /**
     * Remainder double
     */
    public static final int INSN_REM_DOUBLE = 0xaf;

    /**
     * Add integer two addresses
     */
    public static final int INSN_ADD_INT_2ADDR = 0xb0;
    /**
     * Sub integer two addresses
     */
    public static final int INSN_SUB_INT_2ADDR = 0xb1;
    /**
     * Multiply integer two addresses
     */
    public static final int INSN_MUL_INT_2ADDR = 0xb2;
    /**
     * Divide integer two addresses
     */
    public static final int INSN_DIV_INT_2ADDR = 0xb3;
    /**
     * Remainder integer two addresses
     */
    public static final int INSN_REM_INT_2ADDR = 0xb4;
    /**
     * And integer two addresses
     */
    public static final int INSN_AND_INT_2ADDR = 0xb5;
    /**
     * Or integer two addresses
     */
    public static final int INSN_OR_INT_2ADDR = 0xb6;
    /**
     * Xor integer two addresses
     */
    public static final int INSN_XOR_INT_2ADDR = 0xb7;
    /**
     * Shift left integer two addresses
     */
    public static final int INSN_SHL_INT_2ADDR = 0xb8;
    /**
     * Shift right integer two addresses
     */
    public static final int INSN_SHR_INT_2ADDR = 0xb9;
    /**
     * Unsigned shift right integer two addresses
     */
    public static final int INSN_USHR_INT_2ADDR = 0xba;

    /**
     * Add long two addresses
     */
    public static final int INSN_ADD_LONG_2ADDR = 0xbb;
    /**
     * substract long two addresses
     */
    public static final int INSN_SUB_LONG_2ADDR = 0xbc;
    /**
     * multiply long two addresses
     */
    public static final int INSN_MUL_LONG_2ADDR = 0xbd;
    /**
     * divide long two addresses
     */
    public static final int INSN_DIV_LONG_2ADDR = 0xbe;
    /**
     * Remainder long two addresses
     */
    public static final int INSN_REM_LONG_2ADDR = 0xbf;
    /**
     * And long two addresses
     */
    public static final int INSN_AND_LONG_2ADDR = 0xc0;
    /**
     * Or long two addresses
     */
    public static final int INSN_OR_LONG_2ADDR = 0xc1;
    /**
     * Exclusive or long two addresses
     */
    public static final int INSN_XOR_LONG_2ADDR = 0xc2;
    /**
     * Shift left long two addresses
     */
    public static final int INSN_SHL_LONG_2ADDR = 0xc3;
    /**
     * Shift right long two addresses
     */
    public static final int INSN_SHR_LONG_2ADDR = 0xc4;
    /**
     * Unsigned shift right long two addresses
     */
    public static final int INSN_USHR_LONG_2ADDR = 0xc5;

    /**
     * Add float two addresses
     */
    public static final int INSN_ADD_FLOAT_2ADDR = 0xc6;
    /**
     * Substract float two addresses
     */
    public static final int INSN_SUB_FLOAT_2ADDR = 0xc7;
    /**
     * Multiply float two addresses
     */
    public static final int INSN_MUL_FLOAT_2ADDR = 0xc8;
    /**
     * Divide float two addresses
     */
    public static final int INSN_DIV_FLOAT_2ADDR = 0xc9;
    /**
     * Remainder float two addresses
     */
    public static final int INSN_REM_FLOAT_2ADDR = 0xca;
    /**
     * Add double two addresses
     */
    public static final int INSN_ADD_DOUBLE_2ADDR = 0xcb;
    /**
     * Substract double two addresses
     */
    public static final int INSN_SUB_DOUBLE_2ADDR = 0xcc;
    /**
     * Multiply double two addresses
     */
    public static final int INSN_MUL_DOUBLE_2ADDR = 0xcd;
    /**
     * Divide double two addresses
     */
    public static final int INSN_DIV_DOUBLE_2ADDR = 0xce;
    /**
     * Remainder double two addresses
     */
    public static final int INSN_REM_DOUBLE_2ADDR = 0xcf;

    /**
     * Add integer literal 16
     */
    public static final int INSN_ADD_INT_LIT16 = 0xd0;
    /**
     * Reverse substract integer literal 16
     */
    public static final int INSN_RSUB_INT_LIT16 = 0xd1;
    /**
     * Multiply integer literal 16
     */
    public static final int INSN_MUL_INT_LIT16 = 0xd2;
    /**
     * Divide integer literal 16
     */
    public static final int INSN_DIV_INT_LIT16 = 0xd3;
    /**
     * Remainder integer literal 16
     */
    public static final int INSN_REM_INT_LIT16 = 0xd4;
    /**
     * And integer literal 16
     */
    public static final int INSN_AND_INT_LIT16 = 0xd5;
    /**
     * Or integer literal 16
     */
    public static final int INSN_OR_INT_LIT16 = 0xd6;
    /**
     * Exclusive Or integer literal 16
     */
    public static final int INSN_XOR_INT_LIT16 = 0xd7;

    /**
     * Add integer literal 8
     */
    public static final int INSN_ADD_INT_LIT8 = 0xd8;
    /**
     * Reverve substract integer literal 8
     */
    public static final int INSN_RSUB_INT_LIT8 = 0xd9;
    /**
     * Multiply integer literal 8
     */
    public static final int INSN_MUL_INT_LIT8 = 0xda;
    /**
     * Divide integer literal 8
     */
    public static final int INSN_DIV_INT_LIT8 = 0xdb;
    /**
     * Remainder integer literal 8
     */
    public static final int INSN_REM_INT_LIT8 = 0xdc;
    /**
     * And integer literal 8
     */
    public static final int INSN_AND_INT_LIT8 = 0xdd;
    /**
     * Or integer literal 8
     */
    public static final int INSN_OR_INT_LIT8 = 0xde;
    /**
     * Exclusive or integer literal 8
     */
    public static final int INSN_XOR_INT_LIT8 = 0xdf;
    /**
     * Shift left integer literal 8
     */
    public static final int INSN_SHL_INT_LIT8 = 0xe0;
    /**
     * Shift right integer literal 8
     */
    public static final int INSN_SHR_INT_LIT8 = 0xe1;
    /**
     * Unsigned shift right integer literal 8
     */
    public static final int INSN_USHR_INT_LIT8 = 0xe2;

}
