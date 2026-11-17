package com.tns.tests.interfaces.extras;

// Unrelated interfaces declaring the same method with different return types: a class
// implementing two of them is called through a different method descriptor for each.
public final class Covariant {
    private Covariant() {
    }

    public interface NumberBase {
        default Number value() {
            return 1;
        }
    }

    public interface NamedNumberBase {
        default Number value() {
            return 1;
        }
    }

    public interface IntegerExtra {
        default Integer value() {
            return 2;
        }
    }

    public interface AbstractNumberBase {
        Number amount();
    }

    public interface AbstractIntegerExtra {
        Integer amount();
    }
}
