package org.nativescript.staticbindinggenerator.generating.writing.impl;

import org.nativescript.staticbindinggenerator.Writer;
import org.nativescript.staticbindinggenerator.generating.writing.ImportsWriter;

public class ImportsWriterImpl implements ImportsWriter {

    private final Writer writer;

    public ImportsWriterImpl(final Writer writer) {
        this.writer = writer;
    }

    @Override
    public void writeRuntimeImport() {
        writer.writeln("import com.tns.Runtime;");
    }

    @Override
    public void writeRuntimeHelperImport() {
        writer.writeln("import com.tns.RuntimeHelper;");
    }
}
