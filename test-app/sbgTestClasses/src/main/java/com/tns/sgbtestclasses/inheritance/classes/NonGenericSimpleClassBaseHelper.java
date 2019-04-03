package com.tns.sgbtestclasses.inheritance.classes;
import com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase;
public class NonGenericSimpleClassBaseHelper {
    public void work(NonGenericSimpleClassBase arg){
        arg.work();
    }
    public void work(NonGenericSimpleClassBase arg, String expectedMessage){
        arg.work(expectedMessage);
    }
    public void work(NonGenericSimpleClassBase arg, String expectedMessage, int number) {
        arg.work(expectedMessage, number);
    }
    public String getString(NonGenericSimpleClassBase arg){
        return arg.getString();
    }

    public String getString(NonGenericSimpleClassBase arg, String expectedMessage){
        return arg.getString(expectedMessage);
    }
    public String getString(NonGenericSimpleClassBase arg, String expectedMessage, int number){
        return arg.getString(expectedMessage, number);
    }
}
