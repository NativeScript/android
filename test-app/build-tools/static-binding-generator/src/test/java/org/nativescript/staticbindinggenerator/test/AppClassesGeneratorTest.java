package org.nativescript.staticbindinggenerator.test;

import com.example.View;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.nativescript.staticbindinggenerator.DataRow;
import org.nativescript.staticbindinggenerator.Generator;
import org.nativescript.staticbindinggenerator.InputParameters;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import javax.tools.JavaCompiler;
import javax.tools.ToolProvider;

public class AppClassesGeneratorTest {

    @Rule
    public TemporaryFolder temp = new TemporaryFolder();

    private String classesDir;
    private File appDir;
    private File libDir;
    private File outputDir;
    private final List<String> output = new ArrayList<>();

    @Before
    public void setUp() throws Exception {
        InputParameters.parseCommand(new String[0]);
        URL u = View.class.getResource('/' + View.class.getName().replace('.', '/') + ".class");
        classesDir = new File(u.toURI()).getParentFile().getParentFile().getParentFile().getAbsolutePath();
        outputDir = temp.newFolder("out");

        Map<String, String> app = new LinkedHashMap<>();
        app.put("app.Gone", "package app; public class Gone { }");
        app.put("app.Broken", "package app; public abstract class Broken extends Gone { public abstract void run(); }");
        app.put("app.GoneIface", "package app; public interface GoneIface { }");
        app.put("app.BrokenListener", "package app; public interface BrokenListener extends GoneIface { void on(); }");
        app.put("app.OkListener", "package app; public interface OkListener { void on(); }");
        app.put("app.Callback", "package app; public interface Callback { void call(); }");
        app.put("app.Widget", "package app; public abstract class Widget { public abstract void draw(); }");
        app.put("app.SelfTyped", "package app; public abstract class SelfTyped<T extends SelfTyped<T>> { public abstract T self(); }");
        app.put("app.WithDefault", "package app; public interface WithDefault { void a(); default void b() { } }");
        app.put("app.InheritsDefault", "package app; public interface InheritsDefault extends WithDefault { void c(); }");
        app.put("app.InheritsLibraryDefault", "package app; public interface InheritsLibraryDefault extends java.util.Iterator<String> { }");
        app.put("app.StaticAndPrivate", "package app; public interface StaticAndPrivate extends OkListener { static int s() { return 0; } private int p() { return 0; } }");
        appDir = compile("app-classes", app);
        delete(appDir, "app/Gone.class");
        delete(appDir, "app/GoneIface.class");

        Map<String, String> lib = new LinkedHashMap<>();
        lib.put("lib.Gone", "package lib; public class Gone { }");
        lib.put("lib.Broken", "package lib; public abstract class Broken extends Gone { public abstract void run(); }");
        lib.put("lib.Callback", "package lib; public interface Callback { void call(); }");
        lib.put("lib.Holder", "package lib; public abstract class Holder implements app.OkListener { }");
        lib.put("lib.LibWithDefault", "package lib; public interface LibWithDefault { void a(); default void b() { } }");
        libDir = compile("lib-classes", lib, appDir.getAbsolutePath());
        delete(libDir, "lib/Gone.class");
    }

    @After
    public void tearDown() {
        InputParameters.parseCommand(new String[0]);
    }

    private File compile(String name, Map<String, String> sources, String... classpath) throws IOException {
        File srcDir = temp.newFolder(name + "-src");
        File outDir = temp.newFolder(name);
        List<String> args = new ArrayList<>(Arrays.asList("-d", outDir.getAbsolutePath(), "-proc:none", "-nowarn"));
        if (classpath.length > 0) {
            args.add("-cp");
            args.add(String.join(File.pathSeparator, classpath));
        }
        for (Map.Entry<String, String> source : sources.entrySet()) {
            File file = new File(srcDir, source.getKey().replace('.', '/') + ".java");
            file.getParentFile().mkdirs();
            Files.write(file.toPath(), source.getValue().getBytes(StandardCharsets.UTF_8));
            args.add(file.getAbsolutePath());
        }
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        Assert.assertEquals("javac failed", 0, compiler.run(null, null, null, args.toArray(new String[0])));
        return outDir;
    }

    private static void delete(File dir, String path) throws IOException {
        Files.delete(new File(dir, path).toPath());
    }

    private Generator generate(List<String> libs, List<String> appClassDirs, String... rows) throws Exception {
        File rowsFile = temp.newFile();
        Files.write(rowsFile.toPath(), Arrays.asList(rows), StandardCharsets.UTF_8);
        List<DataRow> libRows = libs.stream().map(DataRow::new).collect(Collectors.toList());
        Generator generator = new Generator(outputDir, libRows, false, appClassDirs);

        ByteArrayOutputStream captured = new ByteArrayOutputStream();
        PrintStream originalOut = System.out;
        System.setOut(new PrintStream(captured, true, "UTF-8"));
        try {
            generator.writeBindings(rowsFile.getAbsolutePath());
        } finally {
            System.setOut(originalOut);
            output.addAll(Arrays.asList(new String(captured.toByteArray(), StandardCharsets.UTF_8).split("\\R")));
        }
        return generator;
    }

    private Generator generate(String... rows) throws Exception {
        return generate(Arrays.asList(classesDir, libDir.getAbsolutePath(), appDir.getAbsolutePath()),
                Collections.singletonList(appDir.getAbsolutePath()), rows);
    }

    private List<String> runtimeLines(String site) {
        return output.stream().filter(line -> line.contains(site) && line.contains("generated at runtime")).collect(Collectors.toList());
    }

    private File generated(String path) {
        return new File(outputDir, path);
    }

    private String read(String path) throws IOException {
        return new String(Files.readAllBytes(generated(path).toPath()), StandardCharsets.UTF_8);
    }

    private File onlyFile(String packageDir, String part) {
        File[] files = generated(packageDir).listFiles((dir, name) -> name.contains(part));
        Assert.assertNotNull(packageDir, files);
        Assert.assertEquals(Arrays.toString(files), 1, files.length);
        return files[0];
    }

    @Test
    public void leavesAppRowsItCannotGenerateToTheRuntime() throws Exception {
        Generator generator = generate(
                "app.Broken*tests_a*1*1**run***",
                "app.Widget*tests_a*2*1**draw***",
                "com.example.View*tests_a*3*1**createView***");

        Assert.assertEquals(output.toString(), 1, runtimeLines("tests_a:1:1").size());
        Assert.assertTrue(runtimeLines("tests_a:1:1").get(0), runtimeLines("tests_a:1:1").get(0).contains("app.Gone"));
        Assert.assertEquals(0, generated("com/tns/gen/app").listFiles((dir, name) -> name.startsWith("Broken_")).length);

        File widget = onlyFile("com/tns/gen/app", "Widget_");
        File view = onlyFile("com/tns/gen/com/example", "View_");
        Assert.assertEquals(Collections.singletonList(widget.getAbsolutePath()), generator.getAppBindingFiles());
        Assert.assertTrue(view.isFile());
    }

    @Test
    public void keepsFailingOnOtherRowsItCannotGenerate() throws Exception {
        try {
            generate("lib.Broken*tests_a*1*1**run***");
            Assert.fail("expected the generator to fail on a library row it cannot generate");
        } catch (RuntimeException e) {
            Assert.assertTrue(e.getMessage(), e.getMessage().contains("lib.Gone"));
        }
    }

    @Test
    public void treatsEveryClassAlikeWithoutAListOfAppClasses() throws Exception {
        try {
            generate(Arrays.asList(classesDir, appDir.getAbsolutePath()), Collections.emptyList(), "app.Broken*tests_a*1*1**run***");
            Assert.fail("expected the generator to fail on a row it cannot generate");
        } catch (RuntimeException e) {
            Assert.assertTrue(e.getMessage(), e.getMessage().contains("app.Gone"));
        }
    }

    @Test(timeout = 20000)
    public void leavesEveryRowWithUnresolvableTypeParametersToTheRuntime() throws Exception {
        String boundCycles = "org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.boundcycles.";
        generate(
                boundCycles + "SelfTyped*tests_a*1*1**self***",
                boundCycles + "ComparableSource*tests_a*2*1**next***",
                "app.SelfTyped*tests_a*3*1**self***",
                "com.example.View*tests_a*4*1**createView***");

        for (String site : Arrays.asList("tests_a:1:1", "tests_a:2:1", "tests_a:3:1")) {
            List<String> lines = runtimeLines(site);
            Assert.assertEquals(output.toString(), 1, lines.size());
            Assert.assertTrue(lines.get(0), lines.get(0).contains("type parameters"));
        }
        Assert.assertTrue(onlyFile("com/tns/gen/com/example", "View_").isFile());
        Assert.assertFalse(generated("com/tns/gen/app").exists() && generated("com/tns/gen/app").list().length > 0);
    }

    @Test
    public void leavesSharedClassesOfAppInterfacesWithDefaultMethodsToTheRuntime() throws Exception {
        Generator generator = generate(
                "app.WithDefault*tests_a*1*1**a,b***",
                "app.WithDefault*tests_a*2*1**a***",
                "app.InheritsDefault*tests_a*3*1**a,c***",
                "app.InheritsLibraryDefault*tests_a*4*1**hasNext,next***",
                "app.BrokenListener*tests_a*5*1**on***",
                "app.StaticAndPrivate*tests_a*6*1**on***",
                "app.OkListener*tests_a*7*1**on***",
                "app.WithDefault*tests_a*8*1**a,b***java.lang.Runnable",
                "app.WithDefault*****a,b*com.x.NamedDefaults*app/f.js*",
                "lib.LibWithDefault*tests_a*9*1**a,b***");

        Map<String, String> reasons = new LinkedHashMap<>();
        reasons.put("tests_a:1:1", "app.WithDefault.b");
        reasons.put("tests_a:2:1", "app.WithDefault.b");
        reasons.put("tests_a:3:1", "app.WithDefault.b");
        reasons.put("tests_a:4:1", "java.util.Iterator.");
        reasons.put("tests_a:5:1", "app.GoneIface cannot be loaded");
        for (Map.Entry<String, String> reason : reasons.entrySet()) {
            List<String> lines = runtimeLines(reason.getKey() + " ");
            Assert.assertEquals(output.toString(), 1, lines.size());
            Assert.assertTrue(lines.get(0), lines.get(0).contains("default methods") && lines.get(0).contains(reason.getValue()));
        }
        for (String name : Arrays.asList("WithDefault", "InheritsDefault", "InheritsLibraryDefault", "BrokenListener")) {
            Assert.assertFalse(name, generated("com/tns/gen/app/" + name + ".java").exists());
        }

        List<File> expected = Arrays.asList(
                generated("com/tns/gen/app/StaticAndPrivate.java"),
                generated("com/tns/gen/app/OkListener.java"),
                onlyFile("com/tns/gen/app", "WithDefault_"),
                generated("com/x/NamedDefaults.java"));
        for (File file : expected) {
            Assert.assertTrue(file.toString(), file.isFile());
        }
        Assert.assertTrue(generated("com/tns/gen/lib/LibWithDefault.java").isFile());
        Assert.assertEquals(expected.stream().map(File::getAbsolutePath).sorted().collect(Collectors.toList()),
                generator.getAppBindingFiles().stream().sorted().collect(Collectors.toList()));
    }

    @Test
    public void releasesTheNameOfARowItLeavesToTheRuntime() throws Exception {
        Generator generator = generate(
                "app.BrokenListener*****on*com.x.Same*app/f.js*",
                "app.OkListener*****on*com.x.Same*app/g.js*");

        Assert.assertEquals(output.toString(), 1, runtimeLines("app/f.js").size());
        String source = read("com/x/Same.java");
        Assert.assertTrue(source, source.contains("app.OkListener {") && source.contains("\"./app/g.js\""));
        Assert.assertEquals(Collections.singletonList(generated("com/x/Same.java").getAbsolutePath()), generator.getAppBindingFiles());
    }

    @Test
    public void neverTakesAnInterfaceNameFromTheOtherRows() throws Exception {
        Generator generator = generate(
                "app.Callback*tests_a*1*1**call***",
                "lib.Callback*tests_a*2*1**call***");

        Assert.assertTrue(generated("com/tns/gen/app/Callback.java").isFile());
        Assert.assertTrue(generated("com/tns/gen/lib/Callback.java").isFile());
        Assert.assertEquals(Collections.singletonList(generated("com/tns/gen/app/Callback.java").getAbsolutePath()), generator.getAppBindingFiles());
    }

    @Test
    public void countsAClassAsAnAppClassOnlyWhenItsLastEntryIsAppClasses() throws Exception {
        String appWidgetCopy = compile("widget-copy", Collections.singletonMap("app.Widget",
                "package app; public abstract class Widget { public abstract void draw(); }")).getAbsolutePath();

        Generator replaced = generate(Arrays.asList(classesDir, appDir.getAbsolutePath(), appWidgetCopy),
                Collections.singletonList(appDir.getAbsolutePath()), "app.Widget*tests_a*2*1**draw***");
        Assert.assertTrue(onlyFile("com/tns/gen/app", "Widget_").isFile());
        Assert.assertEquals(Collections.emptyList(), replaced.getAppBindingFiles());

        Generator replacing = generate(Arrays.asList(classesDir, appWidgetCopy, appDir.getAbsolutePath()),
                Collections.singletonList(appDir.getAbsolutePath()), "app.Widget*tests_a*2*1**draw***");
        Assert.assertEquals(Collections.singletonList(onlyFile("com/tns/gen/app", "Widget_").getAbsolutePath()), replacing.getAppBindingFiles());
    }

    @Test
    public void findsAppClassesAmongInterfacesAndSupertypes() throws Exception {
        Generator generator = generate(
                "com.example.View*tests_a*1*1**createView***app.OkListener",
                "lib.Holder*tests_a*2*1**on***",
                "com.example.View*tests_a*3*1**createView***java.lang.Runnable");

        File withInterface = onlyFile("com/tns/gen/com/example", "_1_1_");
        File holder = onlyFile("com/tns/gen/lib", "Holder_");
        Assert.assertTrue(onlyFile("com/tns/gen/com/example", "_3_1_").isFile());
        List<String> expected = Arrays.asList(withInterface.getAbsolutePath(), holder.getAbsolutePath());
        Assert.assertEquals(expected.stream().sorted().collect(Collectors.toList()),
                generator.getAppBindingFiles().stream().sorted().collect(Collectors.toList()));
    }

    @Test
    public void leavesAppBindingsOfOneClassWithDifferentContentsToTheRuntime() throws Exception {
        Generator generator = generate(
                "app.Widget*****draw*com.x.Named*app/f.js*",
                "app.Widget*****draw,toString*com.x.Named*app/g.js*");

        Assert.assertFalse(generated("com/x/Named.java").exists());
        List<String> lines = runtimeLines("app/f.js");
        Assert.assertEquals(output.toString(), 1, lines.size());
        Assert.assertTrue(lines.get(0), lines.get(0).contains("app/g.js"));
        Assert.assertEquals(Collections.emptyList(), generator.getAppBindingFiles());
    }

    @Test
    public void keepsTheOtherRowsBindingOfAClassAnAppBindingAlsoGenerates() throws Exception {
        Generator generator = generate(
                "app.Widget*****draw*com.x.Named*app/f.js*",
                "com.example.View*****createView*com.x.Named*app/g.js*");

        String source = read("com/x/Named.java");
        Assert.assertTrue(source, source.contains("extends com.example.View"));
        Assert.assertEquals(output.toString(), 1, runtimeLines("app/f.js").size());
        Assert.assertEquals(Collections.emptyList(), generator.getAppBindingFiles());
    }
}
