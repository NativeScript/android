package com.tns.bindings;

import org.ow2.asmdex.ApplicationWriter;

/**
 * How a default-overriding class runs the Java body of a default method its JS object does
 * not implement. The two kinds of class have different names, so that one of each can be
 * loaded for the same interface.
 */
public enum DefaultMethodBodies {
    /**
     * With invoke-super naming the interface the class implements. The class's dex file also
     * holds its DefaultSuperProbe class, which has to pass before the class is used.
     */
    INVOKE_SUPER(ApplicationWriter.DEFAULT_METHODS_DEX_VERSION, Dump.DEFAULT_OVERRIDING_SUFFIX),
    /** Through com.tns.Runtime.callInterfaceDefault, which calls the method non-virtually with JNI. */
    RUNTIME_CALL(ApplicationWriter.DEFAULT_DEX_VERSION, Dump.DEFAULT_OVERRIDING_SUFFIX + "-jni");

    /** The dex format version of a file holding such a class. */
    public final int dexVersion;
    /** Appended to the interface's generated class name to name such a class. */
    public final String classSuffix;

    DefaultMethodBodies(int dexVersion, String classSuffix) {
        this.dexVersion = dexVersion;
        this.classSuffix = classSuffix;
    }
}
