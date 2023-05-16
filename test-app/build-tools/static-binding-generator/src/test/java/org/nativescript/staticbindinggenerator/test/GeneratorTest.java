package org.nativescript.staticbindinggenerator.test;

import com.example.ListView;
import com.example.MyAbstractClass2;
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

    @BeforeClass
    public static void setUp() {
        URL u = com.tns.Runtime.class.getResource('/' + com.tns.Runtime.class.getName().replace('.', '/') + ".class");
        try {
            dependenciesDir = new File(u.toURI()).getParentFile().getParentFile().getParentFile().getAbsolutePath();
        } catch (URISyntaxException e) {
            e.printStackTrace();
            Assert.fail();
        }
    }

    @Test
    public void testCanCompileBinding() throws Exception {
        List<String> lines = Utils.getDataRowsFromResource("datarow-named-extend.txt");
        DataRow dataRow = new DataRow(lines.get(0));

        File outputDir = null;
        List<DataRow> libs = new ArrayList<>();
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);
        Assert.assertNotNull(binding);

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

        String s = IOUtils.toString(this.getClass().getResource("datarow-classctor.txt"), "UTF-8");
        List<String> lines = IOUtils.readLines(new StringReader(s));
        DataRow dataRow = new DataRow(lines.get(0));

        File outputDir = null;
        List<DataRow> libs = new ArrayList<>();
        libs.add(new DataRow(f.getAbsolutePath()));
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);
        Assert.assertNotNull(binding);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", f.getAbsolutePath()));
        Class<?> helloClass = InMemoryJavaCompiler.compile("com.tns.gen.com.example.MyInterface", sourceCode.toString(), options);

        Assert.assertNotNull(helloClass);
        Assert.assertEquals(3, helloClass.getDeclaredMethods().length);  // 3 methods (includes 'hashCode__super' and 'equals__super')
    }

    @Test
    public void testCanCompileBindingClassImplementingMultipleInterfaces() throws Exception {
        List<String> lines = Utils.getDataRowsFromResource("datarow-class-extends-interfaces.txt");
        DataRow dataRow = new DataRow(lines.get(0));

        File outputDir = null;
        List<DataRow> libs = new ArrayList<>();
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);
        Assert.assertNotNull(binding);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> ComplexClass = InMemoryJavaCompiler.compile(binding.getClassname(), sourceCode.toString(), options);

        Assert.assertNotNull(ComplexClass);
        Assert.assertEquals(3, ComplexClass.getInterfaces().length); // 2 + 1 (hashcodeprovider)
    }

    @Test
    public void testCanCompileBindingClassExtendingAnExtendedClassWithMethodsWithTheSameSignature() throws Exception {
        URL u = ListView.class.getResource('/' + ListView.class.getName().replace('.', '/') + ".class");
        File f = new File(u.toURI()).getParentFile().getParentFile().getParentFile();

        String dataRowString = "com.example.ListView*app*9*29**createView*com.example.MyListView**";
        DataRow dataRow = new DataRow(dataRowString);

        System.out.println(dataRowString);

        File outputDir = null;
        List<DataRow> libs = new ArrayList<>();
        libs.add(new DataRow(f.getAbsolutePath()));
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);
        Assert.assertNotNull(binding);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> ComplexClass = InMemoryJavaCompiler.compile(binding.getClassname(), sourceCode.toString(), options);

        Assert.assertNotNull(ComplexClass);
        Assert.assertEquals(4, ComplexClass.getDeclaredMethods().length); // 1 + constructor + (equals + hashcode)
    }

    @Test
    public void testCanCompileBindingClassExtendingAnAbstractClassThatExtendsAbstractClass() throws Exception {
        URL u = MyAbstractClass2.class.getResource('/' + MyAbstractClass2.class.getName().replace('.', '/') + ".class");
        File f = new File(u.toURI()).getParentFile().getParentFile().getParentFile();

        String dataRowString = "com.example.MyAbstractClass2**9*29**abstractMethod*com.example.com.example.MyExtendedClass**";
        DataRow dataRow = new DataRow(dataRowString);

        System.out.println(dataRowString);

        File outputDir = null;
        List<DataRow> libs = new ArrayList<>();
        libs.add(new DataRow(f.getAbsolutePath()));
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);
        Assert.assertNotNull(binding);

        StringBuffer sourceCode = new StringBuffer();
        sourceCode.append(binding.getContent());

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> ComplexClass = InMemoryJavaCompiler.compile(binding.getClassname(), sourceCode.toString(), options);

        // class compiles, meaning abstract method of super-super class is extended properly
        Assert.assertNotNull(ComplexClass);
    }
}
