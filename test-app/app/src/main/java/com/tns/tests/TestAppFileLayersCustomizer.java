package com.tns.tests;

import com.tns.AppFileLayer;
import com.tns.AppFileLayers;
import com.tns.AppFileLayersCustomizer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

// Backs tests/testAppFileLayers.js; the layers answer only paths under tests/appFileLayers/.
public class TestAppFileLayersCustomizer implements AppFileLayersCustomizer {
    static final String MAP_LAYER = "test-map";
    static final String DIRECTORY_LAYER = "test-directory";
    static final String ASSETS_LAYER = "test-assets";

    @Override
    public int priority() {
        return 0;
    }

    @Override
    public void customize(AppFileLayers.Builder builder) {
        File dir = new File(builder.context().getNoBackupFilesDir(), "appFileLayersTest");
        write(new File(dir, "tests/appFileLayers/fromDirectory.js"), "module.exports.value = 'directory';\n");
        write(new File(dir, "tests/appFileLayers/fromDirectory.mjs"), "export const value = 'directory-esm';\n");
        write(new File(dir, "tests/appFileLayers/whitedOut.js"), "module.exports.value = 'should be hidden';\n");

        Map<String, AppFileLayer.Entry> entries = new HashMap<>();
        entries.put("tests/appFileLayers/remapped.js", AppFileLayer.Entry.asset("app/tests/appFileLayers/target.js"));
        entries.put("tests/appFileLayers/whitedOut.js", AppFileLayer.Entry.whiteout());

        builder.insertBelow(AppFileLayers.EXTRACTED, AppFileLayer.map(MAP_LAYER, entries));
        builder.insertBelow(MAP_LAYER, AppFileLayer.directory(DIRECTORY_LAYER, dir));
        builder.insertBelow(DIRECTORY_LAYER, AppFileLayer.apkAssets(ASSETS_LAYER, "layertest"));
    }

    private static void write(File file, String text) {
        file.getParentFile().mkdirs();
        try (FileOutputStream out = new FileOutputStream(file)) {
            out.write(text.getBytes(StandardCharsets.UTF_8));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
