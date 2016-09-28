package org.nativescript.staticbindinggenerator.test;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.net.URL;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by pkanev on 6/22/2016.
 */
public class Utils {

    private static String projectPath = new File(".").getAbsolutePath();
    private static String packageName = "./src/test/resources/org/nativescript/staticbindinggenerator/test/";

    static List<String> getDataRowsFromResource(String resourceName) throws IOException {
        String s = IOUtils.toString(Utils.class.getResource(resourceName), "UTF-8");
        List<String> lines = IOUtils.readLines(new StringReader(s));

        return lines;
    }

    static File getBindingsFile(String resourceName) throws IOException {
        URL url = Utils.class.getResource(resourceName);
        File resFile = new File(url.getFile());
        return resFile;
    }
}
