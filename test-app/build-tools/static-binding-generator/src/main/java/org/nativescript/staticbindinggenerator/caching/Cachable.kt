package org.nativescript.staticbindinggenerator.caching

interface Cachable<TKey, TValue> {
    val key: TKey
    val value: TValue
}