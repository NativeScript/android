package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import org.apache.bcel.classfile.JavaClass;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TestName;
import org.nativescript.staticbindinggenerator.files.FileSystemHelper;
import org.nativescript.staticbindinggenerator.files.impl.FileSystemHelperImpl;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericParameters;
import java.util.Map;

public class GenericsAwareClassHierarchyParserImplTest {

    @Rule
    public TestName testName = new TestName();

    private GenericsAwareClassHierarchyParserImpl hierarchyParser;
    private GenericHierarchyViewJsonExpectationComparer genericHierarchyViewJsonExpectationComparer;

    @Before
    public void setUp() {
        genericHierarchyViewJsonExpectationComparer = new GenericHierarchyViewJsonExpectationComparer();
    }

    @Test
    public void test1() {
        resetHierarchyParserForTestCase(testName.getMethodName());
        JavaClass startingClass = hierarchyParser.getClassesCache().get(org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test1.Concrete.class.getName());

        GenericHierarchyView hierarchyView = hierarchyParser.getClassHierarchy(startingClass);

        genericHierarchyViewJsonExpectationComparer.assertMeetsExpectation(hierarchyView, getExpectationJsonPath(testName.getMethodName()));
    }

    @Test
    public void test2() {
        resetHierarchyParserForTestCase(testName.getMethodName());
        JavaClass startingClass = hierarchyParser.getClassesCache().get(org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test2.CarFactory.class.getName());

        GenericHierarchyView hierarchyView = hierarchyParser.getClassHierarchy(startingClass);

        genericHierarchyViewJsonExpectationComparer.assertMeetsExpectation(hierarchyView, getExpectationJsonPath(testName.getMethodName()));
    }

    private String toJson(GenericHierarchyView hierarchyView ){
        String json = new GsonBuilder()
                .addSerializationExclusionStrategy(new JavaClassSerializationExclusionStrategy())
                .setPrettyPrinting()
                .create().toJson(hierarchyView);
        return json;
    }

    @Test
    public void test3() {
        resetHierarchyParserForTestCase(testName.getMethodName());
        JavaClass startingClass = hierarchyParser.getClassesCache().get(org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test3.Concrete.class.getName());

        GenericHierarchyView hierarchyView = hierarchyParser.getClassHierarchy(startingClass);

        genericHierarchyViewJsonExpectationComparer.assertMeetsExpectation(hierarchyView, getExpectationJsonPath(testName.getMethodName()));
    }

    @Test
    public void test4(){
        resetHierarchyParserForTestCase(testName.getMethodName());
        JavaClass startingClass = hierarchyParser.getClassesCache().get(org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test4.Concrete.class.getName());

        GenericHierarchyView hierarchyView = hierarchyParser.getClassHierarchy(startingClass);

        genericHierarchyViewJsonExpectationComparer.assertMeetsExpectation(hierarchyView, getExpectationJsonPath(testName.getMethodName()));
    }

    @Test
    public void test5(){
        resetHierarchyParserForTestCase(testName.getMethodName());
        JavaClass startingClass = hierarchyParser.getClassesCache().get(org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test5.Concrete.class.getName());

        GenericHierarchyView hierarchyView = hierarchyParser.getClassHierarchy(startingClass);

        genericHierarchyViewJsonExpectationComparer.assertMeetsExpectation(hierarchyView, getExpectationJsonPath(testName.getMethodName()));
    }

    @Test
    public void test6(){
        resetHierarchyParserForTestCase(testName.getMethodName());
        JavaClass startingClass = hierarchyParser.getClassesCache().get(org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test6.GenericClass.class.getName());
        GenericHierarchyView hierarchyView = hierarchyParser.getClassHierarchy(startingClass);

//        JavaClass startingClass2 = hierarchyParser.getClassesCache().get(org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.test6.A.class.getName());
//        GenericHierarchyView hierarchyView2 = hierarchyParser.getClassHierarchy(startingClass2);
        int a = 5;
    }

    private void resetHierarchyParserForTestCase(String testCase){
        String currentTestDirectory = GenericsAwareClassHierarchyParserImplTest.class.getResource(testCase).getPath();
        Map<String, JavaClass> classes = createClassesCache(currentTestDirectory);
        hierarchyParser = new GenericsAwareClassHierarchyParserImpl(new GenericSignatureReader(), null, classes);
    }

    private String getExpectationJsonPath(String testCase) {
        return this.getClass().getResource(testCase + "/expectation.json").getPath();
    }

    private GenericParameters createExpectedGenericJavaClass(JavaClass javaClass, Map<String, String> expectedGenericParametersMapping) {
        return new GenericParameters(expectedGenericParametersMapping);
    }

    private Map<String, JavaClass> createClassesCache(String directory) {
        FileSystemHelper fileSystemHelper = new FileSystemHelperImpl(false);
        return fileSystemHelper.readClassesFromDirectory(directory);
    }

}