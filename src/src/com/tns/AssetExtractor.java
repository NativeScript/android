package com.tns;

import java.io.File;

import com.tns.internal.ExtractPolicy;
import com.tns.internal.FileExtractor;

import android.content.Context;
import android.util.Log;

public class AssetExtractor
{
	private static native void extractAssets(String apkPath, String outputDir, boolean checkForNewerFiles);

	public static void extractAssets(Context context, ExtractPolicy extractPolicy)
	{
		FileExtractor extractor = extractPolicy.extractor();
		if (extractor != null)
		{
			boolean success = extractor.extract(context);
			if (Platform.IsLogEnabled)
			{
				Log.d(Platform.DEFAULT_LOG_TAG, "extract returned " + success);
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