package com.tns.bindings;

import com.tns.bindings.desc.ClassDescriptor;
import com.tns.bindings.desc.reflection.ClassInfo;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashSet;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class ProxyFileNameTest {
    // NAME_MAX of the file systems the cache lives on.
    private static final int MAX_FILE_NAME_LENGTH = 255;
    private static final String THUMB = "1731800000000-42";
    private static final String DIGEST = "0123456789";

    @Rule
    public TemporaryFolder temp = new TemporaryFolder();

    private static String repeat(char c, int count) {
        char[] chars = new char[count];
        Arrays.fill(chars, c);
        return new String(chars);
    }

    @Test
    public void savesAProxyWhoseFileNameIsAsLongAsTheFileSystemAllows() throws Exception {
        File dir = temp.newFolder("proxies");
        String fixedParts = Object.class.getName() + Dump.CLASS_NAME_LOCATION_SEPARATOR + "-" + THUMB + "-" + DIGEST + ".dex";
        String proxyName = repeat('x', MAX_FILE_NAME_LENGTH - fixedParts.length());

        ProxyGenerator generator = new ProxyGenerator(dir.getAbsolutePath());
        generator.setProxyThumb(THUMB);
        String path = generator.generateProxy(proxyName, DIGEST, new ClassInfo(Object.class), null,
                new HashSet<ClassDescriptor>(), false, null);

        File dex = new File(path);
        assertEquals(MAX_FILE_NAME_LENGTH, dex.getName().length());
        assertTrue(dex.isFile());
        assertArrayEquals(new String[]{dex.getName()}, dir.list());
    }

    // The JVM's createTempFile shortens a prefix that does not fit, Android's does not,
    // so the temp name has to be short whatever the final name is. A directory in the
    // way of the final name makes the rename fail with the temp file's name.
    @Test
    public void writesAProxyThroughAShortTempFileWhateverItsFinalName() throws Exception {
        File dir = temp.newFolder("blocked");
        String fixedParts = Object.class.getName() + Dump.CLASS_NAME_LOCATION_SEPARATOR + "-" + THUMB + "-" + DIGEST + ".dex";
        String proxyName = repeat('x', MAX_FILE_NAME_LENGTH - fixedParts.length());
        String finalName = Object.class.getName() + Dump.CLASS_NAME_LOCATION_SEPARATOR + proxyName + "-" + THUMB + "-" + DIGEST + ".dex";
        assertTrue(new File(dir, finalName).mkdir());

        ProxyGenerator generator = new ProxyGenerator(dir.getAbsolutePath());
        generator.setProxyThumb(THUMB);
        try {
            generator.generateProxy(proxyName, DIGEST, new ClassInfo(Object.class), null, new HashSet<ClassDescriptor>(), false, null);
            fail("expected the rename onto a directory to fail");
        } catch (IOException e) {
            String message = e.getMessage();
            assertTrue(message, message.startsWith("Could not move "));
            String tempName = new File(message.substring("Could not move ".length(), message.indexOf(" to "))).getName();
            assertTrue(tempName, tempName.contains(THUMB));
            assertTrue(tempName, tempName.endsWith(".tmp"));
            assertTrue(tempName, tempName.length() <= THUMB.length() + 1 + 20 + ".tmp".length());
        }
        assertArrayEquals(new String[]{finalName}, dir.list());
    }

    @Test
    public void namesCacheTempFilesAfterTheThumbOnly() throws Exception {
        File dir = temp.newFolder("cache");
        File first = ProxyGenerator.createCacheTempFile(dir, THUMB);
        File second = ProxyGenerator.createCacheTempFile(dir, THUMB);
        File withoutThumb = ProxyGenerator.createCacheTempFile(dir, null);

        for (File file : new File[]{first, second, withoutThumb}) {
            assertEquals(dir, file.getParentFile());
            assertTrue(file.getName(), file.isFile());
            assertTrue(file.getName(), file.getName().endsWith(".tmp"));
            assertTrue(file.getName(), file.getName().length() <= THUMB.length() + 1 + 20 + ".tmp".length());
        }
        assertTrue(first.getName(), first.getName().contains(THUMB));
        assertTrue(second.getName(), second.getName().contains(THUMB));
        assertFalse(first.getName().equals(second.getName()));
    }
}
