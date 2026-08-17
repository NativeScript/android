package com.telerik.metadata.parsing.kotlin.classes

import com.telerik.metadata.parsing.kotlin.metadata.MetadataAnnotation
import org.apache.bcel.classfile.ClassParser
import org.apache.bcel.classfile.JavaClass
import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Assert.fail
import org.junit.Test
import kotlin.metadata.jvm.KotlinClassMetadata
import kotlin.metadata.jvm.Metadata

class KotlinClassDescriptorMetadataVersionTest {

    companion object {
        // Newer than any Kotlin release the bundled kotlin-metadata-jvm knows about, so the test
        // stays meaningful after the library is upgraded.
        private val FUTURE_METADATA_VERSION = intArrayOf(2, 9, 0)
    }

    private val fixture = KotlinMetadataVersionFixture::class.java

    @Test
    fun `reads class metadata written by a newer Kotlin compiler`() {
        val metadata = descriptorFor(fixture, FUTURE_METADATA_VERSION).kotlinMetadata

        assertTrue("Expected readable class metadata, got $metadata", metadata is KotlinClassMetadata.Class)
        assertTrue((metadata as KotlinClassMetadata.Class).kmClass.name.endsWith("KotlinMetadataVersionFixture"))
    }

    @Test
    fun `exposes members of a class compiled with a newer Kotlin compiler`() {
        val descriptor = descriptorFor(fixture, FUTURE_METADATA_VERSION)

        assertTrue("Class visibility should come from Kotlin metadata", descriptor.isPublic)
        assertTrue("Expected the 'counter' property", descriptor.properties.any { it.name == "counter" })
        assertTrue("Expected the 'greet' method", descriptor.methods.any { it.name == "greet" })
    }

    @Test
    fun `falls back to bytecode parsing when metadata cannot be read at all`() {
        val descriptor = KotlinClassDescriptor(bcelClass(fixture), corruptAnnotation(), false)

        assertEquals(null, descriptor.kotlinMetadata)
        assertTrue("Bytecode-derived visibility should still be available", descriptor.isPublic)
        assertTrue("Bytecode-derived methods should still be available", descriptor.methods.any { it.name == "greet" })
    }

    @Test
    fun `strict reading is what rejects newer metadata`() {
        val annotation = annotationWithVersion(fixture, FUTURE_METADATA_VERSION)

        try {
            KotlinClassMetadata.readStrict(toMetadata(annotation))
            fail("readStrict was expected to reject metadata version 2.9.0")
        } catch (e: IllegalArgumentException) {
            assertTrue(e.message!!.contains("version"))
        }
    }

    private fun descriptorFor(clazz: Class<*>, metadataVersion: IntArray) =
            KotlinClassDescriptor(bcelClass(clazz), annotationWithVersion(clazz, metadataVersion), false)

    private fun bcelClass(clazz: Class<*>): JavaClass {
        val resource = clazz.name.replace('.', '/') + ".class"
        return clazz.classLoader.getResourceAsStream(resource).use {
            ClassParser(it, resource).parse()
        }
    }

    private fun annotationWithVersion(clazz: Class<*>, metadataVersion: IntArray): MetadataAnnotation {
        val real = clazz.getAnnotation(kotlin.Metadata::class.java)
        return object : MetadataAnnotation {
            override val kind = real.kind
            override val metadataVersion = metadataVersion
            override val bytecodeVersion = intArrayOf()
            override val data1 = real.data1
            override val data2 = real.data2
            override val extraString = real.extraString
            override val packageName = real.packageName
            override val extraInt = real.extraInt
        }
    }

    private fun corruptAnnotation(): MetadataAnnotation = object : MetadataAnnotation {
        override val kind = 1
        override val metadataVersion = intArrayOf()
        override val bytecodeVersion = intArrayOf()
        override val data1 = emptyArray<String>()
        override val data2 = emptyArray<String>()
        override val extraString = ""
        override val packageName = ""
        override val extraInt = 0
    }

    private fun toMetadata(annotation: MetadataAnnotation) = Metadata(
            annotation.kind,
            annotation.metadataVersion,
            annotation.data1,
            annotation.data2,
            annotation.extraString,
            annotation.packageName,
            annotation.extraInt)
}
