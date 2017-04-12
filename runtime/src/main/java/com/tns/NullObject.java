package com.tns;

public final class NullObject {
    public NullObject(Class<?> clazz) {
        this.clazz = clazz;
    }

    private final Class<?> clazz;

    public final Class<?> getNullObjectClass() {
        return this.clazz;
    }
}
