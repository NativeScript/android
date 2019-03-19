package org.nativescript.staticbindinggenerator.generating.parsing.checkers.impl;

import org.nativescript.staticbindinggenerator.generating.parsing.checkers.ImplementationObjectChecker;

import java.util.List;

public class ImplementationObjectCheckerImpl implements ImplementationObjectChecker {

    private static final String INIT_METHOD_NAME = "init";

    @Override
    public boolean hasInitMethod(List<String> objectMethodsNames) {
        for (String objectMethodName : objectMethodsNames) {
            if(objectMethodName.equals(INIT_METHOD_NAME)){
                return true;
            }
        }

        return false;
    }
}
