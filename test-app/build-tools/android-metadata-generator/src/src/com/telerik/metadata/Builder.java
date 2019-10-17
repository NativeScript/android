package com.telerik.metadata;

import com.telerik.metadata.TreeNode.FieldInfo;
import com.telerik.metadata.TreeNode.MethodInfo;
import com.telerik.metadata.parsing.classes.bytecode.JarFile;
import com.telerik.metadata.parsing.classes.NativeClassDescriptor;
import com.telerik.metadata.parsing.classes.NativeFieldDescriptor;
import com.telerik.metadata.parsing.classes.MetadataInfoAnnotationDescriptor;
import com.telerik.metadata.parsing.classes.NativeMethodDescriptor;
import com.telerik.metadata.parsing.classes.kotlin.extensions.KotlinExtensionFunctionDescriptor;
import com.telerik.metadata.parsing.classes.kotlin.properties.KotlinPropertyDescriptor;
import com.telerik.metadata.parsing.classes.NativeTypeDescriptor;
import com.telerik.metadata.parsing.classes.kotlin.metadata.ClassMetadataParser;
import com.telerik.metadata.parsing.classes.kotlin.metadata.bytecode.BytecodeClassMetadataParser;
import com.telerik.metadata.parsing.classes.kotlin.extensions.ClassNameAndFunctionPair;
import com.telerik.metadata.parsing.classes.kotlin.extensions.ExtensionFunctionsCollector;
import com.telerik.metadata.storage.functions.FunctionsStorage;
import com.telerik.metadata.parsing.classes.kotlin.extensions.bytecode.BytecodeExtensionFunctionsCollector;
import com.telerik.metadata.storage.functions.extensions.ExtensionFunctionsStorage;
import com.telerik.metadata.parsing.ClassParser;

import java.io.File;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class Builder {
    private static class MethodNameComparator implements Comparator<NativeMethodDescriptor> {
        @Override
        public int compare(NativeMethodDescriptor o1, NativeMethodDescriptor o2) {
            return o1.getName().compareTo(o2.getName());
        }
    }

    private static MethodNameComparator methodNameComparator = new MethodNameComparator();

    static TreeNode build(List<String> paths) throws Exception {
        for (String path : paths) {
            File file = new File(path);
            if (file.exists()) {
                if (file.isFile()) {
                    if (path.endsWith(".jar")) {
                        JarFile jar = JarFile.readJar(path);
                        ClassRepo.addToCache(jar);
                    }
                } else if (file.isDirectory()) {
                    ClassDirectory dir = ClassDirectory.readDirectory(path);
                    ClassRepo.addToCache(dir);
                }
            }
        }

        TreeNode root = TreeNode.getRoot();

        String[] classNames = ClassRepo.getClassNames();

        for (String className : classNames) {
            try {
                NativeClassDescriptor clazz = ClassRepo.findClass(className);
                if (clazz == null) {
                    throw new ClassNotFoundException("Class " + className + " not found in the input android libraries.");
                } else {
                    tryCollectKotlinExtensionFunctions(clazz);
                }
            } catch (Throwable e) {
                System.out.println("Skip " + className);
                System.out.println("\tError: " + e.toString());
                //e.printStackTrace();
            }
        }

        for (String className : classNames) {
            try {
                // possible exceptions here are:
                // - NoClassDefFoundError
                // - ClassNotFoundException
                // both are raised due to some API level mismatch -
                // e.g. we are processing jars with API 21 while we have in
                // our class path API 17
                // Class<?> clazz = Class.forName(className, false, loader);

                NativeClassDescriptor clazz = ClassRepo.findClass(className);
                if (clazz == null) {
                    throw new ClassNotFoundException("Class " + className + " not found in the input android libraries.");
                } else {
                    if(clazz.getClassName().contains("SomePublicClass")){
                        System.out.println("asd");
                    }
                    generate(clazz, root);
                }
            } catch (Throwable e) {
                System.out.println("Skip " + className);
                System.out.println("\tError: " + e.toString());
                //e.printStackTrace();
            }
        }


        return root;
    }

    private static void tryCollectKotlinExtensionFunctions(NativeClassDescriptor classDescriptor) {
        ExtensionFunctionsCollector extensionFunctionsCollector = new BytecodeExtensionFunctionsCollector(new BytecodeClassMetadataParser());
        Collection<ClassNameAndFunctionPair<KotlinExtensionFunctionDescriptor>> extensionFunctions = extensionFunctionsCollector.collectExtensionFunctions(classDescriptor);

        if (!extensionFunctions.isEmpty()) {
            FunctionsStorage<KotlinExtensionFunctionDescriptor> extensionFunctionsStorage = ExtensionFunctionsStorage.getInstance();
            extensionFunctionsStorage.storeFunctions(extensionFunctions);
        }
    }

    private static Boolean isClassPublic(NativeClassDescriptor clazz) {
        boolean isPublic = true;

        try {
            NativeClassDescriptor currClass = clazz;
            while (currClass != null) {
                if (!currClass.isPublic() && !currClass.isProtected()) {
                    isPublic = false;
                    break;
                }

                currClass = ClassUtil.getEnclosingClass(currClass);
            }
        } catch (NoClassDefFoundError e) {
            isPublic = false;
        }

        return isPublic;
    }

    private static void generate(NativeClassDescriptor clazz, TreeNode root) throws Exception {
        if (!isClassPublic(clazz)) {
            return;
        }

        MetadataInfoAnnotationDescriptor metadataInfo = clazz.getMetadataInfoAnnotation();
        boolean hasClassMetadataInfo = metadataInfo != null;
        String predefinedSuperClassname = null;
        if (hasClassMetadataInfo) {
            if (metadataInfo.skip()) {
                return;
            } else {
                predefinedSuperClassname = metadataInfo.getSuperClassname();
            }
        }

        TreeNode node = getOrCreateNode(root, clazz, predefinedSuperClassname);
        if (node != null) {
            setNodeMembers(clazz, node, root, hasClassMetadataInfo);
        }
    }

    private static void setNodeMembers(NativeClassDescriptor clazz, TreeNode node, TreeNode root, boolean hasClassMetadataInfo) throws Exception {
        node.setWentThroughSettingMembers(true);

        Map<String, MethodInfo> existingMethods = new HashMap<>();
        for (MethodInfo mi : node.instanceMethods) {
            existingMethods.put(mi.name + mi.sig, mi);
        }
        NativeMethodDescriptor[] extensionFunctions = ExtensionFunctionsStorage.getInstance().retrieveFunctions(clazz.getClassName()).toArray(new NativeMethodDescriptor[0]);
        NativeMethodDescriptor[] nonExtensionFunctionMethods = ClassUtil.getAllMethods(clazz);
        NativeMethodDescriptor[] allMethods = concatenate(extensionFunctions, nonExtensionFunctionMethods);

        NativeMethodDescriptor[] classImplementedMethods = clazz.getMethods();
        NativeMethodDescriptor[] interfaceImplementedMethods = clazz.isClass() ? getDefaultMethodsFromImplementedInterfaces(clazz, classImplementedMethods) : new NativeMethodDescriptor[0];
        NativeMethodDescriptor[] methods = concatenate(classImplementedMethods, interfaceImplementedMethods, extensionFunctions);
        Arrays.sort(methods, methodNameComparator);

        setMethodsInfo(root, node, clazz, methods, allMethods, existingMethods, hasClassMetadataInfo);

        NativeFieldDescriptor[] fields = clazz.getFields();

        setFieldInfo(clazz, node, root, fields, null);

        setPropertiesInfo(root, node, clazz.getProperties());

        // adds static fields from interface to implementing class because java can call them from implementing class... no problem.
        getFieldsFromImplementedInterfaces(clazz, node, root, fields);
    }

    private static void setMethodsInfo(TreeNode root, TreeNode node, NativeClassDescriptor clazz, NativeMethodDescriptor[] ownMethodDescriptors, NativeMethodDescriptor[] ownAndParentMethodDescriptors, Map<String, MethodInfo> existingNodeMethods, boolean hasClassMetadataInfo) throws Exception {


        for (NativeMethodDescriptor m : ownMethodDescriptors) {
            if (m.isSynthetic()) {
                continue;
            }

            if (hasClassMetadataInfo && !m.getName().equals("<init>")) {
                MetadataInfoAnnotationDescriptor metadataInfo = m.getMetadataInfoAnnotation();
                if ((metadataInfo != null) && metadataInfo.skip()) {
                    continue;
                }
            }

            if (m.isPublic() || m.isProtected()) {
                boolean isStatic = m.isStatic();

                MethodInfo mi = new MethodInfo(m);
                int countUnique = 0;
                for (NativeMethodDescriptor m1 : ownAndParentMethodDescriptors) {
                    boolean m1IsStatic = m1.isStatic();
                    if (!m1.isSynthetic()
                            && (m1.isPublic() || m1.isProtected())
                            && (isStatic == m1IsStatic)
                            && (m1.getName().equals(mi.name) && (m1
                            .getArgumentTypes().length == m
                            .getArgumentTypes().length))) {
                        if (++countUnique > 1) {
                            break;
                        }
                    }
                }
                mi.isResolved = countUnique == 1;


                NativeTypeDescriptor[] params = m.getArgumentTypes();
                mi.signature = getMethodSignature(root, m.getReturnType(),
                        params);

                if (mi.signature != null) {
                    if (isStatic) {
                        if (!mi.isExtensionFunction) {
                            mi.declaringType = getOrCreateNode(root, clazz, null);
                            node.staticMethods.add(mi);
                        } else {
                            mi.declaringType = getOrCreateNode(root, m.getDeclaringClass(), null);
                            node.addExtensionFunction(mi);
                        }
                    } else {
                        String sig = m.getName() + m.getSignature();
                        if (existingNodeMethods.containsKey(sig)) {
                            continue;
                        }
                        node.instanceMethods.add(mi);
                    }
                }

                if (mi.isResolved) {
                    node.resolvedMethods.add(mi);
                }
            }
        }

    }

    private static void setPropertiesInfo(TreeNode root, TreeNode node, KotlinPropertyDescriptor[] propertyDescriptors) throws Exception {
        for (KotlinPropertyDescriptor propertyDescriptor : propertyDescriptors) {
            String propertyName = propertyDescriptor.getName();

            NativeMethodDescriptor getterMethod = propertyDescriptor.getGetterMethod();
            NativeMethodDescriptor setterMethod = propertyDescriptor.getSetterMethod();

            MethodInfo getterMethodInfo = null;
            if (getterMethod != null) {
                getterMethodInfo = new MethodInfo(getterMethod);
                getterMethodInfo.isResolved = true;
                getterMethodInfo.signature = getMethodSignature(root, getterMethod.getReturnType(), getterMethod.getArgumentTypes());
            }


            MethodInfo setterMethodInfo = null;
            if (setterMethod != null) {
                setterMethodInfo = new MethodInfo(setterMethod);
                setterMethodInfo.isResolved = true;
                setterMethodInfo.signature = getMethodSignature(root, setterMethod.getReturnType(), setterMethod.getArgumentTypes());
            }

            TreeNode.PropertyInfo propertyInfo = new TreeNode.PropertyInfo(propertyName, getterMethodInfo, setterMethodInfo);
            node.addProperty(propertyInfo);
        }

    }

    private static void setFieldInfo(NativeClassDescriptor clazz, TreeNode node, TreeNode root, NativeFieldDescriptor[] fields, NativeClassDescriptor interfaceClass) throws Exception {
        for (NativeFieldDescriptor f : fields) {
            if (f.isPublic() || f.isProtected()) {
                FieldInfo fi = new FieldInfo(f.getName());

                NativeTypeDescriptor t = f.getType();
                boolean isPrimitive = ClassUtil.isPrimitive(t);

                fi.valueType = isPrimitive ? TreeNode.getPrimitive(t) : getOrCreateNode(root, t);
                fi.isFinalType = f.isFinal();

                if (f.isStatic()) {
                    if (interfaceClass != null) {
                        // changes declaring type of static fields from implementing class to interface
                        fi.declaringType = getOrCreateNode(root, interfaceClass, null);
                    } else {
                        fi.declaringType = getOrCreateNode(root, clazz, null);
                    }
                    node.staticFields.add(fi);
                } else {
                    node.instanceFields.add(fi);
                }
            }
        }
    }

    private static void getFieldsFromImplementedInterfaces(NativeClassDescriptor clazz, TreeNode node, TreeNode root, NativeFieldDescriptor[] classFields) throws Exception {
        NativeFieldDescriptor[] fields;
        List<NativeFieldDescriptor> originalClassFields = Arrays.asList(classFields);

        NativeClassDescriptor interfaceClass;
        String[] implementedInterfacesNames = clazz.getInterfaceNames();
        if (implementedInterfacesNames.length > 0) {
            for (String currInterface : implementedInterfacesNames) {
                interfaceClass = ClassRepo.findClass(currInterface);
                if (interfaceClass != null) {
                    fields = interfaceClass.getFields();

                    // If the interface iteself extends other interfaces - add their fields too
                    getFieldsFromImplementedInterfaces(interfaceClass, node, root, fields);

                    //if interface and implementing class declare the same static field name the class take precedence
                    if (originalClassFields.size() > 0) {
                        for (NativeFieldDescriptor f : fields) {
                            if (originalClassFields.contains(f)) {
                                return;
                            }
                        }
                    }
                    setFieldInfo(clazz, node, root, fields, interfaceClass);
                }
            }
        }
    }

    private static NativeMethodDescriptor[] getDefaultMethodsFromImplementedInterfaces(NativeClassDescriptor clazz, NativeMethodDescriptor[] originalClassMethodDescriptors) {
        ClassParser parser = ClassParser.forClassDescriptor(clazz);

        Set<NativeMethodDescriptor> defaultMethods = parser.getAllDefaultMethodsFromImplementedInterfaces();
        Set<NativeMethodDescriptor> classMethods = new HashSet<>(Arrays.asList(originalClassMethodDescriptors));

        defaultMethods.removeAll(classMethods);

        return defaultMethods.toArray(new NativeMethodDescriptor[0]);
    }

    private static TreeNode getOrCreateNode(TreeNode root, NativeTypeDescriptor type)
            throws Exception {
        TreeNode node;

        String typeName = type.getSignature();

        if (typeName.startsWith("[")) {
            node = createArrayNode(root, typeName);
        } else {
            String name = ClassUtil.getCanonicalName(type.getSignature());
            NativeClassDescriptor clazz = ClassRepo.findClass(name);

            // if clazz is not found in the ClassRepo, the method/field being analyzed will be skipped
            if (clazz == null) {
                return null;
            }

            node = getOrCreateNode(root, clazz, null);
        }

        return node;
    }

    private static TreeNode getOrCreateNode(TreeNode root, NativeClassDescriptor clazz, String predefinedSuperClassname) throws Exception {

        if (ClassUtil.isPrimitive(clazz)) {
            return TreeNode.getPrimitive(clazz);
        }

        if (ClassUtil.isArray(clazz)) {
            throw new UnsupportedOperationException("unexpected class="
                    + clazz.getClassName());
        }

        TreeNode node = root;
        String name = ClassUtil.getSimpleName(clazz);

        String[] packages = clazz.getPackageName().split("\\.");

        for (String p : packages) {
            TreeNode child = node.getChild(p);
            if (child == null) {
                child = node.createChild(p);
                node.nodeType = TreeNode.Package;
            }
            node = child;
        }

        NativeClassDescriptor outer = ClassUtil.getEnclosingClass(clazz);


        ArrayList<NativeClassDescriptor> outerClasses = new ArrayList<>();
        while (outer != null) {
            if (!outer.isPublic()) {
                return null;
            }
            outerClasses.add(outer);
            outer = ClassUtil.getEnclosingClass(outer);
        }

        if (outerClasses.size() > 0) {
            for (int i = outerClasses.size() - 1; i >= 0; i--) {
                outer = outerClasses.get(i);
                String outerClassname = ClassUtil.getSimpleName(outer);
                TreeNode child = node.getChild(outerClassname);
                if (child == null) {
                    child = node.createChild(outerClassname);
                    child.nodeType = outer.isInterface() ? TreeNode.Interface
                            : TreeNode.Class;
                    if (outer.isStatic()) {
                        child.nodeType |= TreeNode.Static;
                    }
                }
                node = child;
            }
        }

        TreeNode child = node.getChild(name);
        if (child == null) {
            if (isNestedClassKotlinCompanionObject(outer, clazz)) {
                child = node.createCompanionNode(name);
            } else {
                child = node.createChild(name);
            }

            if (ClassUtil.isPrimitive(clazz)) {
                TreeNode tmp = TreeNode.getPrimitive(clazz);
                child.nodeType = tmp.nodeType;
            } else {
                child.nodeType = clazz.isInterface() ? TreeNode.Interface
                        : TreeNode.Class;
                if (clazz.isStatic()) {
                    child.nodeType |= TreeNode.Static;
                }
            }
        }
        node = child;
        if (node.baseClassNode == null) {
            NativeClassDescriptor baseClass = null;
            if (predefinedSuperClassname != null) {
                baseClass = ClassUtil.getClassByName(predefinedSuperClassname);
            } else {
                baseClass = clazz.isInterface()
                        ? ClassUtil.getClassByName("java.lang.Object")
                        : ClassUtil.getSuperclass(clazz);
            }
            if (baseClass != null) {
                TreeNode baseClassNode = getOrCreateNode(root, baseClass, null);
                node.baseClassNode = baseClassNode;

                if (baseClassNode != null && !baseClassNode.hasWentThroughSettingMembers()) {
                    setNodeMembers(baseClass, baseClassNode, root, false);
                }
            }
        }

        return node;
    }

    private static boolean isNestedClassKotlinCompanionObject(NativeClassDescriptor outerClass, NativeClassDescriptor nestedClass) {
        if (outerClass == null || nestedClass == null) {
            return false;
        }

        ClassMetadataParser kotlinClassMetadataParser = new BytecodeClassMetadataParser();
        return kotlinClassMetadataParser.wasKotlinCompanionObject(outerClass, nestedClass);
    }

    private static TreeNode createArrayNode(TreeNode root, String className)
            throws Exception {
        TreeNode currentNode = root;
        String currentClassname = className;

        while (ClassUtil.isArray(currentClassname)) {
            TreeNode child = currentNode.getChild("[");
            if (child == null) {
                child = currentNode.createChild("[");
                child.nodeType = TreeNode.Array;
                child.offsetValue = 1;
            }
            currentClassname = currentClassname.substring(1);
            currentNode = child;
        }

        String name = ClassUtil.getCanonicalName(currentClassname);
        TreeNode child = currentNode.getChild(name);

        if (child == null) {
            child = currentNode.createChild(name);
            if (ClassUtil.isPrimitive(name)) {
                TreeNode node = TreeNode.getPrimitive(name);
                child.nodeType = node.nodeType;
                child.arrayElement = node;
            } else {
                NativeClassDescriptor clazz = ClassRepo.findClass(name);
                child.nodeType = clazz.isInterface() ? TreeNode.Interface
                        : TreeNode.Class;
                if (clazz.isStatic()) {
                    child.nodeType |= TreeNode.Static;
                }
                child.arrayElement = getOrCreateNode(root, clazz, null);
            }
        }

        return child;
    }

    private static ArrayList<TreeNode> getMethodSignature(TreeNode root,
                                                          NativeTypeDescriptor retType, NativeTypeDescriptor[] params) throws Exception {
        ArrayList<TreeNode> sig = new ArrayList<>();
        boolean isVoid = retType.equals(NativeTypeDescriptor.Companion.getVOID());

        TreeNode node = null;
        if (!isVoid) {
            boolean isPrimitive = ClassUtil.isPrimitive(retType);
            node = isPrimitive ? TreeNode.getPrimitive(retType)
                    : getOrCreateNode(root, retType);
        }
        sig.add(node);

        for (NativeTypeDescriptor param : params) {
            boolean isPrimitive = ClassUtil.isPrimitive(param);
            node = isPrimitive ? TreeNode.getPrimitive(param)
                    : getOrCreateNode(root, param);
            if (node == null) {
                return null;
            }
            sig.add(node);
        }

        return sig;
    }

    private static <T> T[] concatenate(T[] a, T[] b) {
        int aLen = a.length;
        int bLen = b.length;

        @SuppressWarnings("unchecked")
        T[] d = (T[]) Array.newInstance(a.getClass().getComponentType(), aLen + bLen);
        System.arraycopy(a, 0, d, 0, aLen);
        System.arraycopy(b, 0, d, aLen, bLen);

        return d;
    }

    private static <T> T[] concatenate(T[] a, T[] b, T[] c) {
        int aLen = a.length;
        int bLen = b.length;
        int cLen = c.length;
        int abLen = aLen + bLen;

        @SuppressWarnings("unchecked")
        T[] d = (T[]) Array.newInstance(a.getClass().getComponentType(), abLen + cLen);
        System.arraycopy(a, 0, d, 0, aLen);
        System.arraycopy(b, 0, d, aLen, bLen);
        System.arraycopy(c, 0, d, abLen, cLen);

        return d;
    }
}
