package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public class NonGenericSimpleClassChildOfGenericClassHelper {

    public String workGeneric(NonGenericSimpleClassChildOfGenericClass clazz, String item) {
        return clazz.workGeneric(item);
    }

    public String workGeneric(NonGenericSimpleClassChildOfGenericClass clazz, String item, List<String> collection) {
        return clazz.workGeneric(item, collection);
    }

    public String workGeneric(NonGenericSimpleClassChildOfGenericClass clazz, String item, List<String> collection, int index) {
        return clazz.workGeneric(item, collection, index);
    }

    public String getGenericItem(NonGenericSimpleClassChildOfGenericClass clazz, String item) {
        return clazz.getGenericItem(item);
    }

    public List<String> getGenericCollection(NonGenericSimpleClassChildOfGenericClass clazz, List<String> collection) {
        return clazz.getGenericCollection(collection);
    }
}
