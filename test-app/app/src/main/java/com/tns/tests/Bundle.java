package com.tns.tests;

import com.telerik.metadata.MetadataInfo;

@MetadataInfo(superClassname = "com.tns.tests.BaseBundle")
public class Bundle
{
    @MetadataInfo(skip = true)
    public int getMyInt() { return 456; }

    @MetadataInfo(skip = true)
    public int getMyInt2() { return 789; }
}
