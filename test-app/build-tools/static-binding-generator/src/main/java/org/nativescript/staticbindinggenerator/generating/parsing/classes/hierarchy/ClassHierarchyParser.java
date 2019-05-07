package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy;

import org.apache.bcel.classfile.JavaClass;

public interface ClassHierarchyParser {

    HierarchyView getClassHierarchy(JavaClass javaClass);
}
