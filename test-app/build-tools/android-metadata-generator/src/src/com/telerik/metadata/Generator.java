package com.telerik.metadata;

import com.telerik.metadata.analytics.AnalyticsConfiguration;
import com.telerik.metadata.security.filtering.input.user.UserPatternsCollection;
import com.telerik.metadata.security.logging.console.MetadataFilterConsoleLogger;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.List;

import static java.lang.System.exit;

public class Generator {

    private static final String VERBOSE_FLAG_NAME = "verbose";
    private static final String SKIP_FLAG_NAME = "skip";
    private static final String ANALYTICS_ARGUMENT_BEGINNING = "analyticsFilePath=";
    private static final String MDG_OUTPUT_DIR = "mdg-output-dir.txt";
    private static final String MDG_JAVA_DEPENDENCIES = "mdg-java-dependencies.txt";
    private static final String MDG_WHITELIST = "whitelist.mdg";
    private static final String MDG_BLACKLIST = "blacklist.mdg";
    private static final String METADATA_JAVA_OUT = "mdg-java-out.txt";

    /**
     * @param args arguments
     */
    public static void main(String[] args) {
        enableFlaggedFeatures(args);
        UserPatternsCollection.INSTANCE.populateWhitelistEntriesFromFile(MDG_WHITELIST);
        UserPatternsCollection.INSTANCE.populateBlacklistEntriesFromFile(MDG_BLACKLIST);

        try {
            String metadataOutputDir;
            List<String> params;
            List<String> classes;

            try {
                metadataOutputDir = getFileRows(MDG_OUTPUT_DIR).get(0);
            } catch (Exception e) {
                throw new InvalidParameterException(String.format("You need to pass a file containing a single line: the output dir for the metadata generator %s\n", e.getMessage()));
            }

            try {
                classes = getFileRows(METADATA_JAVA_OUT);
            } catch (Exception e) {
                throw new InvalidParameterException(String.format("You need to pass a file containing a list of local classes  %s\n", e.getMessage()));
            }

            try {
                params = getFileRows(MDG_JAVA_DEPENDENCIES);
            } catch (Exception e) {
                throw new InvalidParameterException(String.format("You need to pass a file containing a list of jar/class paths, so metadata can be generated for them! %s\n", e.getMessage()));
            }

            TreeNode root = Builder.build(params, classes);

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
            exit(1);
        }
    }

    private static void enableFlaggedFeatures(String[] args) {
        for (String arg : args) {
            if (arg.startsWith(ANALYTICS_ARGUMENT_BEGINNING)) {
                String filePath = arg.replace(ANALYTICS_ARGUMENT_BEGINNING, "");
                AnalyticsConfiguration.enableAnalytics(filePath);
            } else if (VERBOSE_FLAG_NAME.equals(arg)) {
                MetadataFilterConsoleLogger.INSTANCE.setEnabled(true);
            } else if (SKIP_FLAG_NAME.equals(arg)) {
                System.out.println("Skipping metadata generation: skip flag used.");
                exit(0);
            }
        }
    }

    public static List<String> getFileRows(String filename) throws IOException {
        List<String> rows = new ArrayList<>();
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
