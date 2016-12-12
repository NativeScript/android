package com.tns;

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

public class DefaultExtractPolicy implements ExtractPolicy {

	private final static String ASSETS_THUMB_FILENAME = "assetsThumb";

	public boolean shouldExtract(Context context)
	{			
		String assetsThumbFilePath = context.getFilesDir().getPath() + File.separatorChar + ASSETS_THUMB_FILENAME;
		String oldAssetsThumb = getCachedAssetsThumb(assetsThumbFilePath);
		if (oldAssetsThumb == null) {
			return true;
		} else {
			String currentThumb = getAssetsThumb(context);
			
			if(currentThumb != null  && !currentThumb.equals(oldAssetsThumb)) {
				return true;
			}
		}
		
		return false;
	}

	public void setAssetsThumb(Context context) {
		String assetsThumb = generateAssetsThumb(context);
		if(assetsThumb != null) {
			String assetsThumbFilePath = context.getFilesDir().getPath() + File.separatorChar + ASSETS_THUMB_FILENAME;
			saveNewAssetsThumb(assetsThumb, assetsThumbFilePath);
		}
	}
	
	public boolean forceOverwrite() {
		return true;
	}

	public FileExtractor extractor() {
		return null;
	}

	public String getAssetsThumb(Context context) {
		return generateAssetsThumb(context);
	}

	private String generateAssetsThumb(Context context) {
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
			int code = packageInfo.versionCode;
			long updateTime = packageInfo.lastUpdateTime;
			return String.valueOf(updateTime) + "-" + String.valueOf(code);
		} catch (PackageManager.NameNotFoundException e) {
			DefaultTracer.trace(Tracer.Descriptor.EXCEPTION, "[generateAssetsThumb] Error while getting current assets thumb");
			e.printStackTrace();
		}

		return null;
	}

	private String getCachedAssetsThumb(String assetsThumbFilePath) {
		try {
			File cachedThumbFile = new File(assetsThumbFilePath);
			if (cachedThumbFile.exists()) {
				FileInputStream in = new FileInputStream(cachedThumbFile);
				BufferedReader reader = new BufferedReader(new InputStreamReader(in));
				String cachedThumb = reader.readLine();
				reader.close();
				in.close();
				return cachedThumb;
			}
		} catch (FileNotFoundException e) {
			DefaultTracer.trace(Tracer.Descriptor.EXCEPTION, "[getCachedAssetsThumb] Error while getting current assets thumb");
			e.printStackTrace();
		} catch (IOException e) {
			DefaultTracer.trace(Tracer.Descriptor.EXCEPTION, "[getCachedAssetsThumb] Error while getting current asstes thumb");
			e.printStackTrace();
		}

		return null;
	}

	private void saveNewAssetsThumb(String newThumb, String assetsThumbFile) {
		File cachedThumbFile = new File(assetsThumbFile);
		try {
			FileOutputStream out = new FileOutputStream(cachedThumbFile, false);
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
			try {
				writer.write(newThumb);
				writer.newLine();
				writer.flush();
			} finally {
				writer.close();
				out.close();
			}
		} catch (FileNotFoundException e) {
			DefaultTracer.trace(Tracer.Descriptor.EXCEPTION, "[saveNewAssetsThumb] Error while writing current assets thumb");
			e.printStackTrace();
		} catch (IOException e) {
			DefaultTracer.trace(Tracer.Descriptor.EXCEPTION, "[saveNewAssetsThumb] Error while writing current assets thumb");
			e.printStackTrace();
		}
	}

}
