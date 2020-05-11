package com.telerik.metadata;

import com.telerik.metadata.analytics.AnalyticsCollector;
import com.telerik.metadata.analytics.AnalyticsCollectorProvider;
import com.telerik.metadata.parsing.NativeClassDescriptor;
import com.telerik.metadata.parsing.java.classes.JavaClassDescriptor;
import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor;
import com.telerik.metadata.parsing.kotlin.metadata.ClassMetadataParser;
import com.telerik.metadata.parsing.kotlin.metadata.MetadataAnnotation;
import com.telerik.metadata.parsing.kotlin.metadata.bytecode.BytecodeClassMetadataParser;
import com.telerik.metadata.parsing.kotlin.metadata.bytecode.BytecodeMetadataAnnotation;

import org.apache.bcel.classfile.AnnotationEntry;
import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.JavaClass;

import java.io.File;
import java.io.IOException;
import java.util.*;

public class ClassDirectory implements ClassMapProvider {
    private final String path;
    private final Map<String, NativeClassDescriptor> classMap;
    private static final ClassMetadataParser kotlinClassMetadataParser = new BytecodeClassMetadataParser();
    private static final AnalyticsCollector analyticsCollector = AnalyticsCollectorProvider.getInstance().provideAnalyticsCollector();
    private static final String CLASS_EXT = ".class";
    private static final String DEX_EXT = ".dex";

    private ClassDirectory(String path) {
        this.path = path;
        this.classMap = new HashMap<>();
    }

    public Map<String, NativeClassDescriptor> getClassMap() {
        return classMap;
    }

    public String getPath() {
        return path;
    }

    public static ClassDirectory readDirectory(String path) throws IOException {
        ClassDirectory dir = new ClassDirectory(path);
        readDirectory(dir, path);
        return dir;
    }

    private static void readDirectory(ClassDirectory dir, String path)
            throws IOException {
        List<File> subDirs = new ArrayList<>();
        File currentDir = new File(path);
        for (File file : Objects.requireNonNull(currentDir.listFiles())) {
            if (file.isFile()) {
                String name = file.getName();
                if (name.endsWith(CLASS_EXT)) {
                    NativeClassDescriptor clazz = getClassDescriptor(name, file);
                    dir.classMap.put(clazz.getClassName(), clazz);
                }
            } else if (file.isDirectory()) {
                subDirs.add(file);
            }
        }
        for (File sd : subDirs) {
            readDirectory(dir, sd.getAbsolutePath());
        }
    }

    private static NativeClassDescriptor getClassDescriptor(String name, File file) throws IOException {
        NativeClassDescriptor clazz = null;
        if (name.endsWith(CLASS_EXT)) {
            ClassParser cp = new ClassParser(file.getAbsolutePath());
            JavaClass javaClass = cp.parse();

            AnnotationEntry[] annotationEntries = javaClass.getAnnotationEntries();
            if (annotationEntries != null) {
                for (AnnotationEntry annotationEntry : annotationEntries) {
                    String annotationType = annotationEntry.getAnnotationType();
                    if ("Lkotlin/Metadata;".equals(annotationType)) {
                        MetadataAnnotation kotlinClassMetadataAnnotation = new BytecodeMetadataAnnotation(annotationEntry);
                        NativeClassDescriptor kotlinClassDescriptor = new KotlinClassDescriptor(javaClass, kotlinClassMetadataAnnotation);
                        analyticsCollector.markHasKotlinRuntimeClassesIfNotMarkedAlready();
                        return kotlinClassDescriptor;
                    }
                }
            }

            return new JavaClassDescriptor(javaClass);
        }

        return clazz;
    }
}
