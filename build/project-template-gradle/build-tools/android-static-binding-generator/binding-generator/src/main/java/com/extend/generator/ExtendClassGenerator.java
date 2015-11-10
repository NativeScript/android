package com.extend.generator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

public class ExtendClassGenerator {

	public static String jarFilesDir;
	public static String outFilesDir;
	
	public static void main(String[] args) throws Exception {
		
		String jarsDir = "./jars";
		String bindingsPath = "./bindings.txt";
		String outName = "./generated_bindings";
		
		if (args != null )
		{
			if(args.length > 0) {
				bindingsPath = args[0]; 		
			}
			if(args.length > 1) {
				outName = args[1];
			}

			//rest of the arguments are jar paths
		}

		new File(outName).mkdir(); //make shure there is an out folder

		String pathToBindingsFile = new File(bindingsPath).getCanonicalPath();
		outFilesDir = new File(outName).getCanonicalPath();
		
		HashMap<String, HashSet<String>> proxyNames = ReadProxyNames(pathToBindingsFile);

		JarLister.startGenerateBindings(args, proxyNames);
	}
	
private static HashMap<String, HashSet<String>> ReadProxyNames(String bindingsPath) throws IOException {
		
		HashMap<String, HashSet<String>> result = new HashMap<String, HashSet<String>>();
		 
		// Open the file
		FileInputStream fstream = new FileInputStream(bindingsPath);
		BufferedReader br = new BufferedReader(new InputStreamReader(fstream));

		String strLine;

		//Read File Line By Line
		while ((strLine = br.readLine()) != null)   {
			
			String[] lineArguments = strLine.split(" ");

			String key = lineArguments[0];
			

			if((key.indexOf("com.tns.tests") == -1)) {
			
				if(!result.containsKey(key)) {
					result.put(key, new HashSet<String>());
				}
				
				if(lineArguments.length > 1) {
					
					String[] methodArguments = lineArguments[1].split(",");
					
					for(String methodName : methodArguments) {
						result.get(key).add(methodName);
					}	
				}
			}
		}
		
		// for(String key : result.keySet()) {
		// 	System.out.println(key);
		// 	for(String mn : result.get(key)){
		// 		System.out.println("\t" + mn);
		// 	}
		// }

		//Close the input stream
		br.close();
		
		return result;
	}
}
