package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public abstract class GenericAbstractClassChildOfGenericClass<K> extends GenericAbstractClassBase<List<K>, K> {

    private static final String CLASS_NAME = "GenericAbstractClassChildOfGenericClass";

    @Override
    public String workGeneric(K item) {
        return CLASS_NAME + item.toString();
    }

    @Override
    public String workGeneric(K item, List<K> collection) {
        return CLASS_NAME + item.toString() + collection.toString();
    }

    @Override
    public String workGeneric(K item, List<K> collection, int index) {
        return CLASS_NAME + item.toString() + collection.toString() + index;
    }

    @Override
    public K getGenericItem(K item) {
        return item;
    }

    @Override
    public List<K> getGenericCollection(List<K> collection) {
        return collection;
    }

}
