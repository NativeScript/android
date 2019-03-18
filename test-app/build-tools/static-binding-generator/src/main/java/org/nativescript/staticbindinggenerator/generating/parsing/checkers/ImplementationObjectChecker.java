package org.nativescript.staticbindinggenerator.generating.parsing.checkers;

import java.util.List;

public interface ImplementationObjectChecker {
    boolean hasInitMethod(List<String> objectMethods);
}
