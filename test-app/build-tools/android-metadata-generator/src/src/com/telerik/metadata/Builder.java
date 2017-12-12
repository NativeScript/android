package com.telerik.metadata;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.telerik.metadata.TreeNode.FieldInfo;
import com.telerik.metadata.TreeNode.MethodInfo;
import com.telerik.metadata.bcl.JarFile;
import com.telerik.metadata.desc.MetadataInfoAnnotationDescriptor;
import com.telerik.metadata.desc.ClassDescriptor;
import com.telerik.metadata.desc.FieldDescriptor;
import com.telerik.metadata.desc.MethodDescriptor;
import com.telerik.metadata.desc.TypeDescriptor;
import com.telerik.metadata.dx.DexFile;

public class Builder {
    private static class MethodNameComparator implements Comparator<MethodDescriptor> {
        @Override
        public int compare(MethodDescriptor o1, MethodDescriptor o2) {
            return o1.getName().compareTo(o2.getName());
        }
    }

    private static MethodNameComparator methodNameComparator = new MethodNameComparator();

    public static TreeNode build(List<String> paths) throws Exception {
        for (String path : paths) {
            File file = new File(path);
            if (file.exists()) {
                if (file.isFile()) {
                    if (path.endsWith(".jar")) {
                        JarFile jar = JarFile.readJar(path);
                        ClassRepo.addToCache(jar);
                    } else if (path.endsWith(".dex")) {
                        DexFile dex = DexFile.readDex(path);
                        ClassRepo.addToCache(dex);
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
                // possible exceptions here are:
                // - NoClassDefFoundError
                // - ClassNotFoundException
                // both are raised due to some API level mismatch -
                // e.g. we are processing jars with API 21 while we have in
                // our class path API 17
                // Class<?> clazz = Class.forName(className, false, loader);
                ClassDescriptor clazz = ClassRepo.findClass(className);
                if (clazz == null) {
                    throw new ClassNotFoundException("Class " + className + " not found in the input android libraries.");
                } else {
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

    private static Boolean isClassPublic(ClassDescriptor clazz) {
        Boolean isPublic = true;

        try {
            ClassDescriptor currClass = clazz;
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

    private static void generate(ClassDescriptor clazz, TreeNode root) throws Exception {
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

        setNodeMembers(clazz, node, root, hasClassMetadataInfo);
    }

    private static void setNodeMembers(ClassDescriptor clazz, TreeNode node, TreeNode root, boolean hasClassMetadataInfo) throws Exception {
        Map<String, MethodInfo> existingMethods = new HashMap<String, MethodInfo>();
        for (MethodInfo mi : node.instanceMethods) {
            existingMethods.put(mi.name + mi.sig, mi);
        }

        MethodDescriptor[] allMethods = ClassUtil.getAllMethods(clazz);
        MethodDescriptor[] methods = clazz.getMethods();

        Arrays.sort(methods, methodNameComparator);

        for (MethodDescriptor m : methods) {
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
                for (MethodDescriptor m1 : allMethods) {
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

                TypeDescriptor[] params = m.getArgumentTypes();
                mi.signature = getMethodSignature(root, m.getReturnType(),
                                                  params);

                if (mi.signature != null) {
                    if (isStatic) {
                        mi.declaringType = getOrCreateNode(root, clazz, null);
                        node.staticMethods.add(mi);
                    } else {
                        String sig = m.getName() + m.getSignature();
                        if (existingMethods.containsKey(sig)) {
                            continue;
                        }
                        node.instanceMethods.add(mi);
                    }
                }
            }
        }

        FieldDescriptor[] fields = clazz.getFields();

        setFieldInfo(clazz, node, root, fields, null);

        // adds static fields from interface to implementing class because java can call them from implementing class... no problem.
        getFieldsFromImplementedInterfaces(clazz, node, root, fields);
    }

    private static void setFieldInfo(ClassDescriptor clazz, TreeNode node, TreeNode root, FieldDescriptor[] fields, ClassDescriptor interfaceClass) throws Exception {
        for (FieldDescriptor f : fields) {
            if (f.isPublic() || f.isProtected()) {
                FieldInfo fi = new FieldInfo(f.getName());

                TypeDescriptor t = f.getType();
                boolean isPrimitive = ClassUtil.isPrimitive(t);

                fi.valueType = isPrimitive ? TreeNode.getPrimitive(t): getOrCreateNode(root, t);
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

    private static void getFieldsFromImplementedInterfaces(ClassDescriptor clazz, TreeNode node, TreeNode root, FieldDescriptor[] classFields) throws Exception {
        FieldDescriptor[] fields = null;
        List<FieldDescriptor> originalClassFields = Arrays.asList(classFields);

        ClassDescriptor interfaceClass = null;
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
                        for (FieldDescriptor f : fields) {
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

    private static TreeNode getOrCreateNode(TreeNode root, TypeDescriptor type)
    throws Exception {
        TreeNode node;

        String typeName = type.getSignature();

        if (typeName.startsWith("[")) {
            node = createArrayNode(root, typeName);
        } else {
            String name = ClassUtil.getCanonicalName(type.getSignature());
            ClassDescriptor clazz = ClassRepo.findClass(name);

            // if clazz is not found in the ClassRepo, the method/field being analyzed will be skipped
            if (clazz == null) {
                return null;
            }

            node = getOrCreateNode(root, clazz, null);
        }

        return node;
    }

    private static TreeNode getOrCreateNode(TreeNode root, ClassDescriptor clazz, String predefinedSuperClassname) throws Exception {
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

        ClassDescriptor outer = ClassUtil.getEnclosingClass(clazz);
        ArrayList<ClassDescriptor> outerClasses = new ArrayList<ClassDescriptor>();
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
            child = node.createChild(name);
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
            ClassDescriptor baseClass = null;
            if (predefinedSuperClassname != null) {
                baseClass = ClassUtil.getClassByName(predefinedSuperClassname);
            } else {
                baseClass = clazz.isInterface()
                            ? ClassUtil.getClassByName("java.lang.Object")
                            : ClassUtil.getSuperclass(clazz);
            }
            if (baseClass != null) {
                node.baseClassNode = getOrCreateNode(root, baseClass, null);
                copyBasePublicApi(baseClass, node, root);
            }
        }

        return node;
    }

    private static void copyBasePublicApi(ClassDescriptor baseClass, TreeNode node,
                                          TreeNode root) throws Exception {
        while ((baseClass != null) && !baseClass.isPublic()) {
            setNodeMembers(baseClass, node, root, false);
            baseClass = ClassUtil.getSuperclass(baseClass);
        }
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
                ClassDescriptor clazz = ClassRepo.findClass(name);
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
            TypeDescriptor retType, TypeDescriptor[] params) throws Exception {
        ArrayList<TreeNode> sig = new ArrayList<TreeNode>();
        boolean isVoid = retType.equals(TypeDescriptor.VOID);

        TreeNode node = null;
        if (!isVoid) {
            boolean isPrimitive = ClassUtil.isPrimitive(retType);
            node = isPrimitive ? TreeNode.getPrimitive(retType)
                   : getOrCreateNode(root, retType);
        }
        sig.add(node);

        for (TypeDescriptor param : params) {
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
}
