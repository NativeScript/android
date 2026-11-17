package com.tns.tests.interfaces.hiddendefaults;

// Java callers for testInterfaceDefaultMethodOverrides.js: a default inherited from a
// super-interface that classes outside this package cannot access.
public class VisibleDefaultsCaller {
    public static String hidden(VisibleDefaults instance) {
        try {
            return instance.hidden();
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    public static String visible(VisibleDefaults instance) {
        try {
            return instance.visible();
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    // Through the super-interface's Object narrowed(), which the sub-interface narrows.
    public static String narrowed(VisibleDefaults instance) {
        try {
            HiddenDefaults asHidden = instance;
            return java.util.Objects.toString(asHidden.narrowed());
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    public static boolean isDesugared() throws NoSuchMethodException {
        return !VisibleDefaults.class.getMethod("visible").isDefault();
    }
}
