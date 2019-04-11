package com.tns.sgbtestclasses.inheritance.classes;

import java.util.List;

public class GenericSimpleClassChildOfGenericClassHelper {

    public String workGeneric(GenericSimpleClassChildOfGenericClass<Object> clazz, Object item) {
        return clazz.workGeneric(item);
    }

    public String workGeneric(GenericSimpleClassChildOfGenericClass<Object> clazz, Object item, List<Object> collection) {
        return clazz.workGeneric(item, collection);
    }

    public String workGeneric(GenericSimpleClassChildOfGenericClass<Object> clazz, Object item, List<Object> collection, int index) {
        return clazz.workGeneric(item, collection, index);
    }

    public Object getGenericItem(GenericSimpleClassChildOfGenericClass<Object> clazz, Object item) {
        return clazz.getGenericItem(item);
    }

    public List<Object> getGenericCollection(GenericSimpleClassChildOfGenericClass<Object> clazz, List<Object> collection) {
        return clazz.getGenericCollection(collection);
    }
}
