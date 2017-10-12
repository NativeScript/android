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
import java.util.Arrays;
import java.util.List;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.Opcodes;
import org.ow2.asmdex.structureCommon.Label;
import org.ow2.asmdex.structureWriter.Prototype;

/**
 * A node that represents a method.
 *
 * @author Eric Bruneton
 * @author Julien Névo (adaptation to AsmDex)
 */
public class MethodNode extends MethodVisitor {

    /**
     * The method's access flags (see {@link Opcodes}). This field also
     * indicates if the method is synthetic.
     */
    public int access;

    /**
     * The method's name.
     */
    public String name;

    /**
     * The method's descriptor.
     */
    public String desc;

    /**
     * The method's signature. May be <tt>null</tt>.
     */
    public String[] signature;

    /**
     * The method's parameters. May be <tt>null</tt>.
     */
    public String[] parameters;

    /**
     * The internal names of the method's exception classes. May be <tt>null</tt>.
     */
    public List<String> exceptions;

    /**
     * The default value of this annotation interface method. This field must be
     * a {@link Byte}, {@link Boolean}, {@link Character}, {@link Short},
     * {@link Integer}, {@link Long}, {@link Float}, {@link Double},
     * {@link String} , or an two elements String array (for
     * enumeration values), a {@link AnnotationNode}, or a {@link List} of
     * values of one of the preceding types. May be <tt>null</tt>.
     */
    public Object annotationDefault;

    /**
     * The runtime visible parameter annotations of this method. May be <tt>null</tt>.
     */
    public List<List<AnnotationNode>> visibleParameterAnnotations;

    /**
     * The runtime invisible parameter annotations of this method. May be <tt>null</tt>.
     */
    public List<List<AnnotationNode>> invisibleParameterAnnotations;

    /**
     * The runtime visible annotations of this class, field or method.
     */
    public List<AnnotationNode> visibleAnnotations;

    /**
     * The runtime invisible annotations of this class, field or method.
     */
    public List<AnnotationNode> invisibleAnnotations;

    /**
     * The instructions of this method. This list is a list of
     * {@link AbstractInsnNode} objects.
     *
     */
    public InsnList instructions = new InsnList();

    /**
     * The try catch blocks of this method.
     */
    public List<TryCatchBlockNode> tryCatchBlocks;

    /**
     * The maximum stack size of this method.
     */
    public int maxStack;

    /**
     * The local variables of this method. May be <tt>null</tt>
     */
    public List<LocalVariableNode> localVariables;

    /**
     * Constructs an uninitialized {@link MethodNode}.
     */
    public MethodNode(int api) {
        super(api);
    }

    /**
     * Constructs a new {@link MethodNode}.
     *
     * @param api API level
     * @param access the method's access flags (see {@link Opcodes}). This
     *        parameter also indicates if the method is synthetic.
     * @param name the method's name.
     * @param desc the method's descriptor .
     * @param signature the method's signature. May be <tt>null</tt>.
     * @param exceptions the internal names of the method's exception classes.
     *        May be <tt>null</tt>.
     */
    public MethodNode(
        final int api,
        final int access,
        final String name,
        final String desc,
        final String[] signature,
        final String[] exceptions) {
        this(api);
        this.access = access;
        this.name = name;
        this.desc = desc;
        this.signature = signature;
        if (exceptions != null) {
            this.exceptions = new ArrayList<String>(exceptions.length);
            this.exceptions.addAll(Arrays.asList(exceptions));
        }
        boolean isAbstract = (access & Opcodes.ACC_ABSTRACT) != 0;
        if (!isAbstract) {
            this.localVariables = new ArrayList<LocalVariableNode>(5);
        }
        this.tryCatchBlocks = new ArrayList<TryCatchBlockNode>();
    }

    /**
     * Constructs a new {@link MethodNode}.
     *
     * @param access
     * @param name
     * @param desc
     * @param signature
     * @param exceptions
     */
    public MethodNode(
        final int access,
        final String name,
        final String desc,
        final String[] signature,
        final String[] exceptions) {
        this(Opcodes.ASM4, access, name, desc, signature, exceptions);
    }
    // ------------------------------------------------------
    // Visitor Methods.
    // ------------------------------------------------------

    @Override
    public AnnotationVisitor visitAnnotationDefault() {
        AnnotationNode an = new AnnotationNode(new ArrayList<Object>(0) {
            private static final long serialVersionUID = -8912565660163841010L;
            @Override
            public boolean add(final Object o) {
                annotationDefault = o;
                return super.add(o);
            }
        });

        // In Dalvik, it is important to store the name of the method, so that it can be given
        // to the DefaultAnnotation itself.
        an.desc = name;

        return an;
    }

    @Override
    public void visitArrayLengthInsn(int destinationRegister,
                                     int arrayReferenceBearing) {
        instructions.add(new ArrayLengthInsnNode(destinationRegister, arrayReferenceBearing));

    }

    @Override
    public void visitArrayOperationInsn(int opcode, int valueRegister,
                                        int arrayRegister, int indexRegister) {
        instructions.add(new ArrayOperationInsnNode(opcode, valueRegister, arrayRegister, indexRegister));
    }

    @Override
    public void visitCode() {
    }

    @Override
    public void visitFieldInsn(int opcode, String owner, String name,
                               String desc, int valueRegister, int objectRegister) {
        instructions.add(new FieldInsnNode(opcode, owner, name, desc, valueRegister, objectRegister));
    }

    @Override
    public void visitFillArrayDataInsn(int arrayReference, Object[] arrayData) {
        instructions.add(new FillArrayDataInsnNode(arrayReference, arrayData));
    }

    @Override
    public void visitFrame(int type, int nLocal, Object[] local, int nStack,
                           Object[] stack) {
        // Ignored by AsmDex.
    }

    @Override
    public void visitInsn(int opcode) {
        instructions.add(new InsnNode(opcode));
    }

    @Override
    public void visitIntInsn(int opcode, int register) {
        instructions.add(new IntInsnNode(opcode, register));
    }

    @Override
    public void visitJumpInsn(int opcode, Label label, int registerA,
                              int registerB) {
        instructions.add(new JumpInsnNode(opcode, getLabelNode(label), registerA, registerB));

    }

    @Override
    public void visitLabel(Label label) {
        instructions.add(getLabelNode(label));
    }

    @Override
    public void visitLineNumber(int line, Label start) {
        instructions.add(new LineNumberNode(line, getLabelNode(start)));
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, Label end, int index) {
        localVariables.add(new LocalVariableNode(name, desc, signature, getLabelNode(start),
                           getLabelNode(end), index));
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, List<Label> ends, List<Label> restarts, int index) {
        localVariables.add(new LocalVariableNode(name, desc, signature, getLabelNode(start),
                           getLabelNodes(ends), getLabelNodes(restarts), index));
    }

    @Override
    public void visitLookupSwitchInsn(int register, Label dflt, int[] keys,
                                      Label[] labels) {
        instructions.add(new LookupSwitchInsnNode(register, getLabelNode(dflt), keys, getLabelNodes(labels)));
    }

    @Override
    public void visitMaxs(int maxStack, int maxLocals) {
        // maxLocals is ignored by AsmDex.
        this.maxStack = maxStack;
    }

    @Override
    public void visitMethodInsn(int opcode, String owner, String name,
                                String desc, int[] arguments) {
        instructions.add(new MethodInsnNode(opcode, owner, name, desc, arguments));
    }

    @Override
    public void visitMultiANewArrayInsn(String desc, int[] registers) {
        instructions.add(new MultiANewArrayInsnNode(desc, registers));
    }

    @Override
    public void visitOperationInsn(int opcode, int destinationRegister,
                                   int firstSourceRegister, int secondSourceRegister, int value) {
        instructions.add(new OperationInsnNode(opcode, destinationRegister, firstSourceRegister,
                                               secondSourceRegister, value));
    }

    @Override
    public AnnotationVisitor visitParameterAnnotation(int parameter,
            String desc, boolean visible) {
        AnnotationNode an = new AnnotationNode(desc);
        if (visible) {
            if (visibleParameterAnnotations == null) {
                int params = Prototype.getNbParametersFromTypeDescriptor(this.desc);
                visibleParameterAnnotations = new ArrayList<List<AnnotationNode>>(params);
                for (int i = 0; i < params; i++) {
                    visibleParameterAnnotations.add(null);
                }
            }
            if (visibleParameterAnnotations.get(parameter) == null) {
                visibleParameterAnnotations.set(parameter, new ArrayList<AnnotationNode>(1));
            }
            visibleParameterAnnotations.get(parameter).add(an);
        } else {
            if (invisibleParameterAnnotations == null) {
                int params = Prototype.getNbParametersFromTypeDescriptor(this.desc);
                invisibleParameterAnnotations = new ArrayList<List<AnnotationNode>>(params);
                for (int i = 0; i < params; i++) {
                    invisibleParameterAnnotations.add(null);
                }
            }
            if (invisibleParameterAnnotations.get(parameter) == null) {
                invisibleParameterAnnotations.set(parameter, new ArrayList<AnnotationNode>(1));
            }
            invisibleParameterAnnotations.get(parameter).add(an);
        }
        return an;
    }

    @Override
    public void visitParameters(String[] parameters) {
        this.parameters = parameters;
    }

    @Override
    public void visitStringInsn(int opcode, int destinationRegister,
                                String string) {
        instructions.add(new StringInsnNode(opcode, destinationRegister, string));
    }

    @Override
    public void visitTableSwitchInsn(int register, int min, int max,
                                     Label dflt, Label[] labels) {
        instructions.add(new TableSwitchInsnNode(register, min, max, getLabelNode(dflt),
                         getLabelNodes(labels)));
    }

    @Override
    public void visitTryCatchBlock(Label start, Label end, Label handler,
                                   String type) {
        tryCatchBlocks.add(new TryCatchBlockNode(getLabelNode(start), getLabelNode(end),
                           getLabelNode(handler), type));
    }

    @Override
    public void visitTypeInsn(int opcode, int destinationRegister,
                              int referenceBearingRegister, int sizeRegister, String type) {
        instructions.add(new TypeInsnNode(opcode, destinationRegister, referenceBearingRegister,
                                          sizeRegister, type));
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, int var) {
        instructions.add(new VarInsnNode(opcode, destinationRegister, var));
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, long var) {
        instructions.add(new VarInsnLongNode(opcode, destinationRegister, var));
    }


    // ------------------------------------------------------
    // Private and Protected Methods.
    // ------------------------------------------------------

    /**
     * Returns the LabelNode corresponding to the given Label. Creates a new
     * LabelNode if necessary. The default implementation of this method uses
     * the {@link Label#info} field to store associations between labels and
     * label nodes.
     *
     * @param l a Label.
     * @return the LabelNode corresponding to l.
     */
    protected LabelNode getLabelNode(final Label l) {
        if (l == null) {
            return null;
        }

        if (!(l.info instanceof LabelNode)) {
            l.info = (Object) new LabelNode(l); // To avoid false warning
        }
        return (LabelNode) l.info;
    }

    private LabelNode[] getLabelNodes(final Label[] l) {
        if (l == null) {
            return null;
        }

        LabelNode[] nodes = new LabelNode[l.length];
        for (int i = 0; i < l.length; ++i) {
            nodes[i] = getLabelNode(l[i]);
        }
        return nodes;
    }

    private List<LabelNode> getLabelNodes(final List<Label> l) {
        if (l == null) {
            return null;
        }

        List<LabelNode> nodes = new ArrayList<LabelNode>(l.size());
        for (Label label : l) {
            nodes.add(getLabelNode(label));
        }
        return nodes;
    }


    // ------------------------------------------------------
    // Accept Methods.
    // ------------------------------------------------------

    /**
     * Makes the given Class visitor visit this Method.
     *
     * @param cv a Class visitor.
     */
    public void accept(final ClassVisitor cv) {
        String[] exceptions = null;
        if (this.exceptions != null) {
            exceptions = new String[this.exceptions.size()];
            this.exceptions.toArray(exceptions);
        }
        MethodVisitor mv = cv.visitMethod(access, name, desc, signature, exceptions);
        if (mv != null) {
            accept(mv);
        }
    }

    /**
     * Makes the given Method visitor visit this Method.
     *
     * @param mv a Method visitor.
     */
    public void accept(final MethodVisitor mv) {

        // Visits the annotation default.
        int i, j, n;
        if (annotationDefault != null) {
            AnnotationVisitor av = mv.visitAnnotationDefault();
            // In Dalvik, the name of the method to which is connected the DefaultAnnotation
            // must be given.
            AnnotationNode.accept(av, name, annotationDefault);
            if (av != null) {
                av.visitEnd();
            }
        }

        // Visits the method parameters.
        if ((parameters != null) && (parameters.length > 0)) {
            mv.visitParameters(parameters);
        }

        // Visits the annotations.
        n = visibleAnnotations == null ? 0 : visibleAnnotations.size();
        for (i = 0; i < n; ++i) {
            AnnotationNode an = (AnnotationNode) visibleAnnotations.get(i);
            an.accept(mv.visitAnnotation(an.desc, true));
        }
        n = invisibleAnnotations == null ? 0 : invisibleAnnotations.size();
        for (i = 0; i < n; ++i) {
            AnnotationNode an = (AnnotationNode) invisibleAnnotations.get(i);
            an.accept(mv.visitAnnotation(an.desc, false));
        }

        // Visits Parameter Annotations.
        n = visibleParameterAnnotations == null ? 0 : visibleParameterAnnotations.size();
        for (i = 0; i < n; ++i) {
            List<AnnotationNode> l = visibleParameterAnnotations.get(i);
            if (l == null) {
                continue;
            }
            for (j = 0; j < l.size(); ++j) {
                AnnotationNode an = (AnnotationNode) l.get(j);
                an.accept(mv.visitParameterAnnotation(i, an.desc, true));
            }
        }

        n = invisibleParameterAnnotations == null ? 0 : invisibleParameterAnnotations.size();
        for (i = 0; i < n; ++i) {
            List<AnnotationNode> l = invisibleParameterAnnotations.get(i);
            if (l == null) {
                continue;
            }
            for (j = 0; j < l.size(); ++j) {
                AnnotationNode an = (AnnotationNode) l.get(j);
                an.accept(mv.visitParameterAnnotation(i, an.desc, false));
            }
        }

        // Attributes are not supported by AsmDex.

        // Visits the code of the Methods.
        if (instructions.size() > 0) {
            mv.visitCode();
            // visits try catch blocks
            n = tryCatchBlocks == null ? 0 : tryCatchBlocks.size();
            for (i = 0; i < n; ++i) {
                ((TryCatchBlockNode) tryCatchBlocks.get(i)).accept(mv);
            }

            // Visits Instructions.
            instructions.accept(mv);

            // Visits Local Variables.
            n = localVariables == null ? 0 : localVariables.size();
            for (i = 0; i < n; ++i) {
                ((LocalVariableNode) localVariables.get(i)).accept(mv);
            }

            // Visits maxs.
            mv.visitMaxs(maxStack, 0);
        }
        mv.visitEnd();
    }

    /**
     * Visits an annotation of this class, field or method.
     *
     * @param desc the class descriptor of the annotation class.
     * @param visible <tt>true</tt> if the annotation is visible at runtime.
     * @return a visitor to visit the annotation values.
     */
    @Override
    public AnnotationVisitor visitAnnotation(final String desc, final boolean visible) {
        AnnotationNode an = new AnnotationNode(desc);
        if (visible) {
            if (visibleAnnotations == null) {
                visibleAnnotations = new ArrayList<AnnotationNode>(1);
            }
            visibleAnnotations.add(an);
        } else {
            if (invisibleAnnotations == null) {
                invisibleAnnotations = new ArrayList<AnnotationNode>(1);
            }
            invisibleAnnotations.add(an);
        }
        return an;
    }

}
