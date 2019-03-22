package com.tns.sgbtestclasses.inheritance.classes;
import com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase;
public class NonGenericSimpleClassBaseHelper {
    public void work(NonGenericAbstractClassBase arg){
        arg.workAbstract();
    }
    public void work(NonGenericAbstractClassBase arg, String expectedMessage){
        arg.workAbstract(expectedMessage);
    }
    public void work(NonGenericAbstractClassBase arg, String expectedMessage, int number){
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
}
