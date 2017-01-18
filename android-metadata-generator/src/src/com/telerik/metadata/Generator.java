package com.telerik.metadata;

import java.io.File;
import java.io.FileOutputStream;
import java.security.InvalidParameterException;

public class Generator {

    /**
     * @param args
     */
    public static void main(String[] args) throws Exception {
        String outName = "bin";
        String[] params = null;

        if (args != null && args.length > 0) {
            outName = args[0];
            File out = new File(outName);
            if (!out.exists()) {
                out.mkdir();
                System.out
                .println(String
                         .format("We didn't find the folder you specified ( %s ), so it's going to be created!",
                                 out.getAbsolutePath()));
            }

        } else {
            throw new InvalidParameterException(
                "You need to pass an output directory!");
        }

        if (args != null && args.length > 1) {
            params = new String[args.length - 1];
            for (int i = 1; i < args.length; i++) {
                params[i - 1] = args[i];
            }
        }

        if (params == null) {
            throw new InvalidParameterException(
                "You need to pass a list of jar paths, so metadata can be generated for them!");
        }

        TreeNode root = Builder.build(params);

        FileOutputStream ovs = new FileOutputStream(new File(outName,
                "treeValueStream.dat"));
        FileStreamWriter outValueStream = new FileStreamWriter(ovs);

        FileOutputStream ons = new FileOutputStream(new File(outName,
                "treeNodeStream.dat"));
        FileStreamWriter outNodeStream = new FileStreamWriter(ons);

        FileOutputStream oss = new FileOutputStream(new File(outName,
                "treeStringsStream.dat"));
        FileStreamWriter outStringsStream = new FileStreamWriter(oss);

        new Writer(outNodeStream, outValueStream, outStringsStream)
        .writeTree(root);
    }
}
