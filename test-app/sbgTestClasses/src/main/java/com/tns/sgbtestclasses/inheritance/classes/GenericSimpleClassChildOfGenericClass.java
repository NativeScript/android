package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public class GenericSimpleClassChildOfGenericClass<T> extends GenericSimpleClassBase<T, List<T>> {

    private static final String CLASS_NAME = "GenericSimpleClassChildOfGenericClass";

    @Override
    public String workGeneric(T item){
        System.out.println("public void workGeneric(T item)");
        return CLASS_NAME + item.toString();
    }

    @Override
    public String workGeneric(T item, List<T> collection) {
        System.out.println("public void workGeneric(T item, K collection)");
        return CLASS_NAME + item.toString()+collection.get(0).toString();
    }

    @Override
    public String workGeneric(T item, List<T> collection, int index) {
        System.out.println("public void workGeneric(T item, K collection, int index)");
        return CLASS_NAME + item.toString()+collection.get(index).toString();
    }

    @Override
    public T getGenericItem(T item) {
        System.out.println("public K getGenericCollection(T item)");
        return item;
    }

    @Override
    public List<T> getGenericCollection(List<T> collection){
        System.out.println("public T getGenericItem(K collection)");
        return collection;
    }
}
