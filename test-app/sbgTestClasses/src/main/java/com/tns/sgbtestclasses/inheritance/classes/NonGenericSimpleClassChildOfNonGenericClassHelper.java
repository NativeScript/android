package com.tns.sgbtestclasses.inheritance.classes;
import com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassChildOfNonGenericClass;
public class NonGenericSimpleClassChildOfNonGenericClassHelper {
    public void work(NonGenericSimpleClassChildOfNonGenericClass arg){
        arg.work();
    }
    public void work(NonGenericSimpleClassChildOfNonGenericClass arg, String expectedMessage){
        arg.work(expectedMessage);
    }
    public void work(NonGenericSimpleClassChildOfNonGenericClass arg, String expectedMessage, int number) {
        arg.work(expectedMessage, number);
    }
    public String getString(NonGenericSimpleClassChildOfNonGenericClass arg){
        return arg.getString();
    }

    public String getString(NonGenericSimpleClassChildOfNonGenericClass arg, String expectedMessage){
        return arg.getString(expectedMessage);
    }
    public String getString(NonGenericSimpleClassChildOfNonGenericClass arg, String expectedMessage, int number){
        return arg.getString(expectedMessage, number);
    }
    public String getStringNotExtented(NonGenericSimpleClassChildOfNonGenericClass arg, String expectedMessage, int number){
        return arg.getStringNotExtented(expectedMessage, number);
    }
}
