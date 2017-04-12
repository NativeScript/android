package com.tns;

import android.os.Handler;

public final class WorkThreadScheduler implements ThreadScheduler {
    private final Handler myHandler;

    public WorkThreadScheduler(Handler handler) {
        this.myHandler = handler;
    }

    @Override
    public final boolean post(Runnable r) {
        return myHandler.post(r);
    }

    @Override
    public final Thread getThread() {
        return myHandler.getLooper().getThread();
    }

    @Override
    public Handler getHandler() {
        return this.myHandler;
    }
}
