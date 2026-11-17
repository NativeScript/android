package com.tns;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.Arrays;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.concurrent.ConcurrentHashMap;

import org.junit.Test;

public class RuntimeTypeMetadataTest {

    interface PackagePrivateIterator extends Iterator<String> {
    }

    abstract static class ImplementsPackagePrivate implements PackagePrivateIterator {
    }

    abstract static class ImplementsBothListedSuperFirst implements Iterator<String>, ListIterator<String> {
    }

    private static Class<?> concurrentKeyIteratorClass() {
        return new ConcurrentHashMap<String, String>().keySet().iterator().getClass();
    }

    @Test
    public void listsTheDirectPublicInterfacesInDeclarationOrder() {
        assertEquals(Arrays.<Class<?>>asList(Iterator.class, Enumeration.class),
                Runtime.publicInterfacesOf(concurrentKeyIteratorClass()));
    }

    @Test
    public void listsASubInterfaceBeforeItsSuperInterface() {
        assertEquals(Arrays.<Class<?>>asList(ListIterator.class, Iterator.class),
                Runtime.publicInterfacesOf(ImplementsBothListedSuperFirst.class));
    }

    @Test
    public void walksThroughANonPublicInterfaceWithoutListingIt() {
        assertEquals(Arrays.<Class<?>>asList(Iterator.class),
                Runtime.publicInterfacesOf(ImplementsPackagePrivate.class));
    }

    @Test
    public void listsNothingForAClassThatImplementsNoInterface() {
        assertEquals(Arrays.<Class<?>>asList(), Runtime.publicInterfacesOf(Object.class));
    }

    @Test
    public void describesTheInterfacesOfABootClass() {
        String metadata = Runtime.getTypeMetadata(concurrentKeyIteratorClass());

        assertTrue(metadata, metadata.contains("\nN java/util/Iterator\nN java/util/Enumeration\n"));
    }

    @Test
    public void doesNotDescribeTheInterfacesOfAnAppClass() {
        assertFalse(Runtime.isBootClass(ImplementsPackagePrivate.class));
        String metadata = Runtime.getTypeMetadata(ImplementsPackagePrivate.class);

        assertFalse(metadata, metadata.contains("\nN "));
    }

    @Test
    public void doesNotDescribeTheSuperInterfacesOfAnInterface() {
        String metadata = Runtime.getTypeMetadata(ListIterator.class);

        assertFalse(metadata, metadata.contains("\nN "));
    }
}
