package org.nativescript.staticbindinggenerator.generating.parsing.methods;

import java.util.List;

public interface ReifiedJavaMethod extends JavaMethod {
    List<String> getReifiedArguments();
    String getReifiedReturnType();
}
