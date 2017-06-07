package com.tns;

import java.io.File;
import java.io.IOException;

import android.content.Context;
import android.util.Log;

public class AssetExtractor {
    private native void extractAssets(String apkPath, String input, String outputDir, boolean checkForNewerFiles);
    private final Logger logger;

    public AssetExtractor(File libPath, Logger logger) {
        this.logger = logger;
    }

    public void extractAssets(Context context, String inputPath, String outputPath, ExtractPolicy extractPolicy, boolean shouldCleanUpPreviousAssets) {
        FileExtractor extractor = extractPolicy.extractor();
        if (extractor != null) {
            boolean success = extractor.extract(context);
            if (logger.isEnabled()) {
                logger.write("extract returned " + success);
            }
        } else if (extractPolicy.shouldExtract(context)) {
            if (shouldCleanUpPreviousAssets) {
                try {
                    delete(new File(outputPath + inputPath));
                } catch (IOException e) {
                    String logTag = "AssetExtraction";
                    Log.d(logTag, "Problem occurred while deleting assets from previous app version: " + outputPath + inputPath);
                }
            }

            String apkPath = context.getPackageCodePath();

            boolean forceOverwrite = extractPolicy.forceOverwrite();

            extractAssets(apkPath, inputPath, outputPath, forceOverwrite);
        } else {
            if (logger.isEnabled()) {
                logger.write("Skipped extraction of assets in " + inputPath);
            }
        }
    }

    /**
     * Delete a file or a directory and its children.
     * @param file The directory to delete.
     * @throws IOException Exception when problem occurs during deleting the directory.
     */
    private static void delete(File file) throws IOException {
        File[] files = file.listFiles();
        if (files == null) {
            return;
        }

        for (File childFile : files) {
            if (childFile.isDirectory()) {
                delete(childFile);
            } else {
                if (!childFile.delete()) {
                    throw new IOException();
                }
            }
        }

        if (!file.delete()) {
            throw new IOException();
        }
    }
}
