package com.telerik.metadata;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.JavaClass;

public class ClassDirectrory implements ClassMapProvider {
	private final String path;
	private final Map<String, JavaClass> classMap;
	private static final String CLASS_EXT = ".class";

	private ClassDirectrory(String path) {
		this.path = path;
		this.classMap = new HashMap<String, JavaClass>();
	}

	public Map<String, JavaClass> getClassMap() {
		return classMap;
	}

	public String getPath() {
		return path;
	}

	public static ClassDirectrory readDirectory(String path) throws IOException {
		ClassDirectrory dir = new ClassDirectrory(path);
		readDirectory(dir, path);
		return dir;
	}

	public static void readDirectory(ClassDirectrory dir, String path)
			throws IOException {
		List<File> subDirs = new ArrayList<File>();
		File currentDir = new File(path);
		for (File file : currentDir.listFiles()) {
			if (file.isFile()) {
				String name = file.getName();
				if (name.endsWith(CLASS_EXT)) {
					ClassParser cp = new ClassParser(file.getAbsolutePath());
					JavaClass clazz = cp.parse();
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
}
