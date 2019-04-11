package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public class NonGenericAbstractClassChildOfGenericClassHelper {

    public String workGeneric(NonGenericAbstractClassChildOfGenericClass clazz, Integer item) {
        return clazz.workGeneric(item);
    }

    public String workGeneric(NonGenericAbstractClassChildOfGenericClass clazz, Integer item, List<Integer> collection) {
        return clazz.workGeneric(item, collection);
    }

    public String workGeneric(NonGenericAbstractClassChildOfGenericClass clazz, Integer item, List<Integer> collection, int index) {
        return clazz.workGeneric(item, collection, index);
    }

    public Integer getGenericItem(NonGenericAbstractClassChildOfGenericClass clazz, Integer item) {
        return clazz.getGenericItem(item);
    }

    public List<Integer> getGenericCollection(NonGenericAbstractClassChildOfGenericClass clazz, List<Integer> collection) {
        return clazz.getGenericCollection(collection);
    }
}
