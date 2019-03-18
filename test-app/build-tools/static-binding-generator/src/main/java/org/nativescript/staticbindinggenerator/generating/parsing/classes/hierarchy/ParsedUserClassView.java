package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy;

public class ParsedUserClassView {
    private String className;
    private InheritanceChain inheritanceChain;

    public String getClassName() {
        return className;
    }

    public void setClassName(String className) {
        this.className = className;
    }

    public InheritanceChain getInheritanceChain() {
        return inheritanceChain;
    }

    public void setInheritanceChain(InheritanceChain inheritanceChain) {
        this.inheritanceChain = inheritanceChain;
    }
}

