package com.tns;

import android.text.TextUtils;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

class Module
{
	private static String RootPackageDir;
	private static String ApplicationFilesPath;
	private static String ModulesFilesPath;
	private static String NativeScriptModulesFilesPath;
	private static int RootDirsCount;
	private static boolean initialized = false;
	private static String possibleException = null;

	// cache the already resolved absolute paths to folder modules to prevent JSON parsing each time
	private static final HashMap<String, String> folderAsModuleCache = new HashMap<String, String>();
	private static boolean checkForExternalPath = false;

	public static void init(Logger logger, File rootPackageDir, File applicationFilesDir) throws IOException
	{
		if (initialized)
		{
			return;
		}

		RootPackageDir = rootPackageDir.getCanonicalPath();
		ApplicationFilesPath = applicationFilesDir.getCanonicalPath();

		ModulesFilesPath = "/app/";

		RootDirsCount = (ApplicationFilesPath + "/app").split("/").length;

		NativeScriptModulesFilesPath = ApplicationFilesPath + "/app/tns_modules/tns-core-modules";

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

	private static File resolvePathHelper(String path, String baseDir) throws NativeScriptException { //think about special require exception inheriting NativeScriptException

		File foundModule = null;
		if (baseDir == null || baseDir.isEmpty() || path.startsWith("~/"))
		{
			baseDir = ApplicationFilesPath + ModulesFilesPath;
		}

		// relative or absolute path
		if (path.startsWith("./") || path.startsWith("../") || path.startsWith("~/") || path.startsWith("/")) {

			File possibleRelativeAbsoluteModule = prepareRelativeOrAbsoluteDir(path, baseDir);
			if(possibleRelativeAbsoluteModule != null) {
				foundModule = resolveFromFileOrDirectory(baseDir, path, possibleRelativeAbsoluteModule);
				if(foundModule != null) {
					return foundModule;
				}
			}
		}
		else { //native modules or traverse node_modules possiblilities
			File possibleNativeModule = new File(NativeScriptModulesFilesPath, path);
			foundModule = resolveFromFileOrDirectory(NativeScriptModulesFilesPath, path, possibleNativeModule);
			if(foundModule != null) {
				return foundModule;
			}

			// resolve through node_modules
			List<String> possibleSearchDirs = nodeModulesPaths(baseDir);
			for(String possibleDir : possibleSearchDirs) {
				File possibleRelativeModule = new File(possibleDir, path);
				foundModule = resolveFromFileOrDirectory(possibleDir, path, possibleRelativeModule);
				if(foundModule != null) {
					return foundModule;
				}
			}
		}

		// throw error if file could not be resolved
		throw new NativeScriptException(possibleException);
	}

	private static File prepareRelativeOrAbsoluteDir(String path, String baseDir) {
		File fileOrDirectory = null;

		if(path.startsWith("/")) {
			fileOrDirectory = new File(path);
		}
		else if(path.startsWith("~/")) {
			fileOrDirectory = new File(ApplicationFilesPath + ModulesFilesPath, path.substring(2));
		}
		else if(path.startsWith("./") || path.startsWith("../")) {
			fileOrDirectory = new File(baseDir, path);
		}

		return fileOrDirectory;
	}

	private static File resolveFromFileOrDirectory(String baseDir, String path, File fileOrDirectory) {
		File foundModule = null;

		foundModule = loadAsFile(fileOrDirectory);
		if(foundModule != null) {
			return foundModule;
		}
		else {
			if (path.startsWith("~/"))
			{
				possibleException = "Failed to find module: \"" + path + "\", relative to: " + ModulesFilesPath;
			}
			else
			{
				possibleException = "Failed to find module: \"" + path + "\", relative to: " + baseDir.substring(ApplicationFilesPath.length() + 1) + "/";
			}
		}

		foundModule = loadAsDirectory(fileOrDirectory);

		return foundModule;
	}

	//tries to load the path as a file, returns null if that's not possible
	private static File loadAsFile(File path) {
		String fallbackExtension;

		boolean isJSFile = path.getName().endsWith(".js");
		boolean isSOFile = path.getName().endsWith(".so");
		boolean isJSONFile = path.getName().endsWith(".json");

		if (isJSFile || isJSONFile || isSOFile)
		{
			fallbackExtension = "";
		}
		else
		{
			fallbackExtension = ".js";
		}

		File foundFile = new File(path.getAbsolutePath() + fallbackExtension);
		try {
			if(foundFile.getCanonicalFile().exists()) {
                return foundFile;
            }
		} catch (IOException e) {
			// return null
		}

		return null;

		//TODO: plamen5kov: implement later if necessary
//		If X is a file, load X as JavaScript text.  STOP
//		If X.node is a file, load X.node as binary addon.  STOP
	}

	private static File loadAsDirectory(File path) {
		File packageFile = new File(path, "/package.json");
		File foundFile = null;

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

					File pathFromMain = new File(path, mainFile);
					foundFile = loadAsFile(pathFromMain);

					if(foundFile != null) {
						return foundFile;
					}
				}
			}
			catch (IOException e)
			{
				throw new NativeScriptException(e.getMessage());
			}
			catch (JSONException e)
			{
				if(!e.getMessage().contains("No value for main")) {
					throw new NativeScriptException(e.getMessage());
				}
			}
		}

		//fallback to index js
		foundFile = new File(path, "index.js");
		try {
			if(foundFile.getCanonicalFile().exists()) {
                return foundFile;
            }
		} catch (IOException e) {
			return null;
		}

		//TODO: plamen5kov: add later if necessary
//		If X/index.json is a file, parse X/index.json to a JavaScript object. STOP
//		4. If X/index.node is a file, load X/index.node as binary addon.  STOP
		return null;
	}

	private static List<String> nodeModulesPaths(String startDir) {

		File startFile = new File(startDir);

		String[] startDirParts = startFile.getAbsolutePath().split("/");
		List<String> dirs = new LinkedList<String>();
		for(String part : startDirParts) {
			dirs.add(part);
		}

		List<String> possibleDirectories = new ArrayList<String>();
		while (dirs.size() >= RootDirsCount) {

			String lastDir = dirs.get(dirs.size() - 1);
			if(lastDir.equals("node_modules") || lastDir.equals("tns_modules")) {
				dirs.remove(dirs.size() -1);
				continue;
			}

			String currentDir = TextUtils.join("/", dirs);
			String finalDir = currentDir + "/node_modules";

			//TODO: plamen5kov revisit this if we decide to get rid of tns_modules folder
			if(lastDir.equals("app")) {
				finalDir = currentDir + "/tns_modules";
			}

			possibleDirectories.add(finalDir);

			dirs.remove(dirs.size() -1);
		}

		return possibleDirectories;
	}

	////////// REMOVE WITH DEPENDENCIES /////////////
	private static File a(String path, String baseDir)
	{
		File directory = null;
		File file = null;

		if (baseDir == null || baseDir.isEmpty())
		{
			baseDir = ApplicationFilesPath + ModulesFilesPath;
		}

		// absolute path
		if (path.startsWith("/"))
		{
			directory = new File(path);
			file = getFileWithExtension(path);
			if (!file.exists())
			{
				possibleException = "Failed to find module with absolute path: \"" + path + "\".";
			}
		}
		//relative path
		else if (path.startsWith("./") || path.startsWith("../") || path.startsWith("~/") || path.startsWith("/")) {
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
						possibleException = "Failed to find module: \"" + path + "\", relative to: " + paretnFolderPath.substring(ApplicationFilesPath.length() + 1) + "/";
					}
				}
			}
		}
		// search for tns_module
		else
		{
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
