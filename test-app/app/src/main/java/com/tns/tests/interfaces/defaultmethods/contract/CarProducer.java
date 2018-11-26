package com.tns.tests.interfaces.defaultmethods.contract;

public interface CarProducer extends Producer{

    @Override
    default String produce(){
        return "default produce in CarProducer";
    }
}

