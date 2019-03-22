package com.tns.sgbtestclasses.inheritance.classes;
import com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase;
public class NonGenericSimpleClassBaseHelper {
    public void work(){
        System.out.println("public void work()");
    }

    public void work(String str){
        System.out.println("public void work(String str)");
    }

    public void work(String str, int i){
        System.out.println("public void work(String str, int i)");
    }

    public String getString(){
        System.out.println("public String getString()");
        return "string";
    }

    public String getString(String str){
        System.out.println("public String getString(String str)");
        return str;
    }

    public String getString(String str, int i){
        System.out.println("public String getString(String str, int i)");
        return str + i;
    }
}
