package org.nativescript.staticbindinggenerator.test;

import org.junit.Test;
import org.nativescript.staticbindinggenerator.ContentKey;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

/**
 * The runtime computes the same keys in C++ (MetadataNode::CreateContentKey).
 * The vectors marked "paired" are also pinned in the test app's
 * testContentKeyedBindings.js, against classes the runtime generates.
 */
public class ContentKeyTest {

    private static List<String> list(String... values) {
        return Arrays.asList(values);
    }

    private static final List<String> NONE = Collections.emptyList();

    @Test
    public void hashesPayloads() {
        assertEquals("com.tns.tests.Button1||echo,toString", ContentKey.payload("com.tns.tests.Button1", list(""), list("toString", "echo")));
        assertEquals("h4b3d4e7b5dcd7eac", ContentKey.of("com.tns.tests.Button1", list(""), list("toString", "echo")));
        assertEquals("h43486683deee2d72", ContentKey.of("java.lang.Object", list("java.lang.Runnable", "java.io.Closeable"), list("run", "close")));
        assertEquals("he2246413e8de25b9", ContentKey.of("android.view.View", NONE, list("onClick")));
        assertEquals("h55d3a3a72d6a9f05", ContentKey.of("com/tns/tests/Button1", list(""), list("")));
    }

    @Test
    public void pairedWithTheRuntime() {
        assertEquals("he8b99c521d575fca", ContentKey.of("java/lang/Object", NONE, list("toString", "contentKeyPairingPlain")));
        assertEquals("h68c7e45e43205e66", ContentKey.of("com/tns/tests/DummyClass$DummyDerivedClass", NONE, list("toString", "contentKeyPairingNested")));
        assertEquals("java.lang.Object|com.tns.tests.DummyClass.MyInterface,java.lang.Runnable|contentKeyPairingInterfaces,doSomething,echoInt,run",
                ContentKey.payload("java/lang/Object", list("java.lang.Runnable", "com.tns.tests.DummyClass$MyInterface"), list("run", "echoInt", "doSomething", "contentKeyPairingInterfaces")));
        assertEquals("ha59a9a5b07d44d2b", ContentKey.of("java/lang/Object", list("java.lang.Runnable", "com.tns.tests.DummyClass$MyInterface"), list("run", "echoInt", "doSomething", "contentKeyPairingInterfaces")));
        assertEquals("h63c03cfe49192d3d", ContentKey.of("java/lang/Object", NONE, list("toString", "contentKeyPairingNamed")));
        assertEquals("hc5ac23ca0a98a3c1", ContentKey.of("java/lang/Object", NONE, list("toString", "contentKeyPairingTs")));
        assertEquals("h86a36c7f58a561ea", ContentKey.of("java/lang/Object", NONE, NONE));
    }

    @Test
    public void readsNestedTypesInEitherSpelling() {
        assertEquals(ContentKey.of("com.tns.tests.DummyClass.DummyDerivedClass", NONE, list("toString")),
                ContentKey.of("com/tns/tests/DummyClass$DummyDerivedClass", NONE, list("toString")));

        List<String> methods = list("greet", "unpack", "properties", "toString", "formatTo", "update", "addPropertyChangeListener", "pack", "removePropertyChangeListener");
        List<String> dotted = list("java.util.jar.Pack200.Unpacker", "java.util.Formattable", "java.util.Observer", "java.util.jar.Pack200.Packer");
        List<String> binary = list("java/util/jar/Pack200$Unpacker", "java.util.Formattable", "java.util.Observer", "java/util/jar/Pack200$Packer");
        assertEquals("haa054d88a5def37b", ContentKey.of("java.lang.Object", dotted, methods));
        assertEquals("haa054d88a5def37b", ContentKey.of("java.lang.Object", binary, methods));
    }

    @Test
    public void ignoresOrderEmptiesAndDuplicates() {
        assertEquals(ContentKey.of("android.view.View", list("a.B", "c.D"), list("x", "y")),
                ContentKey.of("android.view.View", list("c.D", "", "a.B"), list("y", "x", "")));
        assertEquals("h0c2b7a522f2ece0a", ContentKey.of("java.lang.Object", list("java.lang.Runnable", "java/lang/Runnable"), list("run", "run", "toString")));
        assertEquals(ContentKey.of("java.lang.Object", list("java.lang.Runnable"), list("run", "toString")),
                ContentKey.of("java.lang.Object", list("java.lang.Runnable", "java/lang/Runnable"), list("run", "run", "toString")));
    }

    @Test
    public void separatesDifferentContent() {
        String base = ContentKey.of("android.view.View", NONE, list("onClick"));
        assertNotEquals(base, ContentKey.of("android.view.ViewGroup", NONE, list("onClick")));
        assertNotEquals(base, ContentKey.of("android.view.View", NONE, list("onClick", "onTouch")));
        assertNotEquals(base, ContentKey.of("android.view.View", list("a.B"), list("onClick")));
        assertNotEquals(ContentKey.of("android.view.View", list("a.B"), NONE), ContentKey.of("android.view.View", NONE, list("a.B")));
    }

    @Test
    public void selectsMemberNames() {
        assertEquals(list("$x", "_y", "a1", "onCreate", "toString"),
                ContentKey.memberNames(new String[]{"toString", "onCreate", "constructor", "super", "__parent", "__child", "", "a1", "_y", "$x", "toString", "1a", "a-b"}));
        assertEquals(list("onClick"), ContentKey.memberNames(new String[]{"~getter", "onClick", "~onClick"}));
        assertEquals(NONE, ContentKey.memberNames(new String[]{""}));
        assertEquals(NONE, ContentKey.memberNames(null));
    }

    @Test
    public void ordersInterfacesByCanonicalName() {
        assertEquals(list("a.B$C", "a.B.D", "java.lang.Runnable"),
                ContentKey.interfaceNames(new String[]{"java.lang.Runnable", "a.B.D", "", "a.B$C", "java/lang/Runnable"}));
    }
}
