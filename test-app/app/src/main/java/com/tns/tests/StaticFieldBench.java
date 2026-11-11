package com.tns.tests;

/** A class with few static members, for static field benchmarks; JniBench has too many. */
public class StaticFieldBench {
    public static int counter = 1;

    public static final int CONSTANT = 7;
}
