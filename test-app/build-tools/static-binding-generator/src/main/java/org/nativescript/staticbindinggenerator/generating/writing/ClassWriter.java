package org.nativescript.staticbindinggenerator.generating.writing;

import java.util.List;

public interface ClassWriter extends JavaCodeWriter {
    void writeBeginningOfChildClass(String className, String extendedClassName, List<String> implementedInterfacesNames);
    void writeBeginningOfNamedChildClass(String className, String jsFileName, String extendedClassName, List<String> implementedInterfacesNames);
    void writeClassEnd();
}
