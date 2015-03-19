package com.tns;

import java.io.File;
import java.io.IOException;

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
	//private static final String ModuleContent = "(function(){\n var module = {}; module.exports = arguments[0];" + "var exports = module.exports; var __dirname = \"%s\"; var __filename = \"%s\";" + "function require(moduleName){ return global.require(moduleName, __filename); }" + "module.filename = __filename; this.__extends = global.__extends; \n %s \n return module.exports; \n})";
	private static final String ModuleContent_Part1 = "(function(){\n var module = {}; module.exports = arguments[0];" + "var exports = module.exports; var __dirname = \"";
	private static final String ModuleContent_Part2 = "\"; var __filename = \"";
	private static final String ModuleContent_Part3 = "\";" + "function require(moduleName){ return __global.require(moduleName, __filename); }" + "module.filename = __filename; this.__extends = __global.__extends; \n";
	private static final String ModuleContent_Part4 ="\n return module.exports; \n})";
	private static final StringBuffer ModuleContent = new StringBuffer(65536);

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
		// NOTE: This functionality is temporary and is to be deleted within several months
		File file = new File(ApplicationFilesPath + NativeScriptModulesFilesPath);
		if(!file.exists())
		{
			NativeScriptModulesFilesPath = "/tns_modules/"; 
		}

		initialized = true;
	}

	public static String getApplicationFilesPath()
	{
		return ApplicationFilesPath;
	}

	public static String getAppContent(String appFileName)
	{
		File bootstrapFile = findModuleFile(appFileName, "");
		return getModuleContent(bootstrapFile.getPath());
	}

	public static String getModuleContent(String modulePath)
	{
		File file = new File(modulePath);
		if (file == null || !file.exists())
		{
			// Return empty content.
			// This case will be handled by the NativeScriptRuntime.cpp and a JS
			// exception will be raised.
			return "";
		}

		if (Platform.IsLogEnabled)
		{
			Log.d(Platform.DEFAULT_LOG_TAG, "Loading module from files " + file.getPath());
		}

		try
		{
			String moduleFileContent = FileSystem.readText(file);
			// IMPORTANT: Update MODULE_LINES_OFFSET in NativeScript.h
			// if you change the number of new lines that exists before the
			// moduleFileContent for correct error reporting.
			// We are inserting local require function in the scope of the
			// module to pass the __fileName (calling file) variable in the
			// global.require request.
			ModuleContent.setLength(0);
			ModuleContent.append(ModuleContent_Part1);
			ModuleContent.append(file.getParent());
			ModuleContent.append(ModuleContent_Part2);
			ModuleContent.append(modulePath);
			ModuleContent.append(ModuleContent_Part3);
			ModuleContent.append(moduleFileContent);
			ModuleContent.append(ModuleContent_Part4);
			String content = ModuleContent.toString(); 
			return content;  
		}
		catch (IOException e)
		{
			// Return empty content.
			// This case will be handled by the NativeScriptRuntime.cpp and a JS
			// exception will be raised.
			return "";
		}
	}

	public static String getModulePath(String moduleName, String callingModuleName)
	{
		// This method is called my the NativeScriptRuntime.cpp RequireCallback
		// method.
		// The currentModuleName is the fully-qualified path of the previously
		// loaded module (if any)
		String currentDirectory = null;

		if (callingModuleName != null && !callingModuleName.isEmpty())
		{
			File currentModule = new File(callingModuleName);
			if (currentModule.exists())
			{
				String parentDirectory = currentModule.getParent();
				if (parentDirectory != null)
				{
					currentDirectory = parentDirectory + "/";
				}
			}
		}

		File file = findModuleFile(moduleName, currentDirectory);

		if (file != null && file.exists())
		{
			File projectRootDir = new File(RootPackageDir);
			if (isFileExternal(file, projectRootDir))
			{
				return "EXTERNAL_FILE_ERROR";
			}
			else
			{
				return file.getPath();
			}
		}

		// empty path will be handled by the NativeScriptRuntime.cpp and a JS
		// error will be thrown
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
			jsFile = isJSFile ? new File(directory.getPath()) : new File(directory.getPath() + ".js");
		}
		else
		{
			// search for tns_module
			directory = new File(ApplicationFilesPath + NativeScriptModulesFilesPath, moduleName);
			jsFile = isJSFile ? new File(directory.getPath()) : new File(directory.getPath() + ".js");
		}

		if (!jsFile.exists() && directory.exists() && directory.isDirectory())
		{
			// we are pointing to a directory, search for package.json or
			// index.js
			File packageFile = new File(directory.getPath() + "/package.json");
			if (packageFile.exists())
			{
				try
				{
					JSONObject object = FileSystem.readJSONFile(packageFile);
					if (object != null)
					{
						String mainFile = object.getString("main");
						jsFile = new File(directory.getPath(), mainFile);
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
			else
			{
				// search for index.js
				jsFile = new File(directory.getPath() + "/index.js");
			}
		}

		return jsFile;
	}
}
