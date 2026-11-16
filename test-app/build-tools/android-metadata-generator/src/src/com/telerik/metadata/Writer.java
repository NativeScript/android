package com.telerik.metadata;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.telerik.metadata.TreeNode.FieldInfo;
import com.telerik.metadata.TreeNode.MethodInfo;

import java.io.ByteArrayOutputStream;
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

    // The bytes "NSM" and the format version. The runtime refuses any other value.
    private static final int FORMAT = 'N' | ('S' << 8) | ('M' << 16) | (2 << 24);

    // A method record refers to its signature with 23 bits.
    private static final int MAX_SIGNATURE_BYTES = 1 << 23;

    private static final int SECTION_EXTENSION_FUNCTIONS = 1;
    private static final int SECTION_INSTANCE_METHODS = 1 << 1;
    private static final int SECTION_INSTANCE_FIELDS = 1 << 2;
    private static final int SECTION_PROPERTIES = 1 << 3;
    private static final int SECTION_STATIC_METHODS = 1 << 4;
    private static final int SECTION_STATIC_FIELDS = 1 << 5;

    private static final int FIELD_FINAL = 1;
    private static final int FIELD_HAS_DECLARING_TYPE = 1 << 1;

    private static final int PROPERTY_HAS_GETTER = 1;
    private static final int PROPERTY_HAS_SETTER = 1 << 1;

    // Every distinct signature, once, keyed by its encoded bytes; the value is its offset
    // in the signature region.
    private final HashMap<ByteBuffer, Integer> signatureOffsets = new HashMap<>();
    private final ByteArrayOutputStream signatures = new ByteArrayOutputStream();

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
    private final static byte[] writeSignatureReference_buff = new byte[3];

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

    // One length byte, holding the count of ids less one, then the ids: the return type
    // followed by the parameter types.
    private static ByteBuffer encodeSignature(MethodInfo mi) throws Exception {
        int length = mi.signature.size();
        if (length < 1 || length > 256) {
            throw new Exception("Method " + mi.name + " has a signature of " + length
                    + " types, which the metadata cannot hold. Please report this issue.");
        }

        byte[] record = new byte[1 + 2 * length];
        record[0] = (byte) (length - 1);
        for (int i = 0; i < length; i++) {
            TreeNode type = mi.signature.get(i);
            int id = (type == null) ? 0 : Short.toUnsignedInt(type.id);
            record[1 + 2 * i] = (byte) (id & 0xFF);
            record[2 + 2 * i] = (byte) ((id >> 8) & 0xFF);
        }

        return ByteBuffer.wrap(record);
    }

    private void addSignature(MethodInfo mi) throws Exception {
        ByteBuffer record = encodeSignature(mi);
        if (!signatureOffsets.containsKey(record)) {
            signatureOffsets.put(record, signatures.size());
            signatures.write(record.array());
        }
    }

    private void collectSignatures(TreeNode root) throws Exception {
        ArrayDeque<TreeNode> d = new ArrayDeque<>();
        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();

            if (hasClassValue(n)) {
                for (MethodInfo mi : n.getExtensionFunctions()) {
                    addSignature(mi);
                }
                for (MethodInfo mi : n.getInstanceMethods()) {
                    addSignature(mi);
                }
                for (TreeNode.PropertyInfo propertyInfo : n.getProperties()) {
                    if (propertyInfo.getGetterMethod().isPresent()) {
                        addSignature(propertyInfo.getGetterMethod().get());
                    }
                    if (propertyInfo.getSetterMethod().isPresent()) {
                        addSignature(propertyInfo.getSetterMethod().get());
                    }
                }
                for (MethodInfo mi : n.getStaticMethods()) {
                    addSignature(mi);
                }
            }

            d.addAll(n.children);
        }

        if (signatures.size() >= MAX_SIGNATURE_BYTES) {
            throw new Exception("Metadata is too big: the method signatures take " + signatures.size()
                    + " bytes and the format can address " + MAX_SIGNATURE_BYTES
                    + ". Please report this issue.");
        }
    }

    // An array's element node takes its value from the node it stands for, so it has no
    // class value of its own.
    private static boolean hasClassValue(TreeNode n) {
        return n.arrayElement == null
                && (n.nodeType & TreeNode.Primitive) != TreeNode.Primitive
                && (((n.nodeType & TreeNode.Class) == TreeNode.Class)
                || ((n.nodeType & TreeNode.Interface) == TreeNode.Interface));
    }

    // The name, then three bytes: the signature's offset in the signature region, shifted
    // left by one, with the low bit set when the method is resolved.
    private void writeMethodInfo(MethodInfo mi,
                                 HashMap<String, Integer> uniqueStrings, StreamWriter outValueStream)
            throws Exception {
        int pos = uniqueStrings.get(mi.name);
        writeInt(pos, outValueStream);

        int reference = (signatureOffsets.get(encodeSignature(mi)) << 1) | (mi.isResolved ? 1 : 0);
        writeSignatureReference_buff[0] = (byte) (reference & 0xFF);
        writeSignatureReference_buff[1] = (byte) ((reference >> 8) & 0xFF);
        writeSignatureReference_buff[2] = (byte) ((reference >> 16) & 0xFF);
        outValueStream.write(writeSignatureReference_buff);
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

    private static int writeLength(int length, StreamWriter out)
            throws Exception {
        writeLength_lenBuff[0] = (byte) (length & 0xFF);
        writeLength_lenBuff[1] = (byte) ((length >> 8) & 0xFF);
        out.write(writeLength_lenBuff);

        return length;
    }

    // The type and base class, a byte saying which of the six member sections follow, then
    // each of those: a count and its entries.
    public void writeClassValue(StreamWriter writer,
                                HashMap<String, Integer> stringsMap, TreeNode n) throws Exception {
        writer.write(n.nodeType);

        writeTreeNodeId(n.baseClassNode, writer);

        List<MethodInfo> extensionFunctions = n.getExtensionFunctions();
        List<MethodInfo> instanceMethods = n.getInstanceMethods();
        List<FieldInfo> instanceFields = n.getInstanceFields();
        List<TreeNode.PropertyInfo> propertyInfos = n.getProperties();
        List<MethodInfo> staticMethods = n.getStaticMethods();
        List<FieldInfo> staticFields = n.getStaticFields();

        int sections = (extensionFunctions.isEmpty() ? 0 : SECTION_EXTENSION_FUNCTIONS)
                | (instanceMethods.isEmpty() ? 0 : SECTION_INSTANCE_METHODS)
                | (instanceFields.isEmpty() ? 0 : SECTION_INSTANCE_FIELDS)
                | (propertyInfos.isEmpty() ? 0 : SECTION_PROPERTIES)
                | (staticMethods.isEmpty() ? 0 : SECTION_STATIC_METHODS)
                | (staticFields.isEmpty() ? 0 : SECTION_STATIC_FIELDS);
        writer.write((byte) sections);

        if (!extensionFunctions.isEmpty()) {
            writeLength(extensionFunctions.size(), writer);
            for (MethodInfo extensionFunction : extensionFunctions) {
                writeMethodInfo(extensionFunction, stringsMap, writer);
                writeTreeNodeId(extensionFunction.declaringType, writer);
            }
        }

        if (!instanceMethods.isEmpty()) {
            writeLength(instanceMethods.size(), writer);
            for (MethodInfo instanceMethod : instanceMethods) {
                writeMethodInfo(instanceMethod, stringsMap, writer);
            }
        }

        if (!instanceFields.isEmpty()) {
            writeLength(instanceFields.size(), writer);
            for (FieldInfo fi : instanceFields) {
                writeInt(stringsMap.get(fi.name), writer);
                writeTreeNodeId(fi.valueType, writer);
                writer.write((byte) (fi.isFinalType ? FIELD_FINAL : 0));
            }
        }

        if (!propertyInfos.isEmpty()) {
            writeLength(propertyInfos.size(), writer);
            for (TreeNode.PropertyInfo propertyInfo : propertyInfos) {
                writeInt(stringsMap.get(propertyInfo.getPropertyName()), writer);

                Optional<MethodInfo> propertyGetter = propertyInfo.getGetterMethod();
                Optional<MethodInfo> propertySetter = propertyInfo.getSetterMethod();

                writer.write((byte) ((propertyGetter.isPresent() ? PROPERTY_HAS_GETTER : 0)
                        | (propertySetter.isPresent() ? PROPERTY_HAS_SETTER : 0)));

                if (propertyGetter.isPresent()) {
                    writeMethodInfo(propertyGetter.get(), stringsMap, writer);
                }
                if (propertySetter.isPresent()) {
                    writeMethodInfo(propertySetter.get(), stringsMap, writer);
                }
            }
        }

        if (!staticMethods.isEmpty()) {
            writeLength(staticMethods.size(), writer);
            for (MethodInfo mi : staticMethods) {
                writeMethodInfo(mi, stringsMap, writer);
                writeTreeNodeId(mi.declaringType, writer);
            }
        }

        if (!staticFields.isEmpty()) {
            writeLength(staticFields.size(), writer);
            for (FieldInfo fi : staticFields) {
                // Most static fields are declared by the class listing them, so only the
                // inherited ones name their declaring type.
                boolean hasDeclaringType = fi.declaringType != n;

                writeInt(stringsMap.get(fi.name), writer);
                writeTreeNodeId(fi.valueType, writer);
                writer.write((byte) ((fi.isFinalType ? FIELD_FINAL : 0)
                        | (hasDeclaringType ? FIELD_HAS_DECLARING_TYPE : 0)));
                if (hasDeclaringType) {
                    writeTreeNodeId(fi.declaringType, writer);
                }
            }
        }
    }

    public void writeTree(TreeNode root) throws Exception {
        short curId = 0;

        ArrayDeque<TreeNode> d = new ArrayDeque<>();

        HashMap<String, Integer> uniqueStrings = new HashMap<>();

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

        // The value stream opens with the format, the size of the signature region and
        // the region itself. No value starts at offset 0, which a package node uses to
        // say it has none.
        collectSignatures(root);
        writeInt(FORMAT, outValueStream);
        writeInt(signatures.size(), outValueStream);
        outValueStream.write(signatures.toByteArray());

        final int array_offset = Integer.MAX_VALUE; // 2147483647, which is half of uint32

        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();

            if (n.arrayElement != null) {
                // Set below, once every node it could stand for has an id.
                n.offsetValue = 0;
            } else if (n.nodeType == TreeNode.Package) {
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
