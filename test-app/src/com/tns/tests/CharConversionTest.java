package com.tns.tests;

public class CharConversionTest {
    public static interface Logger  {
        public void print(Object value);
    }

    public static void printWithChar(Logger logger, char value) {
        assert logger != null;
        logger.print(value);
    }
}
