package org.nativescript;

import static org.nativescript.Bootstrap.bootstrapNativeScriptRuntime;

import android.app.Application;

public class NativeScriptApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        bootstrapNativeScriptRuntime(this);
    }
}
