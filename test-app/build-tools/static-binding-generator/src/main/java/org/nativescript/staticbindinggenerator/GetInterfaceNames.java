package org.nativescript.staticbindinggenerator;

import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.JavaClass;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.jar.JarInputStream;
import java.util.zip.ZipEntry;

public class GetInterfaceNames {
    private static final String CLASS_EXT = ".class";
    private static String currentDir;

    /*
     * generate interfaceNames.txt file, needed for js analyzer
     * */
    public static void generateInterfaceFile(List<DataRow> rows)
            throws IOException {
        currentDir = System.getProperty("user.dir");
        String outputFileName = Main.SBG_INTERFACE_NAMES;

        PrintWriter out = ensureOutputFile(outputFileName);
        List<String> interfacesList = Collections.synchronizedList(new ArrayList<>());

        final ThrowingConsumer<DataRow> consumer = dr -> {
            String pathToDependency = dr.getRow();
            if (pathToDependency.endsWith(".jar")) {
                generateInterfaceNames(pathToDependency, interfacesList);
            }
        };

        rows.parallelStream().forEach(consumer);

        for (String line : interfacesList) {
            out.println(line);
        }

        out.close();
    }

    private static void generateInterfaceNames(String pathToJar, List<String> interfacesList) {
        if (pathToJar == null) {
            return;
        }
        JarInputStream jis = null;
        try {
            String name;
            jis = new JarInputStream(new FileInputStream(pathToJar));
            for (ZipEntry ze = jis.getNextEntry(); ze != null; ze = jis.getNextEntry()) {
                try {
                    name = ze.getName();
                    if (name.endsWith(CLASS_EXT)) {
                        name = name.substring(0, name.length() - CLASS_EXT.length()).replace('/', '.').replace('$', '.');
                        ClassParser cp = new ClassParser(jis, name);
                        JavaClass clazz = cp.parse();
                        if (clazz.isInterface()) {
                            String res = clazz.getClassName().replace('$', '.');
                            interfacesList.add(res);
                        }
                    }
                } catch (IOException e) {
                    throw new RuntimeException("Error while parsing class file!", e);
                }
            }
        } catch (IOException ioe) {
            throw new RuntimeException("Error while reading JAR entry!", ioe);
        } finally {
            if (jis != null) {
                try {
                    jis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static PrintWriter ensureOutputFile(String outputFileName) throws IOException {
        File checkFile = new File(currentDir, outputFileName);
        if (checkFile.exists()) {
            checkFile.delete();
        } else {
            checkFile.getParentFile().mkdirs();
            checkFile.createNewFile();
        }

        return new PrintWriter(new BufferedWriter(new FileWriter(checkFile.getAbsolutePath(), true)));
    }
}