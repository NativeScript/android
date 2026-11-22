package com.tns.tests;

// Targets for testWrapperLifetimeFieldWrites.js. The Shadow classes are only for
// specs that change their prototypes.
public class CanaryFieldFixtures {
    public static class Base {
        public int f = 1;
        public final int fixed = 3;

        public int getF() {
            return f;
        }
    }

    public static class Mid extends Base {
    }

    public static class Leaf extends Mid {
    }

    public static class ShadowBase {
        public int f = 1;

        public int getF() {
            return f;
        }
    }

    public static class ShadowMid extends ShadowBase {
    }

    public static class ShadowLeaf extends ShadowMid {
    }
}
