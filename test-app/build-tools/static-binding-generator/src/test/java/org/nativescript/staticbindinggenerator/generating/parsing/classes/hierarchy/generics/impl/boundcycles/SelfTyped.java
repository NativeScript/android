package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.boundcycles;

public abstract class SelfTyped<T extends SelfTyped<T>> {
    public abstract T self();
}
