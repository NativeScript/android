package com.tns.sgbtestclasses.inheritance.classes;

public class NonGenericSimpleClassChildOfNonGenericClass extends NonGenericSimpleClassBase {

    @Override
    public void work(){
        System.out.println("public void work() New");
    }

    @Override
    public void work(String str){
        System.out.println("public void work(String str) New");
    }

    @Override
    public void work(String str, int i){
        System.out.println("public void work(String str, int i) New");
    }

    @Override
    public String getString(){
        System.out.println("public String getString()");
        return "string2";
    }

    @Override
    public String getString(String str){
        System.out.println("public String getString(String str)");
        return str + "New";
    }

    @Override
    public String getString(String str, int i){
        System.out.println("public String getString(String str, int i)");
        return str + i + "New";
    }

    public String getStringNotExtented(String str, int i){
        System.out.println("public String getString(String str, int i)");
        return str + i;
    }

}
