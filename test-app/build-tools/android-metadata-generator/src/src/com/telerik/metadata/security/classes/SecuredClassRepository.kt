package com.telerik.metadata.security.classes

import com.telerik.metadata.ClassMapProvider
import com.telerik.metadata.parsing.NativeClassDescriptor
import com.telerik.metadata.security.filtering.blacklisting.MetadataBlacklist
import com.telerik.metadata.security.filtering.input.user.UserPatternsCollection
import com.telerik.metadata.security.filtering.matching.impl.PatternMatcherImpl
import com.telerik.metadata.security.filtering.whitelisting.MetadataWhitelist
import java.util.*

object SecuredClassRepository {
    private val cachedProviders = ArrayList<ClassMapProvider>()
    private val patternMatcher = PatternMatcherImpl()
    private val whitelist = MetadataWhitelist(UserPatternsCollection.whitelistEntries, patternMatcher)
    private val blacklist = MetadataBlacklist(UserPatternsCollection.blacklistEntries, patternMatcher)

    fun addToCache(classMapProvider: ClassMapProvider) {
        cachedProviders.add(classMapProvider)
    }

    fun findClass(className: String): SecuredNativeClassDescriptor {
        val clazz: NativeClassDescriptor? = findClassFromProviders(className)

        if (clazz != null) {
            val simplifiedClassName = clazz.className.substring(clazz.packageName.length).replace('$', '.').trimStart('.')

            val isAllowedByWhitelist = whitelist.isAllowed(clazz.packageName, simplifiedClassName)
            if (isAllowedByWhitelist) {

                val isAllowedByBlacklist = blacklist.isAllowed(clazz.packageName, simplifiedClassName)
                if (isAllowedByBlacklist) {
                    return SecuredNativeClassDescriptor(true, clazz)
                }
            }
        }

        return SecuredNativeClassDescriptor(false, NativeClassDescriptor.Missing)
    }

    fun findNearestAllowedClass(className: String): SecuredNativeClassDescriptor {
        var clazz: NativeClassDescriptor? = findClassFromProviders(className)

        while (clazz != null) {
            val simplifiedClassName = clazz.className.substring(clazz.packageName.length).replace('$', '.').trimStart('.')

            val isAllowedByWhitelist = whitelist.isAllowed(clazz.packageName, simplifiedClassName)
            if (isAllowedByWhitelist) {

                val isAllowedByBlacklist = blacklist.isAllowed(clazz.packageName, simplifiedClassName)
                if (isAllowedByBlacklist) {
                    return SecuredNativeClassDescriptor(true, clazz)
                }
            }

            clazz = findClassFromProviders(clazz.superclassName)
        }

        return SecuredNativeClassDescriptor(false, NativeClassDescriptor.Missing)
    }

    private fun findClassFromProviders(className: String): NativeClassDescriptor? {
        var clazz: NativeClassDescriptor? = null
        for (classMapProvider in cachedProviders) {
            clazz = classMapProvider.classMap[className]
            if (clazz != null) {
                break
            }
        }

        return clazz
    }

    fun getClassNames(): Array<String> {
        val names = ArrayList<String>()
        for (classMapProvider in cachedProviders) {
            for (className in classMapProvider.classMap.keys) {
                names.add(className)
            }
        }
        val arrClassNames = names.toTypedArray()
        Arrays.sort(arrClassNames)
        return arrClassNames
    }
}