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
import java.util.HashSet;
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
        Assert.assertEquals(7, helloClass.getDeclaredMethods().length);
    }

    @Test
    public void testCanCompileBindingWithSuppressedCallJsMethodExceptions() throws Exception {
        List<String> lines = Utils.getDataRowsFromResource("datarow-named-extend.txt");
        DataRow dataRow = new DataRow(lines.get(0));

        File outputDir = null;
        List<DataRow> libs = new ArrayList<>();
        Generator generator = new Generator(outputDir, libs, true);
        Binding binding = generator.generateBinding(dataRow);
        Assert.assertNotNull(binding);

        String sourceCode = binding.getContent();
        Assert.assertTrue(sourceCode.contains("com.tns.Runtime.passSuppressedExceptionToJs(this,"));

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> helloClass = InMemoryJavaCompiler.compile(binding.getClassname(), sourceCode, options);

        Assert.assertNotNull(helloClass);
    }

    @Test
    public void testBindingCarriesARuntimeTag() throws Exception {
        List<String> lines = Utils.getDataRowsFromResource("datarow-named-extend.txt");
        DataRow dataRow = new DataRow(lines.get(0));

        File outputDir = null;
        List<DataRow> libs = new ArrayList<>();
        Generator generator = new Generator(outputDir, libs);
        Binding binding = generator.generateBinding(dataRow);
        Assert.assertNotNull(binding);

        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> boundClass = InMemoryJavaCompiler.compile(binding.getClassname(), binding.getContent(), options);

        Assert.assertTrue(com.tns.NativeScriptRuntimeBound.class.isAssignableFrom(boundClass));

        com.tns.NativeScriptRuntimeBound instance = (com.tns.NativeScriptRuntimeBound) boundClass.newInstance();
        // zero is the "no runtime yet" tag; com.tns.Runtime never stores it for a real owner
        Assert.assertEquals(0, instance.getRuntimeTag__ns());

        instance.setRuntimeTag__ns(7);
        Assert.assertEquals(7, instance.getRuntimeTag__ns());

        // read on whichever thread a Java->JS call arrives on
        Assert.assertTrue(java.lang.reflect.Modifier.isVolatile(boundClass.getDeclaredField("runtimeTag__ns").getModifiers()));

        instance.setObjectId__ns(42);
        Assert.assertEquals(42, instance.getObjectId__ns());
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
        Assert.assertEquals(7, helloClass.getDeclaredMethods().length);  // 7 methods (includes 'hashCode__super', 'equals__super' and the runtime tag and object id accessors)
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
        Assert.assertEquals(4, ComplexClass.getInterfaces().length); // 2 + hashcodeprovider + runtimebound
    }

    @Test
    public void testAnInterfaceImplementationWithExtraInterfacesGetsItsCallSiteClass() throws Exception {
        Generator generator = new Generator(null, new ArrayList<DataRow>());
        HashSet<String> interfaceNames = new HashSet<>();

        Binding withExtras = generator.generateBinding(new DataRow("java.lang.Runnable*app_main*9*29**run,update***java.util.Observer"), interfaceNames);
        Binding otherExtras = generator.generateBinding(new DataRow("java.lang.Runnable*app_main*12*4**run,formatTo***java.util.Formattable"), interfaceNames);
        Binding shared = generator.generateBinding(new DataRow("java.lang.Runnable*app_main*15*4**run***"), interfaceNames);
        Binding sharedAgain = generator.generateBinding(new DataRow("java.lang.Runnable*app_main*18*4**run***"), interfaceNames);

        String withExtrasName = withExtras.getFile().getName();
        Assert.assertTrue(withExtrasName, withExtrasName.startsWith("Runnable_app_main_"));
        Assert.assertTrue(withExtrasName, withExtrasName.endsWith("_9_29_.java"));
        Assert.assertTrue(otherExtras.getFile().getName().endsWith("_12_4_.java"));
        Assert.assertEquals("Runnable.java", shared.getFile().getName());
        Assert.assertNull(sharedAgain);

        String className = "com.tns.gen.java.lang." + withExtrasName.substring(0, withExtrasName.length() - ".java".length());
        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));
        Class<?> compiled = InMemoryJavaCompiler.compile(className, withExtras.getContent(), options);
        List<Class<?>> interfaces = Arrays.asList(compiled.getInterfaces());
        Assert.assertTrue(interfaces.contains(Runnable.class));
        Assert.assertTrue(interfaces.contains(java.util.Observer.class));

        Class<?> sharedClass = InMemoryJavaCompiler.compile("com.tns.gen.java.lang.Runnable", shared.getContent(), options);
        Assert.assertFalse(Arrays.asList(sharedClass.getInterfaces()).contains(java.util.Observer.class));
    }

    @Test
    public void testACallSiteClassListsTheMembersTheParserSaw() throws Exception {
        Generator generator = new Generator(null, new ArrayList<DataRow>());
        Iterable<String> options = new ArrayList<String>(Arrays.asList("-cp", dependenciesDir));

        Binding callSite = generator.generateBinding(new DataRow("java.lang.Object*app_main*9*29*Members*toString,helper***"));
        String callSiteName = callSite.getFile().getName();
        Class<?> callSiteClass = InMemoryJavaCompiler.compile("com.tns.gen.java.lang." + callSiteName.substring(0, callSiteName.length() - ".java".length()), callSite.getContent(), options);
        java.lang.reflect.Field members = callSiteClass.getDeclaredField("members__ns");
        members.setAccessible(true);
        Assert.assertArrayEquals(new String[]{"toString", "helper"}, (String[]) members.get(null));

        Binding named = generator.generateBinding(new DataRow(Utils.getDataRowsFromResource("datarow-named-extend.txt").get(0)));
        Class<?> namedClass = InMemoryJavaCompiler.compile(named.getClassname(), named.getContent(), options);
        for (java.lang.reflect.Field field : namedClass.getDeclaredFields()) {
            Assert.assertNotEquals("members__ns", field.getName());
        }
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
        Assert.assertEquals(8, ComplexClass.getDeclaredMethods().length); // 1 + constructor + (equals + hashcode) + (runtime tag and object id accessors)
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
