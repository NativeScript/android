package org.nativescript.staticbindinggenerator.caching.classes

interface ClassesCache {
    fun cache(clazz: CachedClass)
    fun invalidate(clazz: CachedClass)
}