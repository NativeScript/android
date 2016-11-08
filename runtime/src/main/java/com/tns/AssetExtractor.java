package com.tns;

import java.io.File;

import android.content.Context;
import android.util.Log;

public class AssetExtractor {
    private native void extractAssets(String apkPath, String input, String outputDir, boolean checkForNewerFiles);

    public AssetExtractor(File libPath) {
    }

    public void extractAssets(Context context, String inputPath, String outputPath, ExtractPolicy extractPolicy) {
        FileExtractor extractor = extractPolicy.extractor();

        if (extractor != null) {
            boolean success = extractor.extract(context);
            Tracer.trace(Tracer.Descriptor.INFO, "[AssetExtractor.extractAssets] returned: " + success);
        } else if (extractPolicy.shouldExtract(context)) {
            String apkPath = context.getPackageCodePath();

            boolean forceOverwrite = extractPolicy.forceOverwrite();

            extractAssets(apkPath, inputPath, outputPath, forceOverwrite);
        } else {
            Tracer.trace(Tracer.Descriptor.INFO, "[AssetExtractor.extractAssets] skipped extraction of assets in " + inputPath);
        }
    }
}