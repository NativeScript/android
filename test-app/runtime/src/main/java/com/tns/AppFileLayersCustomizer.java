package com.tns;

/**
 * Changes the app file layers before the runtime starts. Register an
 * implementation (with a public no-argument constructor) in the manifest:
 * <pre>
 * &lt;meta-data android:name="com.example.MyCustomizer"
 *            android:value="org.nativescript.AppFileLayersCustomizer" /&gt;
 * </pre>
 * Layering applies to the files the runtime reads (modules, JSON, source
 * maps); files that other code opens by path, such as fonts and images, are
 * not layered.
 */
public interface AppFileLayersCustomizer {
    /** Customizers run in ascending priority; each sees the earlier ones' changes. */
    int priority();

    void customize(AppFileLayers.Builder builder);
}
