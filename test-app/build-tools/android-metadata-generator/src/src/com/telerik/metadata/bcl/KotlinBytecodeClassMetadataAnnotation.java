package com.telerik.metadata.bcl;

import com.telerik.metadata.desc.KotlinClassMetadataAnnotation;

import org.apache.bcel.classfile.AnnotationEntry;
import org.apache.bcel.classfile.ArrayElementValue;
import org.apache.bcel.classfile.ElementValue;
import org.apache.bcel.classfile.ElementValuePair;
import org.apache.bcel.classfile.SimpleElementValue;

public class KotlinBytecodeClassMetadataAnnotation implements KotlinClassMetadataAnnotation {

    private Integer kind = null;
    private int[] metadataVersion = null;
    private int[] bytecodeVersion = null;
    private String[] data1 = null;
    private String[] data2 = null;
    private String extraString = null;
    private String packageName = null;
    private Integer extraInt = null;

    KotlinBytecodeClassMetadataAnnotation(AnnotationEntry annotationEntry) {
        if (annotationEntry != null) {
            parseKotlinMetadataAnnotation(annotationEntry);
        }
    }

    private void parseKotlinMetadataAnnotation(AnnotationEntry annotationEntry) {
        ElementValuePair[] elementValuePairs = annotationEntry.getElementValuePairs();
        if (elementValuePairs != null) {
            for (ElementValuePair elementValuePair : annotationEntry.getElementValuePairs()) {

                String elementName = elementValuePair.getNameString();
                ElementValue elementValue = elementValuePair.getValue();

                if ("k".equals(elementName)) {
                    parseKind(elementValue);
                } else if ("mv".equals(elementName)) {
                    parseMetadataVersion(elementValue);
                } else if ("bv".equals(elementName)) {
                    parseBytecodeVersion(elementValue);
                } else if ("d1".equals(elementName)) {
                    parseData1(elementValue);
                } else if ("d2".equals(elementName)) {
                    parseData2(elementValue);
                } else if ("xs".equals(elementName)) {
                    parseExtraString(elementValue);
                } else if ("pn".equals(elementName)) {
                    parsePackageName(elementValue);
                } else if ("xi".equals(elementName)) {
                    parseExtraInt(elementValue);
                }
            }
        }
    }

    private void parseKind(ElementValue elementValue) {
        SimpleElementValue simpleElementValue = (SimpleElementValue) elementValue;
        kind = simpleElementValue.getValueInt();
    }

    private void parseMetadataVersion(ElementValue elementValue) {
        ArrayElementValue arrayElement = (ArrayElementValue) elementValue;

        int metadataVersionArraySize = arrayElement.getElementValuesArraySize();
        metadataVersion = new int[metadataVersionArraySize];

        ElementValue[] arrayElementValues = arrayElement.getElementValuesArray();

        for (int i = 0; i < metadataVersionArraySize; i++) {
            SimpleElementValue simpleElementValue = (SimpleElementValue) arrayElementValues[i];
            metadataVersion[i] = simpleElementValue.getValueInt();
        }
    }

    private void parseBytecodeVersion(ElementValue elementValue) {
        ArrayElementValue arrayElement = (ArrayElementValue) elementValue;

        int bytecodeVersionArraySize = arrayElement.getElementValuesArraySize();
        bytecodeVersion = new int[bytecodeVersionArraySize];

        ElementValue[] arrayElementValues = arrayElement.getElementValuesArray();

        for (int i = 0; i < bytecodeVersionArraySize; i++) {
            SimpleElementValue simpleElementValue = (SimpleElementValue) arrayElementValues[i];
            bytecodeVersion[i] = simpleElementValue.getValueInt();
        }
    }

    private void parseData1(ElementValue elementValue) {
        ArrayElementValue arrayElement = (ArrayElementValue) elementValue;

        int data1ArraySize = arrayElement.getElementValuesArraySize();
        data1 = new String[data1ArraySize];

        ElementValue[] arrayElementValues = arrayElement.getElementValuesArray();

        for (int i = 0; i < data1ArraySize; i++) {
            SimpleElementValue simpleElementValue = (SimpleElementValue) arrayElementValues[i];
            data1[i] = simpleElementValue.getValueString();
        }
    }

    private void parseData2(ElementValue elementValue) {
        ArrayElementValue arrayElement = (ArrayElementValue) elementValue;

        int data2ArraySize = arrayElement.getElementValuesArraySize();
        data2 = new String[data2ArraySize];

        ElementValue[] arrayElementValues = arrayElement.getElementValuesArray();

        for (int i = 0; i < data2ArraySize; i++) {
            SimpleElementValue simpleElementValue = (SimpleElementValue) arrayElementValues[i];
            data2[i] = simpleElementValue.getValueString();
        }
    }

    private void parseExtraString(ElementValue elementValue) {
        SimpleElementValue simpleElementValue = (SimpleElementValue) elementValue;
        extraString = simpleElementValue.getValueString();
    }

    private void parsePackageName(ElementValue elementValue) {
        SimpleElementValue simpleElementValue = (SimpleElementValue) elementValue;
        packageName = simpleElementValue.getValueString();
    }

    private void parseExtraInt(ElementValue elementValue) {
        SimpleElementValue simpleElementValue = (SimpleElementValue) elementValue;
        extraInt = simpleElementValue.getValueInt();
    }

    @Override
    public Integer getKind() {
        return kind;
    }

    @Override
    public int[] getMetadataVersion() {
        return metadataVersion;
    }

    @Override
    public int[] getBytecodeVersion() {
        return bytecodeVersion;
    }

    @Override
    public String[] getData1() {
        return data1;
    }

    @Override
    public String[] getData2() {
        return data2;
    }

    @Override
    public String getExtraString() {
        return extraString;
    }

    @Override
    public String getPackageName() {
        return packageName;
    }

    @Override
    public Integer getExtraInt() {
        return extraInt;
    }
}
