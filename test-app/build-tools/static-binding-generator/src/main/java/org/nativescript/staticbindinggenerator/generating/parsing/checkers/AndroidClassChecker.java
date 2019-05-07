package org.nativescript.staticbindinggenerator.generating.parsing.checkers;

import org.apache.bcel.classfile.JavaClass;

public interface AndroidClassChecker {
    boolean isActivityClass(JavaClass clazz);
    boolean isApplicationClass(JavaClass clazz);
    boolean isServiceClass(JavaClass javaClass);
}
