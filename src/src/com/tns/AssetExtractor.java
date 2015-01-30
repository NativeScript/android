package com.tns;

import android.util.Log;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Vector;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import com.tns.internal.ExtractPolicy;

import android.content.Context;

public class AssetExtractor
{
	final static String ZIP_FILTER = "assets";
	final static int BUFSIZE = 4096;
	final static String LOGTAG = "AssetExtractor";
	final static byte data[] = new byte[BUFSIZE];

	static void copyStreams(InputStream is, FileOutputStream fos)
	{
		BufferedOutputStream os = null;
		try
		{
			int count;
			os = new BufferedOutputStream(fos, BUFSIZE);
			while ((count = is.read(data, 0, BUFSIZE)) != -1)
			{
				os.write(data, 0, count);
			}
			os.flush();
		}
		catch (IOException e)
		{
			if (Platform.IsLogEnabled) Log.d(Platform.DEFAULT_LOG_TAG, "Exception while copying: " + e);
		}
		finally
		{
			try
			{
				os.close();
				is.close();
			}
			catch (IOException e2)
			{
				if (Platform.IsLogEnabled) Log.d(Platform.DEFAULT_LOG_TAG, "Exception while closing the stream: " + e2);
			}
		}
	}

	public static void extractAssets(Context context, ExtractPolicy extractPolicy)
	{
		String appRoot = getAppRoot(context);
		
		if (extractPolicy.extract(appRoot))
		{
			extractAssetsHelper(context, extractPolicy);
		}
	}
	
	private static String getAppRoot(Context context)
	{
		return context.getFilesDir().getPath();
	}
	
	private static void extractAssetsHelper(Context context, ExtractPolicy extractPolicy)
	{
		try
		{
			Runtime runtime = Runtime.getRuntime();
			String appRoot = getAppRoot(context);
			
			String packageCodePath = context.getPackageCodePath();
			Log.i(Platform.DEFAULT_LOG_TAG, "extractAssets: from " + packageCodePath);
			File zipFile = new File(packageCodePath);
			ZipFile zip = new ZipFile(packageCodePath);
			Vector<ZipEntry> files = pluginsFilesFromZip(zip);
			int zipFilterLength = ZIP_FILTER.length();

			Enumeration<?> entries = files.elements();

			while (entries.hasMoreElements())
			{
				ZipEntry entry = (ZipEntry) entries.nextElement();
				String entryName = entry.getName();
				if (entryName.endsWith(".dex"))
					continue;
				String path = entryName.substring(zipFilterLength);
				
				File outputFile = new File(appRoot, path);
				outputFile.getParentFile().mkdirs();

				if (extractPolicy.shouldSkip(outputFile, zipFile, entry))
				{
					if (Platform.IsLogEnabled) Log.d(Platform.DEFAULT_LOG_TAG, outputFile.getName() + " already extracted.");
				}
				else
				{
					FileOutputStream fos = null;
					try
					{
						fos = new FileOutputStream(outputFile);
						copyStreams(zip.getInputStream(entry), fos);
						if (Platform.IsLogEnabled) Log.d(Platform.DEFAULT_LOG_TAG, "Copied " + entry + " to " + appRoot + "/" + path);
					}
					finally
					{
						if (fos != null)
							fos.close();
					}
				}
			}
		}
		catch (IOException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Error extracting assests: " + e.getMessage());
		}
	}

	private static Vector<ZipEntry> pluginsFilesFromZip(ZipFile zip)
	{
		Vector<ZipEntry> list = new Vector<ZipEntry>();
		Enumeration<?> entries = zip.entries();
		while (entries.hasMoreElements())
		{
			ZipEntry entry = (ZipEntry) entries.nextElement();
			if (entry.getName().startsWith(ZIP_FILTER))
			{
				list.add(entry);
			}
		}
		return list;
	}
}