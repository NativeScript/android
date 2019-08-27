package com.telerik.metadata;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.List;
import java.util.Optional;

import com.telerik.metadata.TreeNode.FieldInfo;
import com.telerik.metadata.TreeNode.MethodInfo;

import javax.swing.text.html.Option;

public class Writer {

    private final StreamWriter outNodeStream;
    private final StreamWriter outValueStream;
    private final StreamWriter outStringsStream;

    private int commonInterfacePrefixPosition;

    public Writer(StreamWriter outNodeStream, StreamWriter outValueStream,
                  StreamWriter outStringsStream) {
        this.outNodeStream = outNodeStream;
        this.outValueStream = outValueStream;
        this.outStringsStream = outStringsStream;
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
        outStringsStream.write(name.getBytes("UTF-8"));

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
        int pos = uniqueStrings.get(mi.name).intValue();
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

        int len = writeLength(n.instanceMethods.size(), writer);
        for (int i = 0; i < len; i++) {
            writeMethodInfo(n.instanceMethods.get(i), stringsMap, writer);
        }

        len = writeLength(n.instanceFields.size(), writer);
        for (int i = 0; i < len; i++) {
            FieldInfo fi = n.instanceFields.get(i);
            int pos = stringsMap.get(fi.name).intValue(); // get start position
            // of the name
            writeInt(pos, writer); // write start position of the name of the
            // variable
            writeTreeNodeId(fi.valueType, writer); // pointer to the value type
            // of the variable
            writeFinalModifier(fi, writer);
        }

        List<TreeNode.PropertyInfo> propertyInfos = n.getProperties();
        len = writeLength(propertyInfos.size(), writer);
        for(TreeNode.PropertyInfo propertyInfo: propertyInfos){
            int pos = stringsMap.get(propertyInfo.getPropertyName()); // get start position
            // of the name
            writeInt(pos, writer); // write start position of the name of the
            // variable

            Optional<MethodInfo> propertyGetter = propertyInfo.getGetterMethod();
            Optional<MethodInfo> propertySetter = propertyInfo.getSetterMethod();

            if(propertyGetter.isPresent()){
                writeLength(1, writer);
                MethodInfo getterMethod = propertyGetter.get();
                writeMethodInfo(getterMethod, stringsMap, writer);
            } else {
                writeLength(0, writer);
            }

            if(propertySetter.isPresent()){
                writeLength(1, writer);
                MethodInfo setterMethod = propertySetter.get();
                writeMethodInfo(setterMethod, stringsMap, writer);
            } else {
                writeLength(0, writer);
            }
        }

        len = writeLength(n.staticMethods.size(), writer);
        for (int i = 0; i < len; i++) {
            MethodInfo mi = n.staticMethods.get(i);
            writeMethodInfo(mi, stringsMap, writer);
            writeTreeNodeId(mi.declaringType, writer);
        }

        len = writeLength(n.staticFields.size(), writer);
        for (int i = 0; i < len; i++) {
            FieldInfo fi = n.staticFields.get(i);
            int pos = stringsMap.get(fi.name).intValue();
            writeInt(pos, writer);
            writeTreeNodeId(fi.valueType, writer);
            writeFinalModifier(fi, writer);
            writeTreeNodeId(fi.declaringType, writer);
        }


    }

    public void writeTree(TreeNode root) throws Exception {
        short curId = 0;

        ArrayDeque<TreeNode> d = new ArrayDeque<TreeNode>();

        HashMap<String, Integer> uniqueStrings = new HashMap<String, Integer>();

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
                for (int i = 0; i < n.instanceMethods.size(); i++) {
                    name = n.instanceMethods.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }
                for (int i = 0; i < n.staticMethods.size(); i++) {
                    name = n.staticMethods.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }
                for (int i = 0; i < n.instanceFields.size(); i++) {
                    name = n.instanceFields.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }
                for (int i = 0; i < n.staticFields.size(); i++) {
                    name = n.staticFields.get(i).name;
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                }

                for(TreeNode.PropertyInfo propertyInfo: n.getProperties()){
                    name = propertyInfo.getPropertyName();
                    if (!uniqueStrings.containsKey(name)) {
                        writeUniqueName(name, uniqueStrings, outStringsStream);
                    }
                    Optional<MethodInfo> getterMethodOptional = propertyInfo.getGetterMethod();
                    if(getterMethodOptional.isPresent()){
                        name = getterMethodOptional.get().name;
                        if (!uniqueStrings.containsKey(name)) {
                            writeUniqueName(name, uniqueStrings, outStringsStream);
                        }
                    }
                    Optional<MethodInfo> setterMethodOptional = propertyInfo.getSetterMethod();
                    if(setterMethodOptional.isPresent()){
                        name = setterMethodOptional.get().name;
                        if (!uniqueStrings.containsKey(name)) {
                            writeUniqueName(name, uniqueStrings, outStringsStream);
                        }
                    }
                }
            }

            for (TreeNode child : n.children) {
                d.add(child);
            }
        }

        outStringsStream.flush();
        outStringsStream.close();
        writeInt(0, outValueStream);

        final int array_offset = 1000 * 1000 * 1000;

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

            for (TreeNode child : n.children) {
                d.add(child);
            }
        }

        outValueStream.flush();
        outValueStream.close();

        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();

            if (n.arrayElement != null) {
                n.offsetValue = array_offset + n.arrayElement.id;
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

            nodeData[0] = n.firstChildId + (n.nextSiblingId << 16);
            nodeData[1] = n.offsetName;
            nodeData[2] = n.offsetValue;

            intBuffer.clear();
            intBuffer.put(nodeData);
            outNodeStream.write(byteBuffer.array());

            d.addAll(n.children);
        }

        outNodeStream.flush();
        outNodeStream.close();
    }
}
