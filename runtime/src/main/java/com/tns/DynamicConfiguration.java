package com.tns;

import android.os.Handler;

/**
 * Created by plamen5kov on 8/30/16.
 */
public class DynamicConfiguration {
    private Integer workerId;
    private ThreadScheduler myThreadScheduler;
    private ThreadScheduler mainThreadScheduler;

    public DynamicConfiguration(Integer workerId, ThreadScheduler myThreadScheduler, ThreadScheduler mainThreadScheduler) {
        this.workerId = workerId;
        this.myThreadScheduler = myThreadScheduler;
        this.mainThreadScheduler = mainThreadScheduler;
    }

    public Integer getWorkerId() {
        return workerId;
    }

    public ThreadScheduler getHandler() {
        return myThreadScheduler;
    }

    public ThreadScheduler getMainHandler() {
        return mainThreadScheduler;
    }
}
