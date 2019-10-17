package com.telerik.metadata.analytics.enabled;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.telerik.metadata.analytics.AnalyticsCollector;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public final class DefaultAnalyticsCollector implements AnalyticsCollector {

    private static final String HAS_KOTLIN_RUNTIME_CLASSES_JSON_PROPERTY_NAME = "hasKotlinRuntimeClasses";
    private static final String HAS_USE_KOTLIN_PROPERTY_IN_APP_JSON_PROPERTY_NAME = "hasUseKotlinPropertyInApp";
    private static final String KOTLIN_USAGE_JSON_PROPERTY_NAME = "kotlinUsage";


    private final String analyticsFilePath;
    private final Gson gson;
    private boolean hasMarked;

    public static void main(String... args) {
        DefaultAnalyticsCollector a = new DefaultAnalyticsCollector("/Users/vmutafov/work/android_runtime_release/android-runtime/test-app/analytics/build-statistics.json");
        a.markHasKotlinRuntimeClassesIfNotMarkedAlready();
    }

    public DefaultAnalyticsCollector(String analyticsFilePath) {
        this.analyticsFilePath = analyticsFilePath;
        this.gson = new GsonBuilder().setPrettyPrinting().create();
        this.hasMarked = false;
    }

    @Override
    public void markHasKotlinRuntimeClassesIfNotMarkedAlready() {
        if (!hasMarked) {

            try {
                Path statisticsFilePath = Paths.get(analyticsFilePath);
                String json;

                if (Files.notExists(statisticsFilePath)) {
                    Files.createDirectories(statisticsFilePath.getParent());
                    Files.createFile(statisticsFilePath);

                    JsonObject kotlinUsageObject = createNewKotlinUsageJsonObject(true);
                    json = gson.toJson(kotlinUsageObject);
                } else {
                    byte[] fileContent = Files.readAllBytes(statisticsFilePath);
                    String statisticsJson = new String(fileContent, StandardCharsets.UTF_8);
                    JsonElement modifiedStatisticsJson = modifyExistingAnalyticsJsonObject(statisticsJson, true);
                    json = gson.toJson(modifiedStatisticsJson);
                }

                Files.write(statisticsFilePath, json.getBytes(StandardCharsets.UTF_8));
                hasMarked = true;
            } catch (Exception e) {
                System.out.println(e.getMessage()); // do not fail the build if analytics collection fails
            }
        }
    }

    private JsonObject createNewKotlinUsageJsonObject(boolean hasKotlinRuntimeClasses) {
        JsonObject jsonObject = new JsonObject();

        JsonObject kotlinUsageObject = new JsonObject();
        kotlinUsageObject.addProperty(HAS_KOTLIN_RUNTIME_CLASSES_JSON_PROPERTY_NAME, hasKotlinRuntimeClasses);

        jsonObject.add(KOTLIN_USAGE_JSON_PROPERTY_NAME, kotlinUsageObject);

        return jsonObject;
    }

    private JsonElement modifyExistingAnalyticsJsonObject(String json, boolean hasKotlinRuntimeClasses) {
        JsonElement jsonElement = new Gson().fromJson(json, JsonElement.class);
        JsonObject jsonObject = jsonElement.getAsJsonObject();

        if (jsonObject.has(KOTLIN_USAGE_JSON_PROPERTY_NAME)) {
            JsonObject kotlinUsageObject = jsonObject.getAsJsonObject(KOTLIN_USAGE_JSON_PROPERTY_NAME);
            kotlinUsageObject.addProperty(HAS_KOTLIN_RUNTIME_CLASSES_JSON_PROPERTY_NAME, hasKotlinRuntimeClasses);

            if(!kotlinUsageObject.has(HAS_USE_KOTLIN_PROPERTY_IN_APP_JSON_PROPERTY_NAME)){
                kotlinUsageObject.addProperty(HAS_USE_KOTLIN_PROPERTY_IN_APP_JSON_PROPERTY_NAME, false);
            }
        }

        return jsonElement;
    }
}
