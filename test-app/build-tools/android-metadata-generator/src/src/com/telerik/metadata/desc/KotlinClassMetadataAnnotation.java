package com.telerik.metadata.desc;

public interface KotlinClassMetadataAnnotation {

    Integer getKind(); // k

    int[] getMetadataVersion(); // mv

    int[] getBytecodeVersion(); // bv

    String[] getData1(); // d1

    String[] getData2(); // d2

    String getExtraString(); // xs

    String getPackageName(); // pn

    Integer getExtraInt(); // xi
}
