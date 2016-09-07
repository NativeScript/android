package com.telerik.metadata;

import java.util.ArrayList;

import org.apache.bcel.classfile.JavaClass;
import org.apache.bcel.classfile.Method;
import org.apache.bcel.generic.Type;

public class TreeNode {
	public static class MethodInfo {
		public MethodInfo(Method m) {
			this.name = m.getName();
			this.sig = m.getSignature();
			this.isResolved = false;
			signature = new ArrayList<TreeNode>();
		}

		public String name;
		public String sig;
		public ArrayList<TreeNode> signature;
		public TreeNode declaringType;
		public boolean isResolved;
	}

	public static TreeNode getRoot() {
		TreeNode root = new TreeNode();
		root.setName("");
		root.children.add(BYTE);
		root.children.add(SHORT);
		root.children.add(INTEGER);
		root.children.add(LONG);
		root.children.add(FLOAT);
		root.children.add(DOUBLE);
		root.children.add(BOOLEAN);
		root.children.add(CHAR);

		return root;
	}

	public static class FieldInfo {
		public FieldInfo(String name) {
			this.name = name;
		}

		public String name;
		public TreeNode valueType;
		public TreeNode declaringType;
		public boolean isFinalType;
	}

	public static final byte Package = 0;
	public static final byte Class = 1 << 0;
	public static final byte Interface = 1 << 1;
	public static final byte Static = 1 << 2;
	public static final byte Array = 1 << 3;
	public static final byte Primitive = 1 << 4;

	public static final byte Final = 1;

	public TreeNode() {
		children = new ArrayList<TreeNode>();

		instanceMethods = new ArrayList<TreeNode.MethodInfo>();
		staticMethods = new ArrayList<TreeNode.MethodInfo>();
		instanceFields = new ArrayList<TreeNode.FieldInfo>();
		staticFields = new ArrayList<TreeNode.FieldInfo>();
	}

	public static final TreeNode BYTE = getPrimitive("B", (byte) 1);
	public static final TreeNode SHORT = getPrimitive("S", (byte) 2);
	public static final TreeNode INTEGER = getPrimitive("I", (byte) 3);
	public static final TreeNode LONG = getPrimitive("J", (byte) 4);
	public static final TreeNode FLOAT = getPrimitive("F", (byte) 5);
	public static final TreeNode DOUBLE = getPrimitive("D", (byte) 6);
	public static final TreeNode BOOLEAN = getPrimitive("Z", (byte) 7);
	public static final TreeNode CHAR = getPrimitive("C", (byte) 8);

	public static TreeNode getPrimitive(Type type) throws Exception {
		if (!ClassUtil.isPrimitive(type)) {
			throw new Exception("type must be primitive");
		}

		if (type.equals(Type.BYTE)) {
			return TreeNode.BYTE;
		} else if (type.equals(Type.SHORT)) {
			return TreeNode.SHORT;
		} else if (type.equals(Type.INT)) {
			return TreeNode.INTEGER;
		} else if (type.equals(Type.LONG)) {
			return TreeNode.LONG;
		} else if (type.equals(Type.FLOAT)) {
			return TreeNode.FLOAT;
		} else if (type.equals(Type.DOUBLE)) {
			return TreeNode.DOUBLE;
		} else if (type.equals(Type.BOOLEAN)) {
			return TreeNode.BOOLEAN;
		} else if (type.equals(Type.CHAR)) {
			return TreeNode.CHAR;
		} else if (type.equals(Type.VOID)) {
			return null;
		} else {
			throw new Exception("unknown type=" + type.toString());
		}
	}

	public static TreeNode getPrimitive(JavaClass clazz) throws Exception {
		if (!ClassUtil.isPrimitive(clazz)) {
			throw new Exception("clazz must be primitive");
		}

		String name = clazz.getClassName();

		if (name.equals("byte")) {
			return TreeNode.BYTE;
		} else if (name.equals("short")) {
			return TreeNode.SHORT;
		} else if (name.equals("int")) {
			return TreeNode.INTEGER;
		} else if (name.equals("long")) {
			return TreeNode.LONG;
		} else if (name.equals("float")) {
			return TreeNode.FLOAT;
		} else if (name.equals("double")) {
			return TreeNode.DOUBLE;
		} else if (name.equals("boolean")) {
			return TreeNode.BOOLEAN;
		} else if (name.equals("char")) {
			return TreeNode.CHAR;
		} else if (name.equals("void")) {
			return null;
		} else {
			throw new Exception("unknown type=" + name);
		}
	}

	public static TreeNode getPrimitive(String name)
			throws IllegalArgumentException {
		if (name.equals("B")) {
			return TreeNode.BYTE;
		} else if (name.equals("S")) {
			return TreeNode.SHORT;
		} else if (name.equals("I")) {
			return TreeNode.INTEGER;
		} else if (name.equals("J")) {
			return TreeNode.LONG;
		} else if (name.equals("F")) {
			return TreeNode.FLOAT;
		} else if (name.equals("D")) {
			return TreeNode.DOUBLE;
		} else if (name.equals("Z")) {
			return TreeNode.BOOLEAN;
		} else if (name.equals("C")) {
			return TreeNode.CHAR;
		} else if (name.equals("V")) {
			return null;
		} else {
			throw new IllegalArgumentException("unknown type=" + name);
		}
	}

	private static TreeNode getPrimitive(String name, byte id) {
		TreeNode node = new TreeNode();
		node.setName(name);
		node.nodeType = (byte) (Primitive + id);
		node.offsetValue = 1;
		return node;
	}

	public String getName() {
		return _name;
	}

	public void setName(String value) {
		_name = value;
	}

	private String _name;
	//
	public short id;
	public short firstChildId;
	public short nextSiblingId;
	//
	public byte nodeType;
	public int offsetName;
	public int offsetValue;
	//
	public TreeNode arrayElement;
	//
	public ArrayList<MethodInfo> instanceMethods;
	public ArrayList<MethodInfo> staticMethods;
	public ArrayList<FieldInfo> instanceFields;
	public ArrayList<FieldInfo> staticFields;
	public TreeNode baseClassNode;
	//

	public ArrayList<TreeNode> children;

	public TreeNode getChild(String childName) {
		TreeNode child = null;

		for (TreeNode c : children) {
			boolean found = c.getName().equals(childName);

			if (found) {
				child = c;
				break;
			}
		}

		return child;
	}

	public TreeNode createChild(String childName) {
		TreeNode child = new TreeNode();
		child.setName(childName);
		children.add(child);
		return child;
	}

	public TreeNode attachChild(JavaClass clazz) throws Exception {
		TreeNode child = getPrimitive(clazz);
		children.add(child);
		return child;
	}

}
