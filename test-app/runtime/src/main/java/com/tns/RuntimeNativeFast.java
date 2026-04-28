package com.tns;

import dalvik.annotation.optimization.CriticalNative;
import dalvik.annotation.optimization.FastNative;

final class RuntimeNativeFast {
    private RuntimeNativeFast() {
    }

    @CriticalNative
    static native int generateNewObjectId(int runtimeId);

    @FastNative
    static native boolean notifyGc(Runtime runtime, int runtimeId);

    @CriticalNative
    static native int getCurrentRuntimeId();

    @CriticalNative
    static native int getPointerSize();

    @FastNative
    static native void SetManualInstrumentationMode(String mode);
}
