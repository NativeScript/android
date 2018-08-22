package org.nativescript.staticbindinggenerator;

import java.util.function.Consumer;

// https://stackoverflow.com/a/27252163/613113
@FunctionalInterface
public interface ThrowingConsumer<T> extends Consumer<T> {
    @Override
    default void accept(final T elem) {
        try {
            acceptThrows(elem);
        } catch (final Exception e) {
            throw new RuntimeException(e);
        }
    }
    void acceptThrows(T elem) throws Exception;

}
