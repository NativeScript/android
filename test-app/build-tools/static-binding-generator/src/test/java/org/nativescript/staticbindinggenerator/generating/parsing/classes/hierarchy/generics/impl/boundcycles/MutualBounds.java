package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.boundcycles;

import java.util.List;

public abstract class MutualBounds<A extends List<B>, B extends List<A>> {
    public abstract A first();
}
