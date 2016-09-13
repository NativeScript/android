package com.tns;

import android.os.Handler;

/**
 * Created by plamen5kov on 8/30/16.
 */
public class DynamicConfiguration {
    public final Integer workerId;
    public final ThreadScheduler myThreadScheduler;
    public final ThreadScheduler mainThreadScheduler;
    public String callingJsDir;

    public DynamicConfiguration(Integer workerId, ThreadScheduler myThreadScheduler, ThreadScheduler mainThreadScheduler) {
        this.workerId = workerId;
        this.myThreadScheduler = myThreadScheduler;
        this.mainThreadScheduler = mainThreadScheduler;
    }

    public DynamicConfiguration(Integer workerId, ThreadScheduler myThreadScheduler, ThreadScheduler mainThreadScheduler, String callingJsDir) {
        this(workerId, myThreadScheduler, mainThreadScheduler);
        this.callingJsDir = callingJsDir;
    }
}
