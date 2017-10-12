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

package org.ow2.asmdex.encodedValue;

import org.ow2.asmdex.structureWriter.ConstantPool;

/**
 * Interface of an Encoded Value.
 *
 * @author Julien Névo
 */
public abstract class EncodedValue implements Comparable<EncodedValue> {

    /**
     * Returns the Type of an Encoded Value (see Opcodes).
     * @return the Type of an Encoded Value.
     */
    public abstract int getType();

    /**
     * Returns the bytes of the Encoded Value.
     * @param constantPool the Constant Pool.
     * @return the bytes of the Encoded Value.
     */
    public abstract byte[] encode(ConstantPool constantPool);

    @Override
    public int compareTo(EncodedValue encodedValue) {
        if (this == encodedValue) {
            return 0;
        }

        int t1 = getType();
        int t2 = encodedValue.getType();
        int result = (t1 == t2 ? 0 : (t1 < t2 ? -1 : 1));
        if (result == 0) {
            result = compareValue(encodedValue);
        }
        return result;
    }

    abstract int compareValue(EncodedValue encodedValue);

}
