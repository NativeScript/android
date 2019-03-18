package org.nativescript.staticbindinggenerator.generating.writing;

import java.util.List;

public interface ClassWriter extends JavaCodeWriter {
    void writeBeginningOfClassImplementingSingleInterface(String className, String implementedInterfaceName);
    void writeBeginningOfChildClass(String className, String extendedClassName, List<String> implementedInterfacesNames);
    void writeBeginningOfNamedClassImplementingSingleInterface(String className, String jsFileName, String implementedInterfaceName);
    void writeBeginningOfNamedChildClass(String className, String jsFileName, String extendedClassName, List<String> implementedInterfacesNames);
    void writeClassEnd();
}
