package com.tns;

import java.io.File;

public class StaticConfiguration {
    public final Logger logger;
    public final String appName;
    public final String nativeLibDir;
    public final File rootDir;
    public final File appDir;
    public final ClassLoader classLoader;
    public final File dexDir;
    public final String dexThumb;
    public final AppConfig appConfig;
    public final boolean isDebuggable;

    public StaticConfiguration(Logger logger,
                               String appName, String nativeLibDir, File rootDir, File appDir, ClassLoader classLoader,
                               File dexDir, String dexThumb, AppConfig appConfig, boolean isDebuggable) {
        this.logger = logger;
        this.appName = appName;
        this.nativeLibDir = nativeLibDir;
        this.rootDir = rootDir;
        this.appDir = appDir;
        this.classLoader = classLoader;
        this.dexDir = dexDir;
        this.dexThumb = dexThumb;
        this.appConfig = appConfig;
        this.isDebuggable = isDebuggable;
    }
}
