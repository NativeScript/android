package com.tns.tests;

// Fixtures for testAppModuleBindings.js. Each interface has a single kind of JS
// implementation: RuntimeGenerated, ExtendedAtRuntime and WorkerRuntimeGenerated
// only through calls hidden from the static binding generator, so their classes
// are never prebuilt.
public interface AppModuleBindingTarget {
    String describe();

    interface Nested {
        String describe();
    }

    interface RuntimeGenerated {
        String describe();
    }

    interface ExtendedAtRuntime {
        String describe();
    }

    interface WorkerRuntimeGenerated {
        String describe();
    }
}
