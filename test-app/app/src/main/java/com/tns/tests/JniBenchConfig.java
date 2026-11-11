package com.tns.tests;

/**
 * Benchmark settings that only the harness reads, kept off JniBench: every static added
 * there is another property on the JS constructor its static benchmarks are called through.
 */
public class JniBenchConfig {
    /** Timed rounds per benchmark; 0 keeps the default. */
    public static int getRounds() {
        try {
            return Math.max(0, Integer.parseInt(JniBench.getProp("debug.ns.jnibench.rounds")));
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    /** Comma-separated admission spin budgets to alternate between, round by round, in contention rows. */
    public static String getVariants() {
        return JniBench.getProp("debug.ns.jnibench.variants");
    }
}
