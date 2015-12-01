package com.tns;

import java.io.File;
import java.io.IOException;
import java.security.InvalidParameterException;
import java.util.HashMap;

import org.json.JSONException;
import org.json.JSONObject;


class Module
{
	private static Logger logger;
	private static String RootPackageDir;
	private static String ApplicationFilesPath;
	private static String ModulesFilesPath;
	private static String NativeScriptModulesFilesPath;
	private static boolean initialized = false;
	
	// cache the already resolved absolute paths to folder modules to prevent JSON parsing each time
	private static final HashMap<String, String> folderAsModuleCache = new HashMap<String, String>();
	private static boolean checkForExternalPath = false;

	public static void init(Logger logger, File rootPackageDir, File applicationFilesDir) throws IOException
	{
		Module.logger = logger; 
		if (initialized)
		{
			return;
		}
		
		RootPackageDir = rootPackageDir.getCanonicalPath();
		ApplicationFilesPath = applicationFilesDir.getCanonicalPath();
		
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

	static String getApplicationFilesPath()
	{
		return ApplicationFilesPath;
	}

	@RuntimeCallable
	private static String resolvePath(String path, String baseDir)
	{
		// The baseDir is the directory path of the calling module.
		checkForExternalPath = true;
		File file = resolvePathHelper(path, baseDir);

		return file.getAbsolutePath();
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
	
	private static File resolvePathHelper(String path, String baseDir)
	{
		File directory = null;
		File file = null;
		String possibleException  = null;

		if (baseDir == null || baseDir.isEmpty())
		{
			baseDir = ApplicationFilesPath + ModulesFilesPath;
		}

		if (path.startsWith("/"))
		{
			// absolute path
			directory = new File(path);
			file = getFileWithExtension(path);
			if(!file.exists()) {
				possibleException = "Failed to find module with absolute path: \"" + path + "\".";
			}
		}
		else if (path.startsWith("./") || path.startsWith("../") || path.startsWith("~/"))
		{
			// same or up directory
			String resolvedPath = FileSystem.resolveRelativePath(ApplicationFilesPath, path, baseDir);
			directory = new File(resolvedPath);
			file = getFileWithExtension(directory.getAbsolutePath());
			
			if(!file.exists()) {
				if(path.startsWith("~/")) {
					possibleException = "Failed to find module: \"" + path + "\", relative to: " + ModulesFilesPath;
				}
				else {
					String paretnFolderPath = file.getParentFile().getAbsolutePath();
					if(ApplicationFilesPath.length() <= paretnFolderPath.length()) {
						possibleException = "Failed to find module: \"" + path + "\", relative to: " + paretnFolderPath.substring(ApplicationFilesPath.length()) + "/";
					}
				}
			}
		}
		else
		{
			// search for tns_module
			directory = new File(ApplicationFilesPath + NativeScriptModulesFilesPath, path);
			file = getFileWithExtension(directory.getPath());
			if(!file.exists()) {
				possibleException = "Failed to find module: \"" + path + "\", relative to: " + NativeScriptModulesFilesPath;
			}
		}

		if (!file.exists() && directory.exists() && directory.isDirectory())
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
							if(!mainFile.endsWith(".js")) {
								mainFile += ".js";
							}
							file = new File(directory.getAbsolutePath(), mainFile);
							found = true;
						}
					}
					catch (IOException e)
					{
						throw new NativeScriptException(e.getMessage());
					}
					catch (JSONException e)
					{
						throw new NativeScriptException(e.getMessage());
					}
				}
				
				if (!found)
				{
					// search for index.js
					file = new File(directory.getPath() + "/index.js");
				}
				
				// TODO: search for <folderName>.js ?
				
				// cache the main file for later use
				folderAsModuleCache.put(folderPath, file.getAbsolutePath());
			}
			else {
				// do not check whether this path is external for the application - it is already checked
				checkForExternalPath = false;
				file = new File(cachedPath);
			}
		}
		
		File projectRootDir = new File(ApplicationFilesPath);
		if (checkForExternalPath && isFileExternal(file, projectRootDir))
		{
			throw new NativeScriptException("Module " + path + " is an external path. You can only load modules inside the application!");
		}
		
		if (file.exists())
		{
			return file;
		}
		else {
			throw new NativeScriptException(possibleException);
		}
	}
	
	private static File getFileWithExtension(String path)
	{
		String fallbackExtension;
		
		boolean isJSFile = path.endsWith(".js");
		boolean isSOFile = path.endsWith(".so");
		boolean isJSONFile = path.endsWith(".json");
		
		if (isJSFile || isJSONFile || isSOFile)
		{
			fallbackExtension = "";
		}
		else
		{
			fallbackExtension = ".js";
		}
		
		File file = new File(path + fallbackExtension);
		return file;
	}

	static String bootstrapApp()
	{
		// Bootstrap logic flows like:
		// 1. Check for package.json -> `main` field
		// 2. Check for index.js
		// 3. Check for bootstrap.js

		File bootstrapFile = resolvePathHelper("./", "");
		if (!bootstrapFile.exists())
		{
			bootstrapFile = resolvePathHelper("./bootstrap", "");
		}

		if (!bootstrapFile.exists())
		{
			throw new NativeScriptException("Application entry point file not found. Please specify either package.json with main field, index.js or bootstrap.js!");		
		}

		String modulePath = bootstrapFile.getAbsolutePath();
		return modulePath;
	}
}
