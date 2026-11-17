package com.tns.tests;

import java.time.temporal.ChronoUnit;
import java.time.temporal.Temporal;

// java.time is API 26; specs touch this only on API 26+.
public class TemporalDefaultFixtures {
    // Temporal.minus(long, TemporalUnit) is a default that calls plus(-amount, unit).
    public static boolean minusDays(Temporal temporal, long amount) {
        return temporal.minus(amount, ChronoUnit.DAYS) == temporal;
    }
}
