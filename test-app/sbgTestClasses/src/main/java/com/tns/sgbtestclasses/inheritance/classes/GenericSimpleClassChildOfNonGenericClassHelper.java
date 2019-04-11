package com.tns.sgbtestclasses.inheritance.classes;

public class GenericSimpleClassChildOfNonGenericClassHelper {

    public void work(GenericSimpleClassChildOfNonGenericClass<Object> clazz) {
        clazz.work();
    }

    public void work(GenericSimpleClassChildOfNonGenericClass<Object> clazz, String str) {
        clazz.work(str);
    }

    public void work(GenericSimpleClassChildOfNonGenericClass<Object> clazz, String str, int i) {
        clazz.work(str, i);
    }

    public String getString(GenericSimpleClassChildOfNonGenericClass<Object> clazz) {
        return clazz.getString();
    }

    public String getString(GenericSimpleClassChildOfNonGenericClass<Object> clazz, String str) {
        return clazz.getString(str);
    }

    public String getString(GenericSimpleClassChildOfNonGenericClass<Object> clazz, String str, int i) {
        return clazz.getString(str, i);
    }
}
