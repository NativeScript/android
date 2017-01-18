package com.telerik.metadata;

import java.io.FileOutputStream;
import java.io.IOException;

public class FileStreamWriter implements StreamWriter {
    private final FileOutputStream out;

    public FileStreamWriter(FileOutputStream out) {
        this.out = out;
    }

    @Override
    public int getPosition() throws IOException {
        return (int) out.getChannel().position();
    }

    @Override
    public void write(byte[] buffer) throws IOException {
        out.write(buffer);
    }

    @Override
    public void flush() throws IOException {
        out.flush();
    }

    @Override
    public void close() throws IOException {
        out.close();
    }

    @Override
    public void write(byte b) throws IOException {
        out.write(b);
    }
}
