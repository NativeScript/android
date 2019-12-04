package com.telerik.metadata;

import com.telerik.metadata.parsing.NativeClassDescriptor;

import java.util.ArrayList;
import java.util.Arrays;

public class ClassRepo {
    private ClassRepo() {
    }

    private static ArrayList<ClassMapProvider> cachedProviders = new ArrayList<ClassMapProvider>();

    public static void addToCache(ClassMapProvider classMapProvider) {
        cachedProviders.add(classMapProvider);
    }

    public static NativeClassDescriptor findClass(String className) {
        NativeClassDescriptor clazz = null;
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
