package com.tns.internal;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.util.Log;

import com.tns.Platform;

public class DefaultExtractPolicy implements ExtractPolicy
{
	private final static String ASSETS_THUMB_FILENAME = "assetsThumb";

	public boolean shouldExtract(android.content.Context context)
	{
		String assetsThumb = generateAssetsThumb(context);
		if (assetsThumb != null)
		{
			String assetsThumbFilePath = context.getFilesDir().getPath() + File.separatorChar + ASSETS_THUMB_FILENAME;
			String oldAssetsThumb = getCachedAssetsThumb(assetsThumbFilePath);
			if (oldAssetsThumb == null || !assetsThumb.equals(oldAssetsThumb))
			{
				saveNewAssetsThumb(assetsThumb, assetsThumbFilePath);
				return true;
			}
		}

		return false;
	}

	public boolean forceOverwrite()
	{
		return true;
	}

	public FileExtractor extractor()
	{
		return null;
	}

	private static String generateAssetsThumb(Context context)
	{
		try
		{
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
			int code = packageInfo.versionCode;
			long updateTime = packageInfo.lastUpdateTime;
			return String.valueOf(updateTime) + "-" + String.valueOf(code);
		}
		catch (PackageManager.NameNotFoundException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Error while getting current assets thumb");
			e.printStackTrace();
		}

		return null;
	}

	private static String getCachedAssetsThumb(String assetsThumbFilePath)
	{
		try
		{
			File cachedThumbFile = new File(assetsThumbFilePath);
			if (cachedThumbFile.exists())
			{
				FileInputStream in = new FileInputStream(cachedThumbFile);
				BufferedReader reader = new BufferedReader(new InputStreamReader(in));
				String cachedThumb = reader.readLine();
				reader.close();
				in.close();
				return cachedThumb;
			}
		}
		catch (FileNotFoundException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Error while getting current assets thumb");
			e.printStackTrace();
		}
		catch (IOException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Error while getting current asstes thumb");
			e.printStackTrace();
		}

		return null;
	}

	private void saveNewAssetsThumb(String newThumb, String assetsThumbFile)
	{
		File cachedThumbFile = new File(assetsThumbFile);
		try
		{
			FileOutputStream out = new FileOutputStream(cachedThumbFile, false);
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
			try
			{
				writer.write(newThumb);
				writer.newLine();
				writer.flush();
			}
			finally
			{
				writer.close();
				out.close();
			}
		}
		catch (FileNotFoundException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Error while writting current assets thumb");
			e.printStackTrace();
		}
		catch (IOException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Error while writting current assets thumb");
			e.printStackTrace();
		}
	}

}
