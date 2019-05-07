package org.nativescript.staticbindinggenerator.generating.parsing.checkers.impl;

import org.apache.bcel.classfile.JavaClass;
import org.nativescript.staticbindinggenerator.generating.parsing.checkers.AndroidClassChecker;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.ClassHierarchyParser;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.HierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.impl.ClassHierarchyParserImpl;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class AndroidClassCheckerImpl implements AndroidClassChecker {

    private static final String APPLICATION_CLASS_NAME = "android.app.Application";
    private static final String SERVICE_CLASS_NAME = "android.app.Service";
    private static final List<String> ACTIVITY_TYPES = Arrays.asList("android.app.Activity","android.support.v7.app.AppCompatActivity","androidx.appcompat.app.AppCompatActivity");

    private final ClassHierarchyParser classHierarchyParser;

    public AndroidClassCheckerImpl(Map<String, JavaClass> classesCache) {
        this.classHierarchyParser = new ClassHierarchyParserImpl(classesCache);
    }

    @Override
    public boolean isActivityClass(JavaClass javaClass) {
        for(String activityType: ACTIVITY_TYPES){
            if(javaClass.getClassName().equals(activityType)){
                return true;
            }

            HierarchyView hierarchyView = classHierarchyParser.getClassHierarchy(javaClass);
            boolean hasParentClassActivity = hierarchyView.getAllParentClassesNames().contains(activityType);
            if(hasParentClassActivity){
                return true;
            }
        }

        return false;
    }

    @Override
    public boolean isApplicationClass(JavaClass javaClass) {
        if(javaClass.getClassName().equals(APPLICATION_CLASS_NAME)){
            return true;
        }

        HierarchyView hierarchyView = classHierarchyParser.getClassHierarchy(javaClass);
        return hierarchyView.getAllParentClassesNames().contains(APPLICATION_CLASS_NAME);
    }

    @Override
    public boolean isServiceClass(JavaClass javaClass){
        if(javaClass.getClassName().equals(SERVICE_CLASS_NAME)){
            return true;
        }

        HierarchyView hierarchyView = classHierarchyParser.getClassHierarchy(javaClass);
        return hierarchyView.getAllParentClassesNames().contains(SERVICE_CLASS_NAME);
    }
}
