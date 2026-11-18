package com.tns.bindings;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.HashSet;
import java.util.List;

import org.ow2.asmdex.ApplicationWriter;

import com.tns.bindings.desc.ClassDescriptor;

public class ProxyGenerator {
    private String path;
    private Dump dump;

    public static boolean IsLogEnabled = false;
    private String proxyThumb;

    public ProxyGenerator(String path) {
        this.path = path;
        dump = new Dump();
    }

    public void setProxyThumb(String proxyThumb) {
        this.proxyThumb = proxyThumb;
    }

    public String generateProxy(String proxyName, ClassDescriptor classToProxy, HashSet<String> methodOverrides, HashSet<ClassDescriptor> implementedInterfaces, boolean isInterface, AnnotationDescriptor[] annotations) throws IOException {
        return generateProxy(proxyName, null, classToProxy, methodOverrides, implementedInterfaces, isInterface, annotations);
    }

    /**
     * cacheDigest, when present, becomes part of the proxy's file name. The
     * thumb only changes on reinstall, so name + thumb alone cannot see an
     * edit to the proxy's contents (method overrides, interfaces) - the
     * digest is what makes such an edit miss the cache instead of silently
     * loading the previous dex.
     */
    public String generateProxy(String proxyName, String cacheDigest, ClassDescriptor classToProxy, HashSet<String> methodOverrides, HashSet<ClassDescriptor> implementedInterfaces, boolean isInterface, AnnotationDescriptor[] annotations) throws IOException {
        return generateProxy(proxyName, cacheDigest, classToProxy, methodOverrides, implementedInterfaces, isInterface, annotations, null);
    }

    /** defaultMethods: see Dump.generateProxy. */
    public String generateProxy(String proxyName, String cacheDigest, ClassDescriptor classToProxy, HashSet<String> methodOverrides, HashSet<ClassDescriptor> implementedInterfaces, boolean isInterface, AnnotationDescriptor[] annotations, List<Method> defaultMethods) throws IOException {
        return generateProxy(proxyName, cacheDigest, classToProxy, methodOverrides, implementedInterfaces, isInterface, annotations, defaultMethods, DefaultMethodBodies.INVOKE_SUPER);
    }

    /**
     * bodies: see Dump.generateProxy. With INVOKE_SUPER, a default-overriding class's dex
     * file also holds its DefaultSuperProbe class.
     */
    public String generateProxy(String proxyName, String cacheDigest, ClassDescriptor classToProxy, HashSet<String> methodOverrides, HashSet<ClassDescriptor> implementedInterfaces, boolean isInterface, AnnotationDescriptor[] annotations, List<Method> defaultMethods,
                                DefaultMethodBodies bodies) throws IOException {
        boolean overridesDefaults = isInterface && defaultMethods != null && !proxyName.contains(".");
        ApplicationWriter aw = new ApplicationWriter(overridesDefaults ? bodies.dexVersion : ApplicationWriter.DEFAULT_DEX_VERSION);
        aw.visit();

        dump.generateProxy(aw, proxyName, classToProxy, methodOverrides, implementedInterfaces, annotations, defaultMethods, bodies);
        if (overridesDefaults && bodies == DefaultMethodBodies.INVOKE_SUPER) {
            DefaultSuperProbe.generate(aw, DefaultSuperProbe.descriptor(Dump.defaultOverridingDescriptor(classToProxy, bodies)));
        }

        aw.visitEnd();
        byte[] generatedBytes = aw.toByteArray();

        String proxyFileName;

        if (proxyName.contains(".")) {
            // Thumb-suffix dotted names like the anonymous ones: DexFactory's
            // cache probe (getDexFile) and purge (purgeDexesByThumb) both key
            // on the thumb, so an unsuffixed file regenerates every launch and
            // its stale .jar survives — and gets reused — across app versions.
            proxyFileName = proxyName;
            if (proxyThumb != null) {
                proxyFileName += "-" + proxyThumb;
            }
        } else {
            proxyFileName = classToProxy.getName().replace('$', '_');
            if (!isInterface) {
                proxyFileName += Dump.CLASS_NAME_LOCATION_SEPARATOR + proxyName;
            } else if (defaultMethods != null) {
                proxyFileName += defaultOverridingFileSuffix(defaultMethods, bodies);
            }
            if (proxyThumb != null) {
                proxyFileName += "-" + proxyThumb;
            }
        }
        // After the thumb, so purgeDexesByThumb keeps matching old generations.
        if (cacheDigest != null) {
            proxyFileName += "-" + cacheDigest;
        }

        if (IsLogEnabled) {
            System.out.println("Generator: Saving proxy with file name: " + proxyFileName);
        }
        String proxyPath = saveProxy(proxyFileName, generatedBytes);

        return proxyPath;
    }

    /*
     * The dex file name part after the interface's name for its default-overriding class.
     * The class stubs exactly these methods, and its mask bits follow the order of their
     * names, so a dex cached for another list (a platform update that adds a default method
     * or an overload of one) must not be loaded: the methods' hash keeps it from matching.
     */
    public static String defaultOverridingFileSuffix(List<Method> defaultMethods, DefaultMethodBodies bodies) {
        return bodies.classSuffix + "-" + DefaultMethods.signaturesHash(defaultMethods);
    }

    /**
     * A new file in dir to write a cache file to before renaming it to its final name in
     * the same directory. Unique, because several runtimes can write the same file at
     * once. Its name holds the thumb, so DexFactory's purge of an old thumb also removes
     * one a killed process left behind, and nothing of the final name, which may already
     * be as long as the file system allows (Android's createTempFile does not shorten
     * the prefix to fit). It never ends in .dex or .jar, the names the cache probes.
     */
    public static File createCacheTempFile(File dir, String thumb) throws IOException {
        return File.createTempFile((thumb != null ? thumb : "proxy") + "-", ".tmp", dir);
    }

    /**
     * The dex only appears under its final name once complete: DexFactory
     * trusts any dex that exists, so a process killed mid-write must not leave
     * a truncated one there.
     */
    private String saveProxy(String proxyName, byte[] proxyBytes) throws IOException {
        File file = new File(path + File.separator + proxyName + ".dex");
        File parentDir = file.getParentFile();
        if (parentDir != null && !parentDir.exists()) {
            parentDir.mkdirs();
        }
        File tempFile = createCacheTempFile(parentDir, proxyThumb);
        try {
            FileOutputStream stream = new FileOutputStream(tempFile);
            try {
                stream.write(proxyBytes);
                stream.flush();
            } finally {
                stream.close();
            }
            if (!tempFile.renameTo(file)) {
                throw new IOException("Could not move " + tempFile + " to " + file);
            }
        } finally {
            tempFile.delete();
        }

        return file.getAbsolutePath();
    }
}
