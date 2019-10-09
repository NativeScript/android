package com.telerik.metadata.analytics;

import com.telerik.metadata.analytics.impl.EnabledAnalyticsCollectorImpl;
import com.telerik.metadata.analytics.impl.NoOpAnalyticsCollector;

public class AnalyticsCollectorProvider {
    private static final AnalyticsCollectorProvider ourInstance = new AnalyticsCollectorProvider();

    public static AnalyticsCollectorProvider getInstance() {
        return ourInstance;
    }

    private AnalyticsCollectorProvider() {
    }

    public AnalyticsCollector provideAnalyticsCollector() {
        if (AnalyticsConfiguration.areAnalyticsEnabled()) {
            String analyticsFilePath = AnalyticsConfiguration.getAnalyticsFilePath();
            return new EnabledAnalyticsCollectorImpl(analyticsFilePath);
        } else {
            return new NoOpAnalyticsCollector();
        }
    }
}
