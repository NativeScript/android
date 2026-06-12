package com.tns.tests;

import java.nio.ByteBuffer;

/*
 * Used by shared-array-buffer-test.js to verify that JS (Shared)ArrayBuffers
 * marshal to Java as direct ByteBuffers over the same memory.
 */
public class ByteBufferHolder {
    private ByteBuffer buffer;

    public ByteBuffer hold(ByteBuffer buffer) {
        this.buffer = buffer;
        return this.buffer;
    }

    public byte get(int index) {
        return buffer.get(index);
    }

    public void put(int index, byte value) {
        buffer.put(index, value);
    }
}
