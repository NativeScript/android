package com.telerik.metadata.kotlin.classes.impl;

import com.telerik.metadata.desc.ClassDescriptor;
import com.telerik.metadata.desc.KotlinClassMetadataAnnotation;
import com.telerik.metadata.kotlin.classes.KotlinClassMetadataParser;

import java.lang.reflect.Modifier;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import kotlinx.metadata.KmClass;
import kotlinx.metadata.KmProperty;
import kotlinx.metadata.jvm.KotlinClassHeader;
import kotlinx.metadata.jvm.KotlinClassMetadata;

public class KotlinClassMetadataParserImpl implements KotlinClassMetadataParser {

    @Override
    public boolean wasKotlinClass(ClassDescriptor clazz) {
        return clazz.getKotlinClassMetadataAnnotation().isPresent();
    }

    @Override
    public boolean wasKotlinCompanionObject(ClassDescriptor clazz, ClassDescriptor possibleCompanion) {
        Optional<KotlinClassMetadataAnnotation> kotlinClassMetadataAnnotationOptional = clazz.getKotlinClassMetadataAnnotation();
        if (!kotlinClassMetadataAnnotationOptional.isPresent()) {
            return false;
        }

        KotlinClassMetadataAnnotation kotlinClassMetadataAnnotation = kotlinClassMetadataAnnotationOptional.get();
        KotlinClassMetadata kotlinClassMetadata = buildKotlinClassMetadata(kotlinClassMetadataAnnotation);

        if (kotlinClassMetadata instanceof KotlinClassMetadata.Class) {
            KotlinClassMetadata.Class regularKotlinClassMetadata = (KotlinClassMetadata.Class) kotlinClassMetadata;
            KmClass kmClass = regularKotlinClassMetadata.toKmClass();
            String companion = kmClass.getCompanionObject();
            String fullCompanionName = clazz.getClassName() + "$" + companion;
            return possibleCompanion.getClassName().equals(fullCompanionName);
        }

        return false;
    }

    private KotlinClassMetadata buildKotlinClassMetadata(KotlinClassMetadataAnnotation kotlinClassMetadataAnnotation) {
        KotlinClassHeader kotlinClassHeader = new KotlinClassHeader(
                kotlinClassMetadataAnnotation.getKind(),
                kotlinClassMetadataAnnotation.getMetadataVersion(),
                kotlinClassMetadataAnnotation.getBytecodeVersion(),
                kotlinClassMetadataAnnotation.getData1(),
                kotlinClassMetadataAnnotation.getData2(),
                kotlinClassMetadataAnnotation.getExtraString(),
                kotlinClassMetadataAnnotation.getPackageName(),
                kotlinClassMetadataAnnotation.getExtraInt());

        return KotlinClassMetadata.read(kotlinClassHeader);
    }

    @Override
    public List<KmProperty> getKotlinProperties(ClassDescriptor clazz) {
        Optional<KotlinClassMetadataAnnotation> kotlinClassMetadataAnnotationOptional = clazz.getKotlinClassMetadataAnnotation();
        if (!kotlinClassMetadataAnnotationOptional.isPresent()) {
            return Collections.emptyList();
        }

        KotlinClassMetadataAnnotation kotlinClassMetadataAnnotation = kotlinClassMetadataAnnotationOptional.get();
        KotlinClassMetadata kotlinClassMetadata = buildKotlinClassMetadata(kotlinClassMetadataAnnotation);

        if (kotlinClassMetadata instanceof KotlinClassMetadata.Class) {
            KotlinClassMetadata.Class regularKotlinClassMetadata = (KotlinClassMetadata.Class) kotlinClassMetadata;
            KmClass kmClass = regularKotlinClassMetadata.toKmClass();
            return kmClass.getProperties()
                    .stream()
                    .filter(p -> (Modifier.isPublic(p.getGetterFlags()) || Modifier.isProtected(p.getGetterFlags()))
                            && (Modifier.isPublic(p.getSetterFlags()) || Modifier.isProtected(p.getSetterFlags()))
                            && !p.getName().startsWith("is"))
                    .collect(Collectors.toList());
        }

        return Collections.emptyList();
    }
}
