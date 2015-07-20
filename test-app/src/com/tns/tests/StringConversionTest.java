package com.tns.tests;

import java.io.InputStream;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

import com.tns.Platform;

public class StringConversionTest
{
	public final String s;
	
	public StringConversionTest() throws Exception
	{
		this.s = readString();
	}
	
	public String getString()
	{
		return this.s;
	}
	
	public String getString(int notUsed)
	{
		return this.s;
	}
	
	public String getString(byte notUsed)
	{
		return this.s;
	}
	
	public int getLength()
	{
		return this.s.length();
	}
	
	public boolean equalsString(String other) throws Exception
	{
		byte[] thisBytes = this.s.getBytes("UTF-8");
		byte[] otherBytes = other.getBytes("UTF-8");
		
		boolean isEqualsString = thisBytes.length == otherBytes.length;
		
		int i = 0;
		
		if (isEqualsString)
		{
			for (; i < thisBytes.length; i++)
			{
				isEqualsString = thisBytes[i] == otherBytes[i];
				
				if (!isEqualsString)
					break;
			}
		}

		return isEqualsString;
	}
	
	public String[] getStringArr()
	{
		return new String[10];
	}
	
	public boolean equalsLength(String other)
	{
		int thisLength = this.s.length();
		int otherLength = other.length();
		
		return thisLength == otherLength;
	}
	
	public void triggerCallback()
	{
		this.callback(this.s);
	}
	
	public void callback(String str)
	{
	}

	private String readString() throws Exception
	{
		String str = null;
		
		Context context = Platform.getApplicationContext();
		
		InputStream inputStream = null;
		try
		{
			String assetName = "app/tests/image.jpg";
			
			int fileLength = 0;
			AssetFileDescriptor fd = null;
			try
			{
				fd = context.getAssets().openFd(assetName);
				fileLength = (int)fd.getLength();
			}
			finally
			{
				if (fd != null)
					fd.close();
			}
			
			inputStream = context.getAssets().open(assetName, AssetManager.ACCESS_STREAMING);
			
			byte[] data = new byte[fileLength];
			
			inputStream.read(data);
			
			str = android.util.Base64.encodeToString(data, android.util.Base64.DEFAULT);
			
			// This is not correct - the raw image data is NOT UTF-8 string
			// TODO: Discuss
			//str = new String(data, "UTF-8");
		}
		finally
		{
			if (inputStream != null)
				inputStream.close();
		}
		
		return str;
	}

}
