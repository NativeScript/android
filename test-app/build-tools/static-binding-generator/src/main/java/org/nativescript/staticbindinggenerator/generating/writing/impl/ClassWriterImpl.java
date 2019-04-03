package org.nativescript.staticbindinggenerator.generating.writing.impl;

import org.nativescript.staticbindinggenerator.Writer;
import org.nativescript.staticbindinggenerator.generating.writing.ClassWriter;

import java.util.List;

public class ClassWriterImpl implements ClassWriter {
    private static final String EXTENDS_KEYWORD = "extends";
    private static final String IMPLEMENTS_KEYWORD = "implements";
    private static final String PUBLIC_CLASS_KEYWORD = "public class";
    private static final String JAVASCRIPT_IMPLEMENTATION_FILE_NAME_PATTERN = "@com.tns.JavaScriptImplementation(javaScriptFile = \"./%s\")";
    private static final String NATIVESCRIPT_HASHCODE_PROVIDER_INTERFACE_NAME = "com.tns.NativeScriptHashCodeProvider";
    private static final String NOT_EXTENDING_ANY_CLASS_MESSAGE = "Not extending any class!";

    private final Writer writer;

    public ClassWriterImpl(final Writer writer) {
        this.writer = writer;
    }

    private void writeClassWithName(String className) {
        writer.write(PUBLIC_CLASS_KEYWORD);
        writer.write(SPACE_LITERAL);
        writer.write(className);
    }

    @Override
    public void writeBeginningOfChildClass(String className, String extendedClassName, List<String> implementedInterfacesNames) {
        writeClassWithName(className);

        if (isEmpty(extendedClassName)) {
            throw new RuntimeException(NOT_EXTENDING_ANY_CLASS_MESSAGE);
        }

        writer.write(SPACE_LITERAL);
        writer.write(EXTENDS_KEYWORD);
        writer.write(SPACE_LITERAL);
        writer.write(extendedClassName);

        writer.write(SPACE_LITERAL);
        writer.write(IMPLEMENTS_KEYWORD);
        writer.write(SPACE_LITERAL);
        writer.write(NATIVESCRIPT_HASHCODE_PROVIDER_INTERFACE_NAME);

        if (!isEmpty(implementedInterfacesNames) && !isEmpty(implementedInterfacesNames.get(0))) {
            writer.write(COMMA_LITERAL);
            writer.write(SPACE_LITERAL);

            for (int i = 0; i < implementedInterfacesNames.size(); i += 1) {
                writer.write(implementedInterfacesNames.get(i));

                if (i != implementedInterfacesNames.size() - 1) { // don't write a comma after the last element in the collection
                    writer.write(COMMA_LITERAL);
                }

                writer.write(SPACE_LITERAL);
            }
        }

        writer.write(OPENING_CURLY_BRACKET_LITERAL);
    }

    @Override
    public void writeBeginningOfNamedChildClass(String className, String jsFileName, String extendedClassName, List<String> implementedInterfacesNames) {
        writer.writeln(String.format(JAVASCRIPT_IMPLEMENTATION_FILE_NAME_PATTERN, jsFileName));
        writeBeginningOfChildClass(className, extendedClassName, implementedInterfacesNames);
    }

    @Override
    public void writeClassEnd() {
        writer.write(CLOSING_CURLY_BRACKET_LITERAL);
    }

    private boolean isEmpty(String input) {
        return input == null || input.equals("");
    }

    private <T> boolean isEmpty(List<T> input) {
        return input == null || input.size() == 0;
    }
}
