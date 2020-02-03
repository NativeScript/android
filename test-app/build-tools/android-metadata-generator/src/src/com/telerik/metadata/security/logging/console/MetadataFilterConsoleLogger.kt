package com.telerik.metadata.security.logging.console

import com.telerik.metadata.security.filtering.MetadataFilterResult
import com.telerik.metadata.security.logging.MetadataFilterLogger

object MetadataFilterConsoleLogger : MetadataFilterLogger {

    var isEnabled = false

    override fun logAllowed(packageName: String, className: String, whitelistFilterResult: MetadataFilterResult, blacklistFilterResult: MetadataFilterResult) = logAction("Included", packageName, className, whitelistFilterResult, blacklistFilterResult)

    override fun logDisallowed(packageName: String, className: String, whitelistFilterResult: MetadataFilterResult, blacklistFilterResult: MetadataFilterResult) = logAction("Blacklisted", packageName, className, whitelistFilterResult, blacklistFilterResult)

    private fun logAction(action: String, packageName: String, className: String, whitelistFilterResult: MetadataFilterResult, blacklistFilterResult: MetadataFilterResult) {
        if (isEnabled) {
            val logBuilder = StringBuilder()

            logBuilder.append(action)
                    .append(" ")
                    .append(className)
                    .append(" from ")
                    .append(packageName)


            val hasWhitelistPatternEntry = whitelistFilterResult.matchedPatternEntry.isPresent
            val hasBlacklistPatternEntry = blacklistFilterResult.matchedPatternEntry.isPresent

            if (hasWhitelistPatternEntry || hasBlacklistPatternEntry) {
                logBuilder.append(" (")

                var hasWrittenWhitelistingLog = false

                if (hasWhitelistPatternEntry && whitelistFilterResult.isAllowed) {
                    logBuilder.append("enabled by ")
                            .append(whitelistFilterResult.matchedPatternEntry.get())

                    hasWrittenWhitelistingLog = true
                }

                if (hasBlacklistPatternEntry && !blacklistFilterResult.isAllowed) {
                    if (hasWrittenWhitelistingLog) {
                        logBuilder.append(", ")
                    }

                    logBuilder.append("disabled by ")
                            .append(blacklistFilterResult.matchedPatternEntry.get())
                }

                logBuilder.append(")")


            }

            val log = logBuilder.toString()
            println(log)
        }
    }
}