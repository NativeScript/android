package com.tns.sgbtestclasses.inheritance.classes;

public class NonGenericAbstractClassChildOfNonGenericClassHelper {

    public void work(NonGenericAbstractClassChildOfNonGenericClass clazz) {
        clazz.work();
    }

    public void work(NonGenericAbstractClassChildOfNonGenericClass clazz, String str) {
        clazz.work(str);
    }

    public void work(NonGenericAbstractClassChildOfNonGenericClass clazz, String str, int i) {
        clazz.work(str, i);
    }

    public String getString(NonGenericAbstractClassChildOfNonGenericClass clazz) {
        return clazz.getString();
    }

    public String getString(NonGenericAbstractClassChildOfNonGenericClass clazz, String str) {
        return clazz.getString(str);
    }

    public String getString(NonGenericAbstractClassChildOfNonGenericClass clazz, String str, int i) {
        return clazz.getString(str, i);
    }
}
