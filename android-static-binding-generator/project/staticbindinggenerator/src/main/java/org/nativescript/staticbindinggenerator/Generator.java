package org.nativescript.staticbindinggenerator;

import com.tns.bindings.AnnotationDescriptor;
import com.tns.bindings.ProxyGenerator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.jar.JarInputStream;
import java.util.zip.ZipEntry;

import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.JavaClass;

public class Generator {
    private static final String CLASS_EXT = ".class";

    private final String outputDir;
    private final String[] libs;
    private final Map<String, JavaClass> classes;

    public Generator(String outputDir, String[] libs) throws IOException {
        this(outputDir, libs, false);
    }

    public Generator(String outputDir, String[] libs, boolean throwOnError) throws IOException {
        this.outputDir = outputDir;
        this.libs = libs;
        this.classes = readClasses(libs, throwOnError);
    }

    public void writeBindings(String filename) throws IOException {
        List<DataRow> rows = getRows(filename);

        for (DataRow row : rows) {
            String javaClassname = row.getBaseClassname();
            boolean isJavaExtend = classes.containsKey(javaClassname);
            if (isJavaExtend) {
                ProxyGenerator proxyGenerator = new ProxyGenerator(outputDir);

                String[] methodOverrides = row.getMethods();
                JavaClass clazz = classes.get(javaClassname);
                ClassInfo classInfo = new ClassInfo(clazz, this);

                String dexFilename;
                boolean hasSpecifiedName = !row.getFilename().isEmpty();
                if (hasSpecifiedName) {
                    dexFilename = row.getFilename();
                } else {
                    dexFilename = getSimpleClassname(clazz.getClassName());
                    if (!clazz.isInterface()) {
                        dexFilename += row.getSuffix();
                    }
                }

                AnnotationDescriptor[] annotations = null;
                if (hasSpecifiedName) {
                    annotations = new AnnotationDescriptor[] {
                        new AnnotationDescriptor("Lcom/tns/JavaScriptImplementation;", new AnnotationDescriptor.Parameter[]{new AnnotationDescriptor.Parameter("javaScriptFile", "./" + row.getJsFilename())}, true)
                    };
                }

                String dexFile = proxyGenerator.generateProxy(dexFilename, classInfo, methodOverrides, clazz.isInterface(), annotations);
                //System.out.println(dexFile);
            }
        }
    }

    Map<String, JavaClass> getClasses() {
        return classes;
    }

    private String getSimpleClassname(String classname) {
        int idx = classname.lastIndexOf('.');
        String name = classname.substring(idx + 1, classname.length()).replace("$", "_");
        return name;
    }

    private List<DataRow> getRows(String filename) throws IOException {
        List<DataRow> rows = new ArrayList<DataRow>();
        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream(filename)));
            String line;
            while ((line = br.readLine()) != null) {
                DataRow row = new DataRow(line);
                rows.add(row);
            }
        } finally {
            if (br != null) {
                br.close();
            }
        }
        return rows;
    }

    private Map<String, JavaClass> readClasses(String[] libs, boolean throwOnError) throws FileNotFoundException, IOException {
        Map<String, JavaClass> map = new HashMap<String, JavaClass>();
        if (libs != null) {
            for (String lib : libs) {
                File f = new File(lib);
                Map<String, JavaClass> classes = f.isFile() ? readJar(lib, throwOnError) : readDir(lib, throwOnError);
                map.putAll(classes);
            }
        }
        return map;
    }

    private Map<String, JavaClass> readJar(String path, boolean throwOnError) throws FileNotFoundException, IOException {
        Map<String, JavaClass> classes = new HashMap<String, JavaClass>();
        JarInputStream jis = null;
        try {
            String name = null;
            jis = new JarInputStream(new FileInputStream(path));
            for (ZipEntry ze = jis.getNextEntry(); ze != null; ze = jis.getNextEntry()) {
                try {
                    name = ze.getName();
                    if (name.endsWith(CLASS_EXT)) {
                        name = name.substring(0, name.length() - CLASS_EXT.length()).replace('/', '.').replace('$', '.');
                        ClassParser cp = new ClassParser(jis, name);
                        JavaClass clazz = cp.parse();
                        classes.put(name, clazz);
                    }
                } catch (Exception e) {
                    if (throwOnError) {
                        throw new RuntimeException(e);
                    }
                }
            }
        } finally {
            if (jis != null) {
                jis.close();
            }
        }
        return classes;
    }

    private Map<String, JavaClass> readDir(String path, boolean throwOnError) throws FileNotFoundException, IOException {
        Map<String, JavaClass> classes = new HashMap<String, JavaClass>();

        ArrayDeque<File> d = new ArrayDeque<File>();
        d.add(new File(path));

        while (!d.isEmpty()) {
            File cur = d.pollFirst();
            File[] files = cur.listFiles();
            for (File f : files) {
                if (f.isFile() && f.getName().endsWith(CLASS_EXT)) {
                    ClassParser cp = new ClassParser(f.getAbsolutePath());
                    JavaClass clazz = cp.parse();
                    String name = clazz.getClassName();
                    name = name.replace('/', '.').replace('$', '.');
                    classes.put(name, clazz);
                } else if (f.isDirectory()) {
                    d.addLast(f);
                }
            }
        }

        return classes;
    }
}
