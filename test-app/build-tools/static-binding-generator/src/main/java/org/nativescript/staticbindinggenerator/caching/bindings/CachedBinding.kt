package org.nativescript.staticbindinggenerator.caching.bindings

import org.nativescript.staticbindinggenerator.caching.Cachable

data class CachedBinding(override val key: BindingKey, override val value: BindingValue) : Cachable<BindingKey, BindingValue>
data class BindingValue(val value: String)
data class BindingKey(val value: String)