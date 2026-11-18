package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.boundcycles;

public interface ComparableSource<T extends Comparable<T>> {
    T next();
}
