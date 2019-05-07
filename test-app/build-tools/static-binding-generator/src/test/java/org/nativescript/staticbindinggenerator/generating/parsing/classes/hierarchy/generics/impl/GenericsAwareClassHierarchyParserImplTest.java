package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl;

import org.apache.bcel.classfile.JavaClass;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TestName;
import org.nativescript.staticbindinggenerator.files.FileSystemHelper;
import org.nativescript.staticbindinggenerator.files.impl.FileSystemHelperImpl;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
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

    private void resetHierarchyParserForTestCase(String testCase){
        String currentTestDirectory = GenericsAwareClassHierarchyParserImplTest.class.getResource(testCase).getPath();
        Map<String, JavaClass> classes = createClassesCache(currentTestDirectory);
        hierarchyParser = new GenericsAwareClassHierarchyParserImpl(new GenericSignatureReader(), classes);
    }

    private String getExpectationJsonPath(String testCase) {
        return this.getClass().getResource(testCase + "/expectation.json").getPath();
    }

    private Map<String, JavaClass> createClassesCache(String directory) {
        FileSystemHelper fileSystemHelper = new FileSystemHelperImpl(false);
        return fileSystemHelper.readClassesFromDirectory(directory).getRegularClasses();
    }

}