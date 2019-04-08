package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public class GenericSimpleClassBase<T, K extends List<T>> {

    public String workGeneric(T item){
        System.out.println("public void workGeneric(T item)");
        return item.toString();
    }

    public String workGeneric(T item, K collection) {
        System.out.println("public void workGeneric(T item, K collection)");
        return item.toString()+collection.get(0).toString();
    }

    public String workGeneric(T item, K collection, int index) {
        System.out.println("public void workGeneric(T item, K collection, int index)");
        return item.toString()+collection.get(index).toString();
    }

    public T getGenericItem(T item) {
        System.out.println("public K getGenericCollection(T item)");
        return item;
    }

    public K getGenericCollection(K collection){
        System.out.println("public T getGenericItem(K collection)");
        return collection;
    }
}
