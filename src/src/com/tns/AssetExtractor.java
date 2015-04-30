package com.tns;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;

import com.tns.internal.ExtractPolicy;
import com.tns.internal.FileExtractor;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
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
			String appRoot = context.getFilesDir().getPath() + "/";
			String apkPath = context.getPackageCodePath();

			boolean forceOverwrite = extractPolicy.forceOverwrite();
			extractAssets(apkPath, appRoot, forceOverwrite);
		}
	}
}