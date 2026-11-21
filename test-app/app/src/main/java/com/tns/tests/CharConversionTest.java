package com.tns.tests;

public class CharConversionTest {
    public static interface Logger  {
        public void print(Object value);
    }

    public static interface CharEcho {
        char echo(char value);
    }

    public static void printWithChar(Logger logger, char value) {
        assert logger != null;
        logger.print(value);
    }

    public char charField;
    public static char staticCharField;
    public boolean booleanField;
    public static boolean staticBooleanField;

    public static int codeUnitOf(char value) {
        return value;
    }

    public static char echo(char value) {
        return value;
    }

    public static int[] codeUnitsOf(char[] values) {
        int[] units = new int[values.length];
        for (int i = 0; i < values.length; i++) {
            units[i] = values[i];
        }
        return units;
    }

    public static char[] charsOf(String value) {
        return value.toCharArray();
    }

    public int charFieldCodeUnit() {
        return charField;
    }

    public static int staticCharFieldCodeUnit() {
        return staticCharField;
    }

    public static int codeUnitFromCallback(CharEcho callback, char value) {
        return callback.echo(value);
    }
}
