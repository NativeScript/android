package org.nativescript.staticbindinggenerator.files.impl;

import org.apache.bcel.classfile.JavaClass;

import java.util.Map;
import java.util.Set;

public class ClassesCollection {
    private final Map<String, JavaClass> regularClasses;
    private final Set<String> nonPublicNestedClasses;

    public ClassesCollection(Map<String, JavaClass> regularClasses, Set<String> nonPublicNestedClasses) {
        this.regularClasses = regularClasses;
        this.nonPublicNestedClasses = nonPublicNestedClasses;
    }

    public Map<String, JavaClass> getRegularClasses() {
        return regularClasses;
    }

    public Set<String> getNonPublicNestedClasses() {
        return nonPublicNestedClasses;
    }
}
