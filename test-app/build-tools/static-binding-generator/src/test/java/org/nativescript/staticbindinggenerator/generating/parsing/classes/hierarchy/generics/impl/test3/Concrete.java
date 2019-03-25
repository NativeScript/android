package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test3;

import java.util.List;

public class Concrete extends Base implements GenericInterface1<List<String>>{
    public Concrete(String arg, String arg2, Integer i){
        super(arg);
        new Concrete("");
    }

    public Concrete(String arg, Integer i){
        this(arg, arg, i);
    }
}
