package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy;

import java.util.Map;

public class UserProvidedClassInheritanceMetadata {
    private final Map<String, InheritanceChain> inheritanceMetadata;

    public UserProvidedClassInheritanceMetadata(Map<String, InheritanceChain> inheritanceMetadata) {
        this.inheritanceMetadata = inheritanceMetadata;
    }

    public InheritanceChain getInheritanceChainForClass(String className) {
        return inheritanceMetadata.get(className);
    }
}
