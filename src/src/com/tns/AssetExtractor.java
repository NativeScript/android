package com.tns;

import java.io.File;

import com.tns.internal.ExtractPolicy;
import com.tns.internal.FileExtractor;

import android.content.Context;

public class AssetExtractor
{
	private native void extractAssets(String apkPath, String outputDir, boolean checkForNewerFiles);
	
	private final Logger logger;
	
	public AssetExtractor(File libPath, Logger logger)
	{
		this.logger = logger;
	}
	
	public void extractAssets(Context context, ExtractPolicy extractPolicy)
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
			String appRoot = context.getFilesDir().getPath() + File.separator;
			String apkPath = context.getPackageCodePath();

			boolean forceOverwrite = extractPolicy.forceOverwrite();
			extractAssets(apkPath, appRoot, forceOverwrite);
		}
	}
}