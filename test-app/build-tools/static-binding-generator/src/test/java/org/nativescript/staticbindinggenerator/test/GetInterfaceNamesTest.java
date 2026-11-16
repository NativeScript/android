package org.nativescript.staticbindinggenerator.test;

import com.example.MyClass;
import com.example.MyInterface;

import org.apache.commons.io.FileUtils;
import org.junit.Assert;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.nativescript.staticbindinggenerator.DataRow;
import org.nativescript.staticbindinggenerator.GetInterfaceNames;

import java.io.File;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;

public class GetInterfaceNamesTest {
    public interface NestedInterface {
        void run();
    }

    @Rule
    public TemporaryFolder temporaryFolder = new TemporaryFolder();

    @Test
    public void testCollectsInterfacesFromAClassDirectory() throws Exception {
        File classesDir = temporaryFolder.newFolder("classes");
        copyClassFile(MyInterface.class, classesDir);
        copyClassFile(MyClass.class, classesDir);
        copyClassFile(NestedInterface.class, classesDir);
        FileUtils.writeStringToFile(new File(classesDir, "com/example/NotAClass.class"), "garbage", StandardCharsets.UTF_8);

        List<String> names = GetInterfaceNames.collectInterfaceNames(Collections.singletonList(new DataRow(classesDir.getAbsolutePath())));

        Assert.assertEquals(
                new HashSet<>(Arrays.asList(
                        "com.example.MyInterface",
                        "org.nativescript.staticbindinggenerator.test.GetInterfaceNamesTest.NestedInterface")),
                new HashSet<>(names));
    }

    @Test
    public void testIgnoresAMissingClassDirectory() {
        File missingDir = new File(temporaryFolder.getRoot(), "missing");

        List<String> names = GetInterfaceNames.collectInterfaceNames(Collections.singletonList(new DataRow(missingDir.getAbsolutePath())));

        Assert.assertTrue(names.isEmpty());
    }

    private static void copyClassFile(Class<?> clazz, File classesDir) throws URISyntaxException, java.io.IOException {
        String relativePath = clazz.getName().replace('.', '/') + ".class";
        File source = new File(clazz.getResource('/' + relativePath).toURI());
        FileUtils.copyFile(source, new File(classesDir, relativePath));
    }
}
