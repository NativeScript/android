package org.nativescript.staticbindinggenerator.generating.parsing.methods;

import java.util.List;

public class InheritedMethodsView {

    private final List<ReifiedJavaMethod> overridableImplementedMethods;
    private final List<ReifiedJavaMethod> nonImplementedMethods;

    public InheritedMethodsView(List<ReifiedJavaMethod> overridableImplementedMethods, List<ReifiedJavaMethod> nonImplementedMethods) {
        this.overridableImplementedMethods = overridableImplementedMethods;
        this.nonImplementedMethods = nonImplementedMethods;
    }

    public List<ReifiedJavaMethod> getOverridableImplementedMethods() {
        return overridableImplementedMethods;
    }

    public List<ReifiedJavaMethod> getNonImplementedMethods() {
        return nonImplementedMethods;
    }

    @Override
    public String toString() {
        return "InheritedMethodsView{" +
                "overridableImplementedMethods=" + methodsCollectionToString(overridableImplementedMethods) +
                ", nonImplementedMethods=" + methodsCollectionToString(nonImplementedMethods) +
                '}';
    }

    private String methodsCollectionToString(List<ReifiedJavaMethod> methods) {
        StringBuilder sb = new StringBuilder();
        for (ReifiedJavaMethod method : methods) {
            sb.append(method.toString());
            sb.append(System.lineSeparator());
        }

        return sb.toString();
    }
}
