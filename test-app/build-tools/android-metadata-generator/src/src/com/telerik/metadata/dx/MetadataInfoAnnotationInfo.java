package com.telerik.metadata.dx;

import com.android.dex.Annotation;
import com.android.dex.EncodedValueReader;
import com.telerik.metadata.desc.MetadataInfoAnnotationDescriptor;

import java.util.List;

public class MetadataInfoAnnotationInfo implements MetadataInfoAnnotationDescriptor {
    private final DexFile dexFile;
    private final Annotation annotation;

    public MetadataInfoAnnotationInfo(DexFile dexFile, Annotation annotation) {
        this.dexFile = dexFile;
        this.annotation = annotation;
    }

    @Override
    public boolean skip() {
        List<String> strings = dexFile.getDex().strings();
        EncodedValueReader reader = annotation.getReader();
        int count = reader.readAnnotation();
        for (int i = 0; i < count; i++) {
            String annName = strings.get(reader.readAnnotationName());
            if (annName.equals("skip")) {
                return reader.readBoolean();
            }
        }
        return false;
    }

    @Override
    public String getSuperClassname() {
        List<String> strings = dexFile.getDex().strings();
        EncodedValueReader reader = annotation.getReader();
        int count = reader.readAnnotation();
        for (int i = 0; i < count; i++) {
            String annName = strings.get(reader.readAnnotationName());
            if (annName.equals("superClassname")) {
                return strings.get(reader.readString());
            }
        }
        return null;
    }
}
