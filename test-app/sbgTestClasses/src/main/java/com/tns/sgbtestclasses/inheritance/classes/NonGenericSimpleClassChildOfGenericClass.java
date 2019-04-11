package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public class NonGenericSimpleClassChildOfGenericClass extends GenericSimpleClassBase<String, List<String>> {

    private static final String CLASS_NAME = "NonGenericSimpleClassChildOfGenericClass";

    @Override
    public String workGeneric(String item) {
        System.out.println("public void workGeneric(T item)");
        return CLASS_NAME + item;
    }

    @Override
    public String workGeneric(String item, List<String> collection) {
        return CLASS_NAME + item + collection.get(0);
    }

    @Override
    public String workGeneric(String item, List<String> collection, int index) {
        return CLASS_NAME + item + collection.get(index);
    }

    @Override
    public String getGenericItem(String item) {
        return CLASS_NAME + item;
    }

    @Override
    public List<String> getGenericCollection(List<String> collection) {
        return collection;
    }
}
