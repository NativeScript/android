package com.tns.tests.interfaces.defaultmethods.contract;

public interface Producer{

    default String produce(){
        return "default produce in Producer";
    }
}

