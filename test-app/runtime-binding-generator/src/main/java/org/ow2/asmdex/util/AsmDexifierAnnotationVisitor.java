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

/**
 * An {@link AnnotationVisitor} that prints the AsmDex code that generates the Annotations it
 * visits.
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class AsmDexifierAnnotationVisitor extends AnnotationVisitor {

    /**
     * Identifier of the annotation visitor variable in the produced code.
     */
    protected final int id;

    final private AsmDexPrinter pr;

    /**
     * Constructs a new {@link AsmDexifierAnnotationVisitor}.
     *
     * @param api API level
     * @param id identifier of the annotation visitor variable in the produced code.
     */
    public AsmDexifierAnnotationVisitor(final int api, final int id, final int nbTabulations) {
        super(api);
        this.id = id;
        pr = new AsmDexPrinter();
        pr.currentTabulation = nbTabulations;
    }


    // ------------------------------------------------
    // Visitor Methods.
    // ------------------------------------------------

    @Override
    public void visit(String name, Object value) {
        pr.addTabulation();
        pr.addText("av");
        pr.addNumber(id, false);
        pr.addText(".visit(");
        pr.addConstant(name, true);
        pr.addConstant(value, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public AnnotationVisitor visitAnnotation(String name, String desc) {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("\tAnnotationVisitor av");
        pr.addNumber(id + 1, false);
        pr.addText(" = ");
        pr.addText("av");
        pr.addNumber(id, false);
        pr.addText(".visitAnnotation(");
        pr.addConstant(name, true);
        pr.addConstant(desc, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();

        AsmDexifierAnnotationVisitor av = new AsmDexifierAnnotationVisitor(api, id + 1, pr.currentTabulation + 1);
        pr.addTextToList(av.getTextComponent());

        pr.addTabulation();
        pr.addText("}\n");
        pr.closeText();

        return av;
    }

    TextComponent getTextComponent() {
        return pr.getTextComponent();
    }


    @Override
    public AnnotationVisitor visitArray(String name) {
        pr.addTabulation();
        pr.addText("{\n");
        pr.addTabulation();
        pr.addText("\tAnnotationVisitor av");
        pr.addNumber(id + 1, false);
        pr.addText(" = ");
        pr.addText("av");
        pr.addNumber(id, false);
        pr.addText(".visitArray(");
        pr.addConstant(name, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();

        AsmDexifierAnnotationVisitor av = new AsmDexifierAnnotationVisitor(api, id + 1, pr.currentTabulation + 1);
        pr.addTextToList(av.getTextComponent());

        pr.addTabulation();
        pr.addText("}\n");
        pr.closeText();

        return av;
    }

    @Override
    public void visitEnd() {
        pr.addTabulation();
        pr.addText("av");
        pr.addNumber(id, false);
        pr.addText(".visitEnd();");
        pr.addEOL();
        pr.closeText();
    }

    @Override
    public void visitEnum(String name, String desc, String value) {
        pr.addTabulation();
        pr.addText("av");
        pr.addNumber(id, false);
        pr.addText(".visitEnum(");
        pr.addConstant(name, true);
        pr.addConstant(desc, true);
        pr.addConstant(value, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }


    @Override
    public void visitClass(String annotationName, String className) {
        pr.addTabulation();
        pr.addText("av");
        pr.addNumber(id, false);
        pr.addText(".visitClass(");
        pr.addConstant(annotationName, true);
        pr.addConstant(className, false);
        pr.addText(");");
        pr.addEOL();
        pr.closeText();
    }

}
