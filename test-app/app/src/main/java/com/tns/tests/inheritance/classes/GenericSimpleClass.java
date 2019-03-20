package com.tns.tests.inheritance.classes;

import java.util.List;

public class GenericSimpleClass<T, K extends List<T>> {

    public void workGeneric(T item){
        System.out.println("public void workGeneric(T item)");
    }

    public void workGeneric(T item, K collection) {
        System.out.println("public void workGeneric(T item, K collection)");
    }

    public void workGeneric(T item, K collection, int index) {
        System.out.println("public void workGeneric(T item, K collection, int index)");
    }

    public K getGenericCollection(T item) {
        System.out.println("public K getGenericCollection(T item)");
        return null;
    }

    public T getGenericItem(K collection){
        System.out.println("public T getGenericItem(K collection)");
        return null;
    }
}
