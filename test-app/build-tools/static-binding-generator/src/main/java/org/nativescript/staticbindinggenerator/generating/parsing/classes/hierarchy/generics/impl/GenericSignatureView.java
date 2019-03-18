package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import java.util.List;
import java.util.Map;

public class GenericSignatureView {
    private final String parsedSignature;
    private final List<String> formalTypeParameterNames;
    private final Map<String, String> formalTypeParameterBounds;
    private final ClassGenericDefinition parentClass;
    private final List<ClassGenericDefinition> parentInterfaces;

    public GenericSignatureView(String parsedSignature, List<String> formalTypeParameterNames, Map<String, String> formalTypeParameterBounds, ClassGenericDefinition parentClass, List<ClassGenericDefinition> parentInterfaces) {
        this.parsedSignature = parsedSignature;
        this.formalTypeParameterNames = formalTypeParameterNames;
        this.formalTypeParameterBounds = formalTypeParameterBounds;
        this.parentClass = parentClass;
        this.parentInterfaces = parentInterfaces;
    }

    public List<String> getFormalTypeParameterNames() {
        return formalTypeParameterNames;
    }

    public Map<String, String> getFormalTypeParameterBounds() {
        return formalTypeParameterBounds;
    }

    public ClassGenericDefinition getParentClass() {
        return parentClass;
    }

    public List<ClassGenericDefinition> getParentInterfaces() {
        return parentInterfaces;
    }

    public String getParsedSignature() {
        return parsedSignature;
    }

    public boolean hasFormalTypeParameters() {
        return formalTypeParameterNames.size() > 0;
    }

    @Override
    public String toString() {
        return "GenericSignatureView{" +
                "formalTypeParameterNames=" + listToString(formalTypeParameterNames) +
                ", parentClass='" + parentClass + '\'' +
                ", parentInterfaces=" + listToString(parentInterfaces) +
                '}';
    }

    private String listToString(List<?> list) {
        StringBuilder sb = new StringBuilder();
        int listSize = list.size();
        for (int i = 0; i < listSize; i += 1) {
            sb.append(list.get(i).toString());
            if (i < listSize - 1) {
                sb.append("; ");
            }
        }

        return sb.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        GenericSignatureView that = (GenericSignatureView) o;

        if (!formalTypeParameterNames.equals(that.formalTypeParameterNames)) return false;
        if (!parentClass.equals(that.parentClass)) return false;
        return parentInterfaces.equals(that.parentInterfaces);
    }

    @Override
    public int hashCode() {
        int result = formalTypeParameterNames.hashCode();
        result = 31 * result + parentClass.hashCode();
        result = 31 * result + parentInterfaces.hashCode();
        return result;
    }
}
