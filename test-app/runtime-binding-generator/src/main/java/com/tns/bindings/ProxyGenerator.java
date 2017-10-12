package com.tns.bindings;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashSet;

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
        ApplicationWriter aw = new ApplicationWriter();
        aw.visit();

        dump.generateProxy(aw, proxyName, classToProxy, methodOverrides, implementedInterfaces, annotations);

        aw.visitEnd();
        byte[] generatedBytes = aw.toByteArray();

        String proxyFileName;

        if (proxyName.contains(".")) {
            proxyFileName = proxyName;
        } else {
            proxyFileName = classToProxy.getName().replace('$', '_');
            if (!isInterface) {
                proxyFileName += Dump.CLASS_NAME_LOCATION_SEPARATOR + proxyName;
            }
            if (proxyThumb != null) {
                proxyFileName += "-" + proxyThumb;
            }
        }

        if (IsLogEnabled) {
            System.out.println("Generator: Saving proxy with file name: " + proxyFileName);
        }
        String proxyPath = saveProxy(proxyFileName, generatedBytes);

        return proxyPath;
    }

    private String saveProxy(String proxyName, byte[] proxyBytes) throws IOException {
        File file = new File(path + File.separator + proxyName + ".dex");
        file.createNewFile();
        FileOutputStream stream = new FileOutputStream(file);
        stream.write(proxyBytes);
        stream.flush();
        stream.close();

        return file.getAbsolutePath();
    }
}
