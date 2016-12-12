package com.tns;

/**
 * Created by pkanev on 11/8/2016.
 */

public class TraceMessage {
    public String message;
    public String threadName;
    public long threadId;

    public TraceMessage(String message, String tName, long threadId) {
        this.message = message;
        this.threadName = tName;
        this.threadId = threadId;
    }
}
