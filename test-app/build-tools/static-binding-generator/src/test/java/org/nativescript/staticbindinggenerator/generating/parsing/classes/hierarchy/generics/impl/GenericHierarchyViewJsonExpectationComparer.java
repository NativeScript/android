package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.stream.JsonReader;

import org.junit.Assert;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;

import java.io.FileNotFoundException;
import java.io.FileReader;

public class GenericHierarchyViewJsonExpectationComparer {

    private final Gson gson;

    public GenericHierarchyViewJsonExpectationComparer() {
        this.gson = new GsonBuilder()
                .addSerializationExclusionStrategy(new JavaClassSerializationExclusionStrategy())
                .setPrettyPrinting()
                .create();
    }

    public void assertMeetsExpectation(GenericHierarchyView testedHierarchy, String expectationJsonPath){
        JsonReader reader;
        try {
            reader = new JsonReader(new FileReader(expectationJsonPath));
        } catch (FileNotFoundException e) {
            throw new RuntimeException("Expectation JSON not found!", e);
        }

        GenericHierarchyView expectedHierarchy = gson.fromJson(reader, GenericHierarchyView.class);

        Assert.assertEquals("Unexpected parent interfaces in hierarchy", expectedHierarchy.getAllImplementedInterfaces(), testedHierarchy.getAllImplementedInterfaces());
        Assert.assertEquals("Unexpected parent classes in hierarchy", expectedHierarchy.getAllParentClasses(), testedHierarchy.getAllParentClasses());
    }
}
