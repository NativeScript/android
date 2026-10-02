package com.tns;

/**
 * Implemented by generated binding classes so that a Java-&gt;JS call can be routed
 * to the runtime that created the instance rather than inferred from the calling
 * thread.
 *
 * The runtime id is written once, by the runtime that registers the instance, and
 * is never reassigned: an instance may later be wrapped by other runtimes (each
 * keeps its own object-id map), but only the creating runtime holds the JS
 * implementation the generated overrides dispatch to.
 */
public interface NativeScriptRuntimeBound {
    /**
     * Not 0: that is a real runtime id (the main runtime's), and JsV8InspectorClient
     * hard-codes it as such. Implementations must therefore seed the backing field
     * rather than rely on its natural zero default.
     */
    int INVALID_RUNTIME_ID = -1;

    int getRuntimeId__ns();

    void setRuntimeId__ns(int runtimeId);
}
