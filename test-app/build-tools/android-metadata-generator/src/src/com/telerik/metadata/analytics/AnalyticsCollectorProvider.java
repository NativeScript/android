package com.telerik.metadata.analytics;

import com.telerik.metadata.analytics.enabled.DefaultAnalyticsCollector;
import com.telerik.metadata.analytics.disabled.NoOpAnalyticsCollector;

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
            return new DefaultAnalyticsCollector(analyticsFilePath);
        } else {
            return new NoOpAnalyticsCollector();
        }
    }
}
