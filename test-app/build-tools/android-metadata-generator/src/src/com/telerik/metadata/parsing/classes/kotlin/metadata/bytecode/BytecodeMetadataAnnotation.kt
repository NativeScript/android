package com.telerik.metadata.parsing.classes.kotlin.metadata.bytecode

import com.telerik.metadata.parsing.classes.kotlin.metadata.MetadataAnnotation
import org.apache.bcel.classfile.AnnotationEntry
import org.apache.bcel.classfile.ArrayElementValue
import org.apache.bcel.classfile.ElementValue
import org.apache.bcel.classfile.SimpleElementValue

class BytecodeMetadataAnnotation(annotationEntry: AnnotationEntry?) : MetadataAnnotation {

    override var kind: Int = 1 // 1 comes from the source of the kotlin.Metadata
        private set(value) {
            field = value
        }

    override var metadataVersion: IntArray = intArrayOf()
        private set(value) {
            field = value
        }

    override var bytecodeVersion: IntArray = intArrayOf()
        private set(value) {
            field = value
        }

    override var data1: Array<String> = emptyArray()
        private set(value) {
            field = value
        }

    override var data2: Array<String> = emptyArray()
        private set(value) {
            field = value
        }

    override var extraString: String = ""
        private set(value) {
            field = value
        }

    override var packageName: String = ""
        private set(value) {
            field = value
        }

    override var extraInt: Int = 0 // 0 comes from the source of the kotlin.Metadata
        private set(value) {
            field = value
        }

    init {
        if (annotationEntry != null) {
            parseKotlinMetadataAnnotation(annotationEntry)
        }
    }

    private fun parseKotlinMetadataAnnotation(annotationEntry: AnnotationEntry) {
        val elementValuePairs = annotationEntry.elementValuePairs
        if (elementValuePairs != null) {
            for (elementValuePair in annotationEntry.elementValuePairs) {

                val elementName = elementValuePair.nameString
                val elementValue = elementValuePair.value

                when (elementName) {
                    "k" -> parseKind(elementValue)
                    "mv" -> parseMetadataVersion(elementValue)
                    "bv" -> parseBytecodeVersion(elementValue)
                    "d1" -> parseData1(elementValue)
                    "d2" -> parseData2(elementValue)
                    "xs" -> parseExtraString(elementValue)
                    "pn" -> parsePackageName(elementValue)
                    "xi" -> parseExtraInt(elementValue)
                }
            }
        }
    }

    private fun parseKind(elementValue: ElementValue) {
        val simpleElementValue = elementValue as SimpleElementValue
        kind = simpleElementValue.valueInt
    }

    private fun parseMetadataVersion(elementValue: ElementValue) {
        val arrayElement = elementValue as ArrayElementValue

        val metadataVersionArraySize = arrayElement.elementValuesArraySize
        val arrayElementValues = arrayElement.elementValuesArray

        metadataVersion = IntArray(metadataVersionArraySize) {
            val simpleElementValue = arrayElementValues[it] as SimpleElementValue
            simpleElementValue.valueInt
        }
    }

    private fun parseBytecodeVersion(elementValue: ElementValue) {
        val arrayElement = elementValue as ArrayElementValue

        val bytecodeVersionArraySize = arrayElement.elementValuesArraySize
        val arrayElementValues = arrayElement.elementValuesArray

        bytecodeVersion = IntArray(bytecodeVersionArraySize) {
            val simpleElementValue = arrayElementValues[it] as SimpleElementValue
            simpleElementValue.valueInt
        }
    }

    private fun parseData1(elementValue: ElementValue) {
        val arrayElement = elementValue as ArrayElementValue

        val data1ArraySize = arrayElement.elementValuesArraySize
        val arrayElementValues = arrayElement.elementValuesArray

        data1 = Array(data1ArraySize) {
            val simpleElementValue = arrayElementValues[it] as SimpleElementValue
            simpleElementValue.valueString
        }
    }

    private fun parseData2(elementValue: ElementValue) {
        val arrayElement = elementValue as ArrayElementValue

        val data2ArraySize = arrayElement.elementValuesArraySize
        val arrayElementValues = arrayElement.elementValuesArray

        data2 = Array(data2ArraySize) { i ->
            val simpleElementValue = arrayElementValues[i] as SimpleElementValue
            simpleElementValue.valueString
        }
    }

    private fun parseExtraString(elementValue: ElementValue) {
        val simpleElementValue = elementValue as SimpleElementValue
        extraString = simpleElementValue.valueString
    }

    private fun parsePackageName(elementValue: ElementValue) {
        val simpleElementValue = elementValue as SimpleElementValue
        packageName = simpleElementValue.valueString
    }

    private fun parseExtraInt(elementValue: ElementValue) {
        val simpleElementValue = elementValue as SimpleElementValue
        extraInt = simpleElementValue.valueInt
    }
}
