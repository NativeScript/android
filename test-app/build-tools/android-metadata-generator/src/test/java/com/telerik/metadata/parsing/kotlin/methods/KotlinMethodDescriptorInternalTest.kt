package com.telerik.metadata.parsing.kotlin.methods

import com.telerik.metadata.parsing.kotlin.classes.KotlinClassDescriptor
import com.telerik.metadata.parsing.kotlin.metadata.bytecode.BytecodeMetadataAnnotation
import org.apache.bcel.classfile.ClassParser
import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Test

class KotlinMethodDescriptorInternalTest {

    private val descriptor = descriptorFor(KotlinInternalMembersFixture::class.java)

    @Test
    fun `internal property accessors are internal`() {
        assertTrue(isInternal("getInternalProperty"))
        assertTrue(isInternal("setInternalProperty"))
    }

    @Test
    fun `public property accessors are not internal`() {
        assertFalse(isInternal("getPublicProperty"))
        assertFalse(isInternal("setPublicProperty"))
    }

    @Test
    fun `an internal setter on a public property only hides the setter`() {
        assertFalse(isInternal("getInternalSetterProperty"))
        assertTrue(isInternal("setInternalSetterProperty"))
    }

    @Test
    fun `internal functions are still detected`() {
        assertTrue(isInternal("internalFunction"))
        assertFalse(isInternal("publicFunction"))
    }

    // Kotlin mangles internal members as name$module, and the module segment depends on the
    // compiling project, so tests match on the declared name instead of the full JVM name.
    private fun isInternal(declaredName: String): Boolean {
        val matches = descriptor.methods.filter { it.name == declaredName || it.name.startsWith("$declaredName$") }
        assertTrue("No method found for '$declaredName'", matches.isNotEmpty())
        return matches.single().isInternal
    }

    private fun descriptorFor(clazz: Class<*>): KotlinClassDescriptor {
        val resource = clazz.name.replace('.', '/') + ".class"
        val javaClass = clazz.classLoader.getResourceAsStream(resource).use {
            ClassParser(it, resource).parse()
        }
        val metadataEntry = javaClass.annotationEntries.single { it.annotationType == "Lkotlin/Metadata;" }
        return KotlinClassDescriptor(javaClass, BytecodeMetadataAnnotation(metadataEntry), false)
    }
}
