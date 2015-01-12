package com.tns.internal;

import java.io.File;
import java.util.zip.ZipEntry;

public interface ExtractPolicy
{
	boolean extract(String appRoot);
	boolean shouldSkip(File outputFile, File zipFile, ZipEntry zipEnty);
}
