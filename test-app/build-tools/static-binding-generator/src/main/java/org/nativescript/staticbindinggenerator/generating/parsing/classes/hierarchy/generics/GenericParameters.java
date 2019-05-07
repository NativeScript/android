package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics;

import java.util.HashMap;
import java.util.Map;

public class GenericParameters {
    private final Map<String, String> genericParameters;

    public GenericParameters(Map<String, String> genericParameters) {
        this.genericParameters = genericParameters;
    }

    public GenericParameters() {
        this(new HashMap<>());
    }

    public Map<String, String> getGenericParameters() {
        return genericParameters;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        GenericParameters that = (GenericParameters) o;

        return genericParameters.equals(that.genericParameters);
    }

    @Override
    public int hashCode() {
        return genericParameters.hashCode();
    }
}
