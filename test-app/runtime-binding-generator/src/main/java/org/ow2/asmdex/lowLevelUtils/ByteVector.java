/***
 * ASM: a very small and fast Java bytecode manipulation framework
 * Copyright (c) 2000-2007 INRIA, France Telecom
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

import java.util.Arrays;

/**
 * A dynamically extensible vector of bytes. This class is roughly equivalent to
 * a DataOutputStream on top of a ByteArrayOutputStream, but is more efficient.
 *
 * NOTE :
 * - putMUTF8() uses the same algorithm as "modified" UTF8 from Java (and ASM), with a removed the size at the
 *   beginning and a 0 at the end.
 *
 * @author Eric Bruneton
 * @author Julien Névo (change to little-endian, addition of putULeb128 and
 * 		   putULeb128p1 methods, possibility to write at random access, addPadding,
 * 		   putSleb128, UTF8 to MUTF8).
 */
public class ByteVector {

    /**
     * The content of this vector.
     */
    byte[] data;

    /**
     * Returns the data actually written in this vector.
     * @return the data actually written in this vector.
     */
    public byte[] getData() {
        if (data == null) {
            return null;
        }

        if (length == data.length) {
            return data;
        }

        return Arrays.copyOf(data, length);
    }

    /**
     * Returns the whole buffer, which contains the data but also the blank bytes that
     * (probably) follows. Use this to get the data without wanting to copy them, and
     * use getLength() to treat only what is necessary.
     * @return the whole buffer, which contains the data but also the blank bytes that
     * 		   (probably) follows.
     */
    public byte[] getBuffer() {
        return data;
    }

    /**
     * Actual number of bytes in this vector.
     */
    int length;

    /**
     * Returns the number of bytes this vector contains.
     * @return the number of bytes this vector contains.
     */
    public int getLength() {
        return length;
    }

    /**
     * Constructs a new {@link ByteVector ByteVector} with a default initial
     * size.
     */
    public ByteVector() {
        data = new byte[64];
    }

    /**
     * Constructs a new {@link ByteVector ByteVector} with the given initial
     * size.
     *
     * @param initialSize the initial size of the byte vector to be constructed.
     */
    public ByteVector(final int initialSize) {
        data = new byte[initialSize];
    }

    /**
     * Constructs a new {@link ByteVector ByteVector} with the given byte array.
     * The byte array is <i>NOT</i> copied.
     * @param bytes the byte array.
     */
    public ByteVector(byte[] bytes) {
        data = bytes;
        length = bytes.length;
    }

    /**
     * Puts a byte into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     *
     * @param b a byte.
     * @return this byte vector.
     */
    public ByteVector putByte(final int b) {
        int length = this.length;
        if (length + 1 > data.length) {
            enlarge(1);
        }
        data[length++] = (byte) b;
        this.length = length;
        return this;
    }

    /**
     * Puts a byte into this byte vector at the given position.
     * The byte vector is <i>not</i> enlarged if necessary.
     * @param b a byte.
     * @param position position in byte in the byte vector.
     * @return this byte vector.
     */
    public ByteVector putByte(final int b, final int position) {
        data[position] = (byte) b;
        return this;
    }

    /**
     * Puts a Byte Vector at the end of this byte vector.
     * The byte vector is automatically enlarged if necessary.
     * @param bv the byte vector.
     * @return this byte vector.
     */
    public ByteVector putByteVector(final ByteVector bv) {
        putByteArray(bv.data, 0, bv.length);
        return this;
    }

    /**
     * Puts a Byte Vector into this byte vector at the given position.
     * The byte vector is <i>not</i> enlarged if necessary.
     * @param bv the byte vector.
     * @param position position in byte in the byte vector.
     * @return this byte vector.
     */
    public ByteVector putByteVector(final ByteVector bv, final int position) {
        byte[] b = bv.getData();
        putByteArray(b, position);
        return this;
    }

    /**
     * Puts two bytes into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     *
     * @param b1 a byte.
     * @param b2 another byte.
     * @return this byte vector.
     */
    ByteVector put11(final int b1, final int b2) {
        int length = this.length;
        if (length + 2 > data.length) {
            enlarge(2);
        }
        byte[] data = this.data;
        data[length++] = (byte) b1;
        data[length++] = (byte) b2;
        this.length = length;
        return this;
    }

    /**
     * Puts a short into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     *
     * @param s a short.
     * @return this byte vector.
     */
    public ByteVector putShort(final int s) {
        int length = this.length;
        if (length + 2 > data.length) {
            enlarge(2);
        }
        byte[] data = this.data;
        data[length++] = (byte) s;			// Swapped.
        data[length++] = (byte) (s >>> 8);	// Swapped.
        this.length = length;
        return this;
    }

    /**
     * The byte vector is <i>not</i> enlarged if necessary.
     * @param s an int.
     * @param position position in byte in the byte vector.
     * @return this byte vector.
     */
    public ByteVector putShort(final int s, int position) {
        byte[] data = this.data;
        data[position++] = (byte) s;			// Swapped.
        data[position++] = (byte) (s >>> 8);	// Swapped.
        return this;
    }


    /**
     * Puts a ULeb128 number into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     * @param s an int.
     * @return this byte vector.
     */
    public ByteVector putUleb128(final int s) {
        int nb1 = s & 0x7f;
        int nb2 = 0, nb3 = 0, nb4 = 0, nb5 = 0;

        int nbLength = 1;
        if (s > 0x7f) {
            nbLength++;
            nb1 = nb1 + 0x80;	// Adds the highest bit.

            nb2 = ((s >>> 7) & 0x7f);
            if (s > (0x3fff)) {
                nbLength++;
                nb2 = nb2 + 0x80;

                nb3 = ((s >>> 14) & 0x7f);
                if (s > (0x1fffff)) {
                    nbLength++;
                    nb3 = nb3 + 0x80;

                    nb4 = ((s >>> 21) & 0x7f);
                    if (s > (0xfffffff)) {
                        nbLength++;
                        nb4 = nb4 & 0x7f;
                    }

                    nb5 = (s >>> 28) + 0x80;
                }
            }
        }

        int length = this.length;
        if (length + nbLength > data.length) {
            enlarge(nbLength);
        }
        byte[] data = this.data;
        data[length++] = (byte)nb1;
        if (nbLength > 1) {
            data[length++] = (byte)nb2;
        }
        if (nbLength > 2) {
            data[length++] = (byte)nb3;
        }
        if (nbLength > 3) {
            data[length++] = (byte)nb4;
        }
        if (nbLength > 4) {
            data[length++] = (byte)nb5;
        }
        this.length = length;
        return this;
    }

    /**
     * Puts a ULeb128p1 number into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     * @param s an int.
     * @return this byte vector.
     */
    public ByteVector putUleb128p1(final int s) {
        return putUleb128(s + 1);
    }

    /**
     * Puts a SLeb128 number into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     * @param s an int.
     * @return this byte vector.
     */
    public ByteVector putSleb128(int s) {

        int nb1 = 0, nb2 = 0, nb3 = 0, nb4 = 0, nb5 = 0;
        int remaining = s >> 7;
        int nbLength = 0;
        int end = ((s & Integer.MIN_VALUE) == 0) ? 0 : -1;

        boolean mustContinue = (remaining != end) || ((remaining & 1) != ((s >> 6) & 1));
        nb1 = (s & 0x7f) | (mustContinue ? 0x80 : 0);
        s = remaining;
        remaining = remaining >> 7;
        nbLength++;
        if (mustContinue) {
            mustContinue = (remaining != end) || ((remaining & 1) != ((s >> 6) & 1));
            nb2 = (s & 0x7f) | (mustContinue ? 0x80 : 0);
            s = remaining;
            remaining = remaining >> 7;
            nbLength++;
            if (mustContinue) {
                mustContinue = (remaining != end) || ((remaining & 1) != ((s >> 6) & 1));
                nb3 = (s & 0x7f) | (mustContinue ? 0x80 : 0);
                s = remaining;
                remaining = remaining >> 7;
                nbLength++;
                if (mustContinue) {
                    mustContinue = (remaining != end) || ((remaining & 1) != ((s >> 6) & 1));
                    nb4 = (s & 0x7f) | (mustContinue ? 0x80 : 0);
                    s = remaining;
                    remaining = remaining >> 7;
                    nbLength++;
                }
                if (mustContinue) {
                    nb5 = (s & 0x7f) | (mustContinue ? 0x80 : 0);
                    nbLength++;
                }
            }
        }

        int length = this.length;
        if (length + nbLength > data.length) {
            enlarge(nbLength);
        }
        byte[] data = this.data;
        data[length++] = (byte)nb1;
        if (nbLength > 1) {
            data[length++] = (byte)nb2;
        }
        if (nbLength > 2) {
            data[length++] = (byte)nb3;
        }
        if (nbLength > 3) {
            data[length++] = (byte)nb4;
        }
        if (nbLength > 4) {
            data[length++] = (byte)nb5;
        }
        this.length = length;
        return this;
    }

    /**
     * Puts a byte and a short into this byte vector. The byte vector is
     * automatically enlarged if necessary.
     *
     * @param b a byte.
     * @param s a short.
     * @return this byte vector.
     */
    ByteVector put12(final int b, final int s) {
        int length = this.length;
        if (length + 3 > data.length) {
            enlarge(3);
        }
        byte[] data = this.data;
        data[length++] = (byte) b;
        data[length++] = (byte) s;			// Swapped.
        data[length++] = (byte) (s >>> 8);	// Swapped.
        this.length = length;
        return this;
    }

    /**
     * Puts an int into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     *
     * @param i an int.
     * @return this byte vector.
     */
    public ByteVector putInt(final int i) {
        int length = this.length;
        if (length + 4 > data.length) {
            enlarge(4);
        }
        byte[] data = this.data;
        data[length++] = (byte) i;			// Swapped.
        data[length++] = (byte) (i >>> 8);
        data[length++] = (byte) (i >>> 16);
        data[length++] = (byte) (i >>> 24);
        this.length = length;
        return this;
    }

    /**
     * Puts an int into this byte vector at the given position.
     * The byte vector is <i>not</i> enlarged if necessary.
     * @param i an int.
     * @param position position in byte in the byte vector.
     * @return this byte vector.
     */
    public ByteVector putInt(final int i, int position) {
        byte[] data = this.data;
        data[position++] = (byte) i;			// Swapped.
        data[position++] = (byte) (i >>> 8);
        data[position++] = (byte) (i >>> 16);
        data[position++] = (byte) (i >>> 24);
        return this;
    }


    /**
     * Puts a long into this byte vector. The byte vector is automatically
     * enlarged if necessary.
     *
     * @param l a long.
     * @return this byte vector.
     */
    public ByteVector putLong(final long l) {
        int length = this.length;
        if (length + 8 > data.length) {
            enlarge(8);
        }
        byte[] data = this.data;
        int i = (int) l;
        data[length++] = (byte) i;
        data[length++] = (byte) (i >>> 8);
        data[length++] = (byte) (i >>> 16);
        data[length++] = (byte) (i >>> 24);
        i = (int) (l >>> 32);
        data[length++] = (byte) i;
        data[length++] = (byte) (i >>> 8);
        data[length++] = (byte) (i >>> 16);
        data[length++] = (byte) (i >>> 24);

        this.length = length;
        return this;
    }

    /**
     * Puts an UTF8 string into this byte vector. The byte vector is
     * automatically enlarged if necessary.
     *
     * @param s a String.
     * @return this byte vector.
     */
    public ByteVector putUTF8(final String s) {
        int charLength = s.length();
        int len = length;
        if (len + 2 + charLength > data.length) {
            enlarge(2 + charLength);
        }
        byte[] data = this.data;
        // optimistic algorithm: instead of computing the byte length and then
        // serializing the string (which requires two loops), we assume the byte
        // length is equal to char length (which is the most frequent case), and
        // we start serializing the string right away. During the serialization,
        // if we find that this assumption is wrong, we continue with the
        // general method.
        data[len++] = (byte) (charLength >>> 8);
        data[len++] = (byte) charLength;
        for (int i = 0; i < charLength; ++i) {
            char c = s.charAt(i);
            if (c >= '\001' && c <= '\177') {
                data[len++] = (byte) c;
            } else {
                int byteLength = i;
                for (int j = i; j < charLength; ++j) {
                    c = s.charAt(j);
                    if (c >= '\001' && c <= '\177') {
                        byteLength++;
                    } else if (c > '\u07FF') {
                        byteLength += 3;
                    } else {
                        byteLength += 2;
                    }
                }
                data[length] = (byte) (byteLength >>> 8);
                data[length + 1] = (byte) byteLength;
                if (length + 2 + byteLength > data.length) {
                    length = len;
                    enlarge(2 + byteLength);
                    data = this.data;
                }
                for (int j = i; j < charLength; ++j) {
                    c = s.charAt(j);
                    if (c >= '\001' && c <= '\177') {
                        data[len++] = (byte) c;
                    } else if (c > '\u07FF') {
                        data[len++] = (byte) (0xE0 | c >> 12 & 0xF);
                        data[len++] = (byte) (0x80 | c >> 6 & 0x3F);
                        data[len++] = (byte) (0x80 | c & 0x3F);
                    } else {
                        data[len++] = (byte) (0xC0 | c >> 6 & 0x1F);
                        data[len++] = (byte) (0x80 | c & 0x3F);
                    }
                }
                break;
            }
        }
        length = len;
        return this;
    }

    /**
     * Puts a MUTF8 string into this byte vector. The byte vector is
     * automatically enlarged if necessary.
     *
     * @param s a String.
     * @return this byte vector.
     */
    public ByteVector putMUTF8(final String s) {
        int charLength = s.length();
        int len = length;
        // + 1 because we don't code the size like UTF8, but encode a 0 at the end.
        if (len + 1 + charLength > data.length) {
            enlarge(1 + charLength);
        }
        byte[] data = this.data;
        // optimistic algorithm: instead of computing the byte length and then
        // serializing the string (which requires two loops), we assume the byte
        // length is equal to char length (which is the most frequent case), and
        // we start serializing the string right away. During the serialization,
        // if we find that this assumption is wrong, we continue with the
        // general method.
        for (int i = 0; i < charLength; ++i) {
            char c = s.charAt(i);
            if (c >= '\001' && c <= '\177') {
                data[len++] = (byte) c;
            } else {
                int byteLength = i;
                for (int j = i; j < charLength; ++j) {
                    c = s.charAt(j);
                    if (c >= '\001' && c <= '\177') {
                        byteLength++;
                    } else if (c > '\u07FF') {
                        byteLength += 3;
                    } else {
                        byteLength += 2;
                    }
                }
                if (length + 1 + byteLength > data.length) {
                    length = len;
                    enlarge(1 + byteLength);
                    data = this.data;
                }
                for (int j = i; j < charLength; ++j) {
                    c = s.charAt(j);
                    if (c >= '\001' && c <= '\177') {
                        data[len++] = (byte) c;
                    } else if (c > '\u07FF') {
                        data[len++] = (byte) (0xE0 | c >> 12 & 0xF);
                        data[len++] = (byte) (0x80 | c >> 6 & 0x3F);
                        data[len++] = (byte) (0x80 | c & 0x3F);
                    } else {
                        data[len++] = (byte) (0xC0 | c >> 6 & 0x1F);
                        data[len++] = (byte) (0x80 | c & 0x3F);
                    }
                }
                break;
            }
        }
        data[len++] = 0;
        length = len;
        return this;
    }



    /**
     * Puts an array of bytes into this byte vector. The byte vector is
     * automatically enlarged if necessary.
     *
     * @param b an array of bytes. May be <tt>null</tt> to put <tt>len</tt>
     *        null bytes into this byte vector.
     * @param off index of the fist byte of b that must be copied.
     * @param len number of bytes of b that must be copied.
     * @return this byte vector.
     */
    public ByteVector putByteArray(final byte[] b, final int off, final int len) {
        if (length + len > data.length) {
            enlarge(len);
        }
        if (b != null) {
            System.arraycopy(b, off, data, length, len);
        }
        length += len;
        return this;
    }

    /**
     * Puts an array of bytes into this byte vector. The byte vector is
     * automatically enlarged if necessary.
     *
     * @param b an array of bytes.
     * @return this byte vector.
     */
    public ByteVector putByteArray(final byte[] b) {
        return putByteArray(b, 0, b.length);
    }

    /**
     * Puts an array of bytes into this byte vector, at the given position.
     * The byte vector is <i>not</i> enlarged if necessary.
     * @param b an array of bytes.
     * @param position position in bytes in the vector.
     * @return this byte vector.
     */
    public ByteVector putByteArray(final byte[] b, final int position) {
        if (b != null) {
            System.arraycopy(b, 0, data, position, b.length);
        }
        return this;
    }

    /**
     * Enlarge this byte vector so that it can receive n more bytes.
     *
     * @param size number of additional bytes that this byte vector should be
     *        able to receive.
     */
    private void enlarge(final int size) {
        int length1 = 2 * data.length;
        int length2 = length + size;
        byte[] newData = new byte[length1 > length2 ? length1 : length2];
        System.arraycopy(data, 0, newData, 0, length);
        data = newData;
    }

    /**
     * Adds padding at the end of the file, if necessary.
     * @param alignment alignment of the padding (between 1 and 4).
     */
    public void addPadding(int alignment) {
        int padding = length % alignment;
        switch (padding) {
        case 0:
            break;
        case 1 :
            putByte(0);
            putShort(0);
            break;
        case 2 :
            putShort(0);
            break;
        case 3 :
            putByte(0);
            break;
        }
    }

    /**
     * Adds the default padding of 4 bytes at the end of the file, if necessary.
     */
    public void addPadding() {
        addPadding(4);
    }

}
