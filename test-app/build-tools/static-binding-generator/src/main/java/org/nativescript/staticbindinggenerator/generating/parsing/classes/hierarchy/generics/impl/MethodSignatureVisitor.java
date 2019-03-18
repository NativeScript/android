package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.signature.SignatureVisitor;

import java.util.HashMap;
import java.util.Map;

public class MethodSignatureVisitor extends SignatureVisitor {

    private boolean isParsingReturnType;
    private int parametersCounter;
    private final Map<Integer, String> parametersMap;
    private String returnType;

    public MethodSignatureVisitor() {
        super(Opcodes.ASM7);
        this.parametersMap = new HashMap<>();
        this.parametersCounter = 0;
    }

    @Override
    public SignatureVisitor visitReturnType() {
        isParsingReturnType = true;
        return this;
    }

    @Override
    public void visitTypeVariable(final String name) {
        if (!isParsingReturnType) {
            parametersMap.put(parametersCounter, name);
            parametersCounter += 1;
        } else {
            returnType = name;
        }
    }

    @Override
    public void visitClassType(final String name) {
        parametersCounter += 1;
    }

    @Override
    public void visitBaseType(final char descriptor) {
        parametersCounter += 1;
    }

    public Map<Integer, String> getParametersMap(){
        return parametersMap;
    }

}
