package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics;

import org.apache.bcel.classfile.JavaClass;

public interface GenericsAwareClassHierarchyParser {
    GenericHierarchyView getClassHierarchy(JavaClass javaClass);
}
