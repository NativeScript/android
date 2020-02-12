package com.telerik.metadata.security.filtering

import com.telerik.metadata.security.filtering.input.PatternEntry
import java.util.*

data class MetadataFilterResult(val isAllowed: Boolean, val matchedPatternEntry: Optional<PatternEntry> = Optional.empty())