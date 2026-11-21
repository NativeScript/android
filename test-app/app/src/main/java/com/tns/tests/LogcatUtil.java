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

    // Lines this process logged under the JS tag at warning level or above that contain
    // both strings. Leaves the log in place, so earlier lines of this process count too.
    public static int countJsWarnings(String needle, String otherNeedle) throws Exception {
        // -v brief prefixes each line with "W/JS      ( 1234): "; --pid needs API 24.
        String pidTag = "(" + android.os.Process.myPid() + "):";
        Process process = Runtime.getRuntime().exec(new String[]{"logcat", "-d", "-v", "brief", "-s", "JS:W"});
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
        int count = 0;
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            int prefixEnd = line.indexOf("):");
            if (prefixEnd < 0 || !line.substring(0, prefixEnd + 2).replace(" ", "").endsWith(pidTag)) {
                continue;
            }
            if (line.contains(needle) && line.contains(otherNeedle)) {
                count++;
            }
        }
        process.waitFor();
        return count;
    }
}
