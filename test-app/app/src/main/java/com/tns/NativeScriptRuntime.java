package com.tns;

public final class NativeScriptRuntime {
    private NativeScriptRuntime() {
    }

    public static boolean reloadApplication() {
        return RuntimeHelper.reloadApplication();
    }

    public static boolean reloadApplication(String baseDir) {
        return RuntimeHelper.reloadApplication(baseDir);
    }
}
