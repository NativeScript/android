//package org.nativescript.staticbindinggenerator.defaultmethods.impl;
//
//import org.apache.bcel.classfile.JavaClass;
//import org.apache.bcel.classfile.Method;
//import org.nativescript.staticbindinggenerator.defaultmethods.DefaultMethod;
//import org.nativescript.staticbindinggenerator.defaultmethods.DefaultMethodResolutionException;
//import org.nativescript.staticbindinggenerator.defaultmethods.DefaultMethodsResolver;
//import org.nativescript.staticbindinggenerator.generating.parsing.methods.ClassMethod;
//import org.nativescript.staticbindinggenerator.generating.parsing.methods.ClassMethodsView;
//import org.nativescript.staticbindinggenerator.generating.parsing.methods.InterfaceMethodsView;
//
//import java.util.ArrayList;
//import java.util.HashSet;
//import java.util.List;
//import java.util.Map;
//import java.util.Set;
//
//public class DefaultMethodsResolverImpl implements DefaultMethodsResolver {
//
//    private final Map<String, JavaClass> classesCache;
//
//    public DefaultMethodsResolverImpl(Map<String, JavaClass> classesCache) {
//        this.classesCache = classesCache;
//    }
//
//
//    @Override
//    public List<DefaultMethod> resolveMethods(List<String> userImplementedMethodsNames, ClassMethodsView parentClassMethodsView, InterfaceMethodsView implementedInterfacesMethodsView) {
//        List<DefaultMethod> resolvedMethods = new ArrayList<>();
//        Set<DefaultMethod> topMostDefaultMethods = getOnlyTopMostDefaultMethods(implementedInterfacesMethodsView);
//
//        for (DefaultMethod defaultMethod : topMostDefaultMethods) {
//            Method defaultMethodImpl = defaultMethod.getMethod();
//            ClassMethod classMethod = new ClassMethod(defaultMethodImpl);
//
//            if (parentClassMethodsView.getPrivateMethods().contains(classMethod) || parentClassMethodsView.getPackagePrivateNonAbstractMethods().contains(classMethod)) {
//                throw new DefaultMethodResolutionException("A method in parent class is hiding a default method in interface!");
//            }
//
//            if (parentClassMethodsView.getAbstractMethods().contains(classMethod) && !userImplementedMethodsNames.contains(classMethod.getName())) {
//                throw new DefaultMethodResolutionException("An abstract method with same signature is found in parent class and is not implemented in user code!");
//            }
//
//            if (!parentClassMethodsView.getPublicNonAbstractMethods().contains(classMethod)
//                    && !parentClassMethodsView.getProtectedNonAbstractMethods().contains(classMethod)
//                    && !userImplementedMethodsNames.contains(classMethod.getName())) {
//                resolvedMethods.add(defaultMethod);
//            }
//        }
//
//        checkIfThereAreConflictingInterfaceDefaultMethods(resolvedMethods);
//
//        return resolvedMethods;
//    }
//
//    private Set<DefaultMethod> getOnlyTopMostDefaultMethods(InterfaceMethodsView methods) {
//        Set<DefaultMethod> inputDefaultMethods = methods.getDefaultMethods();
//        Set<DefaultMethod> outputDefaultMethods = new HashSet<>(methods.getDefaultMethods());
//
//        for (DefaultMethod defaultMethod : inputDefaultMethods) {
//            for (DefaultMethod defaultMethod2 : inputDefaultMethods) {
//                if (defaultMethod2 != defaultMethod && isParentOfOrSame(defaultMethod.getJavaClass(), defaultMethod2.getJavaClass())) {
//                    outputDefaultMethods.remove(defaultMethod);
//                }
//            }
//
//        }
//
//        return outputDefaultMethods;
//    }
//
//    private boolean isParentOfOrSame(JavaClass possibleParent, JavaClass possibleChild) {
//        String possibleParentClassName = possibleParent.getClassName();
//
//        if (possibleParentClassName.equals(possibleChild.getClassName())) {
//            return false;
//        }
//
//        String[] currentChildInterfacesNames = possibleChild.getInterfaceNames();
//
//        if (currentChildInterfacesNames.length == 0) {
//            return false;
//        }
//
//        for (String currentChildInterfaceName : currentChildInterfacesNames) {
//            if (currentChildInterfaceName.equals(possibleParentClassName)) {
//                return true;
//            }
//        }
//
//        for (String currentChildInterfaceName : currentChildInterfacesNames) {
//            JavaClass interfaze = classesCache.get(currentChildInterfaceName);
//            isParentOfOrSame(possibleParent, interfaze);
//        }
//
//        return false;
//    }
//
//    private void checkIfThereAreConflictingInterfaceDefaultMethods(List<DefaultMethod> defaultMethods) {
//        Set<Method> methods = new HashSet<>();
//        for (DefaultMethod defaultMethod : defaultMethods) {
//            methods.add(defaultMethod.getMethod());
//        }
//
//        if (methods.size() != defaultMethods.size()) {
//            throw new DefaultMethodResolutionException("Not resolved default methods with same signature in implemented interfaces!");
//        }
//    }
//}
