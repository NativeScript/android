package com.tns.embedding;

import android.app.Application;

public class ApplicationHolder {

    private static Application application;

    private ApplicationHolder() {

    }

    public static void setInstance(Application application) {
        ApplicationHolder.application = application;
    }

    public static Application getInstance() {
        return application;
    }
}
