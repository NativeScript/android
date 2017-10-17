package com.tns.bindings;

import java.util.ArrayList;

public class AnnotationDescriptor {
    public static class Parameter {
        private final String name;
        private final Object value;

        public Parameter(String name, Object value) {
            this.name = name;
            this.value = value;
        }

        public String getName() {
            return this.name;
        }

        public Object getValue() {
            return this.value;
        }
    }

    public AnnotationDescriptor(String classname, Parameter[] parameters, boolean isRuntimeVisible) {
        this.classname = classname;
        this.parameters = parameters;
        this.isRuntimeVisible = isRuntimeVisible;
    }

    public String getAnnotationClassname() {
        return classname;
    }

    public boolean isRuntimeVisible() {
        return isRuntimeVisible;
    }

    public Parameter[] getParams() {
        return parameters;
    }

    private final String classname;
    private final Parameter[] parameters;
    private final boolean isRuntimeVisible;
}