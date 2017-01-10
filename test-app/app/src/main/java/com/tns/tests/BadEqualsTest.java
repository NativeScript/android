package com.tns.tests;

import android.os.Handler;
import android.os.Looper;

public class BadEqualsTest {
    public static final class BadEqualsObject {
        public boolean equals(Object o) {
            return false;
        }
    }

    public interface Callback {
        void onFinish(BadEqualsObject obj);
    }

    private final BadEqualsObject obj;
    private final Callback callback;

    public BadEqualsTest(BadEqualsObject obj, Callback callback) {
        this.obj = obj;
        this.callback = callback;
    }

    public void postCallback() {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                callback.onFinish(obj);
            }
        });
    }
}
