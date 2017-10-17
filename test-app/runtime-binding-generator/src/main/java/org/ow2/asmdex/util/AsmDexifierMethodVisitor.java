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

package org.ow2.asmdex.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.structureCommon.Label;

/**
 * A {@link MethodVisitor} that prints the AsmDex code that generates the Methods it
 * visits.
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class AsmDexifierMethodVisitor extends MethodVisitor {

    private final AsmDexPrinter pr;

    /**
     * Map linking a Label to its name.
     */
    private HashMap<Label, String> labelNames = new HashMap<Label, String>();

    /**
     * Constructs a new {@link AsmDexifierMethodVisitor} object.
     *
     * @param api api level
     * @param nbTabulations number of spaces
     */
    public AsmDexifierMethodVisitor(int api, final int nbTabulations) {
        super(api);
        pr = new AsmDexPrinter();
        pr.currentTabulation = nbTabulations;
    }


    // ------------------------------------------------
    // Visitor Methods.
    // ------------------------------------------------

    @Override
    public AnnotationVisitor visitAnnotation(String desc, boolean visible) {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("\tav0 = mv.visitAnnotation(");
        pr.addConstant(desc, true);
        pr.addBoolean(visible, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();

        AsmDexifierAnnotationVisitor av = new AsmDexifierAnnotationVisitor(api, 0, pr.currentTabulation + 1);
        pr.addTextToList(av.getTextComponent());

        pr.addTabulation();
        pr.addText("}\n");
        pr.closeText();

        return av;
    }

    @Override
    public AnnotationVisitor visitAnnotationDefault() {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("\tav0 = mv.visitAnnotationDefault();");
        pr.addEOL();
        pr.closeText();

        AsmDexifierAnnotationVisitor av = new AsmDexifierAnnotationVisitor(api, 0, pr.currentTabulation + 1);
        pr.addTextToList(av.getTextComponent());

        pr.addTabulation();
        pr.addText("}\n");
        pr.closeText();

        return av;
    }

    @Override
    public void visitArrayLengthInsn(int destinationRegister, int arrayReferenceBearing) {
        pr.addTabulation();
        pr.addText("mv.visitArrayLengthInsn(");
        pr.addNumber(destinationRegister, true);
        pr.addNumber(arrayReferenceBearing, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitArrayOperationInsn(int opcode, int valueRegister,
                                        int arrayRegister, int indexRegister) {
        pr.addTabulation();
        pr.addText("mv.visitArrayOperationInsn(");
        pr.addOpcode(opcode, true);
        pr.addNumber(valueRegister, true);
        pr.addNumber(arrayRegister, true);
        pr.addNumber(indexRegister, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitAttribute(Object attr) {
        // Ignored by AsmDex.
    }

    @Override
    public void visitCode() {
        pr.addTabulation();
        pr.addText("mv.visitCode();\n");
        pr.closeText();
    }

    @Override
    public void visitEnd() {
        pr.addTabulation();
        pr.addText("mv.visitEnd();\n");
        pr.closeText();
    }

    @Override
    public void visitFieldInsn(int opcode, String owner, String name,
                               String desc, int valueRegister, int objectRegister) {
        pr.addTabulation();
        pr.addText("mv.visitFieldInsn(");
        pr.addOpcode(opcode, true);
        pr.addConstant(owner, true);
        pr.addConstant(name, true);
        pr.addConstant(desc, true);
        pr.addNumber(valueRegister, true);
        pr.addNumber(objectRegister, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitFillArrayDataInsn(int arrayReference, Object[] arrayData) {
        pr.addTabulation();
        pr.addText("mv.visitFillArrayDataInsn(");
        pr.addNumber(arrayReference, true);
        pr.addConstant(arrayData, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitFrame(int type, int nLocal, Object[] local, int nStack,
                           Object[] stack) {
        // Ignored by AsmDex.
    }

    @Override
    public void visitInsn(int opcode) {
        pr.addTabulation();
        pr.addText("mv.visitInsn(");
        pr.addOpcode(opcode, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitIntInsn(int opcode, int register) {
        pr.addTabulation();
        pr.addText("mv.visitIntInsn(");
        pr.addOpcode(opcode, true);
        pr.addNumber(register, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitJumpInsn(int opcode, Label label, int registerA,
                              int registerB) {
        String name = declareLabel(label);
        pr.addTabulation();
        pr.addText("mv.visitJumpInsn(");
        pr.addOpcode(opcode, true);
        pr.addText(name, true);
        pr.addNumber(registerA, true);
        pr.addNumber(registerB, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitLabel(Label label) {
        String name = declareLabel(label);
        pr.addTabulation();
        pr.addText("mv.visitLabel(" + name + ");\n");
        pr.closeText();
    }

    @Override
    public void visitLineNumber(int line, Label start) {
        String name = declareLabel(start);
        pr.addTabulation();
        pr.addText("mv.visitLineNumber(");
        pr.addNumber(line, true);
        pr.addText(name + ");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, Label end, int index) {
        String nameStart = declareLabel(start);
        String nameEnd = declareLabel(end);
        pr.addTabulation();
        pr.addText("mv.visitLocalVariable(");
        pr.addConstant(name, true);
        pr.addConstant(desc, true);
        pr.addConstant(signature, true);
        pr.addText(nameStart, true);
        pr.addText(nameEnd, true);
        pr.addNumber(index, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, List<Label> ends, List<Label> restarts, int index) {
        String nameStart = declareLabel(start);
        List<String> nameEnds = declareLabelsToList(ends);
        List<String> nameRestarts = declareLabelsToList(restarts);
        pr.addTabulation();
        pr.addText("mv.visitLocalVariable(");
        pr.addConstant(name, true);
        pr.addConstant(desc, true);
        pr.addConstant(signature, true);
        pr.addText(nameStart, true);
        pr.addStringArrayList(nameEnds, true);
        pr.addStringArrayList(nameRestarts, true);
        pr.addNumber(index, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitLookupSwitchInsn(int register, Label dflt, int[] keys,
                                      Label[] labels) {
        String nameDefault = declareLabel(dflt);
        String[] nameLabels = declareLabels(labels);
        pr.addTabulation();
        pr.addText("mv.visitLookupSwitchInsn(");
        pr.addNumber(register, true);
        pr.addText(nameDefault, true);
        pr.addConstant(keys, true);
        pr.addLabelArray(nameLabels, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitMaxs(int maxStack, int maxLocals) {
        pr.addTabulation();
        pr.addText("mv.visitMaxs(");
        pr.addNumber(maxStack, true);
        pr.addNumber(maxLocals, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitMethodInsn(int opcode, String owner, String name,
                                String desc, int[] arguments) {
        pr.addTabulation();
        pr.addText("mv.visitMethodInsn(");
        pr.addOpcode(opcode, true);
        pr.addConstant(owner, true);
        pr.addConstant(name, true);
        pr.addConstant(desc, true);
        pr.addConstant(arguments, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitMultiANewArrayInsn(String desc, int[] registers) {
        pr.addTabulation();
        pr.addText("mv.visitMultiANewArrayInsn(");
        pr.addConstant(desc, true);
        pr.addConstant(registers, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitOperationInsn(int opcode, int destinationRegister,
                                   int firstSourceRegister, int secondSourceRegister, int value) {
        pr.addTabulation();
        pr.addText("mv.visitOperationInsn(");
        pr.addOpcode(opcode, true);
        pr.addNumber(destinationRegister, true);
        pr.addNumber(firstSourceRegister, true);
        pr.addNumber(secondSourceRegister, true);
        pr.addNumber(value, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public AnnotationVisitor visitParameterAnnotation(int parameter,
            String desc, boolean visible) {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("\tav0 = mv.visitParameterAnnotation(");
        pr.addNumber(parameter, true);
        pr.addConstant(desc, true);
        pr.addBoolean(visible, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();

        AsmDexifierAnnotationVisitor av = new AsmDexifierAnnotationVisitor(api, 0, pr.currentTabulation + 1);
        pr.addTextToList(av.getTextComponent());

        pr.addTabulation();
        pr.addText("}\n");
        pr.closeText();

        return av;
    }

    @Override
    public void visitParameters(String[] parameters) {
        pr.addTabulation();
        pr.addText("mv.visitParameters(");
        pr.addConstant(parameters, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitStringInsn(int opcode, int destinationRegister, String string) {
        pr.addTabulation();
        pr.addText("mv.visitStringInsn(");
        pr.addOpcode(opcode, true);
        pr.addNumber(destinationRegister, true);
        pr.addConstant(string, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitTableSwitchInsn(int register, int min, int max,
                                     Label dflt, Label[] labels) {
        String nameDefault = declareLabel(dflt);
        String[] nameLabels = declareLabels(labels);
        pr.addTabulation();
        pr.addText("mv.visitTableSwitchInsn(");
        pr.addNumber(register, true);
        pr.addNumber(min, true);
        pr.addNumber(max, true);
        pr.addText(nameDefault, true);
        pr.addLabelArray(nameLabels, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitTryCatchBlock(Label start, Label end, Label handler,
                                   String type) {
        String nameStart = declareLabel(start);
        String nameEnd = declareLabel(end);
        String nameHandler = declareLabel(handler);
        pr.addTabulation();
        pr.addText("mv.visitTryCatchBlock(");
        pr.addText(nameStart, true);
        pr.addText(nameEnd, true);
        pr.addText(nameHandler, true);
        pr.addConstant(type, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitTypeInsn(int opcode, int destinationRegister,
                              int referenceBearingRegister, int sizeRegister, String type) {
        pr.addTabulation();
        pr.addText("mv.visitTypeInsn(");
        pr.addOpcode(opcode, true);
        pr.addNumber(destinationRegister, true);
        pr.addNumber(referenceBearingRegister, true);
        pr.addNumber(sizeRegister, true);
        pr.addConstant(type, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, int var) {
        pr.addTabulation();
        pr.addText("mv.visitVarInsn(");
        pr.addOpcode(opcode, true);
        pr.addNumber(destinationRegister, true);
        pr.addNumber(var, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, long var) {
        pr.addTabulation();
        pr.addText("mv.visitVarInsn(");
        pr.addOpcode(opcode, true);
        pr.addNumber(destinationRegister, true);
        pr.addNumber(var, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }


    // ------------------------------------------------
    // Private Methods.
    // ------------------------------------------------

    /**
     * Appends a declaration of the given label to current text. This
     * declaration is of the form "Label lXXX = new Label();". Does nothing if
     * the given label has already been declared, or if the Label is Null.
     *
     * @param label a Label. May be Null.
     * @return the name of the Label, whether it existed or had been just created.
     */
    private String declareLabel(final Label label) {
        String name = null;
        if (label != null) {
            if (!labelNames.containsKey(label)) {
                pr.addTabulation();
                name = "l" + labelNames.size();
                labelNames.put(label, name);
                pr.addText("Label " + name + " = new Label();\n");
            } else {
                name = (String)labelNames.get(label);
            }
        }

        return name;
    }

    /**
     * Appends a declaration of the given label to current text. This
     * declaration is of the form "Label lXXX = new Label();". Does nothing if
     * the given label has already been declared.
     *
     * @param labels the Labels.
     * @return the name of the Labels, whether it existed or had been just created.
     */
    private String[] declareLabels(final Label[] labels) {
        String[] names = new String[labels.length];

        int i = 0;
        for (Label label : labels) {
            names[i] = declareLabel(label);
            i++;
        }

        return names;
    }

    /**
     * Appends a declaration of the given label to current text. This
     * declaration is of the form "Label lXXX = new Label();". Does nothing if
     * the given label has already been declared.
     * May be Null.
     *
     * @param labels the Labels, or Null.
     * @return the name of the Labels, whether it existed or had been just created, or Null.
     */
    private List<String> declareLabelsToList(final List<Label> labels) {
        List<String> names = null;
        if (labels != null) {
            names = new ArrayList<String>(labels.size());
            for (Label label : labels) {
                names.add(declareLabel(label));
            }
        }
        return names;
    }

    /**
     * Returns the Text Component this element holds.
     * @return the Text Component this element holds.
     */
    public TextComponent getTextComponent() {
        return pr.getTextComponent();
    }
}
