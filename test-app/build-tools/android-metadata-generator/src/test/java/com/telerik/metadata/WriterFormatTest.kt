package com.telerik.metadata

import com.telerik.metadata.TreeNode.FieldInfo
import com.telerik.metadata.TreeNode.MethodInfo
import com.telerik.metadata.TreeNode.PropertyInfo
import com.telerik.metadata.parsing.NativeMethodDescriptor
import org.junit.Assert.assertArrayEquals
import org.junit.Assert.assertEquals
import org.junit.Assert.assertFalse
import org.junit.Assert.assertNotEquals
import org.junit.Assert.assertNull
import org.junit.Assert.assertTrue
import org.junit.Assert.fail
import org.junit.Test
import java.io.ByteArrayOutputStream
import java.lang.reflect.Proxy

/**
 * Writes a small hand-built tree and reads the three streams back with a parser
 * that knows only the format, the way the runtime's MetadataReader does. The
 * parser fails unless every byte of the value stream belongs to exactly one
 * record, so a field the writer and the format disagree on shows up here.
 */
class WriterFormatTest {

    @Test
    fun `a small tree gets the narrowest fields that hold it`() {
        val written = write(Fixture(padded = false), widenFields = false)

        assertEquals(1, written.idBytes)
        assertMinimalWidths(written, widened = false)
        assertMatches(written)
    }

    @Test
    fun `more nodes and longer names get wider ids and name offsets`() {
        val written = write(Fixture(padded = true), widenFields = false)

        assertEquals(2, written.idBytes)
        assertEquals(3, written.nameBytes)
        assertMinimalWidths(written, widened = false)
        assertMatches(written)
    }

    @Test
    fun `widening adds a byte to every field width`() {
        val written = write(Fixture(padded = true), widenFields = true)

        assertEquals(3, written.idBytes)
        assertEquals(4, written.nameBytes)
        assertMinimalWidths(written, widened = true)
        assertMatches(written)
    }

    @Test
    fun `a signature of more than 256 types is refused`() {
        val fixture = Fixture(padded = false)
        fixture.wide.addStaticFunction(
                fixture.method("tooMany", List(257) { TreeNode.INTEGER }, declaringType = fixture.wide))

        try {
            write(fixture, widenFields = false)
            fail("expected the writer to refuse a 257-type signature")
        } catch (e: Exception) {
            assertTrue(e.message, e.message!!.contains("257"))
        }
    }

    private fun assertMinimalWidths(written: Written, widened: Boolean) {
        fun bytesFor(largest: Long): Int {
            var bytes = 1
            while (bytes < 4 && (largest ushr (8 * bytes)) != 0L) bytes++
            return if (widened) minOf(4, bytes + 1) else bytes
        }

        assertEquals(bytesFor(written.nodes.size - 1L), written.idBytes)
        assertEquals(bytesFor(written.stringsLength - 1L), written.nameBytes)
        assertEquals(bytesFor(((written.signaturesLength - 1L) shl 1) or 1), written.signatureBytes)
    }

    private fun assertMatches(written: Written) {
        val f = written.fixture

        // Tree links: a node with no child, or no further sibling, names itself.
        assertEquals(f.root.id, written.nodes[f.root.id].nextSiblingId)
        f.allNodes().forEach { node ->
            val parsed = written.nodes[node.id]
            assertEquals(node.name, parsed.name)
            assertEquals(node.children.firstOrNull()?.id ?: node.id, parsed.firstChildId)
            node.children.forEachIndexed { index, child ->
                val next = node.children.getOrNull(index + 1)
                assertEquals(next?.id ?: child.id, written.nodes[child.id].nextSiblingId)
            }
        }

        // Every distinct signature is stored once, and methods that share one refer to
        // the same record.
        val distinctSignatures = f.methods.map { it.signature.map { type -> type?.id ?: 0 } }.toSet()
        assertEquals(distinctSignatures.size, written.signatureCount)
        val hashCode = written.instanceMethod(f.obj, "hashCode")
        val size = written.instanceMethod(f.base, "size")
        assertEquals(listOf(TreeNode.INTEGER.id), hashCode.signature)
        assertEquals(hashCode.signatureOffset, size.signatureOffset)
        assertNotEquals(hashCode.signatureOffset, written.instanceMethod(f.obj, "toString").signatureOffset)

        // A void return type is id 0, and the resolved flag travels with the reference.
        assertEquals(listOf(0), written.instanceMethod(f.obj, "notify").signature)
        assertFalse(written.instanceMethod(f.obj, "notify").resolved)
        val equals = written.instanceMethod(f.obj, "equals")
        assertTrue(equals.resolved)
        assertEquals(listOf(TreeNode.BOOLEAN.id, f.obj.id), equals.signature)

        // The longest signature the format holds.
        val many = written.classOf(f.wide).staticMethods.single()
        assertEquals(256, many.signature.size)
        assertEquals(f.wideTypes.map { it.id }, many.signature)
        assertEquals(f.wide.id, many.declaringId)

        // Base classes.
        assertEquals(f.obj.id, written.classOf(f.base).baseId)
        assertEquals(f.base.id, written.classOf(f.derived).baseId)

        // Only the sections a class has are there.
        assertEquals(SECTION_STATIC_METHODS, written.classOf(f.staticOnly).sections)
        assertEquals(SECTION_INSTANCE_METHODS, written.classOf(f.iface).sections)
        assertEquals(SECTION_PROPERTIES, written.classOf(f.props).sections)
        assertEquals(SECTION_EXTENSION_FUNCTIONS or SECTION_INSTANCE_METHODS or SECTION_STATIC_METHODS
                or SECTION_STATIC_FIELDS, written.classOf(f.derived).sections)

        // Instance fields.
        val baseFields = written.classOf(f.base).instanceFields.associateBy { it.name }
        assertEquals(TreeNode.INTEGER.id, baseFields.getValue("count").typeId)
        assertEquals(0, baseFields.getValue("count").flags)
        assertEquals(f.string.id, baseFields.getValue("label").typeId)
        assertEquals(FIELD_FINAL, baseFields.getValue("label").flags)

        // A static field names its declaring type only when another class declares it.
        val own = written.classOf(f.base).staticFields.single()
        assertEquals("OWN", own.name)
        assertEquals(FIELD_FINAL, own.flags)
        assertNull(own.declaringId)
        val derivedFields = written.classOf(f.derived).staticFields.associateBy { it.name }
        assertEquals(FIELD_FINAL or FIELD_HAS_DECLARING_TYPE, derivedFields.getValue("OWN").flags)
        assertEquals(f.base.id, derivedFields.getValue("OWN").declaringId)
        assertEquals(0, derivedFields.getValue("LOCAL").flags)
        assertNull(derivedFields.getValue("LOCAL").declaringId)

        // Extension functions and static methods carry their declaring type.
        val extension = written.classOf(f.derived).extensionFunctions.single()
        assertEquals("ext", extension.name)
        assertEquals(f.staticOnly.id, extension.declaringId)
        assertEquals(listOf(TreeNode.INTEGER.id, f.derived.id), extension.signature)
        assertEquals(f.derived.id, written.classOf(f.derived).staticMethods.single().declaringId)

        // Property accessors.
        val properties = written.classOf(f.props).properties.associateBy { it.name }
        assertEquals("getReadOnly", properties.getValue("readOnly").getter!!.name)
        assertNull(properties.getValue("readOnly").setter)
        assertNull(properties.getValue("writeOnly").getter)
        assertEquals("setWriteOnly", properties.getValue("writeOnly").setter!!.name)
        assertEquals("getBoth", properties.getValue("both").getter!!.name)
        assertEquals("setBoth", properties.getValue("both").setter!!.name)

        // An array node, and an element node that stands for another node and has no
        // value of its own.
        assertEquals(ARRAY_OFFSET, written.nodes[f.arrays.id].valueOffset)
        assertEquals(ARRAY_OFFSET + f.obj.id, written.nodes[f.objectArrayElement.id].valueOffset)
        assertNull(written.nodes[f.objectArrayElement.id].classValue)
    }

    // ---- the tree under test

    private class Fixture(padded: Boolean) {
        val root: TreeNode = TreeNode.getRoot()
        val methods = ArrayList<MethodInfo>()

        val obj: TreeNode
        val string: TreeNode
        val base: TreeNode
        val derived: TreeNode
        val staticOnly: TreeNode
        val iface: TreeNode
        val props: TreeNode
        val wide: TreeNode
        val arrays: TreeNode
        val objectArrayElement: TreeNode
        val wideTypes: List<TreeNode>

        init {
            if (padded) {
                // Enough nodes for two-byte ids, and enough names for three-byte offsets.
                val padding = root.createChild("padding")
                repeat(300) { padding.createChild("p$it" + "x".repeat(250)) }
            }

            val lang = root.createChild("java").createChild("lang")
            obj = type(lang, "Object", TreeNode.Class, null)
            string = type(lang, "String", TreeNode.Class, obj)

            val test = root.createChild("test")
            base = type(test, "Base", TreeNode.Class, obj)
            derived = type(test, "Derived", TreeNode.Class, base)
            staticOnly = type(test, "StaticOnly", (TreeNode.Class.toInt() or TreeNode.Static.toInt()).toByte(), obj)
            iface = type(test, "Iface", TreeNode.Interface, null)
            props = type(test, "Props", TreeNode.Class, obj)
            wide = type(test, "Wide", TreeNode.Class, obj)

            obj.addInstanceMethod(method("hashCode", listOf(TreeNode.INTEGER)))
            obj.addInstanceMethod(method("equals", listOf(TreeNode.BOOLEAN, obj), resolved = true))
            obj.addInstanceMethod(method("toString", listOf(string)))
            obj.addInstanceMethod(method("notify", listOf(null)))

            base.addInstanceMethod(method("size", listOf(TreeNode.INTEGER)))
            base.addInstanceField(field("count", TreeNode.INTEGER, base, isFinal = false))
            base.addInstanceField(field("label", string, base, isFinal = true))
            base.addStaticField(field("OWN", TreeNode.INTEGER, base, isFinal = true))

            derived.addInstanceMethod(method("get", listOf(obj, TreeNode.INTEGER)))
            derived.addStaticFunction(method("create", listOf(derived), declaringType = derived))
            derived.addExtensionFunction(method("ext", listOf(TreeNode.INTEGER, derived), declaringType = staticOnly))
            derived.addStaticField(field("OWN", TreeNode.INTEGER, base, isFinal = true))
            derived.addStaticField(field("LOCAL", string, derived, isFinal = false))

            staticOnly.addStaticFunction(
                    method("of", listOf(TreeNode.INTEGER, TreeNode.INTEGER), declaringType = staticOnly))

            iface.addInstanceMethod(method("run", listOf(null)))

            props.addProperty(PropertyInfo("readOnly", method("getReadOnly", listOf(TreeNode.INTEGER)), null))
            props.addProperty(PropertyInfo("writeOnly", null, method("setWriteOnly", listOf(null, TreeNode.INTEGER))))
            props.addProperty(PropertyInfo("both", method("getBoth", listOf(string)),
                    method("setBoth", listOf(null, string))))

            wideTypes = List(256) { if (it % 3 == 0) obj else TreeNode.INTEGER }
            wide.addStaticFunction(method("many", wideTypes, declaringType = wide))

            arrays = root.createChild("[")
            arrays.nodeType = TreeNode.Array
            objectArrayElement = arrays.createChild("Object")
            objectArrayElement.nodeType = TreeNode.Class
            objectArrayElement.arrayElement = obj
        }

        fun allNodes(): List<TreeNode> {
            val all = ArrayList<TreeNode>()
            val queue = ArrayDeque(listOf(root))
            while (queue.isNotEmpty()) {
                val node = queue.removeFirst()
                all.add(node)
                queue.addAll(node.children)
            }
            return all
        }

        fun method(name: String, types: List<TreeNode?>, resolved: Boolean = false,
                   declaringType: TreeNode? = null): MethodInfo {
            val signatureText = types.joinToString(",") { it?.name ?: "V" }
            val descriptor = Proxy.newProxyInstance(
                    NativeMethodDescriptor::class.java.classLoader,
                    arrayOf(NativeMethodDescriptor::class.java)) { _, called, _ ->
                when (called.name) {
                    "getName" -> name
                    "getSignature" -> signatureText
                    else -> throw UnsupportedOperationException(called.name)
                }
            } as NativeMethodDescriptor

            val info = MethodInfo(descriptor)
            info.signature.addAll(types)
            info.isResolved = resolved
            info.declaringType = declaringType
            methods.add(info)
            return info
        }

        private fun type(parent: TreeNode, name: String, nodeType: Byte, baseClass: TreeNode?): TreeNode {
            val node = parent.createChild(name)
            node.nodeType = nodeType
            node.baseClassNode = baseClass
            return node
        }

        private fun field(name: String, valueType: TreeNode, declaringType: TreeNode, isFinal: Boolean): FieldInfo {
            val info = FieldInfo(name)
            info.valueType = valueType
            info.declaringType = declaringType
            info.isFinalType = isFinal
            return info
        }
    }

    // ---- writing, and reading back from the format alone

    private class MemoryStream : StreamWriter {
        private val out = ByteArrayOutputStream()

        override fun getPosition(): Int = out.size()

        override fun write(b: Byte) {
            out.write(b.toInt())
        }

        override fun write(buffer: ByteArray) {
            out.write(buffer)
        }

        override fun flush() {}

        override fun close() {}

        fun bytes(): ByteArray = out.toByteArray()
    }

    private class ParsedMethod(val name: String, val resolved: Boolean, val signatureOffset: Int,
                               val signature: List<Int>, val declaringId: Int?)

    private class ParsedField(val name: String, val typeId: Int, val flags: Int, val declaringId: Int?)

    private class ParsedProperty(val name: String, val getter: ParsedMethod?, val setter: ParsedMethod?)

    private class ParsedClass(val type: Int, val baseId: Int, val sections: Int,
                              val extensionFunctions: List<ParsedMethod>,
                              val instanceMethods: List<ParsedMethod>,
                              val instanceFields: List<ParsedField>,
                              val properties: List<ParsedProperty>,
                              val staticMethods: List<ParsedMethod>,
                              val staticFields: List<ParsedField>)

    private class ParsedNode(val name: String, val firstChildId: Int, val nextSiblingId: Int,
                             val valueOffset: Long, val classValue: ParsedClass?)

    private class Written(val fixture: Fixture, val idBytes: Int, val nameBytes: Int, val signatureBytes: Int,
                          val stringsLength: Int, val signaturesLength: Int, val signatureCount: Int,
                          val nodes: List<ParsedNode>) {
        fun classOf(node: TreeNode): ParsedClass = nodes[node.id].classValue!!

        fun instanceMethod(node: TreeNode, name: String): ParsedMethod =
                classOf(node).instanceMethods.single { it.name == name }
    }

    private fun write(fixture: Fixture, widenFields: Boolean): Written {
        val nodeStream = MemoryStream()
        val valueStream = MemoryStream()
        val stringsStream = MemoryStream()
        Writer(nodeStream, valueStream, stringsStream, null, widenFields).writeTree(fixture.root)

        return parse(fixture, nodeStream.bytes(), valueStream.bytes(), stringsStream.bytes())
    }

    private fun parse(fixture: Fixture, nodes: ByteArray, values: ByteArray, strings: ByteArray): Written {
        fun field(buffer: ByteArray, position: Int, width: Int): Long {
            var value = 0L
            for (i in 0 until width) {
                value = value or ((buffer[position + i].toLong() and 0xFF) shl (8 * i))
            }
            return value
        }

        assertEquals(FORMAT, field(values, 0, 4))
        val idBytes = values[4].toInt()
        val nameBytes = values[5].toInt()
        val signatureBytes = values[6].toInt()
        assertEquals(0, values[7].toInt())
        val signaturesLength = field(values, 8, 4).toInt()

        val used = BooleanArray(values.size)
        fun mark(start: Int, end: Int, what: String) {
            for (i in start until end) {
                assertFalse("byte $i of the value stream is used twice ($what)", used[i])
                used[i] = true
            }
        }
        mark(0, HEADER_SIZE, "header")
        assertArrayEquals(ByteArray(PADDING), values.copyOfRange(values.size - PADDING, values.size))
        mark(values.size - PADDING, values.size, "padding")

        val nameStarts = HashMap<Int, String>()
        var position = 0
        while (position < strings.size) {
            val length = field(strings, position, 2).toInt()
            nameStarts[position] = String(strings, position + 2, length, Charsets.UTF_8)
            position += 2 + length
        }
        assertEquals(strings.size, position)
        fun name(buffer: ByteArray, at: Int): String =
                nameStarts[field(buffer, at, nameBytes).toInt()] ?: error("no name starts at that offset")

        val nodeRecordSize = 2 * idBytes + nameBytes + 4
        assertEquals(0, nodes.size % nodeRecordSize)
        val nodeCount = nodes.size / nodeRecordSize
        fun id(buffer: ByteArray, at: Int): Int {
            val value = field(buffer, at, idBytes).toInt()
            assertTrue("id $value is not a node", value in 0 until nodeCount)
            return value
        }

        // The signature region: records back to back, none of them twice.
        val signatures = HashMap<Int, List<Int>>()
        position = HEADER_SIZE
        while (position < HEADER_SIZE + signaturesLength) {
            val length = (values[position].toInt() and 0xFF) + 1
            val types = List(length) { id(values, position + 1 + idBytes * it) }
            signatures[position - HEADER_SIZE] = types
            position += 1 + idBytes * length
        }
        assertEquals(HEADER_SIZE + signaturesLength, position)
        assertEquals(signatures.size, signatures.values.toSet().size)
        mark(HEADER_SIZE, HEADER_SIZE + signaturesLength, "signatures")

        val referenced = HashSet<Int>()
        var cursor = 0
        fun method(declaring: Boolean): ParsedMethod {
            val methodName = name(values, cursor)
            val reference = field(values, cursor + nameBytes, signatureBytes).toInt()
            val offset = reference ushr 1
            referenced.add(offset)
            cursor += nameBytes + signatureBytes
            val declaringId = if (declaring) id(values, cursor).also { cursor += idBytes } else null
            return ParsedMethod(methodName, (reference and 1) == 1, offset,
                    signatures[offset] ?: error("no signature starts at $offset"), declaringId)
        }

        fun count(): Int {
            val entries = field(values, cursor, 2).toInt()
            assertTrue("a section that is present has entries", entries > 0)
            cursor += 2
            return entries
        }

        val parsedNodes = List(nodeCount) { index ->
            val record = index * nodeRecordSize
            val valueOffset = field(nodes, record + 2 * idBytes + nameBytes, 4)

            var classValue: ParsedClass? = null
            if (valueOffset in 1 until ARRAY_OFFSET) {
                val start = valueOffset.toInt()
                val type = values[start].toInt() and 0xFF
                if ((type and PRIMITIVE) != 0) {
                    mark(start, start + 1, "primitive")
                } else {
                    val baseId = id(values, start + 1)
                    val sections = values[start + 1 + idBytes].toInt() and 0xFF
                    assertEquals(0, sections ushr 6)
                    cursor = start + 1 + idBytes + 1

                    val extensionFunctions = if ((sections and SECTION_EXTENSION_FUNCTIONS) != 0)
                        List(count()) { method(declaring = true) } else emptyList()
                    val instanceMethods = if ((sections and SECTION_INSTANCE_METHODS) != 0)
                        List(count()) { method(declaring = false) } else emptyList()
                    val instanceFields = if ((sections and SECTION_INSTANCE_FIELDS) != 0)
                        List(count()) {
                            val parsed = ParsedField(name(values, cursor), id(values, cursor + nameBytes),
                                    values[cursor + nameBytes + idBytes].toInt(), null)
                            cursor += nameBytes + idBytes + 1
                            parsed
                        } else emptyList()
                    val properties = if ((sections and SECTION_PROPERTIES) != 0)
                        List(count()) {
                            val propertyName = name(values, cursor)
                            val accessors = values[cursor + nameBytes].toInt()
                            assertEquals(0, accessors ushr 2)
                            cursor += nameBytes + 1
                            val getter = if ((accessors and 1) != 0) method(declaring = false) else null
                            val setter = if ((accessors and 2) != 0) method(declaring = false) else null
                            ParsedProperty(propertyName, getter, setter)
                        } else emptyList()
                    val staticMethods = if ((sections and SECTION_STATIC_METHODS) != 0)
                        List(count()) { method(declaring = true) } else emptyList()
                    val staticFields = if ((sections and SECTION_STATIC_FIELDS) != 0)
                        List(count()) {
                            val fieldName = name(values, cursor)
                            val typeId = id(values, cursor + nameBytes)
                            val flags = values[cursor + nameBytes + idBytes].toInt()
                            assertEquals(0, flags ushr 2)
                            cursor += nameBytes + idBytes + 1
                            val declaringId = if ((flags and FIELD_HAS_DECLARING_TYPE) != 0)
                                id(values, cursor).also { cursor += idBytes } else null
                            ParsedField(fieldName, typeId, flags, declaringId)
                        } else emptyList()

                    mark(start, cursor, "class value")
                    classValue = ParsedClass(type, baseId, sections, extensionFunctions, instanceMethods,
                            instanceFields, properties, staticMethods, staticFields)
                }
            }

            ParsedNode(name(nodes, record + 2 * idBytes), id(nodes, record), id(nodes, record + idBytes),
                    valueOffset, classValue)
        }

        assertEquals("bytes of the value stream that belong to nothing", 0, used.count { !it })
        assertEquals("signatures nothing refers to", signatures.keys, referenced)

        return Written(fixture, idBytes, nameBytes, signatureBytes, strings.size, signaturesLength,
                signatures.size, parsedNodes)
    }

    private companion object {
        const val FORMAT = ('N'.code or ('S'.code shl 8) or ('M'.code shl 16) or (3 shl 24)).toLong()
        const val HEADER_SIZE = 12
        const val PADDING = 4
        const val ARRAY_OFFSET = Int.MAX_VALUE.toLong()
        const val PRIMITIVE = 1 shl 4

        const val SECTION_EXTENSION_FUNCTIONS = 1
        const val SECTION_INSTANCE_METHODS = 1 shl 1
        const val SECTION_INSTANCE_FIELDS = 1 shl 2
        const val SECTION_PROPERTIES = 1 shl 3
        const val SECTION_STATIC_METHODS = 1 shl 4
        const val SECTION_STATIC_FIELDS = 1 shl 5

        const val FIELD_FINAL = 1
        const val FIELD_HAS_DECLARING_TYPE = 1 shl 1
    }
}
