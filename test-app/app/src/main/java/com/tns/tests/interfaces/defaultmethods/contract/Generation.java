package com.tns.tests.interfaces.defaultmethods.contract;

public interface Generation{
    default String grow(){
        return "default grow";
    }
}