package com.tns.runtimebindinggenerator.tester;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;

import android.util.Log;

import dalvik.system.DexFile;
import dalvik.system.PathClassLoader;

public class ClasspathScanner
{
	public static ArrayList<Class<?>> run(String packageCodePath) throws IOException
	{

		ArrayList<Class<?>> classes = new ArrayList<Class<?>>();

		// TODO (2): check here - in theory, the class loader is not
		// required to be a PathClassLoader
		PathClassLoader classLoader = (PathClassLoader) Thread.currentThread()
			.getContextClassLoader();

		DexFile dex = new DexFile(packageCodePath);

		// DexFile[] dexs = (DexFile[]) dexField.get(classLoader);
		// df.e

		Enumeration<String> entries = dex.entries();
		while (entries.hasMoreElements())
		{
			try
			{
				// (3) Each entry is a class name, like "foo.bar.MyClass"
				String entry = entries.nextElement();
				Log.d("Binding.Gen.Tests", "Entry: " + entry);

				// (4) Load the class
				Class<?> entryClass = dex.loadClass(entry, classLoader);
				if (entryClass != null)
				{
					classes.add(entryClass);

					// Foo annotation = entryClass.getAnnotation(Foo.class);
					// if (annotation != null)
					// {
					// Log.d(TAG, entry + ": " + annotation.value());
					// }
				}
			}
			catch (IllegalAccessError e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		return classes;
	}
}