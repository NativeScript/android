package com.tns;

import android.util.Log;

import com.tns.bindings.AnnotationDescriptor;
import com.tns.bindings.ProxyGenerator;
import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;
import com.tns.system.classes.loading.ClassStorageService;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.InvalidClassException;
import java.io.OutputStreamWriter;
import java.util.HashMap;
import java.util.HashSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import dalvik.system.DexClassLoader;

public class DexFactory {
    private static final String COM_TNS_GEN_PREFIX = "com.tns.gen.";

    private final Logger logger;
    private final File dexDir;
    private final File odexDir;
    private final String dexThumb;
    private final ClassLoader classLoader;
    private final ClassStorageService classStorageService;

    private ProxyGenerator proxyGenerator;
    private HashMap<String, Class<?>> injectedDexClasses = new HashMap<String, Class<?>>();

    DexFactory(Logger logger, ClassLoader classLoader, File dexBaseDir, String dexThumb, ClassStorageService classStorageService) {
        this.logger = logger;
        this.classLoader = classLoader;
        this.dexDir = dexBaseDir;
        this.dexThumb = dexThumb;

        this.odexDir = new File(this.dexDir, "odex");
        this.proxyGenerator = new ProxyGenerator(this.dexDir.getAbsolutePath());
        ProxyGenerator.IsLogEnabled = logger.isEnabled();

        if (!dexDir.exists()) {
            dexDir.mkdirs();
        }

        if (!odexDir.exists()) {
            odexDir.mkdir();
        }

        this.updateDexThumbAndPurgeCache();
        this.proxyGenerator.setProxyThumb(this.dexThumb);
        this.classStorageService = classStorageService;
    }

    static long totalGenTime = 0;
    static long totalMultiDexTime = 0;
    static long totalLoadDexTime = 0;

    public Class<?> resolveClass(String baseClassName, String name, String className, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) throws ClassNotFoundException, IOException {
        String fullClassName = className.replace("$", "_");
        String originalFullClassName = fullClassName;

        // try to get pre-generated binding classes
        try {
            if (logger.isEnabled()) {
                logger.write("getting pre-generated proxy class with name:  " + fullClassName.replace("-", "_"));
            }

            Class<?> pregeneratedClass = classLoader.loadClass(fullClassName.replace("-", "_"));

            if (logger.isEnabled()) {
                logger.write("Pre-generated class found:  " + fullClassName.replace("-", "_"));
            }

            return pregeneratedClass;
        } catch (Exception e) {
            if (logger.isEnabled()) {
                logger.write("Pre-generated class not found:  " + fullClassName.replace("-", "_"));
            }
        }
        //

        // new: com.tns.gen.android.widget.DatePicker_MyActivity_59_56_
        // old: com.tns.tests.Button1_fMyActivity_l56_c44__MyButton
        // ne1: com.tns.tests.Button1_MyActivity_58_44_MyButton_0
        Class<?> existingClass = this.injectedDexClasses.get(fullClassName);
        if (existingClass != null) {
            return existingClass;
        }

        String classToProxy;
        if (!baseClassName.isEmpty()) {
            classToProxy = this.getClassToProxyName(baseClassName);
        } else {
            classToProxy = this.getClassToProxyName(className);
        }

        // strip the `com.tns.gen` off the base extended class name
        String desiredDexClassName = this.getClassToProxyName(fullClassName);

        // when interfaces are extended as classes, we still want to preserve
        // just the interface name without the extra file, line, column information
        if (!baseClassName.isEmpty() && isInterface) {
            fullClassName = COM_TNS_GEN_PREFIX + classToProxy;
        }

        File dexFile = this.getDexFile(desiredDexClassName);

        // generate dex file
        if (dexFile == null) {
            long startGenTime = System.nanoTime();
            if (logger.isEnabled()) {
                logger.write("generating proxy in place");
            }

            String dexFilePath;
            if (isInterface) {
                dexFilePath = this.generateDex(name, classToProxy, methodOverrides, implementedInterfaces, isInterface);
            } else {
                dexFilePath = this.generateDex(desiredDexClassName, classToProxy, methodOverrides, implementedInterfaces, isInterface);
            }
            dexFile = new File(dexFilePath);
            long stopGenTime = System.nanoTime();
            totalGenTime += stopGenTime - startGenTime;
            if (logger.isEnabled()) {
                logger.write("Finished inplace gen took: " + (stopGenTime - startGenTime) / 1000000.0 + "ms");
                logger.write("TotalGenTime:  " + totalGenTime / 1000000.0 + "ms");
            }
        }

        // creates jar file from already generated dex file
        String jarFilePath = dexFile.getPath().replace(".dex", ".jar");
        File jarFile = new File(jarFilePath);

        if (!jarFile.exists()) {
            FileOutputStream jarFileStream = new FileOutputStream(jarFile);
            ZipOutputStream out = new ZipOutputStream(jarFileStream);

            out.putNextEntry(new ZipEntry("classes.dex"));
            byte[] dexData = new byte[(int) dexFile.length()];
            FileInputStream fi = new FileInputStream(dexFile);
            fi.read(dexData, 0, dexData.length);
            fi.close();

            out.write(dexData);
            out.closeEntry();
            out.close();
        }

        Class<?> result;
        DexClassLoader dexClassLoader = new DexClassLoader(jarFilePath, this.odexDir.getAbsolutePath(), null, classLoader);

        if (isInterface) {
            result = dexClassLoader.loadClass(fullClassName);
        } else {
            result = dexClassLoader.loadClass(desiredDexClassName);
        }

        classStorageService.storeClass(result.getName(), result);
        this.injectedDexClasses.put(originalFullClassName, result);

        return result;
    }

    public Class<?> findClass(String className) throws ClassNotFoundException {
        String canonicalName = className.replace('/', '.');
        if (logger.isEnabled()) {
            logger.write(canonicalName);
        }

        Class<?> existingClass = this.injectedDexClasses.get(canonicalName);
        if (existingClass != null) {
            return existingClass;
        }

        return classLoader.loadClass(canonicalName);
    }

    public static String strJoin(String[] array, String separator) {
        if (array == null) {
            return "";
        }

        StringBuilder sbStr = new StringBuilder();
        for (int i = 0, il = array.length; i < il; i++) {
            if (i > 0) {
                sbStr.append(separator);
            }
            sbStr.append(array[i]);
        }
        return sbStr.toString();
    }

    private String getClassToProxyName(String className) throws InvalidClassException {
        String classToProxy = className;

        if (className.startsWith(COM_TNS_GEN_PREFIX)) {
            classToProxy = className.substring(12);
        }

        if (classToProxy.startsWith(COM_TNS_GEN_PREFIX)) {
            throw new InvalidClassException("Can't generate proxy of proxy");
        }

        return classToProxy;
    }

    private File getDexFile(String className) throws InvalidClassException {
        String classToProxyFile = className.replace("$", "_");

        if (this.dexThumb != null) {
            classToProxyFile += "-" + this.dexThumb;
        }

        String dexFilePath = dexDir + "/" + classToProxyFile + ".dex";
        File dexFile = new File(dexFilePath);
        if (dexFile.exists()) {
            if (logger.isEnabled()) {
                logger.write("Looking for proxy file: " + dexFilePath + " Result: proxy file Found. ClassName: " + className);
            }
            return dexFile;
        }

        if (logger.isEnabled()) {
            logger.write("Looking for proxy file: " + dexFilePath + " Result: NOT Found. Proxy Gen needed. ClassName: " + className);
        }

        return null;
    }

    private String generateDex(String proxyName, String className, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) throws ClassNotFoundException, IOException {
        Class<?> classToProxy = Class.forName(className);

        HashSet<String> methodOverridesSet = null;
        HashSet<ClassDescriptor> implementedInterfacesSet = new HashSet<ClassDescriptor>();

        if (methodOverrides != null) {
            methodOverridesSet = new HashSet<String>();
            for (int i = 0; i < methodOverrides.length; i++) {
                String methodOverride = methodOverrides[i];

                methodOverridesSet.add(methodOverride);
            }
        }

        if (implementedInterfaces.length > 0) {
            for (int j = 0; j < implementedInterfaces.length; j++) {
                if (!implementedInterfaces[j].isEmpty()) {
                    implementedInterfacesSet.add(new ClassInfo(Class.forName(implementedInterfaces[j])));
                }
            }
        }

        AnnotationDescriptor[] annotations = null;
        return proxyGenerator.generateProxy(proxyName, new ClassInfo(classToProxy), methodOverridesSet, implementedInterfacesSet, isInterface, annotations);
    }

    private void updateDexThumbAndPurgeCache() {
        if (this.dexThumb == null) {
            throw new RuntimeException("Error generating proxy thumb 1");
        }

        String oldDexThumb = this.getCachedProxyThumb(this.dexDir);
        if (this.dexThumb.equals(oldDexThumb)) {
            return;
        }

        if (oldDexThumb != null) {
            this.purgeDexesByThumb(oldDexThumb, this.dexDir);
            this.purgeDexesByThumb(oldDexThumb, this.odexDir);
        } else {
            // purge all dex files if no thumb file is found. This is crucial for CLI livesync
            purgeAllProxies();
        }

        this.saveNewDexThumb(this.dexThumb, this.dexDir);
    }

    public void purgeAllProxies() {
        this.purgeDexesByThumb(null, this.dexDir);
        this.purgeDexesByThumb(null, this.odexDir);
    }

    private void saveNewDexThumb(String newDexThumb, File dexDir) {
        File cachedThumbFile = new File(dexDir, "proxyThumb");
        try {
            FileOutputStream out = new FileOutputStream(cachedThumbFile, false);
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
            try {
                writer.write(newDexThumb);
                writer.newLine();
                writer.flush();
            } finally {
                writer.close();
                out.close();
            }
        } catch (FileNotFoundException e) {
            Log.w("JS", String.format("Error while writing current proxy thumb: %s", e.getMessage()));
            if (com.tns.Runtime.isDebuggable()) {
                e.printStackTrace();
            }
        } catch (IOException e) {
            Log.w("JS", String.format("Error while writing current proxy thumb: %s", e.getMessage()));
            if (com.tns.Runtime.isDebuggable()) {
                e.printStackTrace();
            }
        }
    }

    private void purgeDexesByThumb(String cachedDexThumb, File pathToPurge) {
        if (!pathToPurge.exists()) {
            return;
        }

        if (!pathToPurge.isDirectory()) {
            logger.write("Purge proxies path not a directory. Path: " + pathToPurge);
            throw new RuntimeException("Purge path not a directory");
        }

        String[] children = pathToPurge.list();

        for (int i = 0; i < children.length; i++) {
            String filename = children[i];
            File purgeCandidate = new File(pathToPurge, filename);
            if (purgeCandidate.isDirectory()) {
                this.purgeDexesByThumb(cachedDexThumb, purgeCandidate);
            } else {
                if (cachedDexThumb != null && !filename.contains(cachedDexThumb)) {
                    continue;
                }

                if (!purgeCandidate.delete()) {
                    logger.write("Error purging cached proxy file: " + purgeCandidate.getAbsolutePath());
                }
            }
        }
    }

    private String getCachedProxyThumb(File proxyDir) {
        try {
            File cachedThumbFile = new File(proxyDir, "proxyThumb");
            if (cachedThumbFile.exists()) {
                FileInputStream in = new FileInputStream(cachedThumbFile);
                BufferedReader reader = new BufferedReader(new InputStreamReader(in));
                String cachedThumb = reader.readLine();
                reader.close();
                in.close();
                return cachedThumb;
            }
        } catch (FileNotFoundException e) {
            Log.w("JS", String.format("Error while writing current proxy thumb: %s", e.getMessage()));
            if (com.tns.Runtime.isDebuggable()) {
                e.printStackTrace();
            }
        } catch (IOException e) {
            Log.w("JS", String.format("Error while writing current proxy thumb: %s", e.getMessage()));
            if (com.tns.Runtime.isDebuggable()) {
                e.printStackTrace();
            }
        }

        return null;
    }
}
