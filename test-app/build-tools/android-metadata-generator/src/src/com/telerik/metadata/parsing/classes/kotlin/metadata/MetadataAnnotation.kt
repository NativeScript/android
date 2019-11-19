package com.telerik.metadata.parsing.classes.kotlin.metadata

interface MetadataAnnotation {

    val kind: Int // k

    val metadataVersion: IntArray // mv

    val bytecodeVersion: IntArray // bv

    val data1: Array<String> // d1

    val data2: Array<String> // d2

    val extraString: String // xs

    val packageName: String // pn

    val extraInt: Int // xi
}
