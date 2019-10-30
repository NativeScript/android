package org.nativescript.staticbindinggenerator.caching.classes

import org.apache.bcel.classfile.JavaClass
import org.nativescript.staticbindinggenerator.caching.Cachable

class CachedClass(override val key: String, override val value: JavaClass) : Cachable<String, JavaClass>