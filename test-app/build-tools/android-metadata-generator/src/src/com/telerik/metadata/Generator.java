package com.telerik.metadata;

import com.telerik.metadata.analytics.AnalyticsConfiguration;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.List;

public class Generator {

    private static final String ANALYTICS_ARGUMENT_BEGINNING = "analyticsFilePath=";
    private static final String MDG_OUTPUT_DIR = "mdg-output-dir.txt";
    private static final String MDG_JAVA_DEPENDENCIES = "mdg-java-dependencies.txt";

    /**
     * @param args
     */
    public static void main(String[] args) {
        enableAnalyticsBasedOnArgs(args);

        try {
            String metadataOutputDir;
            List<String> params;

            try {
                metadataOutputDir = getFileRows(MDG_OUTPUT_DIR).get(0);
            } catch (Exception e) {
                throw new InvalidParameterException(String.format("You need to pass a file containing a single line: the output dir for the metadata generator1\n", e.getMessage()));
            }
            try {
                params = getFileRows(MDG_JAVA_DEPENDENCIES);
            } catch (Exception e) {
                throw new InvalidParameterException(String.format("You need to pass a file containing a list of jar/class paths, so metadata can be generated for them!\n", e.getMessage()));
            }

            TreeNode root = Builder.build(params);

            FileOutputStream ovs = new FileOutputStream(new File(metadataOutputDir, "treeValueStream.dat"));
            FileStreamWriter outValueStream = new FileStreamWriter(ovs);

            FileOutputStream ons = new FileOutputStream(new File(metadataOutputDir, "treeNodeStream.dat"));
            FileStreamWriter outNodeStream = new FileStreamWriter(ons);

            FileOutputStream oss = new FileOutputStream(new File(metadataOutputDir, "treeStringsStream.dat"));
            FileStreamWriter outStringsStream = new FileStreamWriter(oss);

            new Writer(outNodeStream, outValueStream, outStringsStream).writeTree(root);
        } catch (Throwable ex) {
            System.err.println(String.format("Error executing Metadata Generator: %s", ex.getMessage()));
            ex.printStackTrace(System.out);
            System.exit(1);
        }
    }

    private static void enableAnalyticsBasedOnArgs(String[] args){
        for (String arg : args) {
            if (arg.startsWith(ANALYTICS_ARGUMENT_BEGINNING)) {
                String filePath = arg.replace(ANALYTICS_ARGUMENT_BEGINNING, "");
                AnalyticsConfiguration.enableAnalytics(filePath);
            }
        }
    }

    public static List<String> getFileRows(String filename) throws IOException {
        List<String> rows = new ArrayList<String>();
        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream(filename)));
            String line;
            while ((line = br.readLine()) != null) {
                if (!line.endsWith(".aar")) {
                    rows.add(line);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                br.close();
            }
        }
        return rows;
    }
}
