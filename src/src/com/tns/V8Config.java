package com.tns;

import java.io.File;

import org.json.JSONObject;

import android.os.Build;

class V8Config
{
	private static final String AndroidKey = "android";
	private static final String V8FlagsKey = "v8Flags";
	private static final String CodeCacheKey = "codeCache";
	private static final String HeapSnapshotKey = "heapSnapshot";
	private static final String HeapSnapshotScriptKey = "heapSnapshotScript";
	private static final String HeapSnapshotBlobKey = "heapSnapshotBlob";
	private static final String SnapshotFile = "snapshot.blob";
	private static final String ProfilerOutputDirKey = "profilerOutputDir";
	
	public static Object[] fromPackageJSON(File appDir)
	{
		Object[] result = makeDefaultOptions();
		File packageInfo = new File(appDir, "/app/package.json");
		if (!packageInfo.exists())
		{
			return result;
		}

		JSONObject rootObject;
		try
		{
			rootObject = FileSystem.readJSONFile(packageInfo);
			if (rootObject != null && rootObject.has(AndroidKey))
			{
				JSONObject androidObject = rootObject.getJSONObject(AndroidKey);
				if (androidObject.has(V8FlagsKey))
				{
					result[0] = androidObject.getString(V8FlagsKey);
				}
				if (androidObject.has(CodeCacheKey))
				{
					result[1] = androidObject.getBoolean(CodeCacheKey);
				}
				if (androidObject.has(HeapSnapshotKey))
				{
					result[2] = androidObject.getBoolean(HeapSnapshotKey);
				}
				if(androidObject.has(HeapSnapshotScriptKey))
				{
					String value = androidObject.getString(HeapSnapshotScriptKey);
					result[3] = FileSystem.resolveRelativePath(appDir.getPath(), value, appDir + "/app/");
				}
				if(androidObject.has(HeapSnapshotBlobKey))
				{
					String value = androidObject.getString(HeapSnapshotBlobKey);
					String path = FileSystem.resolveRelativePath(appDir.getPath(), value, appDir + "/app/");
					File dir = new File(path);
					if(dir.exists() && dir.isDirectory()) 
					{
						// this path is expected to be a directory, containing three sub-directories: armeabi-v7a, x86 and arm64-v8a 
						path = path + "/" + Build.CPU_ABI + "/" + SnapshotFile;
						result[4] = path;
					}
				}
				if(androidObject.has(ProfilerOutputDirKey))
				{
					result[5] = androidObject.getString(ProfilerOutputDirKey);
				}
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		return result;
	}

	private static Object[] makeDefaultOptions()
	{
		Object[] result = new Object[] {
			// v8 startup flags, defaults to --expose_gc due to tns_modules requirement 
			"--expose_gc",
			// enable v8 code caching, false by default
			false,
			// enable v8 heap snapshot, false by default
			false,
			// arbitrary script to be included in the snapshot
			"",
			// a binary file containing an already saved snapshot
			"",
			// V8 profiler output directory
			""
		};

		return result;
	}
}
