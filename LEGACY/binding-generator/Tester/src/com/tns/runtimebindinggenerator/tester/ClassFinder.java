package com.tns.runtimebindinggenerator.tester;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public final class ClassFinder
{

	private final static char DOT = '.';
	private final static char SLASH = '/';
	private final static String CLASS_SUFFIX = ".class";
	private final static String BAD_PACKAGE_ERROR = "Unable to get resources from path '%s'. Are you sure the given '%s' package exists?";

	public final static List<Class<?>> find(final String scannedPackage)
	{
		final ClassLoader classLoader = Thread.currentThread()
			.getContextClassLoader();
		final String scannedPath = scannedPackage.replace(DOT, SLASH);
		final Enumeration<URL> resources;
		try
		{
			resources = classLoader.getResources(scannedPath);
		}
		catch (IOException e)
		{
			throw new IllegalArgumentException(String.format(BAD_PACKAGE_ERROR, scannedPath, scannedPackage), e);
		}
		final List<Class<?>> classes = new LinkedList<Class<?>>();
		while (resources.hasMoreElements())
		{
			final File file = new File(resources.nextElement()
				.getFile());
			classes.addAll(find(file, scannedPackage));
		}
		return classes;
	}

	private final static List<Class<?>> find(final File file, final String scannedPackage)
	{
		final List<Class<?>> classes = new LinkedList<Class<?>>();
		final String resource = scannedPackage + DOT + file.getName();
		if (file.isDirectory())
		{
			for (File nestedFile : file.listFiles())
			{
				classes.addAll(find(nestedFile, scannedPackage));
			}
		}
		else if (resource.endsWith(CLASS_SUFFIX))
		{
			final int beginIndex = 0;
			final int endIndex = resource.length() - CLASS_SUFFIX.length();
			final String className = resource.substring(beginIndex, endIndex);
			try
			{
				classes.add(Class.forName(className));
			}
			catch (ClassNotFoundException ignore)
			{
			}
		}
		return classes;
	}

	public static ArrayList<Class<?>> getClassesForPackage(Package pkg)
	{
		String pkgname = pkg.getName();
		ArrayList<Class<?>> classes = new ArrayList<Class<?>>();
		// Get a File object for the package
		File directory = null;
		String fullPath;
		String relPath = pkgname.replace('.', '/');
		System.out.println("ClassDiscovery: Package: " + pkgname + " becomes Path:" + relPath);
		URL resource = ClassLoader.getSystemClassLoader()
			.getResource(relPath);
		System.out.println("ClassDiscovery: Resource = " + resource);
		if (resource == null)
		{
			throw new RuntimeException("No resource for " + relPath);
		}
		fullPath = resource.getFile();
		System.out.println("ClassDiscovery: FullPath = " + resource);

		try
		{
			directory = new File(resource.toURI());
		}
		catch (URISyntaxException e)
		{
			throw new RuntimeException(pkgname + " (" + resource + ") does not appear to be a valid URL / URI.  Strange, since we got it from the system...", e);
		}
		catch (IllegalArgumentException e)
		{
			directory = null;
		}
		System.out.println("ClassDiscovery: Directory = " + directory);

		if (directory != null && directory.exists())
		{
			// Get the list of the files contained in the package
			String[] files = directory.list();
			for (int i = 0; i < files.length; i++)
			{
				// we are only interested in .class files
				if (files[i].endsWith(".class"))
				{
					// removes the .class extension
					String className = pkgname + '.' + files[i].substring(0, files[i].length() - 6);
					System.out.println("ClassDiscovery: className = " + className);
					try
					{
						classes.add(Class.forName(className));
					}
					catch (ClassNotFoundException e)
					{
						throw new RuntimeException("ClassNotFoundException loading " + className);
					}
				}
			}
		}
		else
		{
			try
			{
				String jarPath = fullPath.replaceFirst("[.]jar[!].*", ".jar")
					.replaceFirst("file:", "");
				JarFile jarFile = new JarFile(jarPath);
				Enumeration<JarEntry> entries = jarFile.entries();
				while (entries.hasMoreElements())
				{
					JarEntry entry = entries.nextElement();
					String entryName = entry.getName();
					if (entryName.startsWith(relPath) && entryName.length() > (relPath.length() + "/".length()))
					{
						System.out.println("ClassDiscovery: JarEntry: " + entryName);
						String className = entryName.replace('/', '.')
							.replace('\\', '.')
							.replace(".class", "");
						System.out.println("ClassDiscovery: className = " + className);
						try
						{
							classes.add(Class.forName(className));
						}
						catch (ClassNotFoundException e)
						{
							throw new RuntimeException("ClassNotFoundException loading " + className);
						}
					}
				}
			}
			catch (IOException e)
			{
				throw new RuntimeException(pkgname + " (" + directory + ") does not appear to be a valid package", e);
			}
		}
		return classes;
	}

}