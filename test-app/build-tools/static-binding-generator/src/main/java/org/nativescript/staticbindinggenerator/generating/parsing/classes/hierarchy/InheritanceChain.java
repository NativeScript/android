package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy;

import java.util.List;

public class InheritanceChain {
    private String extendedClass;
    private List<String> implementedInterfaces;

    public String getExtendedClass() {
        return extendedClass;
    }

    public void setExtendedClass(String extendedClass) {
        this.extendedClass = extendedClass;
    }

    public List<String> getImplementedInterfaces() {
        return implementedInterfaces;
    }

    public void setImplementedInterfaces(List<String> implementedInterfaces) {
        this.implementedInterfaces = implementedInterfaces;
    }
}
