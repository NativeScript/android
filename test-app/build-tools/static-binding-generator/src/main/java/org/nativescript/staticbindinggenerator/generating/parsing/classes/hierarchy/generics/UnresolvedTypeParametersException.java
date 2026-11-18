package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics;

/**
 * Thrown for type parameters whose bounds refer to themselves or to each other, such as
 * {@code T extends Comparable<T>}. No type argument can be derived from such a bound:
 * its erasure ({@code Comparable}) is not within it, so javac rejects a class using it.
 */
public class UnresolvedTypeParametersException extends RuntimeException {
    private static final long serialVersionUID = 1L;

    public UnresolvedTypeParametersException(String message) {
        super(message);
    }
}
