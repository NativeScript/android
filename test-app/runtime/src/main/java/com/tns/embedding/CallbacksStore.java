package com.tns.embedding;

public class CallbacksStore {

    private static EmbeddableFragmentCallbacks fragmentCallbacks;
    private static EmbeddableActivityCallbacks activityCallbacks;

    public static void setFragmentCallbacks(EmbeddableFragmentCallbacks callbacks) {
        fragmentCallbacks = callbacks;
    }

    public static void setActivityCallbacks(EmbeddableActivityCallbacks callbacks) {
        activityCallbacks = callbacks;
    }

    public static EmbeddableFragmentCallbacks getFragmentCallbacks() {
        if (fragmentCallbacks == null) {
            throw new RuntimeException("{N} Core modules: Fragment callbacks are null");
        }
        return fragmentCallbacks;
    }

    public static EmbeddableActivityCallbacks getActivityCallbacks() {
        if (activityCallbacks == null) {
            throw new RuntimeException("{N} Core modules: Activity callbacks are null");
        }
        return activityCallbacks;
    }
}
