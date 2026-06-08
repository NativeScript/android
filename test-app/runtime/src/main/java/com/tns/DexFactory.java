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
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.HashSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import dalvik.system.BaseDexClassLoader;
import dalvik.system.DexClassLoader;

public class DexFactory {
    private static final String COM_TNS_GEN_PREFIX = "com.tns.gen.";

    // Generated proxy classes always live under names where the original
    // `$` separators in inner-class qualifiers have been replaced with
    // `_`. The proxy generator writes the dex with `_`, so any load via
    // `classLoader.loadClass(...)` must use the same form. This helper
    // is the single canonical entry point for that normalization so
    // every load site agrees.
    //
    // Scoped strictly to the `com.tns.gen.` prefix; unrelated
    // inner-class lookups (e.g. `java.util.HashMap$Entry`) are returned
    // unchanged so JVM inner-class syntax keeps working.
    private static String normalizeProxyClassName(String name) {
        if (name == null) {
            return null;
        }
        if (!name.startsWith(COM_TNS_GEN_PREFIX) || name.indexOf('$') < 0) {
            return name;
        }
        return name.replace('$', '_');
    }

    private final Logger logger;
    private final File dexDir;
    private final File odexDir;
    private final String dexThumb;
    private final ClassLoader classLoader;
    private final ClassStorageService classStorageService;
    private final boolean injectIntoParentClassLoader;

    private ProxyGenerator proxyGenerator;
    private HashMap<String, Class<?>> injectedDexClasses = new HashMap<String, Class<?>>();

    DexFactory(Logger logger, ClassLoader classLoader, File dexBaseDir, String dexThumb, ClassStorageService classStorageService) {
        this(logger, classLoader, dexBaseDir, dexThumb, classStorageService, false);
    }

    DexFactory(Logger logger, ClassLoader classLoader, File dexBaseDir, String dexThumb, ClassStorageService classStorageService, boolean injectIntoParentClassLoader) {
        this.logger = logger;
        this.classLoader = classLoader;
        this.dexDir = dexBaseDir;
        this.dexThumb = dexThumb;
        this.injectIntoParentClassLoader = injectIntoParentClassLoader;

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
        // just the interface name without the extra file, line, column information.
        //
        // The loadable proxy class name uses the `_`-normalized form: the proxy
        // is written into the dex with `_`, while `classToProxy` keeps `$` so
        // `Class.forName(classToProxy)` (in `generateDex`) can resolve the base
        // type via JVM inner-class syntax. Without normalizing here, a
        // `$`-containing base (e.g. unnamed
        // `.extend(android.app.Application.ActivityLifecycleCallbacks, ...)`)
        // fails `loadClass` with ClassNotFoundException even though the dex is
        // present. Surfaces in HMR / dev; production uses an SBG-generated
        // `@JavaProxy(...)` sibling and bypasses this path.
        if (!baseClassName.isEmpty() && isInterface) {
            fullClassName = normalizeProxyClassName(COM_TNS_GEN_PREFIX + classToProxy);
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
        jarFile.setReadOnly();

        Class<?> result;
        String classNameToLoad = isInterface ? fullClassName : desiredDexClassName;

        if (injectIntoParentClassLoader && classLoader instanceof BaseDexClassLoader) {
            injectDexIntoClassLoader((BaseDexClassLoader) classLoader, jarFilePath);
            result = classLoader.loadClass(classNameToLoad);
        } else {
            DexClassLoader dexClassLoader = new DexClassLoader(jarFilePath, this.odexDir.getAbsolutePath(), null, classLoader);
            result = dexClassLoader.loadClass(classNameToLoad);
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

        // Generated proxy classes live under `_`-normalized names (see
        // `normalizeProxyClassName`). If JNI hands us a `$`-containing
        // `com.tns.gen.*` lookup (a metadata dispatch that passed a `$`-bearing
        // base name straight through), try the normalized sibling — both the
        // `injectedDexClasses` cache and `loadClass` — before falling back to
        // the raw form. Scoped to the `com.tns.gen.` prefix so unrelated
        // inner-class lookups (e.g. `java.util.HashMap$Entry`) still resolve via
        // JVM inner-class syntax at the tail of this method.
        String normalizedName = normalizeProxyClassName(canonicalName);
        if (!normalizedName.equals(canonicalName)) {
            existingClass = this.injectedDexClasses.get(normalizedName);
            if (existingClass != null) {
                return existingClass;
            }
            try {
                return classLoader.loadClass(normalizedName);
            } catch (ClassNotFoundException ignored) {
                // fall through to the raw canonical lookup so the original
                // failure (not a noise-from-the-fallback failure) is what
                // propagates to the caller.
            }
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

    /**
     * Injects a DEX jar into the app's PathClassLoader so that classes in it are
     * findable by Class.forName(). This is needed because Android framework components
     * (e.g. FragmentFactory) use Class.forName() to instantiate classes by name, but
     * NativeScript's dynamically-generated classes normally live in isolated DexClassLoaders
     * that Class.forName() doesn't search.
     */
    private void injectDexIntoClassLoader(BaseDexClassLoader targetClassLoader, String jarFilePath) {
        try {
            // Create a temporary DexClassLoader to produce the optimized dex
            DexClassLoader tempLoader = new DexClassLoader(jarFilePath, this.odexDir.getAbsolutePath(), null, targetClassLoader);

            // Get pathList from both classloaders
            Field pathListField = BaseDexClassLoader.class.getDeclaredField("pathList");
            pathListField.setAccessible(true);

            Object targetPathList = pathListField.get(targetClassLoader);
            Object sourcePathList = pathListField.get(tempLoader);

            // Get dexElements from both pathLists
            Field dexElementsField = targetPathList.getClass().getDeclaredField("dexElements");
            dexElementsField.setAccessible(true);

            Object targetElements = dexElementsField.get(targetPathList);
            Object sourceElements = dexElementsField.get(sourcePathList);

            int targetLen = Array.getLength(targetElements);
            int sourceLen = Array.getLength(sourceElements);

            // Create merged array: target + source
            Object merged = Array.newInstance(targetElements.getClass().getComponentType(), targetLen + sourceLen);
            System.arraycopy(targetElements, 0, merged, 0, targetLen);
            System.arraycopy(sourceElements, 0, merged, targetLen, sourceLen);

            dexElementsField.set(targetPathList, merged);
        } catch (Exception e) {
            if (logger.isEnabled()) {
                logger.write("Failed to inject dex into parent classloader: " + e.getMessage());
            }
            // Non-fatal: class will still be loadable via the ClassStorageService fallback
        }
    }
}
