package org.nativescript.staticbindinggenerator;

import org.apache.bcel.generic.Type;

import java.util.HashMap;
import java.util.Map;

public class DefaultValues {
    static final Map<Type, String> defaultValues = new HashMap<>();

    // load
    static {
        defaultValues.put(Type.BOOLEAN, "false");
        defaultValues.put(Type.BYTE, "0");
        defaultValues.put(Type.SHORT, "0");
        defaultValues.put(Type.INT, "0");
        defaultValues.put(Type.LONG, "0L");
        defaultValues.put(Type.CHAR, "'\u0000'");
        defaultValues.put(Type.FLOAT, "0.0F");
        defaultValues.put(Type.DOUBLE, "0.0");
    }

    public static final String defaultStringValueFor(Type type) {
        if(defaultValues.containsKey(type)) {
            return defaultValues.get(type);
        } else {
            return "null";
        }
    }
}
