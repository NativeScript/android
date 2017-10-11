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
import org.ow2.asmdex.FieldVisitor;

/**
 * A {@link FieldVisitor} that prints the AsmDex code that generates the Fields it
 * visits.
 *
 * @author Julien Névo, based on the ASM Framework.
 */
public class AsmDexifierFieldVisitor extends FieldVisitor {

    private final AsmDexPrinter pr;


    /**
     * Constructs a new {@link AsmDexifierFieldVisitor} object.
     *
     * @param api API level
     * @param nbTabulations number of spaces
     */
    public AsmDexifierFieldVisitor(int api, final int nbTabulations) {
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
        pr.addText("\tav0 = fv.visitAnnotation(");
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
        pr.addText("fv.visitEnd();");
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
