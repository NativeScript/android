package org.nativescript.staticbindinggenerator;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.TreeSet;
import java.util.regex.Pattern;

/**
 * The build-time half of content-keyed binding names. The runtime computes the
 * same key in MetadataNode::CreateContentKey and the same member set in
 * CallbackHandlers::CollectContentMemberNames; a disagreement does not fail
 * anything, the class is just generated on the device instead. ContentKeyTest
 * and the test app's content-keyed binding specs pin the same literal vectors.
 */
public final class ContentKey {

    private static final long FNV_OFFSET_BASIS = 0xcbf29ce484222325L;
    private static final long FNV_PRIME = 0x100000001b3L;

    private static final Pattern MEMBER_NAME = Pattern.compile("^[A-Za-z_$][A-Za-z0-9_$]*$");

    // Put on every implementation object by extend() or by ts_helpers' __extends,
    // never by the app.
    private static final Set<String> EXCLUDED_MEMBER_NAMES = new HashSet<>(Arrays.asList("constructor", "super", "__parent", "__child"));

    // The runtime sorts std::string, which compares bytes; String.compareTo
    // compares UTF-16 units, which orders differently above the BMP.
    private static final Comparator<String> UTF8_ORDER = (left, right) -> {
        byte[] a = left.getBytes(StandardCharsets.UTF_8);
        byte[] b = right.getBytes(StandardCharsets.UTF_8);
        int shared = Math.min(a.length, b.length);
        for (int i = 0; i < shared; i++) {
            int diff = (a[i] & 0xff) - (b[i] & 0xff);
            if (diff != 0) {
                return diff;
            }
        }
        return a.length - b.length;
    };

    private ContentKey() {
    }

    /**
     * The members a binding is generated with, from the JS parser's member names:
     * sorted, without duplicates, without the names extend() itself adds, and
     * without accessors, which the runtime leaves out of the key.
     */
    public static List<String> memberNames(String[] parsedNames) {
        TreeSet<String> result = new TreeSet<>(UTF8_ORDER);
        if (parsedNames != null) {
            for (String name : parsedNames) {
                if (name != null && MEMBER_NAME.matcher(name).matches() && !EXCLUDED_MEMBER_NAMES.contains(name)) {
                    result.add(name);
                }
            }
        }
        return new ArrayList<>(result);
    }

    /**
     * Interface names in the order the binding implements them. Ordered and
     * de-duplicated by their canonical form, the form the key hashes.
     */
    public static List<String> interfaceNames(String[] parsedNames) {
        List<String> result = new ArrayList<>();
        Set<String> seen = new HashSet<>();
        if (parsedNames != null) {
            for (String name : parsedNames) {
                if (name != null && !name.isEmpty() && seen.add(canonicalize(name))) {
                    result.add(name);
                }
            }
        }
        result.sort((left, right) -> UTF8_ORDER.compare(canonicalize(left), canonicalize(right)));
        return result;
    }

    /** {@code h} followed by 16 lowercase hex digits. */
    public static String of(String baseClassName, Collection<String> interfaceNames, Collection<String> memberNames) {
        long hash = FNV_OFFSET_BASIS;
        for (byte b : payload(baseClassName, interfaceNames, memberNames).getBytes(StandardCharsets.UTF_8)) {
            hash ^= (b & 0xffL);
            hash *= FNV_PRIME;
        }
        return String.format(Locale.ROOT, "h%016x", hash);
    }

    /**
     * {@code canonical(base)|sorted canonical interfaces|sorted members}, each
     * list comma-separated, without empty or duplicate entries.
     */
    public static String payload(String baseClassName, Collection<String> interfaceNames, Collection<String> memberNames) {
        return canonicalize(baseClassName) + "|" + join(interfaceNames, true) + "|" + join(memberNames, false);
    }

    /**
     * JS source spells a nested type like a package, `a.B.C`; the runtime holds
     * binary names, `a/B$C`. The dotted form is the one both sides can produce.
     */
    static String canonicalize(String name) {
        return name == null ? "" : name.replace('/', '.').replace('$', '.');
    }

    private static String join(Collection<String> values, boolean canonicalize) {
        TreeSet<String> sorted = new TreeSet<>(UTF8_ORDER);
        if (values != null) {
            for (String value : values) {
                if (value != null && !value.isEmpty()) {
                    sorted.add(canonicalize ? canonicalize(value) : value);
                }
            }
        }
        return String.join(",", sorted);
    }
}
