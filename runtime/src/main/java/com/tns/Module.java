package com.tns;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
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

		NativeScriptModulesFilesPath = "/app/tns_modules/tns_modules_core";

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

		String resolvedPath;

		try
		{
			resolvedPath = file.getCanonicalPath();
		}
		catch (IOException e)
		{
			e.printStackTrace();
			resolvedPath = file.getAbsolutePath();
		}

		return resolvedPath;
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

	private static String possibleError = "";

	private static File resolveRequire(String baseDir, String path) throws NativeScriptException { //think about special require exception inheriting NativeScriptException

		File foundModule = null;

		// relative or absolute path
		if (path.startsWith("./") || path.startsWith("../") || path.startsWith("~/")) {
			foundModule = resolveFromFileOrDirectory(baseDir, path);

			if(foundModule != null) {
				return foundModule;
			}
		}
		else { //native modules or traverse node_modules possiblilities
			foundModule = resolveFromFileOrDirectory(NativeScriptModulesFilesPath, path);

			if(foundModule != null) {
				return foundModule;
			}

			// resolve through node_modules
			String[] possibleSearchDirs = nodeModulesPaths(baseDir);
			for(String possibleDir : possibleSearchDirs) {
				foundModule = resolveFromFileOrDirectory(possibleDir, path);
				if(foundModule != null) {
					return foundModule;
				}
			}
		}

		// throw error if file could not be resolved
		if(foundModule == null) {
			throw new NativeScriptException("Could not load module: " + path + " " + possibleError);
		}

		return foundModule;
	}

	private static File resolveFromFileOrDirectory(String baseDir, String path) {
		File foundModule = null;

		foundModule = loadAsFile(baseDir, path);
		if(foundModule != null) {
			return foundModule;
		}

		foundModule = loadAsDirectory(baseDir, path);

		return foundModule;
	}

	//tries to load the path as a file, returns null if that's not possible
	private static File loadAsFile(String baseDir, String path) {
//		1. If X is a file, load X as JavaScript text.  STOP
//		2. If X.js is a file, load X.js as JavaScript text.  STOP
//		3. If X.json is a file, parse X.json to a JavaScript Object.  STOP
//		4. If X.node is a file, load X.node as binary addon.  STOP
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

		File file = new File(baseDir, path + fallbackExtension);

		return file;
	}

	private static File loadAsDirectory(String baseDir, String path) {
//		1. If X/package.json is a file,
//			a. Parse X/package.json, and look for "main" field.
//			b. let M = X + (json main field)
//		c. LOAD_AS_FILE(M)
//		2. If X/index.js is a file, load X/index.js as JavaScript text.  STOP
//		3. If X/index.json is a file, parse X/index.json to a JavaScript object. STOP
//		4. If X/index.node is a file, load X/index.node as binary addon.  STOP
		return null;
	}

	private static File loadNodeModules(String baseDir, String path) {
//		1. let DIRS=NODE_MODULES_PATHS(START)
//		2. for each DIR in DIRS:
//			a. LOAD_AS_FILE(DIR/X)
//			b. LOAD_AS_DIRECTORY(DIR/X)
		return null;
	}

	private static String[] nodeModulesPaths(String startDir) {
//		1. let PARTS = path split(START)
//		2. let I = count of PARTS - 1
//		3. let DIRS = []
//		4. while I >= 0,
//				a. if PARTS[I] = "node_modules" CONTINUE
//				c. DIR = path join(PARTS[0 .. I] + "node_modules")
//				b. DIRS = DIRS + DIR
//				c. let I = I - 1
//		5. return DIRS
		return null;
	}

	private static File resolvePathHelper(String path, String baseDir)
	{
		File directory = null;
		File file = null;
		String possibleException = null;

		if (baseDir == null || baseDir.isEmpty())
		{
			baseDir = ApplicationFilesPath + ModulesFilesPath;
		}
		// ourModule/file
		// file resolution
		if (path.startsWith("/"))
		{
			// absolute path
			directory = new File(path);
			file = getFileWithExtension(path);
			if (!file.exists())
			{
				possibleException = "Failed to find module with absolute path: \"" + path + "\".";
			}
		}
		else if (path.startsWith("./") || path.startsWith("../") || path.startsWith("~/") || path.startsWith("/"))
		{
			// same or up directory
			String resolvedPath = FileSystem.resolveRelativePath(ApplicationFilesPath, path, baseDir);
			directory = new File(resolvedPath);
			file = getFileWithExtension(directory.getAbsolutePath());

			if (!file.exists())
			{
				if (path.startsWith("~/"))
				{
					possibleException = "Failed to find module: \"" + path + "\", relative to: " + ModulesFilesPath;
				}
				else
				{
					String paretnFolderPath = file.getParentFile().getAbsolutePath();
					if (ApplicationFilesPath.length() <= paretnFolderPath.length())
					{
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
			if (!file.exists())
			{
				possibleException = "Failed to find module: \"" + path + "\", relative to: " + NativeScriptModulesFilesPath;
			}
		}

		// directory resolution
		if (!file.exists() && directory.exists() && directory.isDirectory())
		{
			// We are pointing to a directory, check for already resolved file
			String folderPath = directory.getAbsolutePath();
			String cachedPath = folderAsModuleCache.get(folderPath);
			boolean found = false;

			if (cachedPath == null)
			{
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
							if (!mainFile.endsWith(".js"))
							{
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
						file = null;
					}
				}

				if (!found)
				{
					// search for index.js
					file = new File(directory.getPath() + "/index.js");
				}

				// TODO: search for <folderName>.js ?

				if (file != null)
				{
					// cache the main file for later use
					folderAsModuleCache.put(folderPath, file.getAbsolutePath());
				}
			}
			else
			{
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
		else
		{
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

		String notFoundMessage = "Application entry point file not found. Please specify the file in package.json otherwise make sure the file index.js or bootstrap.js exists.\\nIf using typescript make sure your entry point file is transpiled to javascript.";

        // resolvePathHelper() never returns a non-existing file. Instead it throws an exception.
		File bootstrapFile;
		try {
			bootstrapFile = resolvePathHelper("./", "");
		} catch (NativeScriptException ex) {
			throw new NativeScriptException(notFoundMessage, ex);
		}

		if(bootstrapFile == null) {
			try {
				bootstrapFile = resolvePathHelper("./bootstrap", "");
				return bootstrapFile.getAbsolutePath();
			} catch (NativeScriptException ex) {
				throw new NativeScriptException(notFoundMessage, ex);
			}
		}

		return bootstrapFile.getAbsolutePath();
	}
}
