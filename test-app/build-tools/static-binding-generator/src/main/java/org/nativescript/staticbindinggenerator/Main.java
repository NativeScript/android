package org.nativescript.staticbindinggenerator;

import java.io.IOException;
import java.util.Arrays;

public class Main {
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        if (args.length < 3) {
            throw new IllegalArgumentException("Expects at least three arguments");
        }
        String inputBindingFilename = args[0];
        String outputDir = args[1];
        String[] libs = Arrays.copyOfRange(args, 2, args.length);

        new Generator(outputDir, libs).writeBindings(inputBindingFilename);
    }
}
