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

class Module {
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

    public static void init(Logger logger, File rootPackageDir, File applicationFilesDir) throws IOException {
        if (initialized) {
            return;
        }

        RootPackageDir = rootPackageDir.getCanonicalPath();
        ApplicationFilesPath = applicationFilesDir.getCanonicalPath();

        ModulesFilesPath = "/app/";

        RootDirsCount = (ApplicationFilesPath + "/app").split("/").length;

        NativeScriptModulesFilesPath = ApplicationFilesPath + "/app/tns_modules/tns-core-modules";

        initialized = true;
    }

    static String getApplicationFilesPath() {
        return ApplicationFilesPath;
    }

    @RuntimeCallable
    private static String resolvePath(String path, String baseDir) {
        // The baseDir is the directory path of the calling module.
        checkForExternalPath = true;
        File file = null;
        try {
            file = resolvePathHelper(path, baseDir);
        } catch (IOException e) {
            //probably cache failed because of getCanonicalPath
            e.printStackTrace();
        }

        String resolvedPath;

        try {
            resolvedPath = file.getCanonicalPath();
        } catch (IOException e) {
            e.printStackTrace();
            resolvedPath = file.getAbsolutePath();
        }

        return resolvedPath;
    }

    private static boolean isFileExternal(File source, File target) {
        File currentParentDir = source.getParentFile();

        while (currentParentDir != null) {
            if (currentParentDir.equals(target)) {
                return false;
            }

            currentParentDir = currentParentDir.getParentFile();
        }

        return true;
    }

    private static File resolvePathHelper(String path, String baseDir) throws NativeScriptException, IOException { //think about special require exception inheriting NativeScriptException

        File foundModule = null;
        if (baseDir == null || baseDir.isEmpty() || path.startsWith("~/")) {
            baseDir = ApplicationFilesPath + ModulesFilesPath;
        }

        // relative or absolute path
        if (path.startsWith("./") || path.startsWith("../") || path.startsWith("~/") || path.startsWith("/")) {

            File possibleRelativeAbsoluteModule = prepareRelativeOrAbsoluteDir(path, baseDir);

            //cache
            if (folderAsModuleCache.containsKey(possibleRelativeAbsoluteModule.getCanonicalPath())) {
                return new File(folderAsModuleCache.get(possibleRelativeAbsoluteModule.getCanonicalPath()));
            }

            if (possibleRelativeAbsoluteModule != null) {
                foundModule = resolveFromFileOrDirectory(baseDir, path, possibleRelativeAbsoluteModule);
                if (foundModule != null) {
                    return foundModule;
                }
            }
        } else { //native modules or traverse node_modules possiblilities
            File possibleNativeModule = new File(NativeScriptModulesFilesPath, path);

            //cache
            if (folderAsModuleCache.containsKey(possibleNativeModule.getCanonicalPath())) {
                return new File(folderAsModuleCache.get(possibleNativeModule.getCanonicalPath()));
            }

            foundModule = resolveFromFileOrDirectory(NativeScriptModulesFilesPath, path, possibleNativeModule);
            if (foundModule != null) {
                return foundModule;
            }

            // resolve through node_modules
            List<String> possibleSearchDirs = nodeModulesPaths(baseDir);
            for (String possibleDir : possibleSearchDirs) {
                File possibleRelativeModule = new File(possibleDir, path);

                //cache
                if (folderAsModuleCache.containsKey(possibleRelativeModule.getCanonicalPath())) {
                    return new File(folderAsModuleCache.get(possibleRelativeModule.getCanonicalPath()));
                }

                foundModule = resolveFromFileOrDirectory(possibleDir, path, possibleRelativeModule);
                if (foundModule != null) {
                    return foundModule;
                }
            }
        }

        //TODO: plamen5kov: implement later if necessary
//		if (checkForExternalPath && isFileExternal(file, projectRootDir))
//		{
//			throw new NativeScriptException("Module " + path + " is an external path. You can only load modules inside the application!");
//		}
        // throw error if file could not be resolved
        throw new NativeScriptException(possibleException);
    }

    private static File prepareRelativeOrAbsoluteDir(String path, String baseDir) {
        File fileOrDirectory = null;

        if (path.startsWith("/")) {
            fileOrDirectory = new File(path);
        } else if (path.startsWith("~/")) {
            fileOrDirectory = new File(ApplicationFilesPath + ModulesFilesPath, path.substring(2));
        } else if (path.startsWith("./") || path.startsWith("../")) {
            fileOrDirectory = new File(baseDir, path);
        }

        return fileOrDirectory;
    }

    private static File resolveFromFileOrDirectory(String baseDir, String path, File fileOrDirectory) throws IOException {
        File foundModule = null;

        foundModule = loadAsFile(fileOrDirectory);

        if (foundModule != null) {
            return foundModule;
        } else {
            if (path.startsWith("~/")) {
                possibleException = "Failed to find module: \"" + path + "\", relative to: " + ModulesFilesPath;
            } else {
                possibleException = "Failed to find module: \"" + path + "\", relative to: " + baseDir.substring(ApplicationFilesPath.length() + 1) + "/";
            }
        }

        foundModule = loadAsDirectory(baseDir, path, fileOrDirectory);

        //cache resolved directory
        if (foundModule != null) {
            folderAsModuleCache.put(fileOrDirectory.getCanonicalPath(), foundModule.getCanonicalPath());

            return foundModule;
        }

        return foundModule;
    }

    //tries to load the path as a file, returns null if that's not possible
    private static File loadAsFile(File path) {
        String fallbackExtension;

        boolean isJSFile = path.getName().endsWith(".js");
        boolean isSOFile = path.getName().endsWith(".so");
        boolean isJSONFile = path.getName().endsWith(".json");

        if (isJSFile || isJSONFile || isSOFile) {
            fallbackExtension = "";
        } else {
            fallbackExtension = ".js";
        }

        File foundFile = new File(path.getAbsolutePath() + fallbackExtension);
        try {
            File canonicalFile = foundFile.getCanonicalFile();
            if (canonicalFile.exists() && canonicalFile.isFile()) {
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

    private static File loadAsDirectory(String baseDir, String currentPath, File path) {
        File packageFile = new File(path, "/package.json");
        File foundFile = null;

        if (packageFile.exists()) {
            try {
                JSONObject object = FileSystem.readJSONFile(packageFile);
                if (object != null) {
                    String mainFile = object.getString("main");

                    File pathFromMain = new File(path, mainFile);
                    foundFile = resolveFromFileOrDirectory(baseDir, currentPath, pathFromMain);
                    if (foundFile != null) {
                        return foundFile;
                    }
                }
            } catch (IOException e) {
                throw new NativeScriptException(e.getMessage());
            } catch (JSONException e) {
                if (!e.getMessage().contains("No value for main")) {
                    throw new NativeScriptException(e.getMessage());
                }
            }
        }

        //fallback to index js
        foundFile = new File(path, "index.js");
        try {
            if (foundFile.getCanonicalFile().exists()) {
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

    // lists all possible node_modules directories starting from "startDir"
    private static List<String> nodeModulesPaths(String startDir) {

        File startFile = new File(startDir);

        String[] startDirParts = startFile.getAbsolutePath().split("/");
        List<String> dirs = new LinkedList<String>();
        for (String part : startDirParts) {
            dirs.add(part);
        }

        List<String> possibleDirectories = new ArrayList<String>();
        while (dirs.size() >= RootDirsCount) {

            String lastDir = dirs.get(dirs.size() - 1);
            if (lastDir.equals("node_modules") || lastDir.equals("tns_modules")) {
                dirs.remove(dirs.size() -1);
                continue;
            }

            String currentDir = TextUtils.join("/", dirs);
            String finalDir = currentDir + "/node_modules";

            //TODO: plamen5kov revisit this if we decide to get rid of tns_modules folder
            if (lastDir.equals("app")) {
                finalDir = currentDir + "/tns_modules";
            }

            possibleDirectories.add(finalDir);

            dirs.remove(dirs.size() -1);
        }

        return possibleDirectories;
    }

    static String bootstrapApp() {
        // Bootstrap logic flows like:
        // 1. Check for package.json -> `main` field
        // 2. Check for index.js
        // 3. Check for bootstrap.js

        String notFoundMessage = "Application entry point file not found. Please specify the file in package.json otherwise make sure the file index.js or bootstrap.js exists.\\nIf using typescript make sure your entry point file is transpiled to javascript.";

        // resolvePathHelper() never returns a non-existing file. Instead it throws an exception.
        File bootstrapFile = null;
        try {
            bootstrapFile = resolvePathHelper("./", "");
        } catch (NativeScriptException ex) {
            throw new NativeScriptException(notFoundMessage, ex);
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (bootstrapFile == null) {
            try {
                bootstrapFile = resolvePathHelper("./bootstrap", "");
                return bootstrapFile.getAbsolutePath();
            } catch (NativeScriptException ex) {
                throw new NativeScriptException(notFoundMessage, ex);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return bootstrapFile.getAbsolutePath();
    }
}
