package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics;

import java.util.Map;

public class GenericHierarchyView {
    private final String initialClassName;
    private final GenericParameters initialClassGenericParameters;
    private final Map<String, GenericParameters> allImplementedInterfaces;
    private final Map<String, GenericParameters> allParentClasses;

    public GenericHierarchyView(Map<String, GenericParameters> allImplementedInterfaces, Map<String, GenericParameters> allParentClasses, String initialClassName, GenericParameters initialClassGenericParameters) {
        this.allImplementedInterfaces = allImplementedInterfaces;
        this.allParentClasses = allParentClasses;
        this.initialClassName = initialClassName;
        this.initialClassGenericParameters = initialClassGenericParameters;
    }

    public Map<String, GenericParameters> getAllImplementedInterfaces() {
        return allImplementedInterfaces;
    }

    public Map<String, GenericParameters> getAllParentClasses() {
        return allParentClasses;
    }


    public String getInitialClassName() {
        return initialClassName;
    }

    public GenericParameters getInitialClassGenericParameters() {
        return initialClassGenericParameters;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        GenericHierarchyView that = (GenericHierarchyView) o;

        if (!allImplementedInterfaces.equals(that.allImplementedInterfaces)) return false;
        return allParentClasses.equals(that.allParentClasses);
    }

    @Override
    public int hashCode() {
        int result = allImplementedInterfaces.hashCode();
        result = 31 * result + allParentClasses.hashCode();
        return result;
    }
}
