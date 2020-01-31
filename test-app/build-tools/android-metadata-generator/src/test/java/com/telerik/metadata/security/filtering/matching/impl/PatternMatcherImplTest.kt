package com.telerik.metadata.security.filtering.matching.impl

import org.junit.Assert.assertEquals
import org.junit.Test
import org.junit.runner.RunWith
import org.junit.runners.Parameterized


@RunWith(Parameterized::class)
class PatternMatcherImplTest(private var pattern: String, private var input: String, private var shouldMatch: Boolean) {

    private val matcher = PatternMatcherImpl()

    companion object {
        @JvmStatic
        @Parameterized.Parameters(name = "Case {index}: matcher.match({0}, {1}) should be {2}")
        fun data(): Collection<Array<Any>> {
            return listOf(
                    arrayOf("g*ks", "geeks", true),
                    arrayOf("ge?ks*", "geeksforgeeks", true),
                    arrayOf("g*k", "gee", false),
                    arrayOf("*pqrs", "pqrst", false),
                    arrayOf("abc*bcd", "abcdhghgbcd", true),
                    arrayOf("abc*c?d", "abcd", false),
                    arrayOf("*c*d", "abcd", true),
                    arrayOf("*?c*d", "abcd", true)
            )
        }
    }

    @Test
    fun `Test match`() {
        assertEquals("Wrong pattern matcher result!", shouldMatch, matcher.match(pattern, input))
    }
}