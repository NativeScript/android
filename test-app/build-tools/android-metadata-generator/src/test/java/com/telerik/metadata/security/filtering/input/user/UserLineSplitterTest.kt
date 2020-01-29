package com.telerik.metadata.security.filtering.input.user

import org.junit.Assert.assertEquals
import org.junit.Test

class UserLineSplitterTest {

    companion object {
        private const val UNEXPECTED_FIRST_PART_MESSAGE = "Unexpected first part"
        private const val UNEXPECTED_SECOND_PART_MESSAGE = "Unexpected second part"
    }

    private val splitter = UserLineSplitter()

    @Test
    fun `Test split line with one part and without a separator`() {
        val line = "java.util"

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "java.util", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "", inputEntry.classPattern)
    }

    @Test
    fun `Test split line with one part and without a separator but with some spaces`() {
        val line = "   java.util  "

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "java.util", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "", inputEntry.classPattern)
    }

    @Test
    fun `Test split line with first part and with a separator`() {
        val line = "java.util:"

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "java.util", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "", inputEntry.classPattern)
    }

    @Test
    fun `Test split line with second part and with a separator`() {
        val line = ":List"

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "List", inputEntry.classPattern)
    }

    @Test
    fun `Test split line with two parts`() {
        val line = "java.util:List"

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "java.util", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "List", inputEntry.classPattern)
    }

    @Test
    fun `Test split line with two parts and spaces around every part`() {
        val line = "   java.util   :     List  "

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "java.util", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "List", inputEntry.classPattern)
    }

    @Test
    fun `Test split line with two parts and spaces around the separator`() {
        val line = "java.util   :     List"

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "java.util", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "List", inputEntry.classPattern)
    }

    @Test(expected = InvalidUserInputException::class)
    fun `Test split empty line`() {
        val line = ""

        splitter.splitLine(line)
    }

    @Test
    fun `Test split line with only a separator`() {
        val line = ":"

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "", inputEntry.classPattern)
    }

    @Test
    fun `Test split line with only a separator and some spaces`() {
        val line = "   :  "

        val inputEntry = splitter.splitLine(line)

        assertEquals(UNEXPECTED_FIRST_PART_MESSAGE, "", inputEntry.packagePattern)
        assertEquals(UNEXPECTED_SECOND_PART_MESSAGE, "", inputEntry.classPattern)
    }

    @Test(expected = InvalidUserInputException::class)
    fun `Test split line with spaces only`() {
        val line = "   "

        splitter.splitLine(line)
    }

    @Test(expected = InvalidUserInputException::class)
    fun `Test split line with more than two parts`() {
        val line = "java.util:List:toString"

        splitter.splitLine(line)
    }


}