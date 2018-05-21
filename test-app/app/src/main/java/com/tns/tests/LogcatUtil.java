package com.tns.tests;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public final class LogcatUtil {
    public static boolean logcatContainsString(String str) throws Exception {
        Runtime.getRuntime().exec("logcat -c").waitFor();

        Process process = Runtime.getRuntime().exec("logcat -d");
        InputStreamReader inputStreamReader = new InputStreamReader(process.getInputStream());
        BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

        String line;
        StringBuilder sb = new StringBuilder();
        while ((line = bufferedReader.readLine()) != null) {
            sb.append(line);
        }

        String output = sb.toString();

        return output.contains(str);
    }
}
