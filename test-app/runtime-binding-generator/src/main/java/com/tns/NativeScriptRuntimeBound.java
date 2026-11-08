package com.tns;

/**
 * Implemented by generated binding classes so that a Java-&gt;JS call can be routed
 * to the runtime that created the instance rather than inferred from the calling
 * thread.
 *
 * The tag is opaque: only com.tns.Runtime encodes and decodes it. The backing field
 * is left at its zero default, which must read as "no runtime yet" - it is still
 * zero while the superclass constructor runs, before initInstance can record the
 * owner. It is written once, by the runtime that registers the instance, and never
 * reassigned: an instance may later be wrapped by other runtimes, but only the
 * creating runtime holds the JS implementation the generated overrides dispatch to.
 */
public interface NativeScriptRuntimeBound {
    int getRuntimeTag__ns();

    void setRuntimeTag__ns(int tag);
}
