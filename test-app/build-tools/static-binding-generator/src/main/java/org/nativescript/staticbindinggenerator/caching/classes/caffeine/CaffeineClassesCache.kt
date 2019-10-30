package org.nativescript.staticbindinggenerator.caching.classes.caffeine

import org.nativescript.staticbindinggenerator.caching.classes.CachedClass
import org.nativescript.staticbindinggenerator.caching.classes.ClassesCache
import java.util.concurrent.TimeUnit
import com.github.benmanes.caffeine.cache.Caffeine
import com.github.benmanes.caffeine.cache.Cache
import org.apache.bcel.classfile.JavaClass

object CaffeineClassesCache : ClassesCache {

    var cache: Cache<String, JavaClass> = Caffeine.newBuilder()
            .expireAfterWrite(10, java.util.concurrent.TimeUnit.DAYS)
            .maximumSize(1000)
            .build()

    override fun cache(clazz: CachedClass) {

    }

    override fun invalidate(clazz: CachedClass) {

    }
}