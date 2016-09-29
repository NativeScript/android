package com.tns;

import java.io.File;

import android.app.Application;

public class StaticConfiguration
{
	public final Logger logger;
	public final Debugger debugger;
	public final String appName;
	public final File runtimeLibPath;
	public final File rootDir;
	public final File appDir;
	public final ClassLoader classLoader;
	public final File dexDir;
	public final String dexThumb;
	public final Object[] v8Config;
	
	public StaticConfiguration(Logger logger, Debugger debugger,
							   String appName, File runtimeLibPath, File rootDir, File appDir, ClassLoader classLoader,
							   File dexDir, String dexThumb, Object[] v8Config)
	{
		this.logger = logger;
		this.debugger = debugger;
		this.appName = appName;
		this.runtimeLibPath = runtimeLibPath;
		this.rootDir = rootDir;
		this.appDir = appDir;
		this.classLoader = classLoader;
		this.dexDir = dexDir;
		this.dexThumb = dexThumb;
		this.v8Config = v8Config;
	}
}
