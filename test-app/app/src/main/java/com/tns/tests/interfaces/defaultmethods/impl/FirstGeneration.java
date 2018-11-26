package com.tns.tests.interfaces.defaultmethods.impl;

import com.tns.tests.interfaces.defaultmethods.contract.Generation;

public class FirstGeneration implements Generation {
    
    @Override
    public String grow(){
        return "first generation grow";
    }
    
}