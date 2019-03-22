package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public abstract class GenericAbstractClassBase<K extends List<T>, T> {

    public abstract void workGeneric(T item);

    public abstract void workGeneric(T item, K collection);

    public abstract void workGeneric(T item, K collection, int index);

    public abstract K getGenericCollection(T item);

    public abstract T getGenericItem(K collection);

}
