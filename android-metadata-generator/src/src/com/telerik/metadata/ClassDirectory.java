package com.telerik.metadata;

import com.telerik.metadata.desc.ClassDescriptor;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.bcel.classfile.ClassParser;

public class ClassDirectory implements ClassMapProvider {
	private final String path;
	private final Map<String, ClassDescriptor> classMap;
	private static final String CLASS_EXT = ".class";
	private static final String DEX_EXT = ".dex";

	private ClassDirectory(String path) {
		this.path = path;
		this.classMap = new HashMap<String, ClassDescriptor>();
	}

	public Map<String, ClassDescriptor> getClassMap() {
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
		List<File> subDirs = new ArrayList<File>();
		File currentDir = new File(path);
		for (File file : currentDir.listFiles()) {
			if (file.isFile()) {
				String name = file.getName();
				if (name.endsWith(CLASS_EXT)) {
					ClassDescriptor clazz = getClassDescriptor(name, file);
					dir.classMap.put(clazz.getClassName(), clazz);
				}
			} else if (file.isDirectory()) {
				subDirs.add(file);
			}
		}
		for (File sd: subDirs) {
			readDirectory(dir, sd.getAbsolutePath());
		}
	}

	private static ClassDescriptor getClassDescriptor(String name, File file) throws IOException {
		ClassDescriptor clazz = null;
		if (name.endsWith(CLASS_EXT)) {
			ClassParser cp = new ClassParser(file.getAbsolutePath());
			clazz = new com.telerik.metadata.bcl.ClassInfo(cp.parse());
		} else if (name.endsWith(DEX_EXT)) {
			// TODO:
		}

		return clazz;
	}
}
