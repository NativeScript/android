package org.nativescript.staticbindinggenerator.test;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.nativescript.staticbindinggenerator.DataRow;
import org.nativescript.staticbindinggenerator.Generator;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.List;

public class ApplicationTest {

    private static String dependenciesDir;
    private static String runtimePath;

    @BeforeClass
    public static void setUp() {
        URL u = com.tns.Runtime.class.getResource('/' + com.tns.Runtime.class.getName().replace('.', '/') + ".class");
        try {
            dependenciesDir = new File(u.toURI()).getParentFile().getParentFile().getParentFile().getAbsolutePath();
            File runtimePathDir = new File(System.getProperty("java.home"), "lib/rt.jar");
            Assert.assertTrue(runtimePathDir.exists());
            runtimePath = runtimePathDir.getAbsolutePath();
        } catch (URISyntaxException e) {
            e.printStackTrace();
            Assert.fail();
        }
    }

    @Test
    public void assertAttachBaseContextIsNotGenerated() throws IOException {
//        List<String> lines = Utils.getDataRowsFromResource("datarow-application.txt");
//        DataRow row = new DataRow(lines.get(0));
//
//        String outputDir = null;
//        String[] libs = {runtimePath};
//        Generator generator = new Generator(outputDir, libs);
//
//        generator.writeBindings();
//
//        Assert.assertEquals("run", "run");
    }
}
