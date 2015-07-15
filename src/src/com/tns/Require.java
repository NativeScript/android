package com.tns;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.util.Log;

public class Require
{
	private static String RootPackageDir;
	private static String ApplicationFilesPath;
	private static String ModulesFilesPath;
	private static String NativeScriptModulesFilesPath;
	private static boolean initialized = false;
	
	// cache the already resolved absolute paths to folder modules to prevent JSON parsing each time
	private static final HashMap<String, String> folderAsModuleCache = new HashMap<String, String>();
	private static boolean checkForExternalPath = false;

	public static void init(Context context)
	{
		if (initialized)
		{
			return;
		}

		RootPackageDir = context.getApplicationInfo().dataDir;

		ApplicationFilesPath = context.getApplicationContext().getFilesDir().getPath();
		ModulesFilesPath = "/app/";

		NativeScriptModulesFilesPath = "/app/tns_modules/";

		// Support previous tns_modules location for now
		// NOTE: This functionality is temporary and is to be deleted within
		// several months
		File file = new File(ApplicationFilesPath + NativeScriptModulesFilesPath);
		if (!file.exists())
		{
			NativeScriptModulesFilesPath = "/tns_modules/";
		}

		initialized = true;
	}

	public static String getApplicationFilesPath()
	{
		return ApplicationFilesPath;
	}

	public static String bootstrapApp()
	{
		// Bootstrap logic flows like:
		// 1. Check for package.json -> `main` field
		// 2. Check for index.js
		// 3. Check for bootstrap.js

		File bootstrapFile = findModuleFile("./", "");
		if (!bootstrapFile.exists())
		{
			bootstrapFile = findModuleFile("./bootstrap", "");
		}

		if (!bootstrapFile.exists())
		{
			Platform.APP_FAIL("Application entry point file not found. Please specify either package.json with main field, index.js or bootstrap.js!");
		}

		String modulePath = bootstrapFile.getAbsolutePath();

		return modulePath;
	}

	public static String getModulePath(String moduleName, String callingDirName)
	{
		// This method is called my the NativeScriptRuntime.cpp RequireCallback method.
		// The currentModuleDir is the directory path of the calling module.
		checkForExternalPath = true;
		File file = findModuleFile(moduleName, callingDirName);

		if (file != null && file.exists())
		{
			File projectRootDir = new File(RootPackageDir);
			if (checkForExternalPath && isFileExternal(file, projectRootDir))
			{
				return "EXTERNAL_FILE_ERROR";
			}
			else
			{
				return file.getAbsolutePath();
			}
		}

		// empty path will be handled by the NativeScriptRuntime.cpp and a JS error will be thrown
		return "";
	}

	private static boolean isFileExternal(File source, File target)
	{
		File currentParentDir = source.getParentFile();

		while (currentParentDir != null)
		{
			if (currentParentDir.equals(target))
			{
				return false;
			}

			currentParentDir = currentParentDir.getParentFile();
		}

		return true;
	}

	private static File findModuleFile(String moduleName, String currentDirectory)
	{
		File directory = null;
		File jsFile = null;
		boolean isJSFile = moduleName.endsWith(".js");

		if (currentDirectory == null || currentDirectory.isEmpty())
		{
			currentDirectory = ApplicationFilesPath + ModulesFilesPath;
		}

		if (moduleName.startsWith("/"))
		{
			// absolute path
			directory = new File(moduleName);
			jsFile = isJSFile ? new File(moduleName) : new File(moduleName + ".js");
		}
		else if (moduleName.startsWith("./") || moduleName.startsWith("../"))
		{
			// same or up directory
			String resolvedPath = FileSystem.resolveRelativePath(moduleName, currentDirectory);
			directory = new File(resolvedPath);
			jsFile = isJSFile ? new File(directory.getAbsolutePath()) : new File(directory.getAbsolutePath() + ".js");
		}
		else
		{
			// search for tns_module
			directory = new File(ApplicationFilesPath + NativeScriptModulesFilesPath, moduleName);
			jsFile = isJSFile ? new File(directory.getPath()) : new File(directory.getPath() + ".js");
		}

		if (!jsFile.exists() && directory.exists() && directory.isDirectory())
		{
			// We are pointing to a directory, check for already resolved file
			String folderPath = directory.getAbsolutePath();
			String cachedPath = folderAsModuleCache.get(folderPath);
			boolean found = false;
			
			if(cachedPath == null) {
				// Search for package.json or index.js
				File packageFile = new File(directory.getPath() + "/package.json");
				if (packageFile.exists())
				{
					try
					{
						JSONObject object = FileSystem.readJSONFile(packageFile);
						if (object != null)
						{
							String mainFile = object.getString("main");
							jsFile = new File(directory.getAbsolutePath(), mainFile);
							found = true;
						}
					}
					catch (IOException e)
					{
						// json read failed
						jsFile = null;
					}
					catch (JSONException e)
					{
						jsFile = null;
					}
				}
				if (!found)
				{
					// search for index.js
					jsFile = new File(directory.getPath() + "/index.js");
				}
				
				// TODO: search for <folderName>.js ?
				
				if(jsFile != null) {
					// cache the main file for later use
					folderAsModuleCache.put(folderPath, jsFile.getAbsolutePath());
				}
			}
			else {
				// do not check whether this path is external for the application - it is already checked
				checkForExternalPath = false;
				jsFile = new File(cachedPath);
			}
		}

		return jsFile;
	}
}
