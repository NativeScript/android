package org.nativescript.staticbindinggenerator.generating.parsing.methods;

import java.util.List;

public interface InheritedMethodsCollector {

    InheritedMethodsView collect();
    List<ReifiedJavaMethod> collectConstructors();
}
