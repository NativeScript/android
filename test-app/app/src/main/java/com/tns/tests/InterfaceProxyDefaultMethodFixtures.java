package com.tns.tests;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Spliterator;
import java.util.function.Consumer;
import java.util.function.Predicate;

// Platform default methods for testInterfaceProxies.js, called from Java. API 24+ only: kept
// apart from InterfaceProxyFixtures so that class stays usable on older devices.
public class InterfaceProxyDefaultMethodFixtures {
    public static String drain(Iterator<?> it) {
        final List<Object> seen = new ArrayList<>();
        it.forEachRemaining(new Consumer<Object>() {
            @Override
            public void accept(Object item) {
                seen.add(item);
            }
        });
        return android.text.TextUtils.join(",", seen);
    }

    public static String remove(Iterator<?> it) {
        try {
            it.remove();
            return "removed";
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    // Through Iterator, as most callers see a PrimitiveIterator.OfInt.
    public static String next(Iterator<?> it) {
        try {
            return String.valueOf(it.next());
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    @SuppressWarnings("unchecked")
    public static String negate(Predicate<?> p, Object value) {
        Predicate<Object> predicate = (Predicate<Object>) p;
        return predicate.test(value) + "|" + predicate.negate().test(value);
    }

    public static String spliteratorDefaults(Spliterator<?> spliterator) {
        return spliterator.getExactSizeIfKnown() + "|" + spliterator.hasCharacteristics(Spliterator.SIZED)
                + "|" + spliterator.hasCharacteristics(Spliterator.SORTED);
    }
}
