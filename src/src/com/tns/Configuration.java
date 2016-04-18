package com.tns;

import java.io.File;

import android.app.Application;

public class Configuration
{
	public final Application application;
	public final ThreadScheduler threadScheduler;
	public final Logger logger;
	public final String appName;
	public final File runtimeLibPath;
	public final File rootDir;
	public final File appDir;
	public final ClassLoader classLoader;
	public final File dexDir;
	public final String dexThumb;
	
	public Configuration(Application application, ThreadScheduler threadScheduler, Logger logger, 
			String appName, File runtimeLibPath, File rootDir, File appDir, ClassLoader classLoader,
			File dexDir, String dexThumb)
	{
		this.application = application;
		this.threadScheduler = threadScheduler;
		this.logger = logger;
		this.appName = appName;
		this.runtimeLibPath = runtimeLibPath;
		this.rootDir = rootDir;
		this.appDir = appDir;
		this.classLoader = classLoader;
		this.dexDir = dexDir;
		this.dexThumb = dexThumb;
	}
}
