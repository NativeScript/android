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

package org.ow2.asmdex.structureCommon;

import java.util.ArrayList;

import org.ow2.asmdex.instruction.Instruction;

/**
 * A label represents a position in the bytecode of a method. Labels are used
 * for jump, goto, and switch instructions, and for try catch blocks. A label
 * designates the <i>instruction</i> that is just after.
 *
 * Two Labels are equal if their position is the same.
 *
 * Note however that there can be other elements between a label and the instruction it
 * designates (such as other labels, line numbers, etc.).
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class Label {

    /**
     * Indicates if the position of this label is known. Only this flag from ASM is currently implemented.
     */
    static final int RESOLVED = 2;

    private static int counter = 1;

    /**
     * Field used to associate user information to a label. Warning: this field
     * is used by the ASM tree package. In order to use it with the ASM tree
     * package you must override the {@link
     * org.ow2.asmdex.tree.MethodNode#getLabelNode} method.
     */
    public Object info;

    /**
     * Flags that indicate the status of this label.
     *
     * @see #RESOLVED
     */
    int status;

    /**
     * The line number corresponding to this label, if known.
     */
    int line = 0;

    /**
     * The position (offset) of this label in the code in bytes, if known.
     * This offset is computed from the start of the bytecode of the method.
     */
    int position = 0;

    /**
     * List of instructions which refer to this Label. Only jumps/test making reference
     * to this label are added here. These instructions have their offset in the method encoded.
     */
    private ArrayList<Instruction> referringInstructions = new ArrayList<Instruction>();

    final private int id;
    // ------------------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------------------

    /**
     * Constructs a new label.
     */
    public Label() {
        id = counter++ & Integer.MAX_VALUE;
    }


    // ------------------------------------------------------------------------
    // Getters and setters.
    // ------------------------------------------------------------------------

    /**
     * Returns the offset corresponding to this label. This offset is computed
     * from the start of the method's bytecode. <i>This method is normally not needed
     * by class generators or adapters.</i>
     *
     * @return the offset corresponding to this label.
     * @throws IllegalStateException if this label is not resolved yet.
     */
    public int getOffset() {
        if ((status & RESOLVED) == 0) {
            throw new IllegalStateException("Label offset position has not been resolved yet");
        }
        return position;
    }

    /**
     * Sets the offset of this Label, and flags the Label as Resolved.
     * @param offset the offset of this Label in bytes, from the beginning of the bytecode of its method.
     */
    public void setOffset(int offset) {
        position = offset;
        status |= RESOLVED;
    }

    /**
     * Indicates if the Label is resolved or not.
     * @return true if the Label is resolved.
     */
    public boolean isResolved() {
        return ((status & RESOLVED) != 0);
    }

    /**
     * Returns the Line number associated to this Label.
     * @return the Line number associated to this Label.
     */
    public int getLine() {
        return line;
    }

    /**
     * Sets the Line number associated to this Label.
     * @param line the Line number to associate to this Label.
     */
    public void setLine(int line) {
        this.line = line;
    }

    /**
     * Returns the list of instructions which REFER to this Label. Only jumps/tests
     * making reference to this label are added here.
     * @return the list of instructions which refer to this Label
     */
    public ArrayList<Instruction> getReferringInstructions() {
        return referringInstructions;
    }


    // ------------------------------------------------------------------------
    // Public methods
    // ------------------------------------------------------------------------

    /**
     * Adds an Instruction that refers to this Label. The Instruction must be a jump/test.
     * @param instruction the Instruction that refers to this Label.
     */
    public void addReferringInstruction(Instruction instruction) {
        referringInstructions.add(instruction);
    }


    // ------------------------------------------------------------------------
    // Overridden Object methods
    // ------------------------------------------------------------------------

    @Override
    public int hashCode() {
        return position;
    }

    @Override
    public boolean equals(Object obj) {
        boolean result = false;
        if (obj instanceof Label) {
            Label label = (Label)obj;
            // Two Labels can be equals only if they are both resolved, and of course their offset is the same.
            result = ((status & RESOLVED) > 0) && ((label.status & RESOLVED) > 0)
                     && (position == label.position);
        }
        return result;
    };

    /**
     * Returns a string representation of this label. The Offset is written only if known.
     * @return a string representation of this label. The Offset is written only if known.
     */
    @Override
    public String toString() {
        // Bug 316426 - better to use an explicit counter
        StringBuilder result = new StringBuilder('L' + id);
        if ((status & RESOLVED) > 0) {
            result.append(" (offset 0x" + Integer.toHexString(position) + ", line " + line + ")");
        }
        return result.toString();
    }


}
