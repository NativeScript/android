package com.telerik.metadata;

import java.io.IOException;

public interface StreamWriter {
    int getPosition() throws IOException;

    void write(byte b) throws IOException;

    void write(byte[] buffer) throws IOException;

    void flush() throws IOException;

    void close() throws IOException;
}
