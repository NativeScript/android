package com.tns;

import java.io.File;

public class StaticConfiguration
{
	public final Debugger debugger;
	public final String appName;
	public final String nativeLibDir;
	public final File rootDir;
	public final File appDir;
	public final ClassLoader classLoader;
	public final File dexDir;
	public final String dexThumb;
	public final AppConfig appConfig;

	public StaticConfiguration(Debugger debugger,
							   String appName, String nativeLibDir, File rootDir, File appDir, ClassLoader classLoader,
							   File dexDir, String dexThumb, AppConfig appConfig) {
		this.debugger = debugger;
		this.appName = appName;
		this.nativeLibDir = nativeLibDir;
		this.rootDir = rootDir;
		this.appDir = appDir;
		this.classLoader = classLoader;
		this.dexDir = dexDir;
		this.dexThumb = dexThumb;
		this.appConfig = appConfig;
	}
}
