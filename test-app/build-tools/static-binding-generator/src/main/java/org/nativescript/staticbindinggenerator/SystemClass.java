package org.nativescript.staticbindinggenerator;

import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.util.ClassPath;

import java.io.InputStream;
import java.io.IOException;

public class SystemClass {
    public static JavaClass get(String className) {
        try {
            InputStream is = ClassPath.SYSTEM_CLASS_PATH.getInputStream(className);
            ClassParser cp = new ClassParser(is, className);
            return cp.parse();
        } catch (IOException ioe) {
            throw new RuntimeException("Error while reading system class", ioe);
        }
    }
}
