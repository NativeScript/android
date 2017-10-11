package com.tns;

import android.os.Handler;

public interface ThreadScheduler {
    boolean post(Runnable r);

    Thread getThread();

    Handler getHandler();
}
