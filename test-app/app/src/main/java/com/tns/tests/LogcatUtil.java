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

    // Lines this app logged under the JS tag at warning level or above that contain both
    // strings. Leaves the log in place, so earlier lines count too.
    public static int countJsWarnings(String needle, String otherNeedle) throws Exception {
        Process process = Runtime.getRuntime().exec(new String[]{"logcat", "-d", "-v", "raw", "-s", "JS:W"});
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
        int count = 0;
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            if (line.contains(needle) && line.contains(otherNeedle)) {
                count++;
            }
        }
        process.waitFor();
        return count;
    }
}
