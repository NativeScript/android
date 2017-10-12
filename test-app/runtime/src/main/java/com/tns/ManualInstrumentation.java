package com.tns;

import android.util.Log;

import java.util.Stack;

/**
 * Created by cankov on 01/06/2017.
 */
public class ManualInstrumentation {
    private static Mode mode = Mode.Pending;

    public static void setMode(String mode) {
        switch (mode) {
        case "timeline":
            ManualInstrumentation.setMode(ManualInstrumentation.Mode.Timeline);
            break;
        default:
            ManualInstrumentation.setMode(ManualInstrumentation.Mode.Disabled);
        }
        Runtime.SetManualInstrumentationMode(mode);
    }

    public static void setMode(Mode mode) {
        if (ManualInstrumentation.mode == Mode.Pending) {
            switch (mode) {
            case Timeline:
                Mode.PendingFrame.printPending();
                break;
            case Disabled:
                Mode.PendingFrame.discardPending();
                break;
            }
        }
        ManualInstrumentation.mode = mode;
    }

    public interface Frame {
        void close();
    }

    public static Frame start(String name) {
        return mode.start(name);
    }

    public enum Mode {
        /**
         * This mode is used to collect inital frames in pre package.json parsing.
         */
        Pending {
            protected Frame start(String name) {
                return new PendingFrame(name);
            }
        },
        /**
         * This mode will provide void frame implementation for fast performance in release builds.
         */
        Disabled {
            protected Frame start(String name) {
                return DisabledFrame.instance;
            }
        },
        /**
         * This mode creates frames that when closed will simply log in the android log frames that consume time longer than reasonable threshold.
         */
        Timeline {
            protected Frame start(String name) {
                LogFrame frame = LogFrame.frames.isEmpty() ? new LogFrame() : LogFrame.frames.pop();
                frame.start = System.currentTimeMillis();
                frame.name = name;
                return frame;
            }
        };

        protected abstract Frame start(String name);

        private static class DisabledFrame implements Frame {
            static DisabledFrame instance = new DisabledFrame();
            public void close() {}
        }

        private static class LogFrame implements Frame {
            private static Stack<LogFrame> frames = new Stack<LogFrame>();

            private long start;
            private StringBuilder builder = new StringBuilder();
            private String name;

            public void close() {
                try {
                    long end = System.currentTimeMillis();
                    if ((end - start) >= 16) {
                        builder.append("Timeline: Runtime: ");
                        builder.append(name);
                        builder.append("  (");
                        builder.append(start);
                        builder.append("ms. - ");
                        builder.append(end);
                        builder.append("ms.)");
                        Log.v("JS", builder.toString());
                        builder.setLength(0); // Recycle the builder
                    }
                } finally {
                    frames.push(this); // Recycle the frame
                }
            }
        }

        private static class PendingFrame implements Frame {
            private static Stack<Mode.PendingFrame> pendingFrames = new Stack<Mode.PendingFrame>();

            private long start;
            private long end;
            private String name;

            private PendingFrame(String name) {
                this.name = name;
                start = System.currentTimeMillis();
            }

            public void close() {
                end = System.currentTimeMillis();
                if (end - start > 16) {
                    if (mode == Timeline) {
                        print();
                    } else if (mode == Pending) {
                        pendingFrames.add(this);
                    }
                }
            }

            private void print() {
                StringBuilder builder = new StringBuilder();
                builder.append("Timeline: Runtime: ");
                builder.append(name);
                builder.append("  (");
                builder.append(start);
                builder.append("ms. - ");
                builder.append(end);
                builder.append("ms.)");
                Log.v("JS", builder.toString());
            }

            public static void printPending() {
                for (PendingFrame f : pendingFrames) {
                    f.print();
                }
                pendingFrames.clear();
            }

            public static void discardPending() {
                pendingFrames.clear();
            }
        }
    }
}
