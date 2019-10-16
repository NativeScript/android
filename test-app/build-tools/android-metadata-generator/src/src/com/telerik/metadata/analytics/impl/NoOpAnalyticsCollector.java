package com.telerik.metadata.analytics.impl;

import com.telerik.metadata.analytics.AnalyticsCollector;

public final class NoOpAnalyticsCollector implements AnalyticsCollector {

    @Override
    public void markHasKotlinRuntimeClassesIfNotMarkedAlready() {
        // used when analytics are disabled
    }
}
