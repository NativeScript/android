package org.nativescript.staticbindinggenerator.generating.writing.impl;

import org.nativescript.staticbindinggenerator.Writer;
import org.nativescript.staticbindinggenerator.generating.writing.PackageNameWriter;

public class PackageNameWriterImpl implements PackageNameWriter {

    private final Writer writer;

    public PackageNameWriterImpl(Writer writer) {
        this.writer = writer;
    }

    @Override
    public void writePackageName(String packageName) {
        writer.writeln("package " + packageName + ";");
    }
}
