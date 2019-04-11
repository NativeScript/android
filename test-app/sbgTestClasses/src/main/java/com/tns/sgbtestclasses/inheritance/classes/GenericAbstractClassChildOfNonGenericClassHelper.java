package com.tns.sgbtestclasses.inheritance.classes;

public class GenericAbstractClassChildOfNonGenericClassHelper {

    public void work(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz) {
        clazz.work();
    }

    public void work(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str) {
        clazz.work(str);
    }

    public void work(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str, int i) {
        clazz.work(str, i);
    }

    public String getString(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz) {
        return clazz.getString();
    }

    public String getString(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str) {
        return clazz.getString(str);
    }

    public String getString(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str, int i) {
        return clazz.getString(str, i);
    }

    public void workAbstract(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz) {
        clazz.workAbstract();
    }

    public void workAbstract(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str) {
        clazz.workAbstract(str);
    }

    public void workAbstract(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str, int i) {
        clazz.workAbstract();
    }

    public String getStringAbstract(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz) {
        return clazz.getStringAbstract();
    }

    public String getStringAbstract(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str) {
        return clazz.getStringAbstract(str);
    }

    public String getStringAbstract(GenericAbstractClassChildOfNonGenericClass<Object, Object> clazz, String str, int i) {
        return clazz.getStringAbstract(str, i);
    }
}
