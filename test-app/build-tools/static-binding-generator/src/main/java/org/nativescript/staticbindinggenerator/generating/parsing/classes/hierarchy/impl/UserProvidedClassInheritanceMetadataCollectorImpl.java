package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.impl;

import com.google.gson.Gson;

import org.apache.commons.io.FileUtils;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.InheritanceChain;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.ParsedUserClassView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.ParsedUserClassViews;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.UserProvidedClassInheritanceMetadata;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.UserProvidedClassInheritanceMetadataCollector;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Map;

public class UserProvidedClassInheritanceMetadataCollectorImpl implements UserProvidedClassInheritanceMetadataCollector {

    private final String userProvidedMetadataFilePath;

    public UserProvidedClassInheritanceMetadataCollectorImpl(String userProvidedMetadataFilePath) {
        this.userProvidedMetadataFilePath = userProvidedMetadataFilePath;
    }

    @Override
    public UserProvidedClassInheritanceMetadata collect() {
        String userProvidedMetadataFileContent;

        try {
            userProvidedMetadataFileContent = FileUtils.readFileToString(new File(userProvidedMetadataFilePath), Charset.forName("UTF-8"));
        } catch (IOException e) {
            return null;
        }

        Gson gson = new Gson();
        ParsedUserClassViews parsedUserClassViews = gson.fromJson(userProvidedMetadataFileContent, ParsedUserClassViews.class);

        Map<String, InheritanceChain> providedInheritanceMetadata = new HashMap<>();
        for(ParsedUserClassView parsedUserClassView: parsedUserClassViews.getClassViews()){
            String className = parsedUserClassView.getClassName();
            InheritanceChain inheritanceChain = parsedUserClassView.getInheritanceChain();
            providedInheritanceMetadata.put(className, inheritanceChain);
        }

        return new UserProvidedClassInheritanceMetadata(providedInheritanceMetadata);
    }
}
