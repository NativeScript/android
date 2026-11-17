package com.tns.tests.interfaces.hiddendefaults;

public interface VisibleDefaults extends HiddenDefaults {
    String name();

    default String visible() {
        return "visible java";
    }

    @Override
    default String narrowed() {
        return "visible narrowed";
    }
}
