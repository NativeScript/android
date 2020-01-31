package com.telerik.metadata.security.filtering.input.user

import org.junit.Test

import org.junit.Assert.*

class UserLineFilterTest {

    companion object {
        private const val UNEXPECTED_LINE_FILTERING_RESULT_MESSAGE = "Unexpected line filtering result"
    }

    private val filter = UserLineFilter()

    @Test
    fun `Is regular commented line`() {
        val line = "// test"

        val isComment = filter.isCommentLine(line)

        assertTrue(UNEXPECTED_LINE_FILTERING_RESULT_MESSAGE, isComment)
    }

    @Test
    fun `Is bash style commented line`() {
        val line = "# test"

        val isComment = filter.isCommentLine(line)

        assertTrue(UNEXPECTED_LINE_FILTERING_RESULT_MESSAGE, isComment)
    }

    @Test
    fun `Is regular line`() {
        val line = "java.util"

        val isComment = filter.isCommentLine(line)

        assertFalse(UNEXPECTED_LINE_FILTERING_RESULT_MESSAGE, isComment)
    }

    @Test
    fun `Is empty line`(){
        val line = System.lineSeparator()

        val isEmptyLine = filter.isEmptyLine(line)

        assertTrue(UNEXPECTED_LINE_FILTERING_RESULT_MESSAGE, isEmptyLine)
    }

    @Test
    fun `Is whitespace line`(){
        val line = "   "

        val isEmptyLine = filter.isEmptyLine(line)

        assertTrue(UNEXPECTED_LINE_FILTERING_RESULT_MESSAGE, isEmptyLine)
    }
}