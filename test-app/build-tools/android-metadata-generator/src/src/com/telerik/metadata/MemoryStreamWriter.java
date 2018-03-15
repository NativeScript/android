package com.telerik.metadata;

import java.io.IOException;

public class MemoryStreamWriter implements StreamWriter {
    public MemoryStreamWriter() {
    }

    @Override
    public int getPosition() throws IOException {
        return 0;
    }

    @Override
    public void write(byte[] buffer) throws IOException {
    }

    @Override
    public void flush() throws IOException {
    }

    @Override
    public void close() throws IOException {
    }

    @Override
    public void write(byte b) throws IOException {
    }

}
