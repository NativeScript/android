package com.tns.tests;

// Fixture for testContentKeyedBindings.js. It only compiles with the rest of the build, as
// NativeScriptApplication is generated from JS, so the static binding generator cannot load
// it and leaves the extends that list it to the runtime.
public interface ContentKeyedUnseenListener {
    String describe();

    com.tns.NativeScriptApplication application();
}
