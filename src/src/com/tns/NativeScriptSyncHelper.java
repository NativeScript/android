package com.tns;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;

import android.content.Context;
import android.util.Log;

public class NativeScriptSyncHelper
{
	private static String SYNC_ROOT_SOURCE_DIR = "/data/local/tmp/"; 
	private static final String SYNC_SOURCE_DIR = "/sync/";
	private static final String FULL_SYNC_SOURCE_DIR = "/fullsync/";
	private static final String REMOVED_SYNC_SOURCE_DIR = "/removedsync/";
	
	public static void sync(Context context)
	{
		if (BuildConfig.DEBUG)
		{
			String syncPath = SYNC_ROOT_SOURCE_DIR + context.getPackageName() + SYNC_SOURCE_DIR;
			String fullSyncPath = SYNC_ROOT_SOURCE_DIR + context.getPackageName() + FULL_SYNC_SOURCE_DIR;
			String removedSyncPath = SYNC_ROOT_SOURCE_DIR + context.getPackageName() + REMOVED_SYNC_SOURCE_DIR;
			
			
			if (Platform.IsLogEnabled)
			{
				Log.d(Platform.DEFAULT_LOG_TAG, "syncPath: " + syncPath);
				Log.d(Platform.DEFAULT_LOG_TAG, "fullSyncPath: " + fullSyncPath);
				Log.d(Platform.DEFAULT_LOG_TAG, "removedSyncPath: " + removedSyncPath);
			}
			
			File fullSyncDir = new File(fullSyncPath);
			if (fullSyncDir.exists())
			{
				executeFullSync(context, fullSyncDir);
				return;
			}
			
			File syncDir = new File(syncPath);
			if (syncDir.exists())
			{
				executePartialSync(context, syncDir);
			}
			
			File removedSyncDir = new File(removedSyncPath);
			if (removedSyncDir.exists())
			{
				executeRemovedSync(context, removedSyncDir);
			}
		}
	}

	private static void executeFullSync(Context context, File sourceDir)
	{
		try
		{
			//this removes only the app directory from the device to preserver any existing files in /files directory on the device
			String targetPath = context.getFilesDir().getAbsolutePath();
			StringBuilder builder = new StringBuilder();
			builder.append("echo \"Syncing files:\n\"");
			builder.append("du -a \"" + sourceDir.getAbsolutePath() + "\" && "); //list files to sync in logcat
			builder.append("rm -Rf \"" + targetPath + "/app\" && "); //remove app dir
			builder.append(String.format("cp -Rf \"%s/*\" \"%s/\" && ", sourceDir.getAbsolutePath(), targetPath)); //copy all sync files
			builder.append("rm -Rf \"" + sourceDir.getAbsolutePath() + "\""); //remove all source dir files
			
			if (Platform.IsLogEnabled)
			{
				Log.d(Platform.DEFAULT_LOG_TAG, "Executing full sync command: " + builder.toString());
			}
			
			Runtime.getRuntime().exec(builder.toString());
		}
		catch (IOException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Full Sync failed");
			e.printStackTrace();
		}
	}
	
	private static void executePartialSync(Context context, File sourceDir)
	{
		try
		{
			String targetPath = context.getFilesDir().getAbsolutePath();
			
			StringBuilder builder = new StringBuilder();
			builder.append("echo \"Syncing files:\n\"");
			builder.append("du -a \"" + sourceDir.getAbsolutePath() + "\" && "); //list files to sync in logcat
			builder.append(String.format("cp -Rf \"%s/*\" \"%s/\" && ", sourceDir.getAbsolutePath(), targetPath)); //copy all sync files
			builder.append("rm -Rf \"" + sourceDir.getAbsolutePath() + "\""); //remove all source dir files
			
			if (Platform.IsLogEnabled)
			{
				Log.d(Platform.DEFAULT_LOG_TAG, "Executing sync command: " + builder.toString());
			}
			
			Runtime.getRuntime().exec(builder.toString());
		}
		catch (IOException e)
		{
			Log.e(Platform.DEFAULT_LOG_TAG, "Sync failed");
			e.printStackTrace();
		}
	}
	
	private static void executeRemovedSync(final Context context, final File sourceDir)
	{
		final String targetPath = context.getFilesDir().getAbsolutePath();
		final String removedSyncPath = SYNC_ROOT_SOURCE_DIR + context.getPackageName() + REMOVED_SYNC_SOURCE_DIR;

		sourceDir.listFiles(new FileFilter()
		{
			@Override
			public boolean accept(File pathname)
			{
				if (pathname.isFile())
				{
					String filePathToRemove = pathname.getAbsolutePath().replace(removedSyncPath, targetPath);
					if (Platform.IsLogEnabled)
					{
						Log.d(Platform.DEFAULT_LOG_TAG, "Sync removing file: " + filePathToRemove);
					}
					
					File fileToRemove = new File(filePathToRemove);
					fileToRemove.delete();
					pathname.delete();
				}

				return false;
			}
		});
	}
}
