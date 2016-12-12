package com.tns;

public interface TraceListener {
    void trace(TraceMessage message);
    void dumpToFile();
}
