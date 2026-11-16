package com.tns.tests;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class IteratorFixtures {
    private static final String[] ITEMS = {"a", "b", "c"};

    public static Iterator<String> packagePrivateIterator() {
        return new PackagePrivateIterator(ITEMS);
    }

    public static Iterator<String> privateNestedIterator() {
        return new PrivateNestedIterator(ITEMS);
    }

    public static Iterator<String> anonymousIterator() {
        return new Iterator<String>() {
            private int index;

            @Override
            public boolean hasNext() {
                return index < ITEMS.length;
            }

            @Override
            public String next() {
                return ITEMS[index++];
            }
        };
    }

    // hasNext() lives on a base class that does not implement Iterator, the shape
    // java.util.HashMap's iterators have.
    public static Iterator<String> inheritedHasNextIterator() {
        return new KeyCursor(ITEMS);
    }

    public static Iterable<String> lambdaIterable() {
        return () -> new PackagePrivateIterator(ITEMS);
    }

    public static Object[] privateNestedIteratorArray() {
        return new PrivateNestedIterator[] {
            new PrivateNestedIterator(ITEMS), new PrivateNestedIterator(ITEMS)
        };
    }

    private static final class PrivateNestedIterator implements Iterator<String> {
        private final String[] items;
        private int index;

        PrivateNestedIterator(String[] items) {
            this.items = items;
        }

        @Override
        public boolean hasNext() {
            return index < items.length;
        }

        @Override
        public String next() {
            if (index >= items.length) {
                throw new NoSuchElementException();
            }
            return items[index++];
        }
    }

    private abstract static class Cursor {
        final String[] items;
        int index;

        Cursor(String[] items) {
            this.items = items;
        }

        public final boolean hasNext() {
            return index < items.length;
        }
    }

    private static final class KeyCursor extends Cursor implements Iterator<String> {
        KeyCursor(String[] items) {
            super(items);
        }

        @Override
        public String next() {
            return items[index++];
        }
    }
}

class PackagePrivateIterator implements Iterator<String> {
    private final String[] items;
    private int index;

    PackagePrivateIterator(String[] items) {
        this.items = items;
    }

    @Override
    public boolean hasNext() {
        return index < items.length;
    }

    @Override
    public String next() {
        return items[index++];
    }
}
