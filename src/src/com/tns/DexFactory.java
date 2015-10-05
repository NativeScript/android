package com.tns;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.InvalidClassException;
import java.io.OutputStreamWriter;
import java.util.HashMap;
import java.util.HashSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import com.tns.bindings.ProxyGenerator;

import dalvik.system.DexClassLoader;
import dalvik.system.DexFile;

public class DexFactory
{
	private static final char CLASS_NAME_LOCATION_SEPARATOR = '_';

	private final Logger logger;
	private final File dexDir;
	private final File odexDir;
	private final String dexThumb;
	private final ClassLoader classLoader;

	private ProxyGenerator proxyGenerator;
	private HashMap<String, Class<?>> injectedDexClasses = new HashMap<String, Class<?>>();

	public DexFactory(Logger logger, ClassLoader classLoader, File dexBaseDir, String dexThumb)
	{
		this.logger = logger;
		this.classLoader = classLoader;
		this.dexDir = dexBaseDir;
		this.dexThumb = dexThumb;
		
		this.odexDir = new File(this.dexDir, "odex");
		this.proxyGenerator = new ProxyGenerator(this.dexDir.getAbsolutePath());
		ProxyGenerator.IsLogEnabled = logger.isEnabled();

		if (!dexDir.exists())
		{
			dexDir.mkdirs();
		}

		if (!odexDir.exists())
		{
			odexDir.mkdir();
		}

		this.updateDexThumbAndPurgeCache();
		this.proxyGenerator.setProxyThumb(this.dexThumb);
	}

	static long totalGenTime = 0;
	static long totalMultiDexTime = 0;
	static long totalLoadDexTime = 0;

	public Class<?> resolveClass(String name, String className, String[] methodOverrides) throws ClassNotFoundException, IOException
	{
		if (className.contains("NativeScriptActivity"))
		{
			// Do not extend NativeScriptActivity - it is already extended
			return NativeScriptActivity.class;
		}

		String fullClassName = className.replace("$", "_") + CLASS_NAME_LOCATION_SEPARATOR + name;

		// try to get pre-generated binding classes
		try {
			Class<?> pregeneratedClass = classLoader.loadClass(fullClassName.replace("-", "_"));
			
			return pregeneratedClass;
		}
		catch (Exception e)
		{
		}
		//

		Class<?> existingClass = this.injectedDexClasses.get(fullClassName);
		if (existingClass != null)
		{
			return existingClass;
		}

		String classToProxy = this.getClassToProxyName(className);
		String dexFilePath = classToProxy + CLASS_NAME_LOCATION_SEPARATOR + name;
		File dexFile = this.getDexFile(dexFilePath);
		
		//generate dex file
		if (dexFile == null)
		{
			long startGenTime = System.nanoTime();
			if (logger.isEnabled())
			{
				logger.write("generating proxy in place");
			}

			dexFilePath = this.generateDex(name, classToProxy, methodOverrides);
			dexFile = new File(dexFilePath);
			long stopGenTime = System.nanoTime();
			totalGenTime += stopGenTime - startGenTime;
			if (logger.isEnabled())
			{
				logger.write("Finished inplace gen took: " + (stopGenTime - startGenTime) / 1000000.0 + "ms");
				logger.write("TotalGenTime:  " + totalGenTime / 1000000.0 + "ms");
			}
		}

		//creates jar file from already generated dex file
		String jarFilePath = dexFile.getPath().replace(".dex", ".jar");
		File jarFile = new File(jarFilePath);

		if (!jarFile.exists())
		{
			FileOutputStream jarFileStream = new FileOutputStream(jarFile);
			ZipOutputStream out = new ZipOutputStream(jarFileStream);
			
		    out.putNextEntry(new ZipEntry("classes.dex"));
		    byte[] dexData = new byte[(int)dexFile.length()];
		    FileInputStream fi = new FileInputStream(dexFile);
		    fi.read(dexData, 0, dexData.length);
		    fi.close();
		    
		    out.write(dexData);
		    out.closeEntry();
		    out.close();
		}
		//
		
		Class<?> result = null;
		DexFile df = null;
		try
		{
			// use DexFile instead of DexClassLoader to allow class loading
			// within the default class loader
			// Note: According to the official documentation, DexFile should not
			// be directly used.
			// However, this is the only viable way to get our dynamic classes
			// loaded within the system class loader
			df = DexFile.loadDex(jarFilePath, new File(this.odexDir, fullClassName).getAbsolutePath(), 0);
			result = df.loadClass(fullClassName, classLoader);
		}
		catch (IOException e)
		{
			e.printStackTrace();
			// fall back to DexClassLoader
			DexClassLoader dexClassLoader = new DexClassLoader(jarFilePath, this.odexDir.getAbsolutePath(), null, classLoader);
			result = dexClassLoader.loadClass(fullClassName);
		}

		this.injectedDexClasses.put(fullClassName, result);

		return result;
	}

	public Class<?> findClass(String className) throws ClassNotFoundException
	{
		String canonicalName = className.replace('/', '.');
		if (logger.isEnabled())
		{
			logger.write(canonicalName);
		}

		Class<?> existingClass = this.injectedDexClasses.get(canonicalName);
		if (existingClass != null)
		{
			return existingClass;
		}
		return classLoader.loadClass(canonicalName);
	}

	public static String strJoin(String[] array, String separator)
	{
		if (array == null)
		{
			return "";
		}

		StringBuilder sbStr = new StringBuilder();
		for (int i = 0, il = array.length; i < il; i++)
		{
			if (i > 0)
			{
				sbStr.append(separator);
			}
			sbStr.append(array[i]);
		}
		return sbStr.toString();
	}

	private String getClassToProxyName(String className) throws InvalidClassException
	{
		String classToProxy = className;

		if (className.startsWith("com.tns.gen"))
		{
			classToProxy = className.substring(12);
		}

		if (classToProxy.startsWith("com.tns.gen"))
		{
			throw new InvalidClassException("Can't generate proxy of proxy");
		}

		return classToProxy;
	}

	private File getDexFile(String className) throws InvalidClassException
	{
		String classToProxyFile = className.replace("$", "_");

		if (this.dexThumb != null)
		{
			classToProxyFile += "-" + this.dexThumb;
		}

		String dexFilePath = dexDir + classToProxyFile + ".dex";
		File dexFile = new File(dexFilePath);
		if (dexFile.exists())
		{
			if (logger.isEnabled())
			{
				logger.write("Looking for proxy file: " + dexFilePath + " Result: proxy file Found. ClassName: " + className);
			}
			return dexFile;
		}

		if (logger.isEnabled())
		{
			logger.write("Looking for proxy file: " + dexFilePath + " Result: NOT Found. Proxy Gen needed. ClassName: " + className);
		}
		return null;
	}

	private String generateDex(String proxyName, String className, String[] methodOverrides) throws ClassNotFoundException, IOException
	{
		Class<?> classToProxy = Class.forName(className);

		HashSet<String> methodOverridesSet = null;
		if (methodOverrides != null)
		{
			methodOverridesSet = new HashSet<String>();
			for (int i = 0; i < methodOverrides.length; i++)
			{
				String methodOverride = methodOverrides[i];
				methodOverridesSet.add(methodOverride);
			}
		}

		return proxyGenerator.generateProxy(proxyName, classToProxy, methodOverridesSet);
	}

	private void updateDexThumbAndPurgeCache()
	{
		if (this.dexThumb == null)
		{
			throw new RuntimeException("Error generating proxy thumb 1");
		}

		String oldDexThumb = this.getCachedProxyThumb(this.dexDir);
		if (this.dexThumb.equals(oldDexThumb))
		{
			return;
		}

		if (oldDexThumb != null)
		{
			this.purgeDexesByThumb(oldDexThumb, this.dexDir);
			this.purgeDexesByThumb(oldDexThumb, this.odexDir);
		}
		else
		{
			// purge all dex files if no thumb file is found. This is crucial for CLI livesync
			purgeAllProxies();
		}

		this.saveNewDexThumb(this.dexThumb, this.dexDir);
	}

	public void purgeAllProxies()
	{
		this.purgeDexesByThumb(null, this.dexDir);
		this.purgeDexesByThumb(null, this.odexDir);
	}

	private void saveNewDexThumb(String newDexThumb, File dexDir)
	{
		File cachedThumbFile = new File(dexDir, "proxyThumb");
		try
		{
			FileOutputStream out = new FileOutputStream(cachedThumbFile, false);
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
			try
			{
				writer.write(newDexThumb);
				writer.newLine();
				writer.flush();
			}
			finally
			{
				writer.close();
				out.close();
			}
		}
		catch (FileNotFoundException e)
		{
			logger.write("Error while writting current proxy thumb");
			e.printStackTrace();
		}
		catch (IOException e)
		{
			logger.write("Error while writting current proxy thumb");
			e.printStackTrace();
		}
	}

	private void purgeDexesByThumb(String cachedDexThumb, File pathToPurge)
	{
		if (!pathToPurge.exists())
		{
			return;
		}

		if (!pathToPurge.isDirectory())
		{
			logger.write("Purge proxies path not a directory. Path: " + pathToPurge);
			throw new RuntimeException("Purge path not a directory");
		}

		String[] children = pathToPurge.list();

		for (int i = 0; i < children.length; i++)
		{
			String filename = children[i];
			File purgeCandidate = new File(pathToPurge, filename);
			if (purgeCandidate.isDirectory())
			{
				this.purgeDexesByThumb(cachedDexThumb, purgeCandidate);
			}
			else
			{
				if (cachedDexThumb != null && !filename.contains(cachedDexThumb))
				{
					continue;
				}

				if (!purgeCandidate.delete())
				{
					logger.write("Error purging cached proxy file: " + purgeCandidate.getAbsolutePath());
				}
			}
		}
	}

	private String getCachedProxyThumb(File proxyDir)
	{
		try
		{
			File cachedThumbFile = new File(proxyDir, "proxyThumb");
			if (cachedThumbFile.exists())
			{
				FileInputStream in = new FileInputStream(cachedThumbFile);
				BufferedReader reader = new BufferedReader(new InputStreamReader(in));
				String cachedThumb = reader.readLine();
				reader.close();
				in.close();
				return cachedThumb;
			}
		}
		catch (FileNotFoundException e)
		{
			logger.write("Error while getting current proxy thumb");
			e.printStackTrace();
		}
		catch (IOException e)
		{
			logger.write("Error while getting current proxy thumb");
			e.printStackTrace();
		}

		return null;
	}
}
