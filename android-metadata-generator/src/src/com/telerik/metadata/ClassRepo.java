package com.telerik.metadata;

import com.telerik.metadata.desc.ClassDescriptor;

import java.util.ArrayList;
import java.util.Arrays;

public class ClassRepo {
	private ClassRepo() {
	}

	private static ArrayList<ClassMapProvider> cachedProviders = new ArrayList<ClassMapProvider>();

	public static void addToCache(ClassMapProvider classMapProvider) {
		for (String className : classMapProvider.getClassMap().keySet()) {
			for (ClassMapProvider cachedProvider : cachedProviders) {
				ClassDescriptor clazz = cachedProvider.getClassMap().get(className);
				if (clazz != null) {
					String errMsg = "Class " + className + " conflict: "
							+ classMapProvider.getPath() + " and " + cachedProvider.getPath();
					throw new IllegalArgumentException(errMsg);
				}
			}
		}
		cachedProviders.add(classMapProvider);
	}

	public static ClassDescriptor findClass(String className) {
		ClassDescriptor clazz = null;
		for (ClassMapProvider classMapProvider : cachedProviders) {
			clazz = classMapProvider.getClassMap().get(className);
			if (clazz != null) {
				break;
			}
		}
		return clazz;
	}

	public static String[] getClassNames() {
		ArrayList<String> names = new ArrayList<String>();
		for (ClassMapProvider classMapProvider : cachedProviders) {
			for (String className : classMapProvider.getClassMap().keySet()) {
				names.add(className);
			}
		}
		String[] arrClassNames = names.toArray(new String[names.size()]);
		Arrays.sort(arrClassNames);
		return arrClassNames;
	}
}
