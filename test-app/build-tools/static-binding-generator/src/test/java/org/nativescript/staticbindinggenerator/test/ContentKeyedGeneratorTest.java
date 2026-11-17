package org.nativescript.staticbindinggenerator.test;

import com.example.View;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.mdkt.compiler.InMemoryJavaCompiler;
import org.nativescript.staticbindinggenerator.ContentKey;
import org.nativescript.staticbindinggenerator.DataRow;
import org.nativescript.staticbindinggenerator.Generator;
import org.nativescript.staticbindinggenerator.InputParameters;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class ContentKeyedGeneratorTest {

    @Rule
    public TemporaryFolder temp = new TemporaryFolder();

    private String classesDir;
    private File outputDir;

    @Before
    public void setUp() throws Exception {
        URL u = View.class.getResource('/' + View.class.getName().replace('.', '/') + ".class");
        classesDir = new File(u.toURI()).getParentFile().getParentFile().getParentFile().getAbsolutePath();
        outputDir = temp.newFolder("out");
    }

    @After
    public void tearDown() {
        InputParameters.parseCommand(new String[0]);
    }

    private Generator generate(boolean contentKeyed, String... rows) throws Exception {
        InputParameters.parseCommand(contentKeyed ? new String[]{"-content-keyed-bindings"} : new String[0]);
        File rowsFile = temp.newFile();
        Files.write(rowsFile.toPath(), Arrays.asList(rows), StandardCharsets.UTF_8);
        List<DataRow> libs = new ArrayList<>();
        libs.add(new DataRow(classesDir));
        Generator generator = new Generator(outputDir, libs);
        generator.writeBindings(rowsFile.getAbsolutePath());
        return generator;
    }

    private List<String> generatedFiles(String packageDir) {
        File[] files = new File(outputDir, packageDir).listFiles();
        if (files == null) {
            return Collections.emptyList();
        }
        return Arrays.stream(files).map(File::getName).sorted().collect(Collectors.toList());
    }

    private String read(String path) throws IOException {
        return new String(Files.readAllBytes(new File(outputDir, path).toPath()), StandardCharsets.UTF_8);
    }

    private static String key(String... members) {
        return ContentKey.of("com.example.View", Collections.emptyList(), Arrays.asList(members));
    }

    @Test
    public void namesClassExtendsAfterTheirContent() throws Exception {
        generate(true,
                "com.example.View*tests_a*3*5**createView***",
                "com.example.View*tests_a*9*5*Named*createView***",
                "com.example.MyInterface*tests_a*12*5**getTile***");

        List<String> expected = new ArrayList<>(Arrays.asList("MyInterface.java", "View_" + key("createView") + ".java", "View_Named_" + key("createView") + ".java"));
        Collections.sort(expected);
        Assert.assertEquals(expected, generatedFiles("com/tns/gen/com/example"));
    }

    @Test
    public void sharesOneClassBetweenCallSitesWithTheSameContent() throws Exception {
        generate(true,
                "com.example.View*tests_a*3*5**createView,toString***",
                "com.example.View*tests_b*7*9**toString,createView,toString,constructor***");

        String name = "View_" + key("createView", "toString");
        Assert.assertEquals(Collections.singletonList(name + ".java"), generatedFiles("com/tns/gen/com/example"));

        String source = read("com/tns/gen/com/example/" + name + ".java");
        Class<?> compiled = InMemoryJavaCompiler.compile("com.tns.gen.com.example." + name,
                source, Arrays.asList("-cp", classesDir));
        Assert.assertNotNull(compiled);
    }

    @Test
    public void keepsCallSiteNamesForNamedExtendsThatCollide() throws Exception {
        Generator generator = generate(true,
                "com.example.View*tests_a*10*5*Named*createView***",
                "com.example.View*tests_b*12*5*Named*createView***",
                "com.example.View*tests_c*1*1*Unique*createView***",
                "com.example.View*tests_d***NoLocation*createView***");

        List<String> files = generatedFiles("com/tns/gen/com/example");
        Assert.assertEquals(4, files.size());
        Assert.assertTrue(files.toString(), files.stream().anyMatch(f -> f.startsWith("View_tests_a_") && f.endsWith("_10_5_Named.java")));
        Assert.assertTrue(files.toString(), files.stream().anyMatch(f -> f.startsWith("View_tests_b_") && f.endsWith("_12_5_Named.java")));
        Assert.assertTrue(files.contains("View_Unique_" + key("createView") + ".java"));
        Assert.assertTrue(files.contains("View_NoLocation_" + key("createView") + ".java"));

        String collision = "com.tns.gen.com.example.View_Named_" + key("createView");
        Assert.assertEquals(Collections.singleton(collision), generator.getContentNameCollisions());
        Assert.assertEquals(1, generator.getWarnings().size());
        Assert.assertTrue(generator.getWarnings().get(0), generator.getWarnings().get(0).contains("NoLocation"));

        String marker = read("com/tns/gen/ContentKeyedBindings.java");
        Assert.assertTrue(marker.startsWith(Generator.AUTO_GENERATED_FILE_PROLOGUE));
        Class<?> markerClass = InMemoryJavaCompiler.compile("com.tns.gen.ContentKeyedBindings", marker, Arrays.asList("-cp", classesDir));
        Assert.assertArrayEquals(new String[]{collision}, (String[]) markerClass.getField("COLLISIONS").get(null));
    }

    @Test
    public void warnsAboutFragmentsWithoutAJavaProxyName() throws Exception {
        Generator generator = generate(true,
                "com.example.MyFragment*tests_e*2*3**onResume***",
                "com.example.MyFragment*****onResume*com.example.NamedFragment*app/f.js*",
                "com.example.View*tests_e*5*3**createView***");

        Assert.assertEquals(1, generator.getWarnings().size());
        String warning = generator.getWarnings().get(0);
        Assert.assertTrue(warning, warning.contains("tests_e:2:3") && warning.contains("@JavaProxy"));
    }

    @Test
    public void keepsCallSiteNamesWithoutTheOptIn() throws Exception {
        Generator generator = generate(false,
                "com.example.View*tests_a*3*5**createView***",
                "com.example.View*tests_a*9*5*Named*createView***",
                "com.example.MyFragment*tests_e*2*3**onResume***");

        List<String> files = generatedFiles("com/tns/gen/com/example");
        Assert.assertEquals(3, files.size());
        Assert.assertTrue(files.toString(), files.stream().noneMatch(f -> f.contains("_h")));
        Assert.assertFalse(new File(outputDir, "com/tns/gen/ContentKeyedBindings.java").exists());
        Assert.assertTrue(generator.getWarnings().isEmpty());
        Assert.assertTrue(generator.getContentNameCollisions().isEmpty());
    }

    private void assertNoAccessorMarkers() throws IOException {
        try (Stream<Path> paths = Files.walk(outputDir.toPath())) {
            for (Path file : paths.filter(p -> p.toString().endsWith(".java")).collect(Collectors.toList())) {
                String source = new String(Files.readAllBytes(file), StandardCharsets.UTF_8);
                Assert.assertFalse(file.toString(), source.contains(DataRow.ACCESSOR_PREFIX));
            }
        }
    }

    @Test
    public void overridesAccessorsWithoutTheOptIn() throws Exception {
        generate(false,
                "com.example.View*tests_a*3*5**~createView,~init***",
                "com.example.View*****toString,~createView*com.example.NamedView*app/f.js*",
                "com.example.MyInterface*tests_a*12*5**~getTile***");
        assertNoAccessorMarkers();

        List<String> files = generatedFiles("com/tns/gen/com/example");
        Assert.assertEquals(files.toString(), 2, files.size());
        String anonymous = read("com/tns/gen/com/example/" + files.stream().filter(f -> f.startsWith("View_")).findFirst().get());
        Assert.assertTrue(anonymous, anonymous.contains("createView("));
        Assert.assertTrue(anonymous, anonymous.contains("\"init\""));

        String named = read("com/example/NamedView.java");
        Assert.assertTrue(named, named.contains("createView("));
        Assert.assertTrue(read("com/tns/gen/com/example/MyInterface.java").contains("getTile("));
    }

    @Test
    public void leavesAccessorsOutOfContentKeyedBindings() throws Exception {
        generate(true,
                "com.example.View*tests_a*3*5**~createView,~init,toString***",
                "com.example.View*****toString,~createView*com.example.NamedView*app/f.js*",
                "com.example.MyInterface*tests_a*12*5**~getTile***");
        assertNoAccessorMarkers();

        String name = "View_" + key("toString");
        Assert.assertEquals(Arrays.asList("MyInterface.java", name + ".java"), generatedFiles("com/tns/gen/com/example"));
        String anonymous = read("com/tns/gen/com/example/" + name + ".java");
        Assert.assertFalse(anonymous, anonymous.contains("createView("));
        Assert.assertFalse(anonymous, anonymous.contains("\"init\""));

        // Only content-keyed classes leave accessors out.
        Assert.assertTrue(read("com/example/NamedView.java").contains("createView("));
        // An interface's abstract methods are implemented whatever the object defines.
        Assert.assertTrue(read("com/tns/gen/com/example/MyInterface.java").contains("getTile("));
    }

    @Test
    public void sharesIdenticalDuplicatesAndRejectsDifferentOnes() throws Exception {
        generate(false,
                "com.example.View*tests_a*3*5**createView***",
                "com.example.View*tests_a*3*5**createView***");
        Assert.assertEquals(1, generatedFiles("com/tns/gen/com/example").size());

        try {
            generate(false,
                    "com.example.View*tests_a*3*5**createView***",
                    "com.example.View*tests_a*3*5**createView,toString***");
            Assert.fail("expected the generator to reject two different classes with one name");
        } catch (IOException e) {
            Assert.assertTrue(e.getMessage(), e.getMessage().contains("tests_a:3:5") && e.getMessage().contains("different contents"));
        }
    }
}
