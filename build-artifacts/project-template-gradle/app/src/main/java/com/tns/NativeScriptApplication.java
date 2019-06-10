package com.tns;

import android.app.Application;
import android.os.Build;
import androidx.multidex.MultiDex;

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

    public void attachBaseContext(android.content.Context base) {
        super.attachBaseContext(base);
        if (Build.VERSION.SDK_INT < 21) {
            // As the new gradle plugin automatically uses multidex if necessary we need to call this for older android API versions
            MultiDex.install(this);
        }
    }

    public static Application getInstance() {
        return thiz;
    }
}
