package com.telerik.metadata;

import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class FileStreamWriter implements StreamWriter {
    // The metadata is written a few bytes at a time, hundreds of thousands of times.
    private static final int BUFFER_SIZE = 1 << 16;

    private final OutputStream out;

    // Counted here, since the file's own position lags behind what is still buffered.
    // The stream is expected to be at the start of an empty file.
    private int position;

    public FileStreamWriter(FileOutputStream out) {
        this.out = new BufferedOutputStream(out, BUFFER_SIZE);
    }

    @Override
    public int getPosition() throws IOException {
        return position;
    }

    @Override
    public void write(byte[] buffer) throws IOException {
        out.write(buffer);
        position += buffer.length;
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
        position++;
    }
}
