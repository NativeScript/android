package com.tns.tests;

// Fixtures for testAppModuleBindings.js: app-module types the static binding generator
// cannot write a binding javac accepts for, or must not prebuild a shared class for.
public class AppModuleShapes {
    static class Hidden {
    }

    // Only the static binding generator's class for a call site of this interface, which
    // no spec runs, tells the specs that it was shown the app's own classes.
    public interface StepProbe {
        String describe();
    }

    public abstract static class PackagePrivateAbstract {
        abstract String hidden();

        public abstract String describe();
    }

    public abstract static class HiddenReturn {
        protected abstract Hidden make();

        public String describe() {
            return "java hidden return";
        }
    }

    public static class HiddenConstructorArgument {
        public HiddenConstructorArgument() {
        }

        public HiddenConstructorArgument(Hidden hidden) {
        }

        public String describe() {
            return "java hidden constructor argument";
        }
    }

    public abstract static class HiddenParameter {
        public abstract String take(Hidden hidden);

        public static String callTake(HiddenParameter target) {
            return target.take(new Hidden());
        }
    }

    public class Inner {
        public String describe() {
            return "java inner";
        }
    }

    public abstract static class SelfTyped<T extends SelfTyped<T>> {
        public abstract T self();

        public abstract String describe();
    }

    public interface ComparableSource<T extends Comparable<T>> {
        T next();

        String describe();
    }

    public interface TwoDefaults {
        String name();

        default String first() {
            return "java first";
        }

        default String second() {
            return "java second";
        }
    }

    // Java callers, which report an exception a call throws by its class name.
    public static String first(TwoDefaults target) {
        try {
            return target.first();
        } catch (Throwable e) {
            return e.getClass().getName();
        }
    }

    public static String second(TwoDefaults target) {
        try {
            return target.second();
        } catch (Throwable e) {
            return e.getClass().getName();
        }
    }
}
