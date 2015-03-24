package com.tns.internal;

public class DefaultEtractPolicy implements ExtractPolicy
{
	public boolean shouldExtract(android.content.Context context)
	{
		return true;
	}
	public boolean forceOverwrite()
	{
		return true;
	}
	public FileExtractor extractor()
	{
		return null;
	}
}
