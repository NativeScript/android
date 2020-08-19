package com.telerik.metadata;

import com.telerik.metadata.parsing.NativeClassDescriptor;
import com.telerik.metadata.parsing.kotlin.extensions.KotlinExtensionFunctionDescriptor;
import com.telerik.metadata.parsing.NativeMethodDescriptor;
import com.telerik.metadata.parsing.NativeTypeDescriptor;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class TreeNode {
    public static int skippedMethods=0;
    public static int skippedProperties=0;
    public static int skippedFields=0;
    public static int addedMethods=0;
    public static int addedProperties=0;
    public static int addedFields=0;

    public static class MethodInfo {
        public MethodInfo(NativeMethodDescriptor m) {
            this.name = m.getName();
            this.sig = m.getSignature();
            this.isResolved = false;
            this.isExtensionFunction = m instanceof KotlinExtensionFunctionDescriptor;

            signature = new ArrayList<>();
        }

        public boolean isExtensionFunction;
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

    public static class PropertyInfo {

        private final String propertyName;
        private final MethodInfo getterMethod;
        private final MethodInfo setterMethod;

        public PropertyInfo(String propertyName, MethodInfo getterMethod, MethodInfo setterMethod) {
            this.propertyName = propertyName;
            this.getterMethod = getterMethod;
            this.setterMethod = setterMethod;
        }

        public String getPropertyName() {
            return propertyName;
        }

        public Optional<MethodInfo> getGetterMethod() {
            return getterMethod == null ? Optional.empty() : Optional.of(getterMethod);
        }

        public Optional<MethodInfo> getSetterMethod() {
            return setterMethod == null ? Optional.empty() : Optional.of(setterMethod);
        }
    }

    public static final byte Package = 0;
    public static final byte Class = 1 << 0;
    public static final byte Interface = 1 << 1;
    public static final byte Static = 1 << 2;
    public static final byte Array = 1 << 3;
    public static final byte Primitive = 1 << 4;

    public static final byte Final = 1;

    public TreeNode() {
        children = new ArrayList<>();

        instanceMethods = new ArrayList<>();
        staticMethods = new ArrayList<>();
        instanceFields = new ArrayList<>();
        staticFields = new ArrayList<>();
        properties = new ArrayList<>();
        extensionFunctions = new ArrayList<>();
    }

    public static final TreeNode BYTE = getPrimitive("B", (byte) 1);
    public static final TreeNode SHORT = getPrimitive("S", (byte) 2);
    public static final TreeNode INTEGER = getPrimitive("I", (byte) 3);
    public static final TreeNode LONG = getPrimitive("J", (byte) 4);
    public static final TreeNode FLOAT = getPrimitive("F", (byte) 5);
    public static final TreeNode DOUBLE = getPrimitive("D", (byte) 6);
    public static final TreeNode BOOLEAN = getPrimitive("Z", (byte) 7);
    public static final TreeNode CHAR = getPrimitive("C", (byte) 8);

    public static TreeNode getPrimitive(NativeTypeDescriptor type) throws Exception {
        if (!ClassUtil.isPrimitive(type)) {
            throw new Exception("type must be primitive");
        }

        if (type.equals(NativeTypeDescriptor.Companion.getBYTE())) {
            return TreeNode.BYTE;
        } else if (type.equals(NativeTypeDescriptor.Companion.getSHORT())) {
            return TreeNode.SHORT;
        } else if (type.equals(NativeTypeDescriptor.Companion.getINT())) {
            return TreeNode.INTEGER;
        } else if (type.equals(NativeTypeDescriptor.Companion.getLONG())) {
            return TreeNode.LONG;
        } else if (type.equals(NativeTypeDescriptor.Companion.getFLOAT())) {
            return TreeNode.FLOAT;
        } else if (type.equals(NativeTypeDescriptor.Companion.getDOUBLE())) {
            return TreeNode.DOUBLE;
        } else if (type.equals(NativeTypeDescriptor.Companion.getBOOLEAN())) {
            return TreeNode.BOOLEAN;
        } else if (type.equals(NativeTypeDescriptor.Companion.getCHAR())) {
            return TreeNode.CHAR;
        } else if (type.equals(NativeTypeDescriptor.Companion.getVOID())) {
            return null;
        } else {
            throw new Exception("unknown type=" + type.toString());
        }
    }

    public static TreeNode getPrimitive(NativeClassDescriptor clazz) throws Exception {
        if (!ClassUtil.isPrimitive(clazz)) {
            throw new Exception("clazz must be primitive");
        }

        String name = clazz.getClassName();

        switch (name) {
            case "byte":
                return TreeNode.BYTE;
            case "short":
                return TreeNode.SHORT;
            case "int":
                return TreeNode.INTEGER;
            case "long":
                return TreeNode.LONG;
            case "float":
                return TreeNode.FLOAT;
            case "double":
                return TreeNode.DOUBLE;
            case "boolean":
                return TreeNode.BOOLEAN;
            case "char":
                return TreeNode.CHAR;
            case "void":
                return null;
            default:
                throw new Exception("unknown type=" + name);
        }
    }

    public static TreeNode getPrimitive(String name)
            throws IllegalArgumentException {
        switch (name) {
            case "B":
                return TreeNode.BYTE;
            case "S":
                return TreeNode.SHORT;
            case "I":
                return TreeNode.INTEGER;
            case "J":
                return TreeNode.LONG;
            case "F":
                return TreeNode.FLOAT;
            case "D":
                return TreeNode.DOUBLE;
            case "Z":
                return TreeNode.BOOLEAN;
            case "C":
                return TreeNode.CHAR;
            case "V":
                return null;
            default:
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
    private ArrayList<MethodInfo> instanceMethods;
    private ArrayList<MethodInfo> staticMethods;
    private ArrayList<FieldInfo> instanceFields;
    private ArrayList<FieldInfo> staticFields;
    private ArrayList<PropertyInfo> properties;
    private ArrayList<MethodInfo> extensionFunctions;
    public TreeNode baseClassNode;
    TreeNode parentNode;
    //

    void addProperty(PropertyInfo propertyInfo) {
        for (int i=0;i<properties.size();i++) {
            PropertyInfo pi = properties.get(i);
            if (pi.propertyName.equals(propertyInfo.propertyName)) {
                if (pi.getterMethod != null || propertyInfo.getterMethod != null) {
                    if (pi.getterMethod != null && propertyInfo.getterMethod != null) {
                        if (pi.getterMethod.sig != propertyInfo.getterMethod.sig) {
                            continue;
                        }
                    } else {
                        continue;
                    }
                }
                if (pi.setterMethod != null || propertyInfo.setterMethod != null) {
                    if (pi.setterMethod != null && propertyInfo.setterMethod != null) {
                        if (pi.setterMethod.sig != propertyInfo.setterMethod.sig) {
                            continue;
                        }
                    } else {
                        continue;
                    }
                }
                skippedProperties++;
                return;
            }
        }
        properties.add(propertyInfo);
        addedProperties++;
    }

    void addStaticField(FieldInfo fieldInfo) {
        for (int i=0;i<staticFields.size();i++) {
            FieldInfo fi = staticFields.get(i);
            if (fi.name.equals(fieldInfo.name) && fi.isFinalType == fieldInfo.isFinalType) {
                if (fi.declaringType == fieldInfo.declaringType && fi.valueType == fieldInfo.valueType) {
                    skippedFields++;
                    return;
                }
            }
        }
        staticFields.add(fieldInfo);
        addedFields++;
    }


    void addInstanceField(FieldInfo fieldInfo) {
        for (int i=0;i<instanceFields.size();i++) {
            FieldInfo fi = instanceFields.get(i);
            if (fi.name.equals(fieldInfo.name) && fi.isFinalType == fieldInfo.isFinalType) {
                if (fi.declaringType == fieldInfo.declaringType && fi.valueType == fieldInfo.valueType) {
                    skippedFields++;
                    return;
                }
            }
        }
        instanceFields.add(fieldInfo);
        addedFields++;
    }

    void addExtensionFunction(MethodInfo methodInfo) {
        for (int i=0;i<extensionFunctions.size();i++) {
            MethodInfo mi = extensionFunctions.get(i);
            if (mi.name.equals(methodInfo.name) && mi.sig.equals(methodInfo.sig)) {
                if (mi.isResolved == methodInfo.isResolved) {
                    if (mi.declaringType == methodInfo.declaringType) {
                        skippedMethods++;
                        return;
                    }
                }
            }
        }
        extensionFunctions.add(methodInfo);
        addedMethods++;
    }

    void addStaticFunction(MethodInfo methodInfo) {
        for (int i=0;i<staticMethods.size();i++) {
            MethodInfo mi = staticMethods.get(i);
            if (mi.name.equals(methodInfo.name) && mi.sig.equals(methodInfo.sig)) {
                if (mi.isResolved == methodInfo.isResolved) {
                    if (mi.declaringType == methodInfo.declaringType) {
                        skippedMethods++;
                        return;
                    }
                }
            }
        }
        this.staticMethods.add(methodInfo);
        addedMethods++;
    }

    void addInstanceMethod(MethodInfo methodInfo) {
        for (int i=0;i<instanceMethods.size();i++) {
            MethodInfo mi = instanceMethods.get(i);
            if (mi.name.equals(methodInfo.name) && mi.sig.equals(methodInfo.sig)) {
                if (mi.isResolved == methodInfo.isResolved) {
                    if (mi.declaringType == methodInfo.declaringType) {
                        skippedMethods++;
                        return;
                    }
                }
            }
        }
        instanceMethods.add(methodInfo);
        addedMethods++;
    }

    List<PropertyInfo> getProperties() {
        return properties;
    }

    List<FieldInfo> getInstanceFields() {
        return instanceFields;
    }

    List<FieldInfo> getStaticFields() {
        return staticFields;
    }

    List<MethodInfo> getExtensionFunctions() {
        return extensionFunctions;
    }

    List<MethodInfo> getInstanceMethods() {
        return instanceMethods;
    }

    List<MethodInfo> getStaticMethods() {
        return staticMethods;
    }

    private boolean wentThroughSettingMembers = false;

    public boolean hasWentThroughSettingMembers() {
        return wentThroughSettingMembers;
    }

    public void setWentThroughSettingMembers(boolean wentThroughSettingMembers) {
        this.wentThroughSettingMembers = wentThroughSettingMembers;
    }

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
        child.parentNode = this;
        child.setName(childName);
        children.add(child);
        return child;
    }

    TreeNode createCompanionNode(String companionNode) {
        TreeNode companion = new TreeNode();
        companion.parentNode = this.parentNode;
        companion.setName(this.getName() + "$" + companionNode);
        this.parentNode.children.add(companion);
        return companion;
    }

}
