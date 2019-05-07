package org.nativescript.staticbindinggenerator.generating.writing;

public interface ImportsWriter extends JavaCodeWriter {
    void writeRuntimeImport();
    void writeRuntimeHelperImport();
}
