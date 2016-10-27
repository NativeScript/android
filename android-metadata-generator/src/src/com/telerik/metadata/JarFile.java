package com.telerik.metadata;

import com.telerik.metadata.bcl.ClassInfo;
import com.telerik.metadata.desc.ClassDescriptor;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.jar.JarInputStream;
import java.util.zip.ZipEntry;

import org.apache.bcel.classfile.ClassFormatException;
import org.apache.bcel.classfile.ClassParser;

public class JarFile implements ClassMapProvider {
	private final String path;
	private final Map<String, ClassDescriptor> classMap;
	private static final String CLASS_EXT = ".class";

	private JarFile(String path) {
		this.path = path;
		this.classMap = new HashMap<String, ClassDescriptor>();
	}

	public String getPath() {
		return path;
	}

	public Map<String, ClassDescriptor> getClassMap() {
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
					ClassDescriptor clazz = new ClassInfo(cp.parse());
					jar.classMap.put(name, clazz);
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
