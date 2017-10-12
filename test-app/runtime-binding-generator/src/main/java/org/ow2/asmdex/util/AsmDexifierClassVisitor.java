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

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.ClassVisitor;
import org.ow2.asmdex.FieldVisitor;
import org.ow2.asmdex.MethodVisitor;

/**
 * A {@link ClassVisitor} that prints the AsmDex code that generates the classes it
 * visits.
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class AsmDexifierClassVisitor extends ClassVisitor {

    final AsmDexPrinter pr;
    /**
     * Constructs a new {@link AsmDexifierClassVisitor} object.
     *
     * @param api api level
     * @param nbTabulations number of spaces
     */
    public AsmDexifierClassVisitor(int api, final int nbTabulations) {
        super(api);
        pr = new AsmDexPrinter();
        pr.currentTabulation = nbTabulations;
    }


    // ------------------------------------------------
    // Visitor Methods.
    // ------------------------------------------------

    @Override
    public void visit(int version, int access, String name, String[] signature,
                      String superName, String[] interfaces) {
        pr.addTabulation();
        pr.addText("cv.visit(0, ");
        pr.addAccessFlags(access, true);
        pr.addConstant(name, true);
        pr.addConstant(signature, true);
        pr.addConstant(superName, true);
        pr.addConstant(interfaces, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public AnnotationVisitor visitAnnotation(String desc, boolean visible) {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("av0 = cv.visitAnnotation(");
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
    public void visitAttribute(Object attr) {
        // Ignored by AsmDex.
    }

    @Override
    public void visitEnd() {
        pr.addTabulation();
        pr.addText("cv.visitEnd();");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public FieldVisitor visitField(int access, String name, String desc, String[] signature, Object value) {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("\tfv = cv.visitField(");
        pr.addAccessFlags(access, true);
        pr.addConstant(name, true);
        pr.addConstant(desc, true);
        pr.addConstant(signature, true);
        pr.addConstant(value, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();

        AsmDexifierFieldVisitor fv = new AsmDexifierFieldVisitor(api, pr.currentTabulation + 1);
        pr.addTextToList(fv.getTextComponent());

        pr.addTabulation();
        pr.addText("}\n");
        pr.closeText();

        return fv;
    }

    @Override
    public void visitInnerClass(String name, String outerName, String innerName, int access) {
        pr.addTabulation();
        pr.addText("cv.visitInnerClass(");
        pr.addConstant(name, true);
        pr.addConstant(outerName, true);
        pr.addConstant(innerName, true);
        pr.addAccessFlags(access, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }


    @Override
    public void visitMemberClass(String name, String outerName, String innerName) {
        pr.addTabulation();
        pr.addText("cv.visitMemberClass(");
        pr.addConstant(name, true);
        pr.addConstant(outerName, true);
        pr.addConstant(innerName, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public MethodVisitor visitMethod(int access, String name, String desc, String[] signature, String[] exceptions) {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("\tmv = cv.visitMethod(");
        pr.addAccessFlags(access, true);
        pr.addConstant(name, true);
        pr.addConstant(desc, true);
        pr.addConstant(signature, true);
        pr.addConstant(exceptions, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();

        AsmDexifierMethodVisitor mv = new AsmDexifierMethodVisitor(api, pr.currentTabulation + 1);
        pr.addTextToList(mv.getTextComponent());

        pr.addTabulation();
        pr.addText("}\n");
        pr.closeText();

        return mv;
    }

    @Override
    public void visitOuterClass(String owner, String name, String desc) {
        pr.addTabulation();
        pr.addText("cv.visitOuterClass(");
        pr.addConstant(owner, true);
        pr.addConstant(name, true);
        pr.addConstant(desc, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitSource(String source, String debug) {
        pr.addTabulation();
        pr.addText("cv.visitSource(");
        pr.addConstant(source, true);
        pr.addConstant(debug, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    /**
     * Returns the Text Component this element holds.
     * @return the Text Component this element holds.
     */

    public TextComponent getTextComponent() {
        return pr.getTextComponent();
    }

}
