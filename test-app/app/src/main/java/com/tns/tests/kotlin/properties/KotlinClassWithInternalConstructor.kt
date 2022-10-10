package com.tns.tests.kotlin.properties;

class KotlinClassWithInternalConstructor internal constructor(
/** Either "http" or "https". */
@get:JvmName("scheme") val scheme: String,
@get:JvmName("schemeDifferent") val schemeSecond: String
        ) {
    class Builder {
        internal var scheme: String? = null

        fun scheme(scheme: String) = apply {
            when {
                scheme.equals("http", ignoreCase = true) -> this.scheme = "http"
                scheme.equals("https", ignoreCase = true) -> this.scheme = "https"
                else -> throw IllegalArgumentException("unexpected scheme: $scheme")
            }
        }

        fun build(): KotlinClassWithInternalConstructor {
            return KotlinClassWithInternalConstructor(
                scheme = scheme ?: throw IllegalStateException("scheme == null"),
                schemeSecond = scheme ?: throw IllegalStateException("scheme == null"),
            )
        }
    }
}
