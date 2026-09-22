package org.nativescript.staticbindinggenerator.generating.writing.impl;

import org.nativescript.staticbindinggenerator.Writer;
import org.nativescript.staticbindinggenerator.generating.writing.FieldsWriter;

public class FieldsWriterImpl implements FieldsWriter {


    private static final String THIZ_KEYWORD = "thiz";
    private static final String RUNTIME_ID_FIELD_DECLARATION = "private int runtimeId__ns = com.tns.NativeScriptRuntimeBound.INVALID_RUNTIME_ID;";

    private final Writer writer;

    public FieldsWriterImpl(final Writer writer) {
        this.writer = writer;
    }

    @Override
    public void writeRuntimeIdField() {
        writer.write(TABULATION_LITERAL);
        writer.writeln(RUNTIME_ID_FIELD_DECLARATION);
        writer.writeln();
    }

    @Override
    public void writeStaticThizField(String className) {
        writer.write(TABULATION_LITERAL);
        writer.write(PRIVATE_MODIFIER);
        writer.write(SPACE_LITERAL);
        writer.write(STATIC_MODIFIER);
        writer.write(SPACE_LITERAL);
        writer.write(className);
        writer.write(SPACE_LITERAL);
        writer.write(THIZ_KEYWORD);
        writer.writeln(END_OF_STATEMENT_LITERAL);
        writer.writeln();
    }
}
