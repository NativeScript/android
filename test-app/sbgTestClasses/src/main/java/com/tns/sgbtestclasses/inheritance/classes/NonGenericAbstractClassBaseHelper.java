package com.tns.sgbtestclasses.inheritance.classes;

import com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase;

public class NonGenericAbstractClassBaseHelper {

    public void workAbstract(NonGenericAbstractClassBase arg){
        arg.workAbstract();
    }
    public void workAbstract(NonGenericAbstractClassBase arg, String expectedMessage){
        arg.workAbstract(expectedMessage);
    }
    public void workAbstract(NonGenericAbstractClassBase arg, String expectedMessage, int number){
        arg.workAbstract(expectedMessage, number);
    }
    public void getStringAbstract(NonGenericAbstractClassBase arg){
        arg.getStringAbstract();
    }
    public void getStringAbstract(NonGenericAbstractClassBase arg, String expectedMessage){
        arg.getStringAbstract(expectedMessage);
    }
    public void getStringAbstract(NonGenericAbstractClassBase arg, String expectedMessage, int number){
        arg.getStringAbstract(expectedMessage, number);
    }

    public String getString(NonGenericAbstractClassBase arg){
        return arg.getString();
    }
    public String getString(NonGenericAbstractClassBase arg, String expectedMessage){
        return arg.getString(expectedMessage);
    }
    public String getString(NonGenericAbstractClassBase arg, String expectedMessage, int number){
        return arg.getString(expectedMessage, number);
    }
}
