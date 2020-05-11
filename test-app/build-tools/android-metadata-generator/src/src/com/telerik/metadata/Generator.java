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

public class Generator {

    private static final String VERBOSE_FLAG_NAME = "verbose";
    private static final String ANALYTICS_ARGUMENT_BEGINNING = "analyticsFilePath=";
    private static final String MDG_OUTPUT_DIR = "mdg-output-dir.txt";
    private static final String MDG_JAVA_DEPENDENCIES = "mdg-java-dependencies.txt";
    private static final String MDG_WHITELIST = "whitelist.mdg";
    private static final String MDG_BLACKLIST = "blacklist.mdg";

    /**
     * @param args
     */
    public static void main(String[] args) {
        enableFlaggedFeatures(args);
        UserPatternsCollection.INSTANCE.populateWhitelistEntriesFromFile(MDG_WHITELIST);
        UserPatternsCollection.INSTANCE.populateBlacklistEntriesFromFile(MDG_BLACKLIST);

        try {
            String metadataOutputDir;
            List<String> params;

            try {
                metadataOutputDir = getFileRows(MDG_OUTPUT_DIR).get(0);
            } catch (IOException e) {
                throw new InvalidParameterException(String.format("You need to pass a file containing a single line: the output dir for the metadata generator1 %s\n", e.getMessage()));
            }
            try {
                params = getFileRows(MDG_JAVA_DEPENDENCIES);
            } catch (IOException e) {
                throw new InvalidParameterException(String.format("You need to pass a file containing a list of jar/class paths, so metadata can be generated for them! %s\n", e.getMessage()));
            }

            TreeNode root = Builder.build(params);

            try(FileOutputStream ovs = new FileOutputStream(new File(metadataOutputDir, "treeValueStream.dat"));
                FileOutputStream ons = new FileOutputStream(new File(metadataOutputDir, "treeNodeStream.dat"));
                FileOutputStream oss = new FileOutputStream(new File(metadataOutputDir, "treeStringsStream.dat")))
            {
                FileStreamWriter outValueStream = new FileStreamWriter(ovs);
                FileStreamWriter outNodeStream = new FileStreamWriter(ons);
                FileStreamWriter outStringsStream = new FileStreamWriter(oss);
                try(Writer writer = new Writer(outNodeStream, outValueStream, outStringsStream))
                {
                    writer.writeTree(root);
                }
            }
        } catch (Throwable ex) {
            System.err.println(String.format("Error executing Metadata Generator: %s", ex.getMessage()));
            ex.printStackTrace(System.out);
            System.exit(1);
        }
    }

    private static void enableFlaggedFeatures(String[] args) {
        for (String arg : args) {
            if (arg.startsWith(ANALYTICS_ARGUMENT_BEGINNING)) {
                String filePath = arg.replace(ANALYTICS_ARGUMENT_BEGINNING, "");
                AnalyticsConfiguration.enableAnalytics(filePath);
            } else if (VERBOSE_FLAG_NAME.equals(arg)) {
                MetadataFilterConsoleLogger.INSTANCE.setEnabled(true);
            }
        }
    }

    public static List<String> getFileRows(String filename) throws IOException {
        List<String> rows = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(filename)))) {
            String line;
            while ((line = br.readLine()) != null) {
                if (!line.endsWith(".aar")) {
                    rows.add(line);
                }
            }
        }
        return rows;
    }
}
