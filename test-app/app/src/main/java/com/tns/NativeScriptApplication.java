package com.tns;

import android.app.Application;

public class NativeScriptApplication extends Application {

    private static NativeScriptApplication thiz;

    public NativeScriptApplication() {
        thiz = this;
    }

    public void onCreate() {
        ManualInstrumentation.Frame frame = ManualInstrumentation.start("NativeScriptApplication.onCreate");
        try {
            super.onCreate();
            com.tns.Runtime runtime = RuntimeHelper.initRuntime(this);
            if (runtime != null) {
                runtime.run();
            }
        } finally {
            frame.close();
        }
    }

    public static Application getInstance() {
        return thiz;
    }
}
