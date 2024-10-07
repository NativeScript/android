package com.telerik.metadata;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.telerik.metadata.TreeNode.FieldInfo;
import com.telerik.metadata.TreeNode.MethodInfo;

import java.nio.ByteBuffer;
import java.nio.Buffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Optional;

public class Writer {

    private final StreamWriter outNodeStream;
    private final StreamWriter outValueStream;
    private final StreamWriter outStringsStream;
    private final StreamWriter outDebugStream;

    private int commonInterfacePrefixPosition;

    public Writer(StreamWriter outNodeStream, StreamWriter outValueStream,
                  StreamWriter outStringsStream) {
        this(outNodeStream, outValueStream, outStringsStream, null);
    }
    public Writer(StreamWriter outNodeStream, StreamWriter outValueStream,
                  StreamWriter outStringsStream, StreamWriter outDebugStream) {
        this.outNodeStream = outNodeStream;
        this.outValueStream = outValueStream;
        this.outStringsStream = outStringsStream;
        this.outDebugStream = outDebugStream;
    }

    private final static byte[] writeUniqueName_lenBuff = new byte[2];
    private final static byte[] writeInt_buff = new byte[4];
    private final static byte[] writeTreeNodeId_buff = new byte[2];
    private final static byte[] writeLength_lenBuff = new byte[2];
    private final static byte[] writeModifierFinal_buff = new byte[1];

    private static int writeUniqueName(String name,
                                       HashMap<String, Integer> uniqueStrings,
                                       StreamWriter outStringsStream) throws Exception {
        int position = outStringsStream.getPosition();

        int len = name.length();
        writeUniqueName_lenBuff[0] = (byte) (len & 0xFF);
        writeUniqueName_lenBuff[1] = (byte) ((len >> 8) & 0xFF);

        outStringsStream.write(writeUniqueName_lenBuff);
        outStringsStream.write(name.getBytes(StandardCharsets.UTF_8));

        uniqueStrings.put(name, position);

        return position;
    }

    private static void writeInt(int value, StreamWriter out) throws Exception {
        writeInt_buff[0] = (byte) (value & 0xFF);
        writeInt_buff[1] = (byte) ((value >> 8) & 0xFF);
        writeInt_buff[2] = (byte) ((value >> 16) & 0xFF);
        writeInt_buff[3] = (byte) ((value >> 24) & 0xFF);

        out.write(writeInt_buff);
    }

    private static void writeMethodInfo(MethodInfo mi,
                                        HashMap<String, Integer> uniqueStrings, StreamWriter outValueStream)
            throws Exception {
        int pos = uniqueStrings.get(mi.name);
        writeInt(pos, outValueStream);

        byte isResolved = (byte) (mi.isResolved ? 1 : 0);
        outValueStream.write(isResolved);

        int sigLen = writeLength(mi.signature.size(), outValueStream);
        for (int i = 0; i < sigLen; i++) {
            TreeNode arg = mi.signature.get(i);

            writeTreeNodeId(arg, outValueStream);
        }
    }

    private static void writeTreeNodeId(TreeNode node, StreamWriter out)
            throws Exception {
        if (node == null) {
            writeTreeNodeId_buff[0] = writeTreeNodeId_buff[1] = 0;
        } else {
            writeTreeNodeId_buff[0] = (byte) (node.id & 0xFF);
            writeTreeNodeId_buff[1] = (byte) ((node.id >> 8) & 0xFF);
        }
        out.write(writeTreeNodeId_buff);
    }

    private static void writeFinalModifier(FieldInfo fi, StreamWriter out)
            throws Exception {
        writeModifierFinal_buff[0] = fi.isFinalType ? TreeNode.Final : 0;
        out.write(writeModifierFinal_buff);
    }

    private static int writeLength(int length, StreamWriter out)
            throws Exception {
        writeLength_lenBuff[0] = (byte) (length & 0xFF);
        writeLength_lenBuff[1] = (byte) ((length >> 8) & 0xFF);
        out.write(writeLength_lenBuff);

        return length;
    }

    public void writeClassValue(StreamWriter writer,
                                HashMap<String, Integer> stringsMap, TreeNode n) throws Exception {
        writer.write(n.nodeType);

        //
        writeTreeNodeId(n.baseClassNode, writer);
        //

        if ((n.nodeType & TreeNode.Interface) == TreeNode.Interface) {
            writer.write((byte) 1);
            writeInt(commonInterfacePrefixPosition, writer);
        }

        List<MethodInfo> extensionFunctions = n.getExtensionFunctions();

        writeLength(extensionFunctions.size(), writer);
        for (MethodInfo extensionFunction : extensionFunctions) {
            writeMethodInfo(extensionFunction, stringsMap, writer);
            writeTreeNodeId(extensionFunction.declaringType, writer);
        }

        List<MethodInfo> instanceMethods = n.getInstanceMethods();
        int len = writeLength(instanceMethods.size(), writer);
        for (int i = 0; i < len; i++) {
            writeMethodInfo(instanceMethods.get(i), stringsMap, writer);
        }

        List<FieldInfo> instanceFields = n.getInstanceFields();
        len = writeLength(instanceFields.size(), writer);
        for (int i = 0; i < len; i++) {
            FieldInfo fi = instanceFields.get(i);
            int pos = stringsMap.get(fi.name).intValue(); // get start position
            // of the name
            writeInt(pos, writer); // write start position of the name of the
            // variable
            writeTreeNodeId(fi.valueType, writer); // pointer to the value type
            // of the variable
            writeFinalModifier(fi, writer);
        }

        List<TreeNode.PropertyInfo> propertyInfos = n.getProperties();
        writeLength(propertyInfos.size(), writer);
        for (TreeNode.PropertyInfo propertyInfo : propertyInfos) {
            int pos = stringsMap.get(propertyInfo.getPropertyName()); // get start position
            // of the name
            writeInt(pos, writer); // write start position of the name of the
            // variable

            Optional<MethodInfo> propertyGetter = propertyInfo.getGetterMethod();
            Optional<MethodInfo> propertySetter = propertyInfo.getSetterMethod();

            if (propertyGetter.isPresent()) {
                writeLength(1, writer);
                MethodInfo getterMethod = propertyGetter.get();
                writeMethodInfo(getterMethod, stringsMap, writer);
            } else {
                writeLength(0, writer);
            }

            if (propertySetter.isPresent()) {
                writeLength(1, writer);
                MethodInfo setterMethod = propertySetter.get();
                writeMethodInfo(setterMethod, stringsMap, writer);
            } else {
                writeLength(0, writer);
            }
        }

        List<MethodInfo> staticMethods = n.getStaticMethods();
        len = writeLength(staticMethods.size(), writer);
        for (int i = 0; i < len; i++) {
            MethodInfo mi = staticMethods.get(i);
            writeMethodInfo(mi, stringsMap, writer);
            writeTreeNodeId(mi.declaringType, writer);
        }

        List<FieldInfo> staticFields = n.getStaticFields();
        len = writeLength(staticFields.size(), writer);
        for (int i = 0; i < len; i++) {
            FieldInfo fi = staticFields.get(i);
            int pos = stringsMap.get(fi.name).intValue();
            writeInt(pos, writer);
            writeTreeNodeId(fi.valueType, writer);
            writeFinalModifier(fi, writer);
            writeTreeNodeId(fi.declaringType, writer);
        }


    }

    public void writeTree(TreeNode root) throws Exception {
        short curId = 0;

        ArrayDeque<TreeNode> d = new ArrayDeque<>();

        HashMap<String, Integer> uniqueStrings = new HashMap<>();

        commonInterfacePrefixPosition = writeUniqueName("com/tns/gen/",
                uniqueStrings, outStringsStream);

        // this while loop fils the treeStringsStream.dat file with a sequence
        // of the
        // length and name of all the nodes in the built tree + the primitive
        // types used by method signatures
        // the "n" variable holds all the nodes -> n.offsetName is the initial
        // position where you can read the (length/name) pair
        // n.offsetName is used to later find the node names in the
        // treeStringsStream.dat file
        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();
            if (Short.toUnsignedInt((short)(curId + 1)) < Short.toUnsignedInt(curId)) {
                // we have overflowed our maximum (16 bit) metadata size
                throw new Exception("Metadata is too big and has overflown our current limit, please report this issue");
            }
            n.id = n.firstChildId = n.nextSiblingId = curId++;

            String name = n.getName();

            if (uniqueStrings.containsKey(name)) {
                n.offsetName = uniqueStrings.get(name).intValue();
            } else {
                n.offsetName = writeUniqueName(name, uniqueStrings,
                        outStringsStream);
            }

            if (((n.nodeType & TreeNode.Interface) == TreeNode.Interface)
                    || ((n.nodeType & TreeNode.Class) == TreeNode.Class)) {
                List<MethodInfo> instanceMethods = n.getInstanceMethods();
                for (int i = 0; i < instanceMethods.size(); i++) {
                    name = instanceMethods.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }
                List<MethodInfo> staticMethods = n.getStaticMethods();
                for (int i = 0; i < staticMethods.size(); i++) {
                    name = staticMethods.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }
                List<FieldInfo> instanceFields = n.getInstanceFields();
                for (int i = 0; i < instanceFields.size(); i++) {
                    name = instanceFields.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }
                List<FieldInfo> staticFields = n.getStaticFields();
                for (int i = 0; i < staticFields.size(); i++) {
                    name = staticFields.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }

                for (MethodInfo extensionFunction : n.getExtensionFunctions()) {
                    name = extensionFunction.name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }

                for (TreeNode.PropertyInfo propertyInfo : n.getProperties()) {
                    name = propertyInfo.getPropertyName();
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                    Optional<MethodInfo> getterMethodOptional = propertyInfo.getGetterMethod();
                    if (getterMethodOptional.isPresent()) {
                        name = getterMethodOptional.get().name;
                        if (!uniqueStrings.containsKey(name)) {
                            writeUniqueName(name, uniqueStrings, outStringsStream);
                        }
                    }
                    Optional<MethodInfo> setterMethodOptional = propertyInfo.getSetterMethod();
                    if (setterMethodOptional.isPresent()) {
                        name = setterMethodOptional.get().name;
                        if (!uniqueStrings.containsKey(name)) {
                            writeUniqueName(name, uniqueStrings, outStringsStream);
                        }
                    }
                }
            }

            d.addAll(n.children);
        }

        outStringsStream.flush();
        outStringsStream.close();
        writeInt(0, outValueStream);

        final int array_offset = Integer.MAX_VALUE; // 2147483647, which is half of uint32

        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();

            if (n.nodeType == TreeNode.Package) {
                n.offsetValue = 0;
            } else if ((n.nodeType & TreeNode.Primitive) == TreeNode.Primitive) {
                n.offsetValue = outValueStream.getPosition();

                outValueStream.write(n.nodeType);
            } else if (((n.nodeType & TreeNode.Class) == TreeNode.Class)
                    || ((n.nodeType & TreeNode.Interface) == TreeNode.Interface)) {
                n.offsetValue = outValueStream.getPosition();

                writeClassValue(outValueStream, uniqueStrings, n);
            } else if ((n.nodeType & TreeNode.Array) == TreeNode.Array) {
                n.offsetValue = array_offset;
            } else {
                throw new Exception("should not happen");
            }

            if ((n.nodeType & TreeNode.Array) != TreeNode.Array && Integer.toUnsignedLong(n.offsetValue) >= Integer.toUnsignedLong(array_offset)) {
                throw new Exception("Non-array metadata has overflown array space. Please report this issue.");
            }

            d.addAll(n.children);
        }

        outValueStream.flush();
        outValueStream.close();

        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();

            if (n.arrayElement != null) {
                n.offsetValue = array_offset + Short.toUnsignedInt(n.arrayElement.id);
            }

            if (!n.children.isEmpty()) {
                n.firstChildId = n.children.get(0).id;
            }

            for (int i = 0; i < n.children.size(); i++) {
                if (i > 0) {
                    n.children.get(i - 1).nextSiblingId = n.children.get(i).id;
                }

                d.add(n.children.get(i));
            }
        }
        int[] nodeData = new int[3];

        ByteBuffer byteBuffer = ByteBuffer.allocate(nodeData.length * 4);
        byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
        IntBuffer intBuffer = byteBuffer.asIntBuffer();

        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();

            nodeData[0] = (n.firstChildId & 0xFFFF) | (n.nextSiblingId << 16);
            nodeData[1] = n.offsetName;
            nodeData[2] = n.offsetValue;

            ((Buffer)intBuffer).clear();
            intBuffer.put(nodeData);
            outNodeStream.write(byteBuffer.array());

            d.addAll(n.children);
        }

        outNodeStream.flush();
        outNodeStream.close();

        if (outDebugStream != null) {
            d.push(root);
            JsonArray rootArray = new JsonArray();
            while (!d.isEmpty()) {
                TreeNode n = d.pollFirst();
                JsonObject obj = new JsonObject();
                obj.addProperty("id", Short.toUnsignedInt(n.id));
                obj.addProperty("nextSiblingId", Short.toUnsignedInt(n.nextSiblingId));
                obj.addProperty("firstChildId", Short.toUnsignedInt(n.firstChildId));
                obj.addProperty("offsetName", Integer.toUnsignedLong(n.offsetName));
                obj.addProperty("offsetValue", Integer.toUnsignedLong(n.offsetValue));
                obj.addProperty("name", n.getName());
                obj.addProperty("nodeType", n.nodeType);
                rootArray.add(obj);
                d.addAll(n.children);
            }
            outDebugStream.write(rootArray.toString().getBytes());
            outDebugStream.flush();
            outDebugStream.close();
        }
    }
}
