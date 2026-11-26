package com.tns;

import android.util.Log;

import com.tns.bindings.AnnotationDescriptor;
import com.tns.bindings.DefaultMethodBodies;
import com.tns.bindings.DefaultMethods;
import com.tns.bindings.Dump;
import com.tns.bindings.ProxyGenerator;
import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;
import com.tns.system.classes.loading.ClassStorageService;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.InvalidClassException;
import java.io.OutputStreamWriter;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import dalvik.system.BaseDexClassLoader;
import dalvik.system.DexClassLoader;

public class DexFactory {
    static final String COM_TNS_GEN_PREFIX = "com.tns.gen.";
    // Appended to the name of a class generated in place of a prebuilt one that does not match.
    static final String MISMATCHED_PREBUILT_SUFFIX = "_runtime";
    // The static binding generator lists in it the members it saw of a call site's JS class.
    private static final String PREBUILT_JS_MEMBERS_FIELD = "members__ns";
    // Functions __extends puts on a TypeScript class's prototype, which no Java method overrides.
    private static final Set<String> TS_HELPER_MEMBERS = new HashSet<String>(Arrays.asList("constructor", "__parent", "__child"));

    private static final HashSet<String> warnedPrebuiltMismatches = new HashSet<String>();

    private final Logger logger;
    private final File dexDir;
    private final File odexDir;
    private final String dexThumb;
    private final ClassLoader classLoader;
    private final ClassStorageService classStorageService;
    private final boolean injectIntoParentClassLoader;

    private final boolean useInterfaceProxies;

    private ProxyGenerator proxyGenerator;
    private HashMap<String, Class<?>> injectedDexClasses = new HashMap<String, Class<?>>();

    // Process-wide: injection puts a generated class in the app's class loader, where every
    // runtime's lookup of a prebuilt class by that name then finds it.
    private static final Set<String> generatedClassNames = Collections.newSetFromMap(new ConcurrentHashMap<String, Boolean>());
    // Process-wide, like the native class cache that hands a resolved class to every runtime.
    private static final ConcurrentHashMap<Class<?>, Constructor<?>> interfaceProxyConstructors = new ConcurrentHashMap<Class<?>, Constructor<?>>();

    DexFactory(Logger logger, ClassLoader classLoader, File dexBaseDir, String dexThumb, ClassStorageService classStorageService) {
        this(logger, classLoader, dexBaseDir, dexThumb, classStorageService, false, false);
    }

    DexFactory(Logger logger, ClassLoader classLoader, File dexBaseDir, String dexThumb, ClassStorageService classStorageService, boolean injectIntoParentClassLoader, boolean useInterfaceProxies) {
        this.logger = logger;
        this.classLoader = classLoader;
        this.dexDir = dexBaseDir;
        this.dexThumb = dexThumb;
        this.injectIntoParentClassLoader = injectIntoParentClassLoader;
        this.useInterfaceProxies = useInterfaceProxies;

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

    // Process-wide: every runtime (main and workers) has its own DexFactory, and
    // each one resolves the same classes again, so entries are kept unique.
    private static final LinkedHashSet<String> runtimeGeneratedClasses = new LinkedHashSet<String>();

    /**
     * The classes this process resolved without a prebuilt class from the APK,
     * one line each: {@code <generated|cached|proxy> <interface|class> <loaded name> <requested name>}.
     * "cached" means the dex was reused from the on-disk dex cache, "proxy" that
     * the class is a java.lang.reflect.Proxy class (the interfaceProxies option).
     * The requested name is the one native resolved, which for an interface whose JS
     * implementation overrides default methods is the interface's class name with
     * Dump.DEFAULT_OVERRIDING_SUFFIX, whatever the loaded class is named.
     */
    public static String[] getRuntimeGeneratedClasses() {
        synchronized (runtimeGeneratedClasses) {
            return runtimeGeneratedClasses.toArray(new String[0]);
        }
    }

    private static void recordRuntimeGeneratedClass(String requestedName, Class<?> loadedClass, boolean isInterface, String source) {
        String entry = source + " " + (isInterface ? "interface" : "class") + " " + loadedClass.getName() + " " + requestedName;
        boolean added;
        synchronized (runtimeGeneratedClasses) {
            added = runtimeGeneratedClasses.add(entry);
        }
        if (added && com.tns.Runtime.isDebuggable()) {
            String what = source.equals("generated") ? "Generated class at runtime: "
                    : source.equals("cached") ? "Loaded runtime-generated class from the dex cache: "
                    : "Implemented interface with a java.lang.reflect.Proxy: ";
            Log.i("JS", what + loadedClass.getName() + " (requested as " + requestedName + ")");
        }
    }

    static boolean isInterfaceProxyClass(Class<?> clazz) {
        return interfaceProxyConstructors.containsKey(clazz);
    }

    static Object newInterfaceProxy(Class<?> proxyClass, InvocationHandler handler) throws ReflectiveOperationException {
        return interfaceProxyConstructors.get(proxyClass).newInstance(handler);
    }

    /**
     * A proxy class for an anonymous implementation of the interface, or null when a
     * proxy cannot behave like the class that would be generated for it, in which case
     * the class is generated.
     */
    private Class<?> getInterfaceProxyClass(String interfaceName, String requestedName) {
        try {
            Class<?> iface = Class.forName(interfaceName, false, classLoader);
            // The proxy class is defined in this loader, which must see both interfaces; the
            // boot class loader of platform interfaces cannot see the runtime's.
            ClassLoader loader = iface.getClassLoader() == Object.class.getClassLoader()
                    ? NativeScriptRuntimeBound.class.getClassLoader()
                    : iface.getClassLoader();
            Class<?> proxyClass = Proxy.getProxyClass(loader, iface, NativeScriptRuntimeBound.class);
            if (!interfaceProxyConstructors.containsKey(proxyClass)) {
                Constructor<?> constructor = proxyClass.getConstructor(InvocationHandler.class);
                // A JS wrapper created for an instance by its class name uses the interface's
                // metadata, so the mapping is registered before any runtime can see the class.
                Runtime.registerInterfaceProxyClass(proxyClass.getName().replace('.', '/'), iface.getName().replace('.', '/'));
                interfaceProxyConstructors.putIfAbsent(proxyClass, constructor);
            }
            classStorageService.storeClass(proxyClass.getName(), proxyClass);
            recordRuntimeGeneratedClass(requestedName, proxyClass, true, "proxy");
            return proxyClass;
        } catch (Throwable e) {
            Log.d("JS", "Generating a class for " + interfaceName + ": a java.lang.reflect.Proxy cannot implement it (" + e + ")");
            return null;
        }
    }

    /**
     * @param hasExplicitName className is the name the script passed to extend(), which
     *                        may itself start with com.tns.gen.
     */
    public Class<?> resolveClass(String baseClassName, String name, String className, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface, boolean hasExplicitName) throws ClassNotFoundException, IOException {
        // The interface's class name plus this suffix asks for the class that also stubs its
        // default methods. A prebuilt class of the interface is used instead when there is one.
        // Only native asks for it, without a base class name; an extend() name the script
        // chose always comes with the base class's.
        boolean overridesDefaults = isInterface && baseClassName.isEmpty() && className.startsWith(COM_TNS_GEN_PREFIX) && className.endsWith(Dump.DEFAULT_OVERRIDING_SUFFIX);
        String requestedClassName = className;
        if (overridesDefaults) {
            className = className.substring(0, className.length() - Dump.DEFAULT_OVERRIDING_SUFFIX.length());
        }

        // Without a base class name, an interface's shared class is asked for (by
        // `new Interface({...})` or for its default-overriding variant). It cannot depend on
        // the interfaces one implementation object lists.
        if (isInterface && baseClassName.isEmpty()) {
            implementedInterfaces = new String[0];
        }

        String fullClassName = className.replace("$", "_");

        // A named proxy (`Base.extend('a.b.C', {...})` / @JavaProxy) asks for
        // exactly that Java class name; the substitutions below are for the
        // anonymous form only, where the name is derived from the base.
        boolean isNamedProxy = !fullClassName.startsWith(COM_TNS_GEN_PREFIX) && fullClassName.contains(".");

        // An anonymous extend of an interface asks for a per-call-site name, but
        // it gets the class `new Interface({...})` uses, whose body depends only
        // on the interface. Resolving it under that name lets the APK's prebuilt
        // class and the dex cache serve every call site. Extra implemented
        // interfaces add methods the shared class lacks.
        boolean usesSharedInterfaceClass = !baseClassName.isEmpty() && isInterface && !isNamedProxy && !hasImplementedInterfaces(implementedInterfaces);
        if (usesSharedInterfaceClass) {
            fullClassName = COM_TNS_GEN_PREFIX + this.getClassToProxyName(baseClassName).replace("$", "_");
        }
        String originalFullClassName = fullClassName;

        // The static binding generator keeps a `$` from the script's file name in an
        // anonymous extend's class name; the runtime already replaced the base class's `$`.
        Class<?> pregeneratedClass = null;
        if (!usesSharedInterfaceClass || hasExplicitName) {
            String requestedFullClassName = className.replace("$", "_");
            if (!requestedFullClassName.equals(className)) {
                pregeneratedClass = loadPregeneratedClass(className.replace("-", "_"));
            }
            if (pregeneratedClass == null) {
                pregeneratedClass = loadPregeneratedClass(requestedFullClassName.replace("-", "_"));
            }
        }
        if (pregeneratedClass == null && usesSharedInterfaceClass) {
            pregeneratedClass = loadPregeneratedClass(fullClassName.replace("-", "_"));
            // Once an extend of the interface has resolved, a later JS override of a default
            // method gets the class generated for that, not the prebuilt one, which stubs
            // only the defaults of the call site the generator saw first.
            if (pregeneratedClass != null) {
                generatedClassNames.add(pregeneratedClass.getName());
            }
        }
        // Once injected, the class generated for the interface loads by the same name.
        if (overridesDefaults && pregeneratedClass != null && generatedClassNames.contains(pregeneratedClass.getName())) {
            pregeneratedClass = null;
        }
        // An anonymous class's prebuilt counterpart is found by call site alone, so it is only
        // used when it was built from everything the script defines. The class generated instead
        // needs a name of its own: the APK's class would answer the prebuilt one first.
        if (pregeneratedClass != null && !hasExplicitName && !isNamedProxy) {
            String mismatch = describePrebuiltMismatch(pregeneratedClass, methodOverrides, implementedInterfaces);
            if (mismatch != null) {
                warnPrebuiltClassMismatch(pregeneratedClass.getName(), mismatch);
                pregeneratedClass = null;
                fullClassName += MISMATCHED_PREBUILT_SUFFIX;
            }
        }
        if (pregeneratedClass != null) {
            return pregeneratedClass;
        }

        if (overridesDefaults) {
            // Cached under the name native asks for, whichever class answers it.
            originalFullClassName = fullClassName + Dump.DEFAULT_OVERRIDING_SUFFIX;
        }

        // new: com.tns.gen.android.widget.DatePicker_MyActivity_59_56_
        // old: com.tns.tests.Button1_fMyActivity_l56_c44__MyButton
        // ne1: com.tns.tests.Button1_MyActivity_58_44_MyButton_0
        Class<?> existingClass = this.injectedDexClasses.get(originalFullClassName);
        if (existingClass != null) {
            return existingClass;
        }

        DefaultMethodBodies bodies = null;
        if (overridesDefaults) {
            bodies = DefaultSuperCalls.bodies(Class.forName(this.getClassToProxyName(className)));
            fullClassName += bodies.classSuffix;
        }

        String classToProxy;
        if (!baseClassName.isEmpty()) {
            classToProxy = this.getClassToProxyName(baseClassName);
        } else {
            classToProxy = this.getClassToProxyName(className);
        }

        if (useInterfaceProxies && isInterface && !isNamedProxy && !hasImplementedInterfaces(implementedInterfaces)) {
            Class<?> proxyClass = getInterfaceProxyClass(classToProxy, requestedClassName);
            if (proxyClass != null) {
                this.injectedDexClasses.put(originalFullClassName, proxyClass);
                return proxyClass;
            }
        }

        // strip the `com.tns.gen` off the base extended class name
        String desiredDexClassName = this.getClassToProxyName(fullClassName);
        if (overridesDefaults) {
            // The same file name ProxyGenerator writes the class under.
            desiredDexClassName = this.getClassToProxyName(className.replace("$", "_"))
                    + ProxyGenerator.defaultOverridingFileSuffix(DefaultMethods.collect(Class.forName(classToProxy)), bodies);
        }

        // An anonymous extend of an interface that lists interfaces of its own gets a class of
        // its call site, under the name asked for, which is also the name a prebuilt class of
        // the call site has: the interface's shared class cannot implement them for one call.
        boolean isCallSiteInterfaceClass = isInterface && !baseClassName.isEmpty() && !isNamedProxy && implementedInterfaces.length > 0;
        if (isCallSiteInterfaceClass) {
            desiredDexClassName = fullClassName;
        }

        // when interfaces are extended as classes, we still want to preserve
        // just the interface name without the extra file, line, column information
        if (!baseClassName.isEmpty() && isInterface && !isNamedProxy && !isCallSiteInterfaceClass) {
            fullClassName = COM_TNS_GEN_PREFIX + classToProxy.replace("$", "_");
        }

        // The thumb only changes on reinstall, so a cache key of name + thumb
        // cannot see an edit to the proxy's contents: under HMR a named
        // proxy's new method overrides would silently load the previous dex.
        // The digest carries the contents into the file name. The interface's
        // shared class gets every abstract method whatever the JS object
        // defines, so its overrides stay out of the key and all its call sites
        // share one cached dex. A call site's own class stubs the default
        // methods its JS object names, so its overrides stay in.
        String[] digestedOverrides = isInterface && !isNamedProxy && !isCallSiteInterfaceClass ? null : methodOverrides;
        String contentDigest = computeContentDigest(classToProxy, digestedOverrides, implementedInterfaces, isInterface);

        File dexFile = this.getDexFile(desiredDexClassName, contentDigest);
        boolean generated = dexFile == null;

        // generate dex file
        if (generated) {
            long startGenTime = System.nanoTime();
            if (logger.isEnabled()) {
                logger.write("generating proxy in place");
            }

            String dexFilePath;
            if (isInterface && !isNamedProxy && !isCallSiteInterfaceClass) {
                dexFilePath = this.generateDex(name, contentDigest, classToProxy, methodOverrides, implementedInterfaces, isInterface, bodies);
            } else {
                dexFilePath = this.generateDex(desiredDexClassName, contentDigest, classToProxy, methodOverrides, implementedInterfaces, isInterface, null);
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
        String jarFilePath = dexFile.getPath().replaceFirst("\\.dex$", ".jar");
        File jarFile = new File(jarFilePath);

        if (!jarFile.exists()) {
            writeJar(dexFile, jarFile);
        }
        jarFile.setReadOnly();

        Class<?> result = null;
        String classNameToLoad = isInterface && !isCallSiteInterfaceClass ? fullClassName : desiredDexClassName;

        boolean injectIntoParent = injectIntoParentClassLoader && classLoader instanceof BaseDexClassLoader;
        if (injectIntoParent && ProxyRegistrationGc.isRequired()) {
            // The dex is registered by the first loadClass below, so the blocking
            // collection has to stay immediately before it.
            java.lang.Runtime.getRuntime().gc();
        }

        if (injectIntoParent && injectDexIntoClassLoader((BaseDexClassLoader) classLoader, jarFilePath)) {
            try {
                result = classLoader.loadClass(classNameToLoad);
            } catch (ClassNotFoundException e) {
                // fall through to the isolated DexClassLoader below
            }
        }

        if (result == null) {
            DexClassLoader dexClassLoader = new DexClassLoader(jarFilePath, this.odexDir.getAbsolutePath(), null, classLoader);
            result = dexClassLoader.loadClass(classNameToLoad);
        }

        if (bodies == DefaultMethodBodies.INVOKE_SUPER && !DefaultSuperCalls.verify(result)) {
            return resolveClass(baseClassName, name, requestedClassName, methodOverrides, implementedInterfaces, isInterface, hasExplicitName);
        }

        generatedClassNames.add(result.getName());
        classStorageService.storeClass(result.getName(), result);
        this.injectedDexClasses.put(originalFullClassName, result);
        recordRuntimeGeneratedClass(requestedClassName, result, isInterface, generated ? "generated" : "cached");

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

        String underscored = canonicalName.replace('$', '_');
        if (!underscored.equals(canonicalName)) {
            existingClass = this.injectedDexClasses.get(underscored);
            if (existingClass != null) {
                return existingClass;
            }
        }

        try {
            return classLoader.loadClass(canonicalName);
        } catch (ClassNotFoundException e) {
            if (!underscored.equals(canonicalName)) {
                return classLoader.loadClass(underscored);
            }
            throw e;
        }
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

    /**
     * The jar only appears under its final name once complete and read-only:
     * its existence is the cache hit, so a process killed mid-write must not
     * leave a truncated one there.
     */
    private void writeJar(File dexFile, File jarFile) throws IOException {
        byte[] dexData = new byte[(int) dexFile.length()];
        DataInputStream in = new DataInputStream(new FileInputStream(dexFile));
        try {
            in.readFully(dexData);
        } finally {
            in.close();
        }

        File tempFile = ProxyGenerator.createCacheTempFile(jarFile.getParentFile(), dexThumb);
        try {
            ZipOutputStream out = new ZipOutputStream(new FileOutputStream(tempFile));
            try {
                out.putNextEntry(new ZipEntry("classes.dex"));
                out.write(dexData);
                out.closeEntry();
            } finally {
                out.close();
            }
            tempFile.setReadOnly();

            // A runtime that finished first already published identical contents,
            // and may have a class loader on that file.
            if (!jarFile.exists() && !tempFile.renameTo(jarFile) && !jarFile.exists()) {
                throw new IOException("Could not move " + tempFile + " to " + jarFile);
            }
        } finally {
            tempFile.delete();
        }
    }

    // Why prebuiltClass cannot stand for the JS class, or null when it can.
    private static String describePrebuiltMismatch(Class<?> prebuiltClass, String[] methodOverrides, String[] implementedInterfaces) {
        String missing = missingInterfaces(prebuiltClass, implementedInterfaces);
        if (missing != null) {
            return "does not implement " + missing + ", which its JavaScript class lists";
        }
        String unseen = unseenMembers(prebuiltClass, methodOverrides);
        if (unseen != null) {
            return "was built without " + unseen + " of its JavaScript class (defined outside the class body, or by a getter)";
        }
        return null;
    }

    // The JS functions the static binding generator did not see when it built prebuiltClass,
    // joined, or null. Java could never call them through it.
    private static String unseenMembers(Class<?> prebuiltClass, String[] methodOverrides) {
        if (methodOverrides == null || methodOverrides.length == 0) {
            return null;
        }
        String[] seen;
        try {
            Field field = prebuiltClass.getDeclaredField(PREBUILT_JS_MEMBERS_FIELD);
            field.setAccessible(true);
            seen = (String[]) field.get(null);
        } catch (NoSuchFieldException e) {
            return null;
        } catch (IllegalAccessException e) {
            return null;
        }
        HashSet<String> seenMembers = new HashSet<String>(Arrays.asList(seen));
        StringBuilder unseen = null;
        for (String member : methodOverrides) {
            if (seenMembers.contains(member) || TS_HELPER_MEMBERS.contains(member) || !isJavaIdentifier(member)) {
                continue;
            }
            if (unseen == null) {
                unseen = new StringBuilder(member);
            } else {
                unseen.append(", ").append(member);
            }
        }
        return unseen == null ? null : unseen.toString();
    }

    // No Java method can have any other name.
    private static boolean isJavaIdentifier(String name) {
        if (name.isEmpty() || !Character.isJavaIdentifierStart(name.charAt(0))) {
            return false;
        }
        for (int i = 1; i < name.length(); i++) {
            if (!Character.isJavaIdentifierPart(name.charAt(i))) {
                return false;
            }
        }
        return true;
    }

    // The interfaces in implementedInterfaces that prebuiltClass does not implement, joined, or
    // null if it implements them all.
    private static String missingInterfaces(Class<?> prebuiltClass, String[] implementedInterfaces) {
        if (implementedInterfaces == null) {
            return null;
        }
        StringBuilder missing = null;
        for (String implementedInterface : implementedInterfaces) {
            if (implementedInterface == null || implementedInterface.isEmpty()) {
                continue;
            }
            boolean implemented;
            try {
                implemented = Class.forName(implementedInterface).isAssignableFrom(prebuiltClass);
            } catch (ClassNotFoundException e) {
                // Generating the class reports it.
                implemented = false;
            }
            if (!implemented) {
                if (missing == null) {
                    missing = new StringBuilder(implementedInterface);
                } else {
                    missing.append(", ").append(implementedInterface);
                }
            }
        }
        return missing == null ? null : missing.toString();
    }

    private static void warnPrebuiltClassMismatch(String prebuiltClassName, String mismatch) {
        synchronized (warnedPrebuiltMismatches) {
            if (!warnedPrebuiltMismatches.add(prebuiltClassName)) {
                return;
            }
        }
        Log.w("JS", "The prebuilt class " + prebuiltClassName + " " + mismatch
                + ", so the class is generated at runtime instead. Rebuild the app if its JavaScript"
                + " changed since; otherwise, please report this with the class's source.");
    }

    private static boolean hasImplementedInterfaces(String[] implementedInterfaces) {
        if (implementedInterfaces != null) {
            for (String implementedInterface : implementedInterfaces) {
                if (implementedInterface != null && !implementedInterface.isEmpty()) {
                    return true;
                }
            }
        }
        return false;
    }

    private Class<?> loadPregeneratedClass(String name) {
        try {
            if (logger.isEnabled()) {
                logger.write("getting pre-generated proxy class with name:  " + name);
            }

            Class<?> pregeneratedClass = classLoader.loadClass(name);

            if (logger.isEnabled()) {
                logger.write("Pre-generated class found:  " + name);
            }

            return pregeneratedClass;
        } catch (Exception e) {
            if (logger.isEnabled()) {
                logger.write("Pre-generated class not found:  " + name);
            }
            return null;
        }
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

    /**
     * Digest of everything that shapes the generated proxy besides its name,
     * so the dex cache key changes when the proxy's contents do. Sorted, so
     * JS property-enumeration order cannot produce a spurious miss.
     */
    private static String computeContentDigest(String classToProxy, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface) {
        StringBuilder canonical = new StringBuilder(classToProxy).append('\n').append(isInterface);
        if (methodOverrides != null) {
            String[] sortedOverrides = methodOverrides.clone();
            java.util.Arrays.sort(sortedOverrides);
            for (String override : sortedOverrides) {
                canonical.append('\n').append(override);
            }
        }
        if (implementedInterfaces != null) {
            String[] sortedInterfaces = implementedInterfaces.clone();
            java.util.Arrays.sort(sortedInterfaces);
            for (String iface : sortedInterfaces) {
                canonical.append('').append(iface);
            }
        }
        try {
            byte[] hash = java.security.MessageDigest.getInstance("SHA-256")
                          .digest(canonical.toString().getBytes(java.nio.charset.StandardCharsets.UTF_8));
            StringBuilder hex = new StringBuilder(10);
            for (int i = 0; i < 5; i++) {
                hex.append(String.format("%02x", hash[i]));
            }
            return hex.toString();
        } catch (java.security.NoSuchAlgorithmException e) {
            // SHA-256 is mandatory on Android; a digestless key only loses
            // cache freshness, never correctness of a fresh generation.
            return null;
        }
    }

    private File getDexFile(String className, String contentDigest) throws InvalidClassException {
        String classToProxyFile = className.replace("$", "_");

        if (this.dexThumb != null) {
            classToProxyFile += "-" + this.dexThumb;
        }
        if (contentDigest != null) {
            classToProxyFile += "-" + contentDigest;
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

    // defaultBodies: those of the default-overriding class of the interface className; null for
    // any other class.
    private String generateDex(String proxyName, String contentDigest, String className, String[] methodOverrides, String[] implementedInterfaces, boolean isInterface,
                               DefaultMethodBodies defaultBodies) throws ClassNotFoundException, IOException {
        boolean overridesDefaults = defaultBodies != null;
        Class<?> classToProxy = Class.forName(className);

        HashSet<String> methodOverridesSet = null;
        HashSet<ClassDescriptor> implementedInterfacesSet = new HashSet<ClassDescriptor>();
        List<Class<?>> implementedInterfaceClasses = new ArrayList<Class<?>>();

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
                    Class<?> implementedInterface = Class.forName(implementedInterfaces[j]);
                    implementedInterfacesSet.add(new ClassInfo(implementedInterface));
                    implementedInterfaceClasses.add(implementedInterface);
                }
            }
        }

        AnnotationDescriptor[] annotations = null;
        List<Method> defaultMethods = null;
        if (overridesDefaults) {
            defaultMethods = DefaultMethods.collect(classToProxy);
        } else if (!classToProxy.isInterface() || proxyName.contains(".")) {
            // An interface implementation with a dotted name is one extend() call's class, so
            // like a class it stubs only the default methods its implementation object names.
            List<Method> inheritedDefaults;
            if (classToProxy.isInterface()) {
                List<Class<?>> interfaces = new ArrayList<Class<?>>(implementedInterfaceClasses);
                interfaces.add(classToProxy);
                inheritedDefaults = DefaultMethods.collectWithConflicts(Object.class, interfaces);
            } else {
                inheritedDefaults = DefaultMethods.collectWithConflicts(classToProxy, implementedInterfaceClasses);
            }
            defaultMethods = new ArrayList<Method>();
            for (Method inherited : inheritedDefaults) {
                if (methodOverridesSet == null || methodOverridesSet.contains(inherited.getName())) {
                    defaultMethods.add(inherited);
                }
            }
        }
        return proxyGenerator.generateProxy(proxyName, contentDigest, new ClassInfo(classToProxy), methodOverridesSet, implementedInterfacesSet, isInterface, annotations, defaultMethods,
                                            overridesDefaults ? defaultBodies : DefaultMethodBodies.INVOKE_SUPER);
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
     *
     * The jar must be added through the target class loader's own DexPathList so that
     * the resulting DexFile has the PathClassLoader as its only owner. Opening the jar
     * through a separate DexClassLoader first and splicing its dex element would leave
     * the same DexFile claimed by two loaders, which ART rejects on non-debuggable
     * builds with "Attempt to register dex file ... with multiple class loaders".
     *
     * @return true if the jar was injected and the class can be loaded through the
     *         target class loader, false if the caller should fall back to an
     *         isolated DexClassLoader.
     */
    private boolean injectDexIntoClassLoader(BaseDexClassLoader targetClassLoader, String jarFilePath) {
        try {
            Method addDexPath = BaseDexClassLoader.class.getDeclaredMethod("addDexPath", String.class);
            addDexPath.setAccessible(true);
            addDexPath.invoke(targetClassLoader, jarFilePath);
            return true;
        } catch (Exception e) {
            Log.w("JS", "Failed to inject dex into parent classloader: " + e);
            return false;
        }
    }
}
