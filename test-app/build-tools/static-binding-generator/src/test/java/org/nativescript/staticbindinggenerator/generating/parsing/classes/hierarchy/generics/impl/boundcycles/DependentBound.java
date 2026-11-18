package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.boundcycles;

import java.util.List;

public abstract class DependentBound<T, U extends List<T>> {
    public abstract U all();
}
