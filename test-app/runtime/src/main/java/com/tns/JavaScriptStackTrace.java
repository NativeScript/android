package com.tns;

import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Carries a JavaScript stack trace on a Java exception. Never thrown - it is
 * attached via {@link Throwable#addSuppressed} so the JS frames render
 * automatically in stack dumps and crash reporters ("Suppressed: ..."), with
 * the frames synthesized as real {@link StackTraceElement}s.
 *
 * Currently attached by the runtime when a JS error branded with
 * {@code interop.escapeException} forwards an original Java throwable to a
 * native caller; the API is intentionally generic so other exception paths can
 * adopt it. Crash-SDK integrations can look the carrier up via
 * {@link Throwable#getSuppressed} and read the raw stacks through
 * {@link #getJavaScriptStack()} / {@link #getEscapeSiteStack()}.
 */
public final class JavaScriptStackTrace extends Throwable {
    private static final long serialVersionUID = 1L;

    private final String javaScriptStack;
    private final String escapeSiteStack;

    private JavaScriptStackTrace(String message, String javaScriptStack, String escapeSiteStack) {
        super(message);
        this.javaScriptStack = javaScriptStack;
        this.escapeSiteStack = escapeSiteStack;
    }

    /**
     * The JS stack of the error itself (where it was created/thrown), as the
     * raw V8-formatted string. May be empty when the escaped value carried no
     * stack (e.g. a non-Error value).
     */
    public String getJavaScriptStack() {
        return javaScriptStack;
    }

    /**
     * The JS stack of the {@code interop.escapeException(...)} call site, as
     * the raw V8-formatted string.
     */
    public String getEscapeSiteStack() {
        return escapeSiteStack;
    }

    /**
     * The carrier's own Java capture point is machinery noise - its stack is
     * replaced with the synthesized JS frames, so skip the capture entirely.
     */
    @Override
    public synchronized Throwable fillInStackTrace() {
        return this;
    }

    /**
     * Attaches a carrier to {@code target} as a suppressed exception. The
     * rendered frames come from {@code javaScriptStack}, falling back to
     * {@code escapeSiteStack} when the error carried no stack of its own.
     * Idempotent: a target already carrying an identical stack is left
     * unchanged (e.g. the same throwable escaping twice through nested
     * overrides). No-ops when suppression is disabled on the target.
     */
    @RuntimeCallable
    public static void attach(Throwable target, String message, String javaScriptStack, String escapeSiteStack) {
        if (target == null) {
            return;
        }
        String stackToRender = (javaScriptStack != null && !javaScriptStack.isEmpty())
                               ? javaScriptStack
                               : escapeSiteStack;
        if (stackToRender == null || stackToRender.isEmpty()) {
            return;
        }
        for (Throwable suppressed : target.getSuppressed()) {
            if (suppressed instanceof JavaScriptStackTrace &&
                    stackToRender.equals(((JavaScriptStackTrace) suppressed).renderedStack())) {
                return;
            }
        }
        JavaScriptStackTrace carrier = new JavaScriptStackTrace(message, javaScriptStack, escapeSiteStack);
        carrier.setStackTrace(parseFrames(stackToRender));
        target.addSuppressed(carrier);
    }

    /**
     * Replaces {@code target}'s stack trace with frames synthesized from the
     * JS stack, so crash reporters group JS-originated exceptions by their
     * actual JS frames instead of the (identical) JNI boundary machinery.
     * Only ever called on throwables the runtime itself constructs.
     */
    @RuntimeCallable
    public static void applyFrames(Throwable target, String javaScriptStack, String escapeSiteStack) {
        if (target == null) {
            return;
        }
        String stackToRender = (javaScriptStack != null && !javaScriptStack.isEmpty())
                               ? javaScriptStack
                               : escapeSiteStack;
        if (stackToRender == null || stackToRender.isEmpty()) {
            return;
        }
        StackTraceElement[] frames = parseFrames(stackToRender);
        if (frames.length > 0) {
            target.setStackTrace(frames);
        }
    }

    private String renderedStack() {
        return (javaScriptStack != null && !javaScriptStack.isEmpty()) ? javaScriptStack : escapeSiteStack;
    }

    /*
     * Both V8's standard format ("    at fn (file:line:col)", "    at
     * file:line:col") and the runtime's own GetErrorStackTrace format
     * ("fn(file:line:col)") end each frame with "(file:line:col)" or
     * "file:line:col". A line that matches neither still becomes a frame
     * (with the raw text as the method name) so no information is dropped.
     */
    private static final Pattern FRAME_TAIL =
            Pattern.compile("^(?:\\s*at\\s+)?(?:(.*?)\\s*\\()?\\s*(\\S+?):(\\d+)(?::\\d+)?\\)?\\s*$");

    private static StackTraceElement[] parseFrames(String stack) {
        ArrayList<StackTraceElement> frames = new ArrayList<StackTraceElement>();
        for (String line : stack.split("\n")) {
            String trimmed = line.trim();
            if (trimmed.isEmpty()) {
                continue;
            }
            Matcher m = FRAME_TAIL.matcher(trimmed);
            if (!m.matches()) {
                // Leading unparseable lines are the "Error: message" heading
                // of a standard V8 stack; later ones are kept as raw frames
                // so no information is dropped.
                if (!frames.isEmpty()) {
                    frames.add(new StackTraceElement("<js>", trimmed, null, -1));
                }
                continue;
            }
            String fn = m.group(1);
            if (fn == null || fn.isEmpty()) {
                fn = "<anonymous>";
            } else if (fn.startsWith("at ")) {
                fn = fn.substring(3).trim();
            }
            String file = m.group(2);
            int lineNumber;
            try {
                lineNumber = Integer.parseInt(m.group(3));
            } catch (NumberFormatException e) {
                lineNumber = -1;
            }
            frames.add(new StackTraceElement("<js>", fn, file, lineNumber));
        }
        return frames.toArray(new StackTraceElement[0]);
    }
}
