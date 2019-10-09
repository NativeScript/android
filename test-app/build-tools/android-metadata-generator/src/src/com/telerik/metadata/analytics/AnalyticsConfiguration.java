package com.telerik.metadata.analytics;

public final class AnalyticsConfiguration {
    private static String analyticsFilePath;
    private static boolean areAnalyticsEnabled;

    private AnalyticsConfiguration(){}

    public static void enableAnalytics(String analyticsJsonFilePath){
        analyticsFilePath = analyticsJsonFilePath;
        areAnalyticsEnabled = true;
    }

    static boolean areAnalyticsEnabled(){
        return areAnalyticsEnabled;
    }

    static String getAnalyticsFilePath(){
        return analyticsFilePath;
    }
}
