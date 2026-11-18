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
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.List;

import javax.tools.ToolProvider;

/**
 * Kotlin gives a class whose constructor has default arguments a public synthetic
 * constructor taking an extra {@code int} mask and a {@code DefaultConstructorMarker}.
 */
public class SyntheticConstructorsTest {

    private static final String MARKER = "kotlin/jvm/internal/DefaultConstructorMarker";

    @Rule
    public TemporaryFolder temp = new TemporaryFolder();

    private String classesDir;
    private File kotlinDir;
    private File outputDir;

    @Before
    public void setUp() throws Exception {
        InputParameters.parseCommand(new String[0]);
        URL u = View.class.getResource('/' + View.class.getName().replace('.', '/') + ".class");
        classesDir = new File(u.toURI()).getParentFile().getParentFile().getParentFile().getAbsolutePath();
        outputDir = temp.newFolder("out");
        kotlinDir = temp.newFolder("kotlin-classes");

        write(MARKER, markerClass());
        write("kt/DefaultArgs", kotlinStyleClass("kt/DefaultArgs", Opcodes.ACC_PUBLIC));
        write("kt/OnlySynthetic", kotlinStyleClass("kt/OnlySynthetic", Opcodes.ACC_PRIVATE));
    }

    @After
    public void tearDown() {
        InputParameters.parseCommand(new String[0]);
    }

    private void write(String internalName, byte[] bytes) throws IOException {
        File file = new File(kotlinDir, internalName + ".class");
        file.getParentFile().mkdirs();
        Files.write(file.toPath(), bytes);
    }

    private static byte[] markerClass() {
        ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
        cw.visit(Opcodes.V1_8, Opcodes.ACC_PUBLIC | Opcodes.ACC_FINAL | Opcodes.ACC_SUPER, MARKER, null, "java/lang/Object", null);
        MethodVisitor ctor = cw.visitMethod(Opcodes.ACC_PRIVATE, "<init>", "()V", null, null);
        ctor.visitCode();
        ctor.visitVarInsn(Opcodes.ALOAD, 0);
        ctor.visitMethodInsn(Opcodes.INVOKESPECIAL, "java/lang/Object", "<init>", "()V", false);
        ctor.visitInsn(Opcodes.RETURN);
        ctor.visitMaxs(0, 0);
        ctor.visitEnd();
        cw.visitEnd();
        return cw.toByteArray();
    }

    /** {@code open class X(name: String = "") { open fun describe(): String? }}, with the given primary constructor access. */
    private static byte[] kotlinStyleClass(String internalName, int primaryAccess) {
        ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
        cw.visit(Opcodes.V1_8, Opcodes.ACC_PUBLIC | Opcodes.ACC_SUPER, internalName, null, "java/lang/Object", null);

        MethodVisitor primary = cw.visitMethod(primaryAccess, "<init>", "(Ljava/lang/String;)V", null, null);
        primary.visitCode();
        primary.visitVarInsn(Opcodes.ALOAD, 0);
        primary.visitMethodInsn(Opcodes.INVOKESPECIAL, "java/lang/Object", "<init>", "()V", false);
        primary.visitInsn(Opcodes.RETURN);
        primary.visitMaxs(0, 0);
        primary.visitEnd();

        MethodVisitor defaults = cw.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_SYNTHETIC, "<init>", "(Ljava/lang/String;IL" + MARKER + ";)V", null, null);
        defaults.visitCode();
        defaults.visitVarInsn(Opcodes.ALOAD, 0);
        defaults.visitVarInsn(Opcodes.ALOAD, 1);
        defaults.visitMethodInsn(Opcodes.INVOKESPECIAL, internalName, "<init>", "(Ljava/lang/String;)V", false);
        defaults.visitInsn(Opcodes.RETURN);
        defaults.visitMaxs(0, 0);
        defaults.visitEnd();

        MethodVisitor describe = cw.visitMethod(Opcodes.ACC_PUBLIC, "describe", "()Ljava/lang/String;", null, null);
        describe.visitCode();
        describe.visitInsn(Opcodes.ACONST_NULL);
        describe.visitInsn(Opcodes.ARETURN);
        describe.visitMaxs(0, 0);
        describe.visitEnd();

        cw.visitEnd();
        return cw.toByteArray();
    }

    private void generate(String... rows) throws Exception {
        File rowsFile = temp.newFile();
        Files.write(rowsFile.toPath(), Arrays.asList(rows), StandardCharsets.UTF_8);
        List<DataRow> libs = Arrays.asList(new DataRow(classesDir), new DataRow(kotlinDir.getAbsolutePath()));
        new Generator(outputDir, libs).writeBindings(rowsFile.getAbsolutePath());
    }

    @Test
    public void leavesSyntheticConstructorsOutOfTheBinding() throws Exception {
        generate("kt.DefaultArgs*****describe*com.x.Described*app/f.js*");

        File file = new File(outputDir, "com/x/Described.java");
        String source = new String(Files.readAllBytes(file.toPath()), StandardCharsets.UTF_8);
        Assert.assertFalse(source, source.contains("DefaultConstructorMarker"));
        Assert.assertTrue(source, source.contains("public Described(java.lang.String"));

        File compiledDir = temp.newFolder("compiled");
        int exitCode = ToolProvider.getSystemJavaCompiler().run(null, null, null,
                "-proc:none", "-nowarn", "-d", compiledDir.getAbsolutePath(),
                "-cp", classesDir + File.pathSeparator + kotlinDir.getAbsolutePath(),
                file.getAbsolutePath());
        Assert.assertEquals("javac rejected the binding", 0, exitCode);
    }

    @Test
    public void doesNotExtendAClassWhoseOnlyAccessibleConstructorIsSynthetic() throws Exception {
        generate("kt.OnlySynthetic*****describe*com.x.OnlySyntheticChild*app/f.js*");

        Assert.assertFalse(new File(outputDir, "com/x/OnlySyntheticChild.java").exists());
    }
}
