package com.telerik.metadata.dx;

import com.android.dex.Annotation;
import com.android.dex.EncodedValueReader;
import com.telerik.metadata.desc.KotlinClassMetadataAnnotation;

import java.util.List;

public class KotlinDexClassMetadataAnnotation implements KotlinClassMetadataAnnotation {

    private Integer kind = null;
    private int[] metadataVersion = null;
    private int[] bytecodeVersion = null;
    private String[] data1 = null;
    private String[] data2 = null;
    private String extraString = null;
    private String packageName = null;
    private Integer extraInt = null;

    KotlinDexClassMetadataAnnotation(Annotation annotation, DexFile dexFile) {
        if (annotation != null) {
            parseKotlinMetadataAnnotation(annotation, dexFile);
        }
    }

    private void parseKotlinMetadataAnnotation(Annotation annotation, DexFile dexFile) {
        List<String> strings = dexFile.getDex().strings();
        EncodedValueReader reader = annotation.getReader();
        int count = reader.readAnnotation();
        for (int i = 0; i < count; i++) {
            String annotationFieldName = strings.get(reader.readAnnotationName());
            if (annotationFieldName.equals("k")) {
                kind = reader.readInt();
            } else if (annotationFieldName.equals("mv")) {
                metadataVersion = readIntArray(reader);
            } else if (annotationFieldName.equals("bv")) {
                bytecodeVersion = readIntArray(reader);
            } else if (annotationFieldName.equals("d1")) {
                data1 = readStringArray(reader, strings);
            } else if (annotationFieldName.equals("d2")) {
                data2 = readStringArray(reader, strings);
            } else if (annotationFieldName.equals("xs")) {
                extraString = strings.get(reader.readString());
            } else if (annotationFieldName.equals("pn")) {
                packageName = strings.get(reader.readString());
            } else if (annotationFieldName.equals("xi")) {
                extraInt = reader.readInt();
            }
        }
    }

    private int[] readIntArray(EncodedValueReader reader) {
        int size = reader.readArray();
        int[] intArray = new int[size];

        for (int i = 0; i < size; i++) {
            intArray[i] = reader.readInt();
        }

        return intArray;
    }

    private String[] readStringArray(EncodedValueReader reader, List<String> strings) {
        int size = reader.readArray();
        String[] stringArray = new String[size];

        for (int a = 0; a < size; a++) {
            stringArray[a] = strings.get(reader.readString());
        }

        return stringArray;
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
