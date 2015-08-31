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

		String jarsDir = currentDir + "\\jars";
		String outputFileName = "interfaces-names.txt";

		if (args != null) {
			
			if(args.length > 0) {
				jarsDir = args[0];
			}
			if(args.length > 1) {
				outputFileName = args[1];	
			}
		}

		if(!new File(jarsDir).exists()) {
			System.out.println("Please provide a valid dir with jars to parse! Default dir is '.\\jars'");
			System.exit(1);
		}

		String[] pathsToJars = getAllFilesPaths(jarsDir);

		PrintWriter out = ensureOutputFile(outputFileName);
		
		for(String pathToJar : pathsToJars) {
			generateInterfaceNames(pathToJar, out);
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

	private static String[] getAllFilesPaths(String path) {
		File folder = new File(path);
		File[] listOfFiles = folder.listFiles();
		String[] paths = new String[listOfFiles.length];
		
		for (int i = 0; i < listOfFiles.length; i++) {

			if (listOfFiles[i].isFile()) {

				if(listOfFiles[i].getName().indexOf(".jar") != -1) {
					paths[i] = listOfFiles[i].getAbsolutePath();	
				}
			}
		}

		return paths;
	}

}
