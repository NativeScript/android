package com.tns;

import java.io.File;
import org.json.JSONObject;

class V8Config
{
	private static final String AndroidKey = "android";
	private static final String V8FlagsKey = "v8Flags";
	private static final String CodeCacheKey = "codeCache";
	private static final String HeapSnapshotKey = "heapSnapshot";
	
	public static Object[] fromPackageJSON(File appDir)
	{
		Object[] result = makeDefaultOptions();
		File packageInfo = new File (appDir, "/app/package.json");
		if(!packageInfo.exists())
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
				if(androidObject.has(V8FlagsKey))
				{
					result[0] = androidObject.getString(V8FlagsKey); 
				}
				if(androidObject.has(CodeCacheKey))
				{
					result[1] = androidObject.getBoolean(CodeCacheKey);
				}
				if(androidObject.has(HeapSnapshotKey))
				{
					result[2] = androidObject.getBoolean(HeapSnapshotKey);
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
			false
		};
		
		return result;
	}
}
