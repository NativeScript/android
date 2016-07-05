package com.tns;

import android.app.Application;

public class NativeScriptApplication extends Application {

    private static NativeScriptApplication thiz;

    public NativeScriptApplication() {
        thiz = this;
    }

    public void onCreate() {
		super.onCreate();
		Runtime runtime = RuntimeHelper.initRuntime(this);
        if (runtime !=null) {
            runtime.run();
        }
    }

    public static Application getInstance() {
        return thiz;
    }
}
