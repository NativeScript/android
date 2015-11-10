package com.ig;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class GetInterfaceNames {

	private static String currentDir;
	
	public static void main(String[] args) throws IOException, ClassNotFoundException {
		currentDir = System.getProperty("user.dir");
		String outputFileName = "interfaces-names.txt";

		// System.out.println("inside: " + args);
		if (args != null) {
			if(args.length < 1) {
				throw new IllegalArgumentException("There are no parameters passed!");
			}
		}

		PrintWriter out = ensureOutputFile(outputFileName);
		
		for(String pathToJar : args) {
			// System.out.println("\t+jar: " + pathToJar);
			if(pathToJar.endsWith(".jar")) {
				generateInterfaceNames(pathToJar, out);
			}
		}

		out.close();
	}

	private static void generateInterfaceNames(String pathToJar, PrintWriter out) throws IOException, ClassNotFoundException {

		if(pathToJar == null) {
			return;
		}
		
		JarFile jarFile = new JarFile(pathToJar);
		Enumeration<?> currentJarFile = jarFile.entries();

		URLClassLoader cl = getClassLoader(pathToJar); 

		while (currentJarFile.hasMoreElements()) {
			JarEntry jarEntry = (JarEntry) currentJarFile.nextElement();

			if (jarEntry.isDirectory() || !jarEntry.getName().endsWith(".class")) {
				continue;
			}

			String className = jarEntry.getName().substring(0, jarEntry.getName().length() - 6);// -6 because of .class
			className = className.replace('/', '.');
			
			Class<?> c = null;
			try {
				c = cl.loadClass(className);	
			}
			catch (NoClassDefFoundError e) {
			}
			
			if (c != null && c.isInterface() == true) {
				String res = c.getName().replace('$', '.');
				// System.out.println(res);
				out.println(res);
			}
		}
		jarFile.close();
	}

	private static PrintWriter ensureOutputFile(String outputFileName) throws IOException {
		File checkFile = new File(currentDir, outputFileName);
		if(checkFile.exists()) {
			checkFile.delete();
		}
		else {
			checkFile.getParentFile().mkdirs();
			checkFile.createNewFile();
		}
		
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(checkFile.getAbsolutePath(), true)));
		return out;
	}

	private static URLClassLoader getClassLoader(String pathToJar) throws MalformedURLException {
		URL[] urls = { new URL("jar:file:" + pathToJar + "!/") };
		URLClassLoader cl = URLClassLoader.newInstance(urls);
		return cl;
	}
}
