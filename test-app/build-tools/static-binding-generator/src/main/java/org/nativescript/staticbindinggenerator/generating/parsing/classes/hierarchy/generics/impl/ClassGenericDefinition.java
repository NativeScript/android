package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import java.util.ArrayList;
import java.util.List;

class ClassGenericDefinition {
    private final String className;
    private final List<String> genericArguments;

    ClassGenericDefinition(String className) {
        this.className = className;
        this.genericArguments = new ArrayList<>();
    }

    void addGenericArgument(String genericArgument) {
        genericArguments.add(genericArgument);
    }

    String getClassName() {
        return className;
    }

    List<String> getGenericArguments() {
        return genericArguments;
    }

    @Override
    public String toString() {
        return "GenericDefinition{" +
                "className='" + className + '\'' +
                ", genericArguments=" + genericArguments +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        ClassGenericDefinition that = (ClassGenericDefinition) o;

        if (!className.equals(that.className)) return false;
        return genericArguments.equals(that.genericArguments);
    }

    @Override
    public int hashCode() {
        int result = className.hashCode();
        result = 31 * result + genericArguments.hashCode();
        return result;
    }
}
