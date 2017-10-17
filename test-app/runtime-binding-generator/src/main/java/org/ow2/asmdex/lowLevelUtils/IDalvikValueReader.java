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

package org.ow2.asmdex.lowLevelUtils;


/**
 * Interface to the DalvikValueReader.
 *
 * @author Pierre Crégut
 */
public interface IDalvikValueReader {

    /**
     * Returns the byte array included to this reader.
     * @return the byte array included to this reader.
     */
    public byte[] getContents();

    /**
     * Reads next signed byte value.
     * @return value read from stream
     */
    public abstract byte sbyte();

    /**
     * Reads next unsigned byte value.
     * @return value read from stream
     */
    public abstract short ubyte();

    /**
     * Reads next signed short value.
     * @return value read from stream
     */
    public abstract short sshort();

    /**
     * Reads next unsigned short value.
     * @return value read from stream
     */
    public abstract int ushort();

    /**
     * Reads next signed integer value.
     * @return value read from stream
     */
    public abstract int sint();

    /**
     * Reads next unsigned integer value.
     * @return value read from stream
     */
    public abstract int uint();

    /**
     * Reads next signed integer value coded in leb128 format
     * @return value read from stream
     */
    public abstract int sleb128();

    /**
     * Reads next unsigned integer value coded in leb128 format
     * @return value read from stream
     */
    public abstract int uleb128();

    /**
     * Reads next unsigned integer value coded in leb128_p1 format
     * @return value read from stream
     */
    public abstract int uleb128_p1();

    /**
     * Reads next unsigned integer value coded in leb128 format but with 16 bits chars.
     * @return value read from stream
     */
    public abstract long uleb128_16();

    /**
     * Reads a long of a given size. Considered as unsigned.
     * @param sz
     * @return a positive long
     */
    public abstract long sizedLong(int sz);

    /**
     * Extends a long read with SizedLong of length sz according to its sign.
     * @param l
     * @param sz
     * @return long value
     */
    public abstract long completeSignSizedLong(long l, int sz);

    /**
     * Reads a null terminated UTF8 string as handled by Dalvik (limited to unicode)
     * @return string
     */
    public abstract String utf8String();

    /**
     * Set the position of the pointer in the stream.
     * @param pos positive absolute position
     */
    public abstract void seek(int pos);

    /**
     * Adds an offset to the current position of the pointer in the stream.
     * @param offset positive or negative offset to add.
     */
    public abstract void relativeSeek(int offset);

    /**
     * Get the current position of the pointer in the stream. Usually to make a save restore.
     * @return positive absolute position
     */
    public abstract int getPos();

    /**
     * Parse a string coded as 16 bit character
     * @param strSize
     * @return string
     */
    public abstract String unicodeString(int strSize);

    /**
     * Check if there are still data to read in the stream.
     * @return true if more data
     */
    public abstract boolean hasMore();

    /**
     * Skips an Int (signed or not).
     */
    void skipInt();

    /**
     * Skips a Short (signed or not).
     */
    void skipShort();

    /**
     * Skips a Byte (signed or not).
     */
    void skipByte();
}