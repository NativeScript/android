package org.nativescript.staticbindinggenerator.files.impl;

import org.apache.bcel.classfile.Attribute;
import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.InnerClass;
import org.apache.bcel.classfile.InnerClasses;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Utility;
import org.nativescript.staticbindinggenerator.Generator;
import org.nativescript.staticbindinggenerator.files.FileSystemHelper;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.jar.JarInputStream;
import java.util.zip.ZipEntry;

public class FileSystemHelperImpl implements FileSystemHelper {

    private static final String CLASS_EXT = ".class";

    private final boolean shouldThrowOnError;

    public FileSystemHelperImpl(boolean shouldThrowOnError) {
        this.shouldThrowOnError = shouldThrowOnError;
    }


    @Override
    public ClassesCollection readClassesFromJar(String jarPath) {
        Map<String, JavaClass> classes = new HashMap<String, JavaClass>();
        Set<String> nonPublicNestedClasses = new HashSet<>();

        JarInputStream jis = null;
        try {
            String name = null;
            jis = new JarInputStream(new FileInputStream(jarPath));
            for (ZipEntry ze = jis.getNextEntry(); ze != null; ze = jis.getNextEntry()) {
                try {
                    name = ze.getName();
                    if (name.endsWith(CLASS_EXT)) {
                        name = name.substring(0, name.length() - CLASS_EXT.length()).replace('/', '.').replace('$', '.');
                        ClassParser cp = new ClassParser(jis, name);
                        JavaClass clazz = cp.parse();
                        Set<String> currentClassNonPublicNestedClasses = collectNonPublicNestedClasses(clazz);
                        classes.put(name, clazz);
                        nonPublicNestedClasses.addAll(currentClassNonPublicNestedClasses);
                    }
                } catch (IOException e) {
                    if (shouldThrowOnError) {
                        throw new RuntimeException("Error while parsing class file!", e);
                    } else {
                        e.printStackTrace();
                    }
                }
            }
        } catch (IOException ioe) {
            if (shouldThrowOnError) {
                throw new RuntimeException("Error while reading JAR entry!", ioe);
            } else {
                ioe.printStackTrace();
            }
        } finally {
            if (jis != null) {
                try {
                    jis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return new ClassesCollection(classes, nonPublicNestedClasses);
    }

    private Set<String> collectNonPublicNestedClasses(JavaClass javaClass) {
        if(javaClass.getClassName().contains("MediaBrowserServiceCompat")){
            int asddsa = 5;
        }

        Set<String> foundInners = new HashSet<>();
        Attribute attrs[] = javaClass.getAttributes();
        for (int i = 0; i < attrs.length; i++) {
            // Find the InnerClasses attribute, if any.
            if (attrs[i] instanceof InnerClasses) {
                // The InnerClasses attribute is found.
                InnerClasses innerClasses = (InnerClasses) attrs[i];

                // Get an array of the inner classes.
                InnerClass inners[] = innerClasses.getInnerClasses();
                for (int j = 0; j < inners.length; j++) {
                    InnerClass innerClass = inners[j];
                    int accessFlags = innerClass.getInnerAccessFlags();
                    if ((accessFlags & 0x0001) == 0 && (accessFlags & 0x0004) == 0) { // magic numbers - TOP, trololo
                        // Get the inner class name from a constant pool.
                        String innerClassName = Utility.compactClassName(
                                innerClasses.getConstantPool().getConstantString(
                                        innerClass.getInnerClassIndex(),
                                        (byte) 7), //Constants.CONSTANT_Class
                                false);

                        // The inner class has the InnerClasses attribute as well
                        // as its outer class. So, we should ignore such an inner
                        // class.
                        if (!innerClassName.equals(javaClass.getClassName())) {
                            foundInners.add(BcelNamingUtil.resolveClassName(innerClassName));
                        }
                    }
                }
                break;
            }
        }

        return foundInners;
    }

    @Override
    public ClassesCollection readClassesFromDirectory(String directoryPath) {
        Map<String, JavaClass> classes = new HashMap<String, JavaClass>();
        Set<String> nonPublicNestedClasses = new HashSet<>();

        ArrayDeque<File> d = new ArrayDeque<File>();
        d.add(new File(directoryPath));

        while (!d.isEmpty()) {
            File cur = d.pollFirst();
            File[] files = cur.listFiles();
            for (File f : files) {
                if (f.isFile() && f.getName().endsWith(CLASS_EXT)) {
                    ClassParser cp = new ClassParser(f.getAbsolutePath());
                    JavaClass clazz = null;
                    try {
                        clazz = cp.parse();
                    } catch (IOException e) {
                        if (shouldThrowOnError) {
                            throw new RuntimeException("Error while parsing class file!", e);
                        } else {
                            e.printStackTrace();
                        }
                    }
                    String name = clazz.getClassName();
                    name = name.replace('/', '.').replace('$', '.');

                    Set<String> currentClassNonPublicNestedClasses = collectNonPublicNestedClasses(clazz);

                    classes.put(name, clazz);
                    nonPublicNestedClasses.addAll(currentClassNonPublicNestedClasses);
                } else if (f.isDirectory()) {
                    d.addLast(f);
                }
            }
        }

        return new ClassesCollection(classes, nonPublicNestedClasses);
    }

    @Override
    public void cleanPreviouslyAutoGeneratedSbgFiles(File folder) {
        // Recursively traverse all files in the output folder and
        // remove them if they were auto-generated from a previous run
        // of the static binding generator. This will ensure that if
        // some javascript files containing native extends are removed,
        // their corresponding java classes will also be removed.
        File[] files = folder.listFiles();
        for (File file : files) {
            if (file.isDirectory()) {
                cleanPreviouslyAutoGeneratedSbgFiles(file);
            } else if ("java".equalsIgnoreCase(getFileExtension(file.toString()))) {
                if (checkIfFileIsGeneratedByTheSbg(file)) {
                    file.delete();
                }
            }
        }
    }

    private String getFileExtension(String filename) {
        String extension = "";

        int i = filename.lastIndexOf('.');
        if (i > 0) {
            extension = filename.substring(i + 1);
        }

        return extension;
    }

    private boolean checkIfFileIsGeneratedByTheSbg(File file) {

        int counter = 0;
        int prologueLength = Generator.AUTO_GENERATED_FILE_PROLOGUE.length();
        StringBuilder sb = new StringBuilder();

        try {
            FileInputStream fis = new FileInputStream(file);
            while (fis.available() > 0 && counter < prologueLength) {
                sb.append((char) fis.read());
                counter += 1;
            }
        } catch (IOException e) {
            throw new RuntimeException("Error while reading the file!", e);
        }

        return sb.toString().equals(Generator.AUTO_GENERATED_FILE_PROLOGUE);
    }
}
