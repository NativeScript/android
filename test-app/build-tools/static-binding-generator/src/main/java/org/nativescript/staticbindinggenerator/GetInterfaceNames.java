package org.nativescript.staticbindinggenerator;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class GetInterfaceNames {
    private static String currentDir;

    /*
    * generate interfaceNames.txt file, needed for js analyzer
    * */
    public static void generateInterfaceFile(List<DataRow> rows)
    throws IOException, ClassNotFoundException {
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

//        for (DataRow dr : rows) {
//            String pathToDependency = dr.getRow();
//            if (pathToDependency.endsWith(".jar")) {
//                generateInterfaceNames(pathToDependency, interfacesList);
//            }
//        }

        for(String line: interfacesList) {
            out.println(line);
        }

        out.close();
    }

    private static void generateInterfaceNames(String pathToJar, List<String> interfacesList) throws IOException, ClassNotFoundException {
        if (pathToJar == null) {
            return;
        }

        JarFile jarFile = new JarFile(pathToJar);
        Enumeration<JarEntry> currentJarFile = jarFile.entries();

        URLClassLoader cl = getClassLoader(pathToJar);

        while (currentJarFile.hasMoreElements()) {
            JarEntry jarEntry = currentJarFile.nextElement();

            if ((!jarEntry.isDirectory()) && (jarEntry.getName().endsWith(".class"))) {
                String className = jarEntry.getName().substring(0, jarEntry.getName().length() - 6);
                className = className.replace('/', '.');

                @SuppressWarnings("rawtypes")
                Class c = null;
                try {
                    c = cl.loadClass(className);
                } catch (IllegalAccessError e) {
                } catch (NoClassDefFoundError localNoClassDefFoundError) {
                }
                if ((c != null) && (c.isInterface() == true)) {
                    String res = c.getName().replace('$', '.');
                    interfacesList.add(res);
                }
            }
        }
        jarFile.close();
    }

    public static PrintWriter ensureOutputFile(String outputFileName) throws IOException {
        File checkFile = new File(currentDir, outputFileName);
        if (checkFile.exists()) {
            checkFile.delete();
        } else {
            checkFile.getParentFile().mkdirs();
            checkFile.createNewFile();
        }

        PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(checkFile.getAbsolutePath(), true)));
        return out;
    }

    private static URLClassLoader getClassLoader(String pathToJar) throws MalformedURLException {
        URL[] urls = { new URL("jar:file:" + pathToJar + "!/") };
        URLClassLoader cl = URLClassLoader.newInstance(urls);
        return cl;
    }
}