package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import com.google.gson.ExclusionStrategy;
import com.google.gson.FieldAttributes;

import org.apache.bcel.classfile.JavaClass;

public class JavaClassSerializationExclusionStrategy implements ExclusionStrategy {

    public boolean shouldSkipClass(Class<?> arg0) {
        return false;
    }

    public boolean shouldSkipField(FieldAttributes f) {
        boolean res = f.getDeclaredClass() == JavaClass.class;
        return res;
    }


}
