package com.extend.generator;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;

public class NSClassLoader extends URLClassLoader
{
	private static NSClassLoader instance;
	private ArrayList<ArrayList<String>> classNames;
	private ArrayList<String> jarFiles;

	public NSClassLoader(URL[] urls, ClassLoader parent)
	{
		super(urls, parent);

		this.classNames = new ArrayList<ArrayList<String>>();
		this.jarFiles = new ArrayList<String>();
	}

	public ArrayList<ArrayList<String>> getClassNames()
	{
		return this.classNames;
	}
	
	public String[] getJarNames() {
		String[] jarNames = new String[this.jarFiles.size()];
		
		for(int i = 0; i < this.jarFiles.size(); i++) {
			
			String jarName = this.jarFiles.get(i);
			// String currentName = jarName.substring(jarName.lastIndexOf('\\') + 1);
			jarNames[i] = jarName;
		}
		
		return jarNames;
	}

	public void loadDir(String path)
	{
		File dir = new File(path);
		this.traverseDir(dir);
		this.populateClassNames();
	}

	public void loadJars(String[] args) {
		for(int i = 2; i < args.length; i ++) {
			String currentJar = args[i];
			onFile(new File(currentJar));
		}
	}

	private void populateClassNames()
	{
		JarInputStream input = null;

		for (String jar : this.jarFiles)
		{
			ArrayList<String> jarClassNames = new ArrayList<String>();
			try
			{
				input = new JarInputStream(new FileInputStream(jar));
				JarEntry entry = input.getNextJarEntry();

				while (entry != null)
				{
					String name = entry.getName();
					if (this.isValidClass(jar, name))
					{
						name = name.substring(0, name.length() - 6)
							.replace('/', '.');
						jarClassNames.add(name);
					}
					entry = input.getNextJarEntry();
				}

			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
				continue;
			}
			finally
			{
				if (input != null)
				{
					try
					{
						input.close();
					}
					catch (IOException e)
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}

			Collections.sort(jarClassNames);
			this.classNames.add(jarClassNames);
		}
	}

	private boolean isValidClass(String jar, String className)
	{
		if (!className.endsWith(".class"))
		{
			return false;
		}

		if (jar.equals("nativescript.jar") && className.startsWith("com.tns.com.tns.tests."))
		{
			return false;
		}

		return true;
	}

	private void traverseDir(File file)
	{
		if (!file.exists() || !file.isDirectory())
		{
			return;
		}

		File[] files = file.listFiles();
		for (File childFile : files)
		{
			if (childFile.isDirectory())
			{
				traverseDir(childFile);
			}
			else
			{
				onFile(childFile);
			}
		}
	}

	private void onFile(File file)
	{
		if (!isJarFile(file))
		{
			return;
		}

		try
		{
			URL url = file.toURI()
				.toURL();
			this.addURL(url);
			this.jarFiles.add(file.getPath());
		}
		catch (MalformedURLException e)
		{
			e.printStackTrace();
		}
	}

	private boolean isJarFile(File file)
	{
		return file.getPath()
			.endsWith(".jar");
	}

	public static NSClassLoader getInstance()
	{
		if (instance == null)
		{
			URL[] urls;
			ClassLoader systemLoader = ClassLoader.getSystemClassLoader();
			if (systemLoader instanceof URLClassLoader)
			{
				urls = ((URLClassLoader) systemLoader).getURLs();
			}
			else
			{
				urls = new URL[0];
			}

			instance = new NSClassLoader(urls, systemLoader);
		}

		return instance;
	}
}
