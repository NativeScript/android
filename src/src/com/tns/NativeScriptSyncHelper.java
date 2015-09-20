package com.tns;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;

public class NativeScriptSyncHelper
{
	private static String SYNC_ROOT_SOURCE_DIR = "/data/local/tmp/";
	private static final String SYNC_SOURCE_DIR = "/sync/";
	private static final String FULL_SYNC_SOURCE_DIR = "/fullsync/";
	private static final String REMOVED_SYNC_SOURCE_DIR = "/removedsync/";
	
	private static Logger logger;

	public static void sync(Logger logger, Context context)
	{
		NativeScriptSyncHelper.logger = logger;
		
		boolean shouldExecuteSync = getShouldExecuteSync(context);
		
		if (!shouldExecuteSync)
		{
			if (logger.isEnabled())
			{
				logger.write("Sync is not enabled.");
			}
			return;
		}

		String syncPath = SYNC_ROOT_SOURCE_DIR + context.getPackageName() + SYNC_SOURCE_DIR;
		String fullSyncPath = SYNC_ROOT_SOURCE_DIR + context.getPackageName() + FULL_SYNC_SOURCE_DIR;
		String removedSyncPath = SYNC_ROOT_SOURCE_DIR + context.getPackageName() + REMOVED_SYNC_SOURCE_DIR;
		
		if (logger.isEnabled())
		{
			logger.write("Sync is enabled:");
			logger.write("Sync path              : " + syncPath);
			logger.write("Full sync path         : " + fullSyncPath);
			logger.write("Removed files sync path: " + removedSyncPath);
		}
		
		
		File fullSyncDir = new File(fullSyncPath);
		if (fullSyncDir.exists())
		{
			executeFullSync(context, fullSyncDir);
			deleteRecursive(fullSyncDir);
			return;
		}

		File syncDir = new File(syncPath);
		if (syncDir.exists())
		{
			executePartialSync(context, syncDir);			
			deleteRecursive(syncDir);
		}

		File removedSyncDir = new File(removedSyncPath);
		if (removedSyncDir.exists())
		{
			executeRemovedSync(context, removedSyncDir);
			deleteRecursive(removedSyncDir);
		}
	}
	
	private static void deleteRecursive(File fileOrDirectory) {
		
		if (fileOrDirectory.isDirectory()){
			for (File child : fileOrDirectory.listFiles()){
				deleteRecursive(child);
			}
		}

	    fileOrDirectory.delete();
	}

	private static boolean getShouldExecuteSync(Context context)
	{
		int flags;
		boolean shouldExecuteSync = false;
		try
		{
			flags = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).applicationInfo.flags;
			shouldExecuteSync = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
		}
		catch (NameNotFoundException e)
		{
			e.printStackTrace();
			return false;
		}
		
		return shouldExecuteSync;
	}
	
	final static FileFilter deletingFilesFilter = new FileFilter()
	{
		@Override
		public boolean accept(File pathname)
		{
			if (pathname.isDirectory())
			{
				return true;
			}
			
			boolean success = pathname.delete();
			if (!success)
			{
				logger.write("Syncing: file not deleted: " + pathname.getAbsolutePath().toString());
			}
			return false;
		}
	};
	
	private static void deleteDir(File directory)
	{
		File[] subDirectories = directory.listFiles(deletingFilesFilter);
		if (subDirectories != null)
		{
			for (int i = 0; i < subDirectories.length; i++)
			{
				File subDir = subDirectories[i];
				deleteDir(subDir);
			}
		}
		
		boolean success = directory.delete();
		if (!success && directory.exists())
		{
			logger.write("Syncing: directory not deleted: " + directory.getAbsolutePath().toString());
		}
	}
	
	private static void moveFiles(File sourceDir, String sourceRootAbsolutePath, String targetRootAbsolutePath)
	{
		File[] files = sourceDir.listFiles();
		
		if (files != null)
		{
			for (int i = 0; i < files.length; i++)
			{
				File file = files[i];
				if (file.isFile())
				{
					if (logger.isEnabled())
					{
						logger.write("Syncing: " + file.getAbsolutePath().toString());
					}
					
					String targetFilePath = file.getAbsolutePath().replace(sourceRootAbsolutePath, targetRootAbsolutePath);
					File targetFileDir = new File(targetFilePath);
					
					File targetParent = targetFileDir.getParentFile();
					if (targetParent != null)	
					{
						targetParent.mkdirs();
					}
					
				    boolean success = copyFile(file.getAbsolutePath(), targetFilePath);
					if (!success)
					{
						logger.write("Sync failed: " + file.getAbsolutePath().toString());
					}
				}
				else
				{
					moveFiles(file, sourceRootAbsolutePath, targetRootAbsolutePath);
				}
			}
		}
	}
	
	// this removes only the app directory from the device to preserve
	// any existing files in /files directory on the device
	private static void executeFullSync(Context context, final File sourceDir)
	{
		String appPath = context.getFilesDir().getAbsolutePath() + "/app";
		final File appDir = new File(appPath);
		
		if (appDir.exists())
		{
			deleteDir(appDir);
			moveFiles(sourceDir, sourceDir.getAbsolutePath(), appDir.getAbsolutePath());
		}
	}

	private static void executePartialSync(Context context, File sourceDir)
	{
		String appPath = context.getFilesDir().getAbsolutePath() + "/app";
		final File appDir = new File(appPath);
		
		if (appDir.exists())
		{
			moveFiles(sourceDir, sourceDir.getAbsolutePath(), appDir.getAbsolutePath());
		}
	}

	private static void deleteRemovedFiles(File sourceDir,  String sourceRootAbsolutePath, String targetRootAbsolutePath)
	{
		File[] files = sourceDir.listFiles();
		
		if (files != null)
		{
			for (int i = 0; i < files.length; i++)
			{
				File file = files[i];
				if (file.isFile())
				{
					if (logger.isEnabled())
					{
						logger.write("Syncing removed file: " + file.getAbsolutePath().toString());
					}
					
					String targetFilePath = file.getAbsolutePath().replace(sourceRootAbsolutePath, targetRootAbsolutePath);
					File targetFile = new File(targetFilePath);
					targetFile.delete();
				}
				else
				{
					deleteRemovedFiles(file, sourceRootAbsolutePath, targetRootAbsolutePath);
				}
			}
		}
	}
	
	private static void executeRemovedSync(final Context context, final File sourceDir)
	{
		String appPath = context.getFilesDir().getAbsolutePath() + "/app";
		deleteRemovedFiles(sourceDir, sourceDir.getAbsolutePath(), appPath);
	}
	
	private static boolean copyFile(String sourceFile, String destinationFile)
	{
		FileInputStream fis = null;
		FileOutputStream fos = null;

		try
		{
			fis = new FileInputStream(sourceFile);
			fos = new FileOutputStream(destinationFile, false);

			byte[] buffer = new byte[4096];
			int read = 0;

			while ((read = fis.read(buffer)) != -1)
			{
				fos.write(buffer, 0, read);
			}
		}
		catch (FileNotFoundException e)
		{
			logger.write("Error copying file " + sourceFile);
			e.printStackTrace();
			return false;
		}
		catch (IOException e)
		{
			logger.write("Error copying file " + sourceFile);
		    e.printStackTrace();
		    return false;
		}
		finally
		{
			try
			{
				if (fis != null)
				{
					fis.close();
				}
				if (fos != null)
				{
					fos.close();
				}
			}
			catch (IOException e)
			{
			}
		}
		
		return true;
	}
	
//	private static String getSyncThumb(String syncThumbFilePath)
//	{
//		try
//		{
//			File syncThumbFile = new File(syncThumbFilePath);
//			if (syncThumbFile.exists())
//			{
//				return null;
//			}
//				
//			FileInputStream in = new FileInputStream(syncThumbFile);
//			BufferedReader reader = new BufferedReader(new InputStreamReader(in));
//			String syncThumb = reader.readLine();
//			reader.close();
//			in.close();
//			return syncThumb;
//		}
//		catch (FileNotFoundException e)
//		{
//			Log.e(Platform.DEFAULT_LOG_TAG, "Error while reading sync command");
//			e.printStackTrace();
//		}
//		catch (IOException e)
//		{
//			Log.e(Platform.DEFAULT_LOG_TAG, "Error while reading sync command");
//			e.printStackTrace();
//		}
//
//		return null;
//	}
}
