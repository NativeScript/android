package org.nativescript.staticbindinggenerator.caching.bindings

interface BindingsCache {
    fun cache(binding: CachedBinding)
    fun invalidate(binding: CachedBinding)
}