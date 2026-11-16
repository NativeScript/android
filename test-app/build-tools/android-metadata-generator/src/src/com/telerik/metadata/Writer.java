package com.telerik.metadata;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.telerik.metadata.TreeNode.FieldInfo;
import com.telerik.metadata.TreeNode.MethodInfo;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
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
    private static final int FORMAT = 'N' | ('S' << 8) | ('M' << 16) | (3 << 24);

    // The runtime numbers the nodes it creates itself from here.
    private static final int MAX_NODES = 1 << 30;

    // The widest a field can be. The runtime reads every field as this many bytes and
    // masks off what is not the field's.
    private static final int MAX_FIELD_BYTES = 4;

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

    // How many bytes a node id, an offset into the strings and a signature reference take.
    // Each is the fewest that hold the largest value this metadata needs, and the value
    // stream's header tells the runtime what they are.
    private int idBytes;
    private int nameBytes;
    private int signatureBytes;

    // Makes every one of those a byte wider than needed. The runtime's tests are built
    // this way, so the widths only a very large app would get are the ones they cover.
    private final boolean widenFields;

    public Writer(StreamWriter outNodeStream, StreamWriter outValueStream,
                  StreamWriter outStringsStream) {
        this(outNodeStream, outValueStream, outStringsStream, null);
    }
    public Writer(StreamWriter outNodeStream, StreamWriter outValueStream,
                  StreamWriter outStringsStream, StreamWriter outDebugStream) {
        this(outNodeStream, outValueStream, outStringsStream, outDebugStream, false);
    }
    public Writer(StreamWriter outNodeStream, StreamWriter outValueStream,
                  StreamWriter outStringsStream, StreamWriter outDebugStream,
                  boolean widenFields) {
        this.outNodeStream = outNodeStream;
        this.outValueStream = outValueStream;
        this.outStringsStream = outStringsStream;
        this.outDebugStream = outDebugStream;
        this.widenFields = widenFields;
    }

    private int fieldBytes(long largestValue) {
        int bytes = 1;
        while (bytes < MAX_FIELD_BYTES && (largestValue >>> (8 * bytes)) != 0) {
            bytes++;
        }

        return widenFields ? Math.min(MAX_FIELD_BYTES, bytes + 1) : bytes;
    }

    private static void writeField(int value, int bytes, StreamWriter out) throws Exception {
        for (int i = 0; i < bytes; i++) {
            out.write((byte) ((value >>> (8 * i)) & 0xFF));
        }
    }

    private final static byte[] writeUniqueName_lenBuff = new byte[2];
    private final static byte[] writeInt_buff = new byte[4];
    private final static byte[] writeLength_lenBuff = new byte[2];

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
    private ByteBuffer encodeSignature(MethodInfo mi) throws Exception {
        int length = mi.signature.size();
        if (length < 1 || length > 256) {
            throw new Exception("Method " + mi.name + " has a signature of " + length
                    + " types, which the metadata cannot hold. Please report this issue.");
        }

        byte[] record = new byte[1 + idBytes * length];
        record[0] = (byte) (length - 1);
        for (int i = 0; i < length; i++) {
            TreeNode type = mi.signature.get(i);
            int id = (type == null) ? 0 : type.id;
            for (int b = 0; b < idBytes; b++) {
                record[1 + idBytes * i + b] = (byte) ((id >>> (8 * b)) & 0xFF);
            }
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

    }

    // An array's element node takes its value from the node it stands for, so it has no
    // class value of its own.
    private static boolean hasClassValue(TreeNode n) {
        return n.arrayElement == null
                && (n.nodeType & TreeNode.Primitive) != TreeNode.Primitive
                && (((n.nodeType & TreeNode.Class) == TreeNode.Class)
                || ((n.nodeType & TreeNode.Interface) == TreeNode.Interface));
    }

    // The name, then the signature reference: the signature's offset in the signature
    // region, shifted left by one, with the low bit set when the method is resolved.
    private void writeMethodInfo(MethodInfo mi,
                                 HashMap<String, Integer> uniqueStrings, StreamWriter outValueStream)
            throws Exception {
        writeField(uniqueStrings.get(mi.name), nameBytes, outValueStream);

        int reference = (signatureOffsets.get(encodeSignature(mi)) << 1) | (mi.isResolved ? 1 : 0);
        writeField(reference, signatureBytes, outValueStream);
    }

    private void writeTreeNodeId(TreeNode node, StreamWriter out)
            throws Exception {
        writeField((node == null) ? 0 : node.id, idBytes, out);
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
                writeField(stringsMap.get(fi.name), nameBytes, writer);
                writeTreeNodeId(fi.valueType, writer);
                writer.write((byte) (fi.isFinalType ? FIELD_FINAL : 0));
            }
        }

        if (!propertyInfos.isEmpty()) {
            writeLength(propertyInfos.size(), writer);
            for (TreeNode.PropertyInfo propertyInfo : propertyInfos) {
                writeField(stringsMap.get(propertyInfo.getPropertyName()), nameBytes, writer);

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

                writeField(stringsMap.get(fi.name), nameBytes, writer);
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
        int curId = 0;

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
            if (curId == MAX_NODES) {
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

        idBytes = fieldBytes(curId - 1);
        nameBytes = fieldBytes(outStringsStream.getPosition() - 1);

        outStringsStream.flush();
        outStringsStream.close();

        collectSignatures(root);
        signatureBytes = fieldBytes(((long) Math.max(signatures.size() - 1, 0) << 1) | 1);

        // The value stream opens with the format, the three field widths, the size of the
        // signature region and the region itself. No value starts at offset 0, which a
        // package node uses to say it has none.
        writeInt(FORMAT, outValueStream);
        outValueStream.write((byte) idBytes);
        outValueStream.write((byte) nameBytes);
        outValueStream.write((byte) signatureBytes);
        outValueStream.write((byte) 0);
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

        // Room for the runtime to read the stream's last field at full width.
        outValueStream.write(new byte[MAX_FIELD_BYTES]);

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
        // A node record: the ids of its first child and next sibling, its name and the
        // offset of its value, which is always four bytes because its top range marks
        // arrays.
        d.push(root);
        while (!d.isEmpty()) {
            TreeNode n = d.pollFirst();

            writeField(n.firstChildId, idBytes, outNodeStream);
            writeField(n.nextSiblingId, idBytes, outNodeStream);
            writeField(n.offsetName, nameBytes, outNodeStream);
            writeInt(n.offsetValue, outNodeStream);

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
                obj.addProperty("id", n.id);
                obj.addProperty("nextSiblingId", n.nextSiblingId);
                obj.addProperty("firstChildId", n.firstChildId);
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
