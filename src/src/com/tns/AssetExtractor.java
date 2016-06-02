package com.tns;

import java.io.File;


import android.content.Context;
import android.os.Build;

public class AssetExtractor
{
	private native void extractAssets(String apkPath, String outputDir, boolean checkForNewerFiles, String architecture);

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

			String arch = Build.CPU_ABI; //Build.SUPPORTED_ABIS;
//			String abi2 = Build.CPU_ABI2;
			
			boolean forceOverwrite = extractPolicy.forceOverwrite();
//			String arch = System.getProperty("os.arch");
			extractAssets(apkPath, appRoot, forceOverwrite, arch);
		}
	}
}