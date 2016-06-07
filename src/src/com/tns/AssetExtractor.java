package com.tns;

import java.io.File;

import android.content.Context;
import android.util.Log;

public class AssetExtractor
{
	private native void extractAssets(String apkPath, String input, String outputDir, boolean checkForNewerFiles);
	private final Logger logger;

	public AssetExtractor(File libPath, Logger logger)
	{
		this.logger = logger;
	}

	public void extractAssets(Context context, String inputPath, String outputPath, ExtractPolicy extractPolicy)
	{
		FileExtractor extractor = extractPolicy.extractor();
		if (extractor != null)
		{
			boolean success = extractor.extract(context);
			if (logger.isEnabled())
			{
				logger.write("extract returned " + success);
			}
		}
		else if (extractPolicy.shouldExtract(context))
		{
			String apkPath = context.getPackageCodePath();

			boolean forceOverwrite = extractPolicy.forceOverwrite();
			
			extractAssets(apkPath, inputPath, outputPath, forceOverwrite);
		}
		else
		{
			Log.d("~~~~~~~~", "SKIPPED EXTRACTION");
			if (logger.isEnabled())
			{
				logger.write("Skipped extraction of assets in " + inputPath);
			}
		}
	}
}