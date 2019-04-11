package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public abstract class NonGenericAbstractClassChildOfGenericClass extends GenericAbstractClassBase<List<Integer>, Integer> {

    private static final String CLASS_NAME = "NonGenericAbstractClassChildOfGenericClass";

    @Override
    public String workGeneric(Integer item) {
        return CLASS_NAME + item.toString();
    }

    @Override
    public String workGeneric(Integer item, List<Integer> collection) {
        return CLASS_NAME + item.toString() + collection.toString();
    }

    @Override
    public String workGeneric(Integer item, List<Integer> collection, int index) {
        return CLASS_NAME + item.toString() + collection.toString() + index;
    }

    @Override
    public Integer getGenericItem(Integer item) {
        return item;
    }

    @Override
    public List<Integer> getGenericCollection(List<Integer> collection) {
        return collection;
    }
}
