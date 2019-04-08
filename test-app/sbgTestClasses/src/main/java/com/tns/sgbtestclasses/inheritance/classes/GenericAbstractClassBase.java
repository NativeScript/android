package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public abstract class GenericAbstractClassBase<K extends List<T>, T> {

    public abstract String workGeneric(T item);

    public abstract String workGeneric(T item, K collection);

    public abstract String workGeneric(T item, K collection, int index);

    public abstract T getGenericItem(T item);

    public abstract K getGenericCollection(K collection);

}
