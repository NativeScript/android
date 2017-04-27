package org.nativescript.staticbindinggenerator.test;

import com.example.ListView;
import com.example.MyInterface;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.mdkt.compiler.InMemoryJavaCompiler;
import org.nativescript.staticbindinggenerator.Binding;
import org.nativescript.staticbindinggenerator.DataRow;
import org.nativescript.staticbindinggenerator.Generator;

import java.io.File;
import java.io.StringReader;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class GeneratorTest {
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
    public void testCanCompileBinding() throws Exception {
        List<String> lines = Utils.getDataRowsFromResource("datarow-named-extend.txt");
        DataRow dataRow = new DataRow(lines.get(0));

        String outputDir = null;
        String[] libs = {runtimePath};
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> helloClass = InMemoryJavaCompiler.compile(binding.getClassname(), sourceCode.toString(), options);

        Assert.assertNotNull(helloClass);
        Assert.assertEquals(3, helloClass.getDeclaredMethods().length);
    }

    @Test
    public void testCanCompileBindingOfInterfaceWithStaticInitializer() throws Exception {
        URL u = MyInterface.class.getResource('/' + MyInterface.class.getName().replace('.', '/') + ".class");
        File f = new File(u.toURI()).getParentFile().getParentFile().getParentFile();

        File rt = new File(System.getProperty("java.home"), "lib/rt.jar");
        Assert.assertTrue(rt.exists());

        String s = IOUtils.toString(this.getClass().getResource("datarow-classctor.txt"), "UTF-8");
        List<String> lines = IOUtils.readLines(new StringReader(s));
        DataRow dataRow = new DataRow(lines.get(0));

        String outputDir = null;
        String[] libs = {rt.getAbsolutePath(), f.getAbsolutePath()};
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", f.getAbsolutePath()));
        Class<?> helloClass = InMemoryJavaCompiler.compile("com.tns.gen.com.example.MyInterface", sourceCode.toString(), options);

        Assert.assertNotNull(helloClass);
        Assert.assertEquals(1, helloClass.getDeclaredMethods().length);
    }

    @Test
    public void testCanCompileBindingClassImplementingMultipleInterfaces() throws Exception {
        List<String> lines = Utils.getDataRowsFromResource("datarow-class-extends-interfaces.txt");
        DataRow dataRow = new DataRow(lines.get(0));

        String outputDir = null;
        String[] libs = {runtimePath};
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> ComplexClass = InMemoryJavaCompiler.compile(binding.getClassname(), sourceCode.toString(), options);

        Assert.assertNotNull(ComplexClass);
        Assert.assertEquals(5, ComplexClass.getInterfaces().length); // 4 + 1 (hashcodeprovider)
    }

    @Test
    public void testCanCompileBindingClassExtendingAnExtendedClassWithMethodsWithTheSameSignature() throws Exception {
        URL u = ListView.class.getResource('/' + ListView.class.getName().replace('.', '/') + ".class");
        File f = new File(u.toURI()).getParentFile().getParentFile().getParentFile();

        String dataRowString = "com.example.ListView*_fapp_l9_c29__*createView*com.example.MyListView**";
        DataRow dataRow = new DataRow(dataRowString);

        System.out.println(dataRowString);

        String outputDir = null;
        String[] libs = {runtimePath, f.getAbsolutePath()};
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> ComplexClass = InMemoryJavaCompiler.compile(binding.getClassname(), sourceCode.toString(), options);

        Assert.assertNotNull(ComplexClass);
        Assert.assertEquals(4, ComplexClass.getDeclaredMethods().length); // 1 + constructor + (equals + hashcode)
    }
}
