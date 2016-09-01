package org.nativescript.staticbindinggenerator;

import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        try {
            if (args.length < 3) {
                throw new IllegalArgumentException("Expects at least three arguments");
            }
            String inputBindingFilename = args[0];
            String outputDir = args[1];
            String[] libs = Arrays.copyOfRange(args, 2, args.length);

            new Generator(outputDir, libs).writeBindings(inputBindingFilename);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
