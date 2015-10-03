package com.tns;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

public class FileSystem
{
	private static final String appRootPrefix = "~/";
	
	public static String readAll(InputStream inputStream) throws IOException
	{
		StringBuilder text;
		BufferedReader buffReader = null;

		try
		{
			buffReader = new BufferedReader(new InputStreamReader(inputStream));
			String line;
			text = new StringBuilder();

			while ((line = buffReader.readLine()) != null)
			{
				text.append(line);
				text.append('\n');
			}
		}
		catch (IOException e){
			return "";
		}
		finally
		{
			if (buffReader != null){
				buffReader.close();
			}
		}

		return text.toString();
	}
	
	
	private final static byte[] buff = new byte[65536];
	
	public static String readText(File file) throws FileNotFoundException, IOException{
		int fileLength = (int)file.length();
		if (fileLength < buff.length)
		{
			FileInputStream fis = null;
			try
			{
				fis = new FileInputStream(file);
				int length = fis.read(buff, 0, fileLength);
				return new String(buff, 0, length);
			}
			finally
			{
				if (fis != null)
				{
					fis.close();
				}
			}
		}
		else
		{
			BufferedInputStream inputStream = null;
			try
			{
				inputStream = new BufferedInputStream(new FileInputStream(file));
				return readAll(inputStream);
			}
			catch (FileNotFoundException e){
				return "";
			}
			finally
			{
				if (inputStream != null){
					inputStream.close();
				}
			}
		}
	}
	
	public static JSONObject readJSONFile(File file) throws IOException, JSONException
	{
		String content = readText(file);
		return new JSONObject(content);
	}
	
	public static String resolveRelativePath(String applicationFilesDir, String path, String currentDirectory)
	{
		String baseDir;
		if (path.startsWith(appRootPrefix))
		{
			baseDir = new File(applicationFilesDir, "app").getAbsolutePath();
			path = path.substring(appRootPrefix.length());
		}
		else
		{
			baseDir = currentDirectory;
		}
		File temp = new File(baseDir, path);
		try
		{
			return temp.getCanonicalPath();
		}
		catch (IOException e)
		{
			try
			{
				URI uri = new URI(baseDir);
				return uri.resolve(path).getPath();
			}
			catch(URISyntaxException e1){
				return null;
			}
		}
	}
}
