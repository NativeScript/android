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
        File parentDir = file.getParentFile();
        if (parentDir != null && !parentDir.exists()) {
            parentDir.mkdirs();
        }
        try {
            file.createNewFile();
            FileOutputStream stream = new FileOutputStream(file);
            try {
                stream.write(proxyBytes);
                stream.flush();
            } finally {
                stream.close();
            }
        } catch (IOException e) {
            throw new IOException("Failed to save proxy dex '" + file.getAbsolutePath()
                    + "' (" + proxyBytes.length + " bytes): " + e.getMessage()
                    + " | " + describeDexDirState(file, parentDir), e);
        }

        return file.getAbsolutePath();
    }

    private static String describeDexDirState(File file, File parentDir) {
        StringBuilder sb = new StringBuilder();
        sb.append("file.exists=").append(file.exists());
        if (parentDir != null) {
            sb.append(" parent=").append(parentDir.getAbsolutePath());
            sb.append(" parent.exists=").append(parentDir.exists());
            sb.append(" parent.isDir=").append(parentDir.isDirectory());
            sb.append(" parent.canWrite=").append(parentDir.canWrite());
            File grand = parentDir.getParentFile();
            if (grand != null) {
                sb.append(" grand=").append(grand.getAbsolutePath());
                sb.append(" grand.exists=").append(grand.exists());
                sb.append(" grand.canWrite=").append(grand.canWrite());
            }
            try {
                sb.append(" parent.freeSpace=").append(parentDir.getFreeSpace());
                sb.append(" parent.usableSpace=").append(parentDir.getUsableSpace());
            } catch (Throwable ignored) {
            }
        } else {
            sb.append(" parent=null");
        }
        return sb.toString();
    }
}
