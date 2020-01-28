package com.telerik.metadata.security.filtering.input.user

import org.junit.Test

import org.junit.Assert.*

class UserLineCommentFilterTest {

    companion object {
        private const val UNEXPECTED_LINE_FILTERING_RESULT_MESSAGE = "Unexpected line filtering result"
    }

    private val filter = UserLineCommentFilter()

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
}