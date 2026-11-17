package com.tns.tests.interfaces.defaultshapes;

// Java callers for testInterfaceDefaultMethodOverrides.js. Each reports a value or the
// exception's class and message as text.
public class ShapesCaller {
    public static String all(Shapes<String> shapes) {
        StringBuilder out = new StringBuilder();
        out.append(shapes.wide(7, 2.5, 3)).append('|');
        out.append(shapes.half(5)).append('|');
        out.append(shapes.third(1.5f)).append('|');
        out.append(shapes.upper('q')).append('|');
        out.append(shapes.odd(3)).append('|');
        out.append(shapes.negated((byte) 4)).append('|');
        out.append(shapes.doubled((short) 21)).append('|');
        shapes.record(out, Long.MAX_VALUE, -0.25);
        out.append('|').append(shapes.same("v"));
        return out.toString();
    }

    public static String fail(Shapes<?> shapes) {
        try {
            return shapes.fail("from java");
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    public static boolean isDesugared() throws NoSuchMethodException {
        return !Shapes.class.getMethod("half", long.class).isDefault();
    }
}
