package com.tns;

import android.os.Environment;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public final class DefaultTracer {
    public static void enable() {
        Tracer.setEnabled(true);
    }

    public static void start() {
        enable();
        addDefaultLogcatListeners();
        removeListener(Tracer.Descriptor.BENCHMARK);
    }

    public static void startBenchmarking() {
        enable();
        addSpecialListener(Tracer.Descriptor.BENCHMARK);
    }

    public static void stop() {
        Tracer.setEnabled(false);
    }

    public static void trace(int descriptor, String message) {
        Tracer.trace(descriptor, message);
    }

    public static void addListener(int descriptor, LogListener listener) {
        Tracer.addListener(descriptor, listener);
    }

    public static void removeListener(int descriptor) {
        Tracer.removeListener(descriptor);
    }

    public static void addDefaultLogcatListeners() {
        Map<Integer, String> descriptors = Tracer.getDescriptors();

        for (final Map.Entry<Integer, String> entry : descriptors.entrySet()) {
            LogListener defaultLogcatListener = new LogListener() {
                @Override
                public void dumpToFile() {

                }

                @Override
                public void trace(TraceMessage message) {
                    final String tName = message.threadName;
                    final long tid = message.threadId;
                    final String logMessage = message.message;
                    final String tag = entry.getValue();

                    StringBuilder sb = new StringBuilder();
                    sb.append("[");
                    sb.append(tName, 0, tName.length() >= 10 ? 10 : tName.length()); // limit Thread Name to 10 symbols in log
                    sb.append(String.format("%" + (10 - tName.length()) + "s", " "));
                    sb.append(" | ");
                    sb.append(tid + "]: ");

                    sb.append(logMessage);

                    String paddedTag = String.format("%" + (20 - tag.length()) + "s", " ") + tag;

                    android.util.Log.v(paddedTag, sb.toString());
                }
            };

            addListener(entry.getKey(), defaultLogcatListener);
        }
    }

    public static void addSpecialListener(final int descriptor) {
        LogListener fileWriterListener = new LogListener() {
            private List<String> logs = new ArrayList<>();

            @Override
            public void dumpToFile() {
                try {
                    File externalStorage = Environment.getExternalStorageDirectory();
                    File file = new File(externalStorage, Tracer.getDescriptors().get(descriptor) + "-log.xml");
                    if (!file.exists()) {
                        file.createNewFile();
                    } else {
                        file.delete();
                    }

                    FileWriter fw = new FileWriter(file, true);
                    BufferedWriter bw = new BufferedWriter(fw);
                    PrintWriter pw = new PrintWriter(bw);

                    for(String line : logs) {
                        pw.println(line);
                    }

                    pw.close();
                } catch (IOException ioe) {

                }
            }

            @Override
            public void trace(TraceMessage message) {
                logs.add(message.message);
            }
        };

        addListener(descriptor, fileWriterListener);
    }
}
