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

package org.ow2.asmdex.specificAnnotationParser;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.ApplicationReader;
import org.ow2.asmdex.lowLevelUtils.DexFileReader;
import org.ow2.asmdex.specificAnnotationVisitors.InnerClassAnnotationVisitor;

/**
 * Specific annotation parser for Inner Classes. Recovers the access flag
 * and the simple name of the inner class.
 *
 * @author Julien Névo
 */
public class InnerClassSpecificAnnotationParser implements ISpecificAnnotationParser {

    private String annotationName;
    @Override
    public String getAnnotationName() {
        return annotationName;
    }

    /**
     * Parser for an annotation specifying an inner class
     * @param annotationName
     */
    public InnerClassSpecificAnnotationParser(String annotationName) {
        this.annotationName = annotationName;
    }

    private String simpleNameInnerClass = null;
    /**
     * Simple name of the inner class
     * @return string
     */
    public String getSimpleNameInnerClass() {
        return simpleNameInnerClass;
    }
    private int accessFlagsInnerClass = 0;
    /**
     * Access flags for inner class
     * @return or of flags
     */
    public int getAccessFlagsInnerClass() {
        return accessFlagsInnerClass;
    }

    @Override
    public void treat(DexFileReader dexFile, ApplicationReader applicationReader,
                      AnnotationVisitor annotationVisitor) {

        InnerClassAnnotationVisitor iav = (InnerClassAnnotationVisitor)annotationVisitor;
        accessFlagsInnerClass = iav.getAccess();
        simpleNameInnerClass = iav.getSimpleName();
    }
}