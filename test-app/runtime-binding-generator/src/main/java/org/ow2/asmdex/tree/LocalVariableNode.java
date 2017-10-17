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

package org.ow2.asmdex.tree;

import java.util.ArrayList;
import java.util.List;

import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.structureCommon.Label;

/**
 * A node that represents a local variable declaration.
 *
 * @author Eric Bruneton
 * @author Julien Névo (adaptation to AsmDex).
 */
public class LocalVariableNode {

    /**
     * The name of a local variable.
     */
    public String name;

    /**
     * The type descriptor of this local variable.
     */
    public String desc;

    /**
     * The signature of this local variable. May be <tt>null</tt>.
     */
    public String signature;

    /**
     * The first instruction corresponding to the scope of this local variable
     * (inclusive).
     */
    public LabelNode start;

    /**
     * The last instructions corresponding to the scope of this local variable
     * (exclusive). May be <tt>null</tt>.
     */
    public List<LabelNode> ends;

    /**
     * The instructions corresponding to the restarts of this local variable
     * (inclusive). May be <tt>null</tt>.
     */
    public List<LabelNode> restarts;

    /**
     * The local variable's index.
     */
    public int index;

    /**
     * Constructs a new {@link LocalVariableNode}.
     *
     * @param name the name of a local variable.
     * @param desc the type descriptor of this local variable.
     * @param signature the signature of this local variable. May be
     *        <tt>null</tt>.
     * @param start the first instruction corresponding to the scope of this
     *        local variable (inclusive). May be <tt>null</tt>.
     * @param end the last instruction corresponding to the scope of this local
     *        variable (exclusive). May be <tt>null</tt>.
     * @param index the local variable's index.
     */
    public LocalVariableNode( final String name, final String desc, final String signature,
                              final LabelNode start, final LabelNode end, final int index) {
        this.name = name;
        this.desc = desc;
        this.signature = signature;
        this.start = start;
        if (end != null) {
            ends = new ArrayList<LabelNode>(1);
            ends.add(end);
        }
        this.index = index;
    }

    /**
     * Constructs a new {@link LocalVariableNode}.
     *
     * @param name the name of a local variable.
     * @param desc the type descriptor of this local variable.
     * @param signature the signature of this local variable. May be
     *        <tt>null</tt>.
     * @param start the first instruction corresponding to the scope of this
     *        local variable (inclusive). May be <tt>null</tt>.
     * @param ends the last instructions corresponding to the scope of this local
     *        variable (exclusive). May be <tt>null</tt>.
     * @param restarts the first instructions corresponding to the restarts of this local
     *        variable (exclusive). May be <tt>null</tt>.
     * @param index the local variable's index.
     */
    public LocalVariableNode( final String name, final String desc, final String signature,
                              final LabelNode start, final List<LabelNode> ends, final List<LabelNode> restarts, final int index) {
        this.name = name;
        this.desc = desc;
        this.signature = signature;
        this.start = start;
        this.ends = ends;
        this.restarts = restarts;
        this.index = index;
    }

    /**
     * Makes the given visitor visit this local variable declaration.
     *
     * @param mv a method visitor.
     */
    public void accept(final MethodVisitor mv) {
        mv.visitLocalVariable(name, desc, signature, start == null ? null : start.getLabel(), getLabels(ends), getLabels(restarts), index);
    }

    /**
     * Returns a list of Labels from a list of LabelNodes, or Null if the list is Null.
     * @param labelNodes the list of Label Nodes.
     * @return a list of Labels from a list of LabelNodes, or Null if the list is Null.
     */
    private List<Label> getLabels(final List<LabelNode> labelNodes) {
        List<Label> labels = null;
        if (labelNodes != null) {
            labels = new ArrayList<Label>(labelNodes.size());
            for (LabelNode labelNode : labelNodes) {
                labels.add(labelNode.getLabel());
            }
        }
        return labels;
    }

}
