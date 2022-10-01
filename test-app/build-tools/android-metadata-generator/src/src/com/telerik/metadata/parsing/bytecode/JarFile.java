package com.telerik.metadata.parsing.bytecode;

import com.telerik.metadata.Builder;
import com.telerik.metadata.ClassMapProvider;
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
import org.apache.bcel.classfile.ClassFormatException;
import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.JavaClass;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.jar.JarInputStream;
import java.util.zip.ZipEntry;

public class JarFile implements ClassMapProvider {
    private final String path;
    private final Map<String, NativeClassDescriptor> classMap;
    private static final ClassMetadataParser kotlinClassMetadataParser = new BytecodeClassMetadataParser();
    private static final AnalyticsCollector analyticsCollector = AnalyticsCollectorProvider.getInstance().provideAnalyticsCollector();
    private static final String CLASS_EXT = ".class";

    private JarFile(String path) {
        this.path = path;
        this.classMap = new HashMap<>();
    }

    public String getPath() {
        return path;
    }

    public Map<String, NativeClassDescriptor> getClassMap() {
        return classMap;
    }

    public static JarFile readJar(String path) throws ClassFormatException,
            IOException {
        JarFile jar;
        JarInputStream jis = null;
        try {
            jis = new JarInputStream(new FileInputStream(path));

            jar = new JarFile(path);

            for (ZipEntry ze = jis.getNextEntry(); ze != null; ze = jis
                    .getNextEntry()) {
                String name = ze.getName();
                if (name.endsWith(CLASS_EXT)) {
                    name = name
                            .substring(0, name.length() - CLASS_EXT.length())
                            .replace('/', '.');

                    ClassParser cp = new ClassParser(jis, name);
                    JavaClass javaClass = cp.parse();
                    boolean isPackagePrivate = Builder.getIsPackagePrivate(javaClass.getPackageName());
                    boolean isKotlinClass = false;

                    AnnotationEntry[] annotationEntries = javaClass.getAnnotationEntries();
                    if (annotationEntries != null) {
                        for (AnnotationEntry annotationEntry : annotationEntries) {
                            String annotationType = annotationEntry.getAnnotationType();
                            if ("Lkotlin/Metadata;".equals(annotationType)) {
                                MetadataAnnotation kotlinClassMetadataAnnotation = new BytecodeMetadataAnnotation(annotationEntry);
                                NativeClassDescriptor kotlinClassDescriptor = new KotlinClassDescriptor(javaClass, kotlinClassMetadataAnnotation, isPackagePrivate);
                                isKotlinClass = true;
                                analyticsCollector.markHasKotlinRuntimeClassesIfNotMarkedAlready();
                                jar.classMap.put(name, kotlinClassDescriptor);
                            }
                        }
                    }

                    if (!isKotlinClass) {
                        NativeClassDescriptor javaClassDescriptor = new JavaClassDescriptor(javaClass, isPackagePrivate);
                        jar.classMap.put(name, javaClassDescriptor);
                    }

                }
            }
        } finally {
            if (jis != null) {
                jis.close();
            }
        }
        return jar;
    }
}
