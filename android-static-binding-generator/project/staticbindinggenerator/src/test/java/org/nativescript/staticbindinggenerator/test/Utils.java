package org.nativescript.staticbindinggenerator.test;

import org.apache.commons.io.IOUtils;

import java.io.IOException;
import java.io.StringReader;
import java.util.List;

/**
 * Created by pkanev on 6/22/2016.
 */
public class Utils {
    static List<String> getDataRowsFromResource(String resourceName) throws IOException {
        String s = IOUtils.toString(Utils.class.getResource(resourceName), "UTF-8");
        List<String> lines = IOUtils.readLines(new StringReader(s));

        return lines;
    }
}
