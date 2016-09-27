package org.nativescript.staticbindinggenerator.test;

import org.apache.commons.io.FileUtils;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.nativescript.staticbindinggenerator.DataRow;
import org.nativescript.staticbindinggenerator.Generator;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

import com.android.dex.Dex;
import com.android.dex.MethodId;

/*
* All tests are ran against the android-24.jar and some may fail if ran against a lower level android.jar because of missing methods.
 */
public class CustomClassesTest {

    private static String outputDir = new File("./out").getAbsolutePath();
    private static String[] libs = {new File(".", "lib/android.jar").getAbsolutePath()};

    @BeforeClass
    public static void setUpClass() {
        new File(outputDir).mkdirs();
    }

    @AfterClass
    public static void tearDownClass() {
        try {
            FileUtils.deleteDirectory(new File(outputDir));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void assertAttachBaseContextIsNotGenerated() throws IOException {
        File bindingsFile = Utils.getBindingsFile("datarow-unexpected-application-methods.txt");

        runGeneratorWithFile(bindingsFile);

        List<String> methodsFromGeneratedDex = getMethodsFromDex("com.tns.NativeScriptApplication.dex");

        boolean methodIsGenerated = false;

        if(methodsFromGeneratedDex.contains("attachBaseContext")) {
           methodIsGenerated = true;
        }

        Assert.assertEquals(false, methodIsGenerated);
    }

    @Test
    public void assertAllExpectedApplicationMethodsAreGenerated() throws IOException {
        File bindingsFile = Utils.getBindingsFile("datarow-expected-application-methods.txt");

        runGeneratorWithFile(bindingsFile);

        List<String> methodsFromGeneratedDex = getMethodsFromDex("com.tns.NativeScriptApplication.dex");
        List<String> expectedMethods = getOverriddenMethodsFromBindings(bindingsFile);

        int currentMethodCount = 0;
        for(String expectedName: expectedMethods) {
            if(methodsFromGeneratedDex.contains(expectedName)) {
                currentMethodCount++;
            }
        }

        Assert.assertEquals(expectedMethods.size(), currentMethodCount);
    }

    @Test
    public void assertAllExpectedActivityMethodsAreGenerated() throws IOException {
        File bindingsFile = Utils.getBindingsFile("datarow-all-activity-methods.txt");

        runGeneratorWithFile(bindingsFile);

        List<String> methodsFromGeneratedDex = getMethodsFromDex("com.tns.NativeScriptActivity.dex");
        List<String> expectedMethods = getOverriddenMethodsFromBindings(bindingsFile);

        int currentMethodCount = 0;
        for(String expectedName: expectedMethods) {
            if(methodsFromGeneratedDex.contains(expectedName)) {
                currentMethodCount++;
            }
        }

        Assert.assertEquals(expectedMethods.size(), currentMethodCount);
    }

    @Test
    public void assertFinalActivityMethodsAreNotGenerated() throws IOException {
        File bindingsFile = Utils.getBindingsFile("datarow-final-activity-methods.txt");

        runGeneratorWithFile(bindingsFile);

        List<String> methodsFromGeneratedDex = getMethodsFromDex("com.tns.NativeScriptActivity.dex");
        List<String> expectedMethods = getOverriddenMethodsFromBindings(bindingsFile);

        int currentMethodCount = 0;
        for(String expectedName: expectedMethods) {
            if(methodsFromGeneratedDex.contains(expectedName)) {
                currentMethodCount++;
            }
        }

        Assert.assertEquals(0, currentMethodCount);
    }

    private List<String> getOverriddenMethodsFromBindings(File bindingsFile) throws FileNotFoundException {
        List<String> expectedMethods = new ArrayList<>();
        Scanner scanner = new Scanner(bindingsFile);
        while(scanner.hasNextLine()) {
            DataRow row = new DataRow(scanner.nextLine());
            expectedMethods.addAll(Arrays.asList(row.getMethods()));
        }

        return expectedMethods;
    }

    private void runGeneratorWithFile(File bindingsFile) throws IOException {
        Generator generator = new Generator(outputDir, libs);
        generator.writeBindings(bindingsFile.getAbsolutePath());
    }

    private List<String> getMethodsFromDex(String dexName) throws IOException {
        Dex dex = new Dex(new File(outputDir, dexName));
        List<MethodId> methodTable = dex.methodIds();
        List<String> namesTable = dex.strings();
        List<String> methodsFromGeneratedDex = new ArrayList<String>();

        for(MethodId mId: methodTable) {
            int nameIndex = mId.getNameIndex();
            methodsFromGeneratedDex.add(namesTable.get(nameIndex));
        }
        return methodsFromGeneratedDex;
    }
}
