package com.tns;

import android.app.Application;
import android.content.Context;

public class NativeScriptApplication extends android.app.Application {

    private static NativeScriptApplication thiz;

    public NativeScriptApplication() {
        thiz = this;
    }

    public void onCreate() {
		super.onCreate();
		com.tns.Runtime runtime = RuntimeHelper.initRuntime(this);
        if (runtime != null) {
            runtime.run();
        }
    }

    public static Application getInstance() {
        return thiz;
    }
}
