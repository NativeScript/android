package com.telerik.metadata.security.filtering.whitelisting

import com.telerik.metadata.security.filtering.MetadataFilter
import com.telerik.metadata.security.filtering.MetadataFilterResult
import com.telerik.metadata.security.filtering.input.PatternEntry
import com.telerik.metadata.security.filtering.matching.PatternMatcher
import java.util.*

class MetadataWhitelist(private val userInput: Collection<PatternEntry>, private val patternMatcher: PatternMatcher) : MetadataFilter {

    override fun isAllowed(packageName: String, className: String): MetadataFilterResult {
        val responsiblePatternEntry = userInput.firstOrNull { inputEntry ->
            (inputEntry.packagePattern.isEmpty() || patternMatcher.match(inputEntry.packagePattern, packageName))
                    && (inputEntry.classPattern.isEmpty() || patternMatcher.match(inputEntry.classPattern, className))
        }

        return if (responsiblePatternEntry != null) MetadataFilterResult(true, Optional.of(responsiblePatternEntry)) else MetadataFilterResult(false)
    }

}