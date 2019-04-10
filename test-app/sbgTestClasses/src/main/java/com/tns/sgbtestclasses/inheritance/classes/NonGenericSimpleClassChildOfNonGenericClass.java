package com.tns.sgbtestclasses.inheritance.classes;

public class NonGenericSimpleClassChildOfNonGenericClass extends NonGenericSimpleClassBase {
    public void work(){
        System.out.println("public void work() New");
    }

    public void work(String str){
        System.out.println("public void work(String str) New");
    }

    public void work(String str, int i){
        System.out.println("public void work(String str, int i) New");
    }

    public String getString(){
        System.out.println("public String getString()");
        return "string2";
    }

    public String getString(String str){
        System.out.println("public String getString(String str)");
        return str + "New";
    }

    public String getString(String str, int i){
        System.out.println("public String getString(String str, int i)");
        return str + i + "New";
    }

    public String getStringNotExtented(String str, int i){
        System.out.println("public String getString(String str, int i)");
        return str + i;
    }

}
