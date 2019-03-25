package org.nativescript.staticbindinggenerator.generating.parsing.methods.impl;

import org.apache.bcel.classfile.JavaClass;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.GenericHierarchyView;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.GenericSignatureReader;
import org.nativescript.staticbindinggenerator.generating.parsing.classes.hierarchy.generics.impl.GenericsAwareClassHierarchyParserImpl;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.InheritedMethodsCollector;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.InheritedMethodsView;
import org.nativescript.staticbindinggenerator.generating.parsing.methods.ReifiedJavaMethod;
import org.nativescript.staticbindinggenerator.naming.BcelNamingUtil;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class InheritedMethodsCollectorImpl implements InheritedMethodsCollector {
    private final Map<String, JavaClass> classesCache;
    private final JavaMethodUtils javaMethodUtils;
    private final JavaClassUtils javaClassUtils;
    private final GenericHierarchyView genericHierarchyView;
    private final String packageName;
    private final JavaClass javaClass;
    private final List<JavaClass> implementedInterfaces;

    private InheritedMethodsCollectorImpl(Builder builder) {
        this.classesCache = builder.classesCache;
        this.genericHierarchyView = builder.genericHierarchyView;
        this.packageName = builder.packageName;
        this.javaClass = builder.javaClass;
        this.implementedInterfaces = builder.interfaces;

        this.javaMethodUtils = new JavaMethodUtils(classesCache);
        this.javaClassUtils = new JavaClassUtils(classesCache);
    }

    @Override
    public InheritedMethodsView collect() {
        ArrayList<ReifiedJavaMethod> allMethods = new ArrayList<>();
        ArrayList<ReifiedJavaMethod> toImplement = new ArrayList<>();
        ArrayList<ReifiedJavaMethod> overridable = new ArrayList<>();


        JavaClass superClass = javaClass;
        while (superClass != null) {

            List<ReifiedJavaMethod> typeMethods = javaClassUtils.getReifiedMethodsFromClass(superClass, genericHierarchyView);
            for (int i = 0; i < typeMethods.size(); i++) {
                ReifiedJavaMethod curr = typeMethods.get(i);

                if (!curr.isConstructor() && !curr.isStatic() && !curr.isPrivate()) {
                    if (!curr.isSynthetic()) {
                        if (findMethodBinding(curr, allMethods) == null) {
                            allMethods.add(curr);
                            if (!curr.isFinal() && !curr.isAbstract()) {
                                overridable.add(curr);
                            }
                        }
                    }
                }
            }


            superClass = javaClassUtils.getSuperClass(superClass);
        }

        for (int i = 0; i < allMethods.size(); i++) {
            ReifiedJavaMethod curr = allMethods.get(i);
            if (curr.isAbstract() && !curr.isSynthetic()) {
                toImplement.add(curr);
            }
        }

        HashSet<JavaClass> visited = new HashSet<>(); // saves some interface traversing

        for (JavaClass interfaze : implementedInterfaces) {
            GenericHierarchyView interfaceGenView = new GenericsAwareClassHierarchyParserImpl(new GenericSignatureReader(), classesCache).getClassHierarchy(interfaze);
            findUnimplementedInterfaceMethods(interfaze, visited, allMethods, packageName, toImplement, overridable, interfaceGenView);
        }

        JavaClass curr = javaClass;
        while (curr != null) {
            JavaClass[] superInterfaces = new JavaClass[curr.getInterfaceNames().length];

            for (int i = 0; i < curr.getInterfaceNames().length; i += 1) {
                superInterfaces[i] = getClassFromCache(curr.getInterfaceNames()[i]);
            }

            for (int i = 0; i < superInterfaces.length; i++) {
                findUnimplementedInterfaceMethods(superInterfaces[i], visited, allMethods, packageName, toImplement, overridable, genericHierarchyView);
            }

            curr = javaClassUtils.getSuperClass(curr);

        }

        return new InheritedMethodsView(overridable, toImplement);
    }

    private ReifiedJavaMethod findMethodBinding(ReifiedJavaMethod method, List<ReifiedJavaMethod> allMethods) {
        for (int i = 0; i < allMethods.size(); i++) {
            ReifiedJavaMethod curr = allMethods.get(i);
            if (javaMethodUtils.isSubSignature(method, curr)) {
                return curr;
            }
        }

        return null;
    }


    private void findUnimplementedInterfaceMethods(JavaClass typeBinding, HashSet<JavaClass> visited,
                                                   ArrayList<ReifiedJavaMethod> allMethods, String packageName, ArrayList<ReifiedJavaMethod> toImplement, ArrayList<ReifiedJavaMethod> overridable, GenericHierarchyView initialClassGenericHierarchyView) {
        if (visited.add(typeBinding)) {
            List<ReifiedJavaMethod> typeMethods = javaClassUtils.getReifiedMethodsFromClass(typeBinding, initialClassGenericHierarchyView);

            nextMethod:
            for (int i = 0; i < typeMethods.size(); i++) {
                ReifiedJavaMethod current = typeMethods.get(i);

                for (Iterator<ReifiedJavaMethod> allIter = allMethods.iterator(); allIter.hasNext(); ) {
                    ReifiedJavaMethod oneMethod = allIter.next();
                    if (javaMethodUtils.isSubSignature(oneMethod, current)) {
                        // we've matched a method which is a subsignature of current
                        if (!javaMethodUtils.isSubSignature(current, oneMethod)) {
                            // oneMethod is a true subsignature of current, use oneMethod
                            continue nextMethod;
                        }

                        // subsignatures are equivalent.
                        // check visibility and return types
                        if (javaMethodUtils.isVisibleInHierarchy(oneMethod, packageName)
                                && javaMethodUtils.isSubTypeCompatible(current.getReturnType(), oneMethod.getReturnType())) {
                            // oneMethod is visible and curr doesn't have a stricter return type; let's go with oneMethod
                            continue nextMethod;
                        }

                        // current is stricter than oneMethod, remove oneMethod
                        allIter.remove();
                        toImplement.remove(oneMethod);
                        overridable.remove(oneMethod);
                    } else if (javaMethodUtils.isSubSignature(current, oneMethod)) {
                        // current is a true subsignature of oneMethod, remove oneMethod.
                        allIter.remove();
                        toImplement.remove(oneMethod);
                        overridable.remove(oneMethod);
                    }
                }
                if (!current.isStatic()) {
                    allMethods.add(current);
                    if (current.isAbstract()) {
                        toImplement.add(current);
                    } else if (!current.isFinal()) {
                        overridable.add(current);
                    }
                }
            }
            JavaClass[] superInterfaces = javaClassUtils.getInterfaces(typeBinding);
            for (int i = 0; i < superInterfaces.length; i++) {
                findUnimplementedInterfaceMethods(superInterfaces[i], visited, allMethods, packageName, toImplement, overridable, initialClassGenericHierarchyView);
            }
        }
    }

    private JavaClass getClassFromCache(String className) {
        return tryGetClassFromCache(BcelNamingUtil.resolveClassName(className));
    }

    private JavaClass tryGetClassFromCache(String name) {
        JavaClass javaClass = classesCache.get(name);

        if (javaClass == null) {
            throw new RuntimeException("Class not found: " + name);
        }

        return javaClass;
    }

    public static class Builder {
        private Map<String, JavaClass> classesCache;
        private GenericHierarchyView genericHierarchyView;
        private JavaClass javaClass;
        private List<JavaClass> interfaces;
        private String packageName;

        public Builder withClassesCache(Map<String, JavaClass> classesCache) {
            this.classesCache = classesCache;
            return this;
        }

        public Builder withGenericHierarchyView(GenericHierarchyView genericHierarchyView) {
            this.genericHierarchyView = genericHierarchyView;
            return this;
        }

        public Builder forJavaClass(JavaClass javaClass) {
            this.javaClass = javaClass;
            return this;
        }

        public Builder withAdditionalImplementedInterfaces(List<JavaClass> interfaces) {
            this.interfaces = interfaces;
            return this;
        }

        public Builder withPackageName(String packageName) {
            this.packageName = packageName;
            return this;
        }

        public InheritedMethodsCollector build() {
            return new InheritedMethodsCollectorImpl(this);
        }
    }

}
