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

import org.apache.bcel.classfile.Attribute;
import org.apache.bcel.classfile.ConstantUtf8;
import org.apache.bcel.classfile.InnerClass;
import org.apache.bcel.classfile.InnerClasses;
import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.classfile.Field;
import org.apache.bcel.generic.Type;

public class Builder {
	private static class MethodNameComparator implements Comparator<Method> {
		@Override
		public int compare(Method o1, Method o2) {
			return o1.getName().compareTo(o2.getName());
		}
	}

	private static MethodNameComparator methodNameComparator = new MethodNameComparator();

	public static TreeNode build(String[] paths) throws Exception {
		for (String path : paths) {
			File file = new File(path);
			if (file.exists()) {
				if (file.isFile() && path.endsWith(".jar")) {
					JarFile jar = JarFile.readJar(path);
					ClassRepo.cacheJarFile(jar);
				} else if (file.isDirectory()) {
					ClassDirectrory dir = ClassDirectrory.readDirectory(path);
					ClassRepo.cacheJarFile(dir);
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
				JavaClass clazz = ClassRepo.findClass(className);
				generate(clazz, root);
			} catch (Throwable e) {
				System.out.println("Skip " + className);
				System.out.println("\tError: " + e.toString());
			}
		}

		return root;
	}

	private static Boolean isClassPublic(JavaClass clazz) {
		Boolean isPublic = true;

		try {
			JavaClass currClass = clazz;
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

	private static void generate(JavaClass clazz, TreeNode root) throws Exception {
		if (!isClassPublic(clazz)) {
			return;
		}
		TreeNode node = getOrCreateNode(root, clazz);

		setNodeMembers(clazz, node, root);
	}

	private static void setNodeMembers(JavaClass clazz, TreeNode node, TreeNode root) throws Exception {
		Map<String, MethodInfo> existingMethods = new HashMap<String, MethodInfo>();
		for (MethodInfo mi : node.instanceMethods) {
			existingMethods.put(mi.name + mi.sig, mi);
		}

		Method[] allMethods = ClassUtil.getAllMethods(clazz);
		Method[] methods = clazz.getMethods();

		Arrays.sort(methods, methodNameComparator);

		for (Method m : methods) {
			if (m.isSynthetic())
				continue;

			if (m.isPublic() || m.isProtected()) {
				boolean isStatic = m.isStatic();

				MethodInfo mi = new MethodInfo(m);
				int countUnique = 0;
				for (Method m1 : allMethods) {
					boolean m1IsStatic = m1.isStatic();
					if (!m1.isSynthetic()
							&& (m1.isPublic() || m1.isProtected())
							&& (isStatic == m1IsStatic)
							&& (m1.getName().equals(mi.name) && (m1
									.getArgumentTypes().length == m
									.getArgumentTypes().length))) {
						if (++countUnique > 1)
							break;
					}
				}
				mi.isResolved = countUnique == 1;

				Type[] params = m.getArgumentTypes();
				mi.signature = getMethodSignature(root, m.getReturnType(),
						params);

				if (mi.signature != null) {
					if (isStatic) {
						mi.declaringType = getOrCreateNode(root, clazz);
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

		Field[] fields = clazz.getFields();

		setFieldInfo(clazz, node, root, fields, null);

		// adds static fields from interface to implementing class because java can call them from implementing class... no problem.
		getFieldsFromImplementedInterfaces(clazz, node, root, fields);
	}

	private static void setFieldInfo(JavaClass clazz, TreeNode node, TreeNode root, Field[] fields, JavaClass interfaceClass) throws Exception {
		for (Field f : fields) {
			if (f.isPublic() || f.isProtected()) {
				FieldInfo fi = new FieldInfo(f.getName());

				Type t = f.getType();
				boolean isPrimitive = ClassUtil.isPrimitive(t);

				fi.valueType = isPrimitive ? TreeNode.getPrimitive(t): getOrCreateNode(root, t);
				fi.isFinalType = f.isFinal();

				if (f.isStatic()) {
					if(interfaceClass != null) {
						// changes declaring type of static fields from implementing class to interface
						fi.declaringType = getOrCreateNode(root, interfaceClass);	
					}
					else {
						fi.declaringType = getOrCreateNode(root, clazz);
					}
					node.staticFields.add(fi);
				} else {
					node.instanceFields.add(fi);
				}
			}
		}
	}

	private static void getFieldsFromImplementedInterfaces(JavaClass clazz, TreeNode node, TreeNode root, Field[] classFields) throws Exception {
		Field[] fields = null;
		List<Field> originalClassFields = Arrays.asList(classFields);
		
		JavaClass interfaceClass = null;
		String[] implementedInterfacesNames = clazz.getInterfaceNames();
		if(implementedInterfacesNames.length > 0) {
			for(String currInterface : implementedInterfacesNames) {
				interfaceClass = ClassRepo.findClass(currInterface);
				if(interfaceClass != null) {
					fields = interfaceClass.getFields();
					
					//if interface and implementing class declare the same static field name the class take precedence
					if(originalClassFields.size() > 0) {
						for(Field f : fields) {
							if(originalClassFields.contains(f)) {
								return;
							}
						}
					}
					setFieldInfo(clazz, node, root, fields, interfaceClass);
				}
			}
		}
	}

	private static TreeNode getOrCreateNode(TreeNode root, Type type)
			throws Exception {
		TreeNode node;

		String typeName = type.getSignature();

		if (typeName.startsWith("[")) {
			node = createArrayNode(root, typeName);
		} else {
			String name = ClassUtil.getCanonicalName(type.getSignature());
			JavaClass clazz = ClassRepo.findClass(name);
			node = getOrCreateNode(root, clazz);
		}

		return node;
	}

	private static TreeNode getOrCreateNode(TreeNode root, JavaClass clazz) throws Exception {
		if (ClassUtil.isPrimitive(clazz)) {
			return TreeNode.getPrimitive(clazz);
		}

		if (ClassUtil.isArray(clazz)) {
			throw new UnsupportedOperationException("unexpected class="
					+ clazz.getClassName());
		}

		TreeNode node = root;
		String name = ClassUtil.getSimpleName(clazz);

		if (clazz.getClassName().contains("$")) {
			boolean found = false;
			for (Attribute a : clazz.getAttributes()) {
				if (a instanceof InnerClasses) {
					InnerClass[] i = ((InnerClasses) a).getInnerClasses();
					for (InnerClass ic : i) {

						ConstantUtf8 cname = (ConstantUtf8) clazz
								.getConstantPool().getConstant(ic.getInnerNameIndex());
						String innerClassname = cname.getBytes();

						if (name.equals(innerClassname)) {
							int flags = ic.getInnerAccessFlags();
							clazz.setAccessFlags(flags);
							found = true;
							break;
						}
					}
				}
				if (found)
					break;
			}
		}

		String[] packages = clazz.getPackageName().split("\\.");

		for (String p : packages) {
			TreeNode child = node.getChild(p);
			if (child == null) {
				child = node.createChild(p);
				node.nodeType = TreeNode.Package;
			}
			node = child;
		}

		JavaClass outer = ClassUtil.getEnclosingClass(clazz);
		ArrayList<JavaClass> outerClasses = new ArrayList<JavaClass>();
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
			JavaClass baseClass = clazz.isInterface() ? ClassUtil
					.getClassByName("java.lang.Object") : ClassUtil
					.getSuperclass(clazz);
			if (baseClass != null) {
				node.baseClassNode = getOrCreateNode(root, baseClass);
				copyBasePublicApi(baseClass, node, root);
			}
		}

		return node;
	}

	private static void copyBasePublicApi(JavaClass baseClass, TreeNode node,
			TreeNode root) throws Exception {
		while ((baseClass != null) && !baseClass.isPublic()) {
			setNodeMembers(baseClass, node, root);
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
				JavaClass clazz = ClassRepo.findClass(name);
				child.nodeType = clazz.isInterface() ? TreeNode.Interface
						: TreeNode.Class;
				if (clazz.isStatic()) {
					child.nodeType |= TreeNode.Static;
				}
				child.arrayElement = getOrCreateNode(root, clazz);
			}
		}

		return child;
	}

	private static ArrayList<TreeNode> getMethodSignature(TreeNode root,
			Type retType, Type[] params) throws Exception {
		ArrayList<TreeNode> sig = new ArrayList<TreeNode>();
		boolean isVoid = retType.equals(Type.VOID);

		TreeNode node = null;
		if (!isVoid) {
			boolean isPrimitive = ClassUtil.isPrimitive(retType);
			node = isPrimitive ? TreeNode.getPrimitive(retType)
					: getOrCreateNode(root, retType);
		}
		sig.add(node);

		for (Type param : params) {
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
