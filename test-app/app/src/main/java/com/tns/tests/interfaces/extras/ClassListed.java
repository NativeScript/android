package com.tns.tests.interfaces.extras;

// A class listed in an extend's `interfaces` that implements the base's default method.
public final class ClassListed {
    private ClassListed() {
    }

    public interface Base {
        default String value() {
            return "java";
        }
    }

    public abstract static class Implementor implements Base {
        @Override
        public String value() {
            return "class";
        }
    }
}
