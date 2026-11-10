package com.tns;

public interface NativeScriptRuntimeBound {
    int getRuntimeTag__ns();

    void setRuntimeTag__ns(int tag);

    default int getObjectId__ns() {
        return -1;
    }

    default void setObjectId__ns(int id) {
    }
}
