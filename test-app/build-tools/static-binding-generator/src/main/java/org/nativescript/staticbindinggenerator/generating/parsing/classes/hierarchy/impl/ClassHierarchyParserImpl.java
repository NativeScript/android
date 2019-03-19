package org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.impl;

import org.apache.bcel.classfile.JavaClass;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.ClassHierarchyParser;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.HierarchyView;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;
import org.nativescript.staticbindinggenerator.naming.JavaClassNames;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

public class ClassHierarchyParserImpl implements ClassHierarchyParser {

    private static final Map<JavaClass, HierarchyView> hierarchiesCache = new HashMap<>();

    private final Map<String, JavaClass> classesCache;

    public ClassHierarchyParserImpl(Map<String, JavaClass> classesCache) {
        this.classesCache = classesCache;
    }

    @Override
    public HierarchyView getClassHierarchy(JavaClass javaClass) {
        HierarchyView cachedView = hierarchiesCache.get(javaClass);
        if(cachedView != null){
            return cachedView;
        }

        HashSet<String> parentClassesNames = new HashSet<>();
        HashSet<String> implementedInterfacesNames = new HashSet<>();
        HashSet<String> visitedInterfaces = new HashSet<>();

        collectParentsRecursively(javaClass, parentClassesNames, implementedInterfacesNames, visitedInterfaces);

        HierarchyView hierarchyView = new HierarchyView(parentClassesNames, implementedInterfacesNames);
        hierarchiesCache.put(javaClass, hierarchyView);

        return hierarchyView;
    }

    private void collectParentsRecursively(JavaClass currentClass, HashSet<String> parentClassesNames, HashSet<String> implementedInterfacesNames, HashSet<String> visitedInterfaces) {
        String parentClassName = currentClass.getSuperclassName();
        if (parentClassName != null && !parentClassName.equals("")) { // check in case of currentClass being an interface
            parentClassesNames.add(parentClassName);
        }

        String[] parentInterfacesNames = currentClass.getInterfaceNames();

        for (String parentInterfaceName : parentInterfacesNames) {
            if(!visitedInterfaces.add(parentInterfaceName)){ // add returns true if element already in set
                implementedInterfacesNames.add(parentInterfaceName);
                JavaClass parentInterface = getClassFromCache(parentInterfaceName);
                collectParentsRecursively(parentInterface, parentClassesNames, implementedInterfacesNames, visitedInterfaces);
            }
        }

        if (currentClass.getClassName().equals(JavaClassNames.BASE_JAVA_CLASS_NAME) || currentClass.isInterface()) {
            return;
        }

        JavaClass parentClass = getClassFromCache(parentClassName);
        collectParentsRecursively(parentClass, parentClassesNames, implementedInterfacesNames, visitedInterfaces);
    }

    private JavaClass getClassFromCache(String name) {
        JavaClass clazz = classesCache.get(BcelNamingUtil.resolveClassName(name));

        if (clazz == null) {
            throw new RuntimeException("Class not found: " + name);
        }

        return clazz;
    }
}
