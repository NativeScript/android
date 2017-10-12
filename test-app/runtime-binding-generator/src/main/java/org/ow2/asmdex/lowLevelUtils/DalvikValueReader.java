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

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;


/**
 * Reads the different kinds of primitive Dalvik values from an encapsulated stream.
 *
 * @author Pierre Crégut
 * @author Julien névo (slight modifications)
 */
public class DalvikValueReader implements IDalvikValueReader {

    private byte[] contents;
    /**
     * Position in stream
     */
    protected int pos = 0;

    /**
     * Constructor encapsulating an array of bytes.
     * @param contents
     */
    public DalvikValueReader(byte[] contents) {
        this.contents = contents;
    }

    /**
     * Returns the byte array included to this reader. It is <i>not</i> copied.
     * @return the byte array included to this reader.
     */
    public byte[] getContents() {
        return contents;
    }

    /**
     * Constructor encapsulating an input stream. We need to find the size of the file
     * This is given by the offset. We first read up to the size, and then fill the byte
     * array. The size in the byte array is NOT correct.
     * @param dexBytes the bytes containing the resource/dex
     * @param size_offset offset from the begining to find the total file size
     * @throws IOException
     */
    public DalvikValueReader(byte[] dexBytes, int size_offset) throws IOException {
        InputStream stream = new ByteArrayInputStream(dexBytes);
        byte[] header = new byte[size_offset];
        if (stream.read(header) != size_offset) {
            throw new IOException("Cannot skip Resource header");
        }
        int file_size = sint(stream);
        byte [] file = new byte [file_size];
        int to_skip = size_offset + 4;
        while (file_size - to_skip != 0) {
            int read = stream.read(file, to_skip, file_size - to_skip);
            if (read == 0) {
                throw new IOException("Truncated Resource file " + read + "/" + file_size + "/" + to_skip);
            }
            to_skip += read;
        }
        System.arraycopy(header, 0, file, 0, size_offset);
        this.contents = file;
    }

    /**
     * Reads an integer directly from an input stream. Usually used
     * while builder the value reader.
     * @param stream
     * @return the value read
     * @throws IOException
     */
    final public static int sint(InputStream stream) throws IOException {
        byte [] contents = new byte [4];
        if (stream.read(contents) != 4) {
            throw new RuntimeException("Cannot read integer");
        }
        return ((contents [0] & 0xFF) | ((contents[1] & 0xFF) << 8) | ((contents[2] & 0xFF) << 16) | ((contents[3] & 0xFF) << 24));
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sbyte()
     */
    final public byte sbyte()  {
        return contents[pos++];
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#ubyte()
     */
    final public short ubyte()  {
        return (short) (contents[pos++] & 0xff);
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sshort()
     */
    final public short sshort() {
        short v = (short) ((contents [pos] & 0xff) | ((contents[pos + 1] & 0xff) << 8));
        pos += 2;
        return v;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#ushort()
     */
    final public int ushort() {
        return ((contents [pos++] & 0xff) | ((contents[pos++] & 0xff) << 8));
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sint()
     */
    final public int sint() {
        int v = ((contents [pos] & 0xff)| ((contents[pos + 1] & 0xff) << 8) | ((contents[pos + 2] & 0xff) << 16) | ((contents[pos + 3] & 0xff) << 24));
        pos += 4;
        return v;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uint()
     */
    final public int uint() {
        // WARNING : int returned whereas a long *should* be used (but slower !). For now, it works fine.
        int v = ((contents [pos] & 0xff) | ((contents[pos + 1] & 0xff) << 8) | ((contents[pos + 2] & 0xff) << 16)) | ((contents[pos + 3] & 0xff) << 24);
        pos += 4;
        return v;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sleb128()
     */
    final public int sleb128() {
        int r, v;
        v = contents[pos++] & 0xff;
        r = v & 0x7f;
        if (v >= 0x80) {
            v = contents[pos++] & 0xff;
            r |= (v & 0x7f) << 7;
            if (v >= 0x80) {
                v = contents[pos++] & 0xff;
                r |= (v & 0x7f) << 14;
                if (v >= 0x80) {
                    v = contents[pos++] & 0xff;
                    r |= (v & 0x7f) << 21;
                    if (v >= 0x80) {
                        v = contents[pos++] & 0xff;
                        r |= (v & 0x7f) << 28;
                        if (v >= 0x80) {
                            throw new RuntimeException("Bad sleb128");
                        }
                    } else if ((v & 0x40) != 0) {
                        r |= 0xf0000000;
                    }
                } else if ((v & 0x40) != 0) {
                    r |= 0xffe00000;
                }
            } else if ((v & 0x40) != 0) {
                r |= 0xffffc000;
            }
        } else if ((v & 0x40) != 0) {
            r |= 0xffffff80;
        }
        return r;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uleb128()
     */
    //final public long uleb128(){
    final public int uleb128() {
        // WARNING : int returned whereas a long *should* be used (but slower !). For now, no problem...
        int r;

        int v;
        v = contents[pos++] & 0xff;
        r = v & 0x7f;
        if (v >= 0x80) {
            v = contents[pos++] & 0xff;
            r |= (v & 0x7f) << 7;
            if (v >= 0x80) {
                v = contents[pos++] & 0xff;
                r |= (v & 0x7f) << 14;
                if (v >= 0x80) {
                    v = contents[pos++] & 0xff;
                    r |= (v & 0x7f) << 21;
                    if (v >= 0x80) {
                        v = contents[pos++] & 0xff;
                        r |= (v & 0x7f) << 28;
                        if (v >= 0x80) {
                            throw new RuntimeException("Bad uleb128");
                        }
                    }
                }
            }
        }
        return r;

    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uleb128_p1()
     */
    //final public long uleb128_p1() {
    final public int uleb128_p1() {
        return uleb128() - 1;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#uleb128_16()
     */
    final public long uleb128_16() {
        long r;
        int v;
        v = ushort();
        r = v & 0x7fff;
        if (v > 0x8000) {
            v = ushort();
            r |= (v & 0x7fff) << 15;
        }
        return r;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#sizedLong(int)
     */
    final public long sizedLong(int sz) {
        long result = 0;
        int length = sz + 1;
        for (int i=0; i < length; i++) {
            short v = ubyte();
            result = result | (long)v << (8*i); // v must be cast, else we may lose data !
        }

        return result;
    }

    /**
     * Extends a long read with SizedLong of length sz according to its sign.
     * @param l
     * @param sz size-1 of the encoded number.
     * @return the value read
     */
    final public long completeSignSizedLong(long l, int sz) {
        sz++;
        int shift = (8 - sz) * 8;
        return l << shift >> shift; // Generates the sign.
    }

    /**
     * Reads a given number of bytes and fills the given array.
     * @param b the byte array to be filled.
     */
    public void bytes(byte [] b) {
        for (int i=0; i < b.length; i++) {
            b[i] = contents[pos+i];
        }
        pos += b.length;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#utf8String()
     */
    public String utf8String() {
        StringBuilder buf = new StringBuilder();
        int c;
        int v;
        while ( (c = (contents[pos++] & 0xff)) != 0) {
            if ((c & 0x80) == 0x80) {
                if ((c & 0xe0) == 0xc0) {
                    c &= 0x1f;
                    v = contents[pos++] & 0x3f;
                    c =  c << 6 | v;
                } else if ((c & 0xf0) == 0xe0) {
                    v = contents[pos++] & 0x3f;
                    c = c << 6 | v;
                    v = contents[pos++] & 0x3f;
                    c = c << 6 | v;
                } else {
                    System.out.println("Bad (point 4) UTF 8 " + Integer.toBinaryString(c));
                }
            }
            buf.append((char) c);
        }
        return buf.toString();
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#seek(int)
     */
    final public void seek(int pos) {
        this.pos = pos;
    }

    public void relativeSeek(int offset) {
        pos += offset;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#getPos()
     */
    final public int getPos() {
        return pos;
    }

    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#unicodeString(int)
     */
    public String unicodeString(int strSize) {
        char [] content = new char [strSize];
        for (int i=0; i < strSize; i++) {
            content[i] = (char) ushort();
        }
        int c;
        if ((c = ushort()) != 0) {
            System.out.println("Did not find the ending character\n " + Arrays.toString(content) + " " + c );
        }
        return new String(content);
    }


    /* (non-Javadoc)
     * @see org.ow2.asmdex.IDalvikValueReader#hasMore()
     */
    final public boolean hasMore() {
        return pos < contents.length;
    }

    // For debug only - not exposed.
    int peek(int i) {
        return ((int) contents[i]) & 0xff;
    }

    @Override
    final public void skipInt() {
        pos += 4;
    }

    @Override
    final public void skipShort() {
        pos += 2;
    }

    @Override
    final public void skipByte() {
        pos += 1;
    }
}
