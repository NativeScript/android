package com.tns;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

import com.tns.Logger;
import com.tns.ExtractPolicy;
import com.tns.FileExtractor;

public class DefaultExtractPolicy implements ExtractPolicy {
    private final Logger logger;

    private final static String ASSETS_THUMB_FILENAME = "assetsThumb";

    public DefaultExtractPolicy(Logger logger) {
        this.logger = logger;
    }

    public boolean shouldExtract(Context context) {
        String assetsThumbFilePath = getFilesDir(context) + File.separatorChar + ASSETS_THUMB_FILENAME;
        String oldAssetsThumb = getCachedAssetsThumb(assetsThumbFilePath);
        if (oldAssetsThumb == null) {
            return true;
        } else {
            String currentThumb = getAssetsThumb(context);

            if (currentThumb != null  && !currentThumb.equals(oldAssetsThumb)) {
                return true;
            }
        }

        return false;
    }

    public void setAssetsThumb(Context context) {
        String assetsThumb = generateAssetsThumb(context);
        if (assetsThumb != null) {
            String assetsThumbFilePath = getFilesDir(context) + File.separatorChar + ASSETS_THUMB_FILENAME;
            saveNewAssetsThumb(assetsThumb, assetsThumbFilePath);
        }
    }

    public boolean forceOverwrite() {
        return true;
    }

    public FileExtractor extractor() {
        return null;
    }

    public String getAssetsThumb(Context context) {
        return generateAssetsThumb(context);
    }

    private String generateAssetsThumb(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            int code = packageInfo.versionCode;
            long updateTime = packageInfo.lastUpdateTime;
            return String.valueOf(updateTime) + "-" + String.valueOf(code);
        } catch (NameNotFoundException e) {
            logger.write("Error while getting current assets thumb");
            e.printStackTrace();
        }

        return null;
    }

    private String getCachedAssetsThumb(String assetsThumbFilePath) {
        try {
            File cachedThumbFile = new File(assetsThumbFilePath);
            if (cachedThumbFile.exists()) {
                FileInputStream in = new FileInputStream(cachedThumbFile);
                BufferedReader reader = new BufferedReader(new InputStreamReader(in));
                String cachedThumb = reader.readLine();
                reader.close();
                in.close();
                return cachedThumb;
            }
        } catch (FileNotFoundException e) {
            logger.write("Error while getting current assets thumb");
            e.printStackTrace();
        } catch (IOException e) {
            logger.write("Error while getting current asstes thumb");
            e.printStackTrace();
        }

        return null;
    }

    private void saveNewAssetsThumb(String newThumb, String assetsThumbFile) {
        File cachedThumbFile = new File(assetsThumbFile);
        try {
            FileOutputStream out = new FileOutputStream(cachedThumbFile, false);
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
            try {
                writer.write(newThumb);
                writer.newLine();
                writer.flush();
            } finally {
                writer.close();
                out.close();
            }
        } catch (FileNotFoundException e) {
            logger.write("Error while writing current assets thumb");
            e.printStackTrace();
        } catch (IOException e) {
            logger.write("Error while writing current assets thumb");
            e.printStackTrace();
        }
    }

    /*
        Write assetsThumb file to a no-backup directory to prevent the thumb from being
        backed up on devices of API Level 23 and up.
        Devices Level 22 and lower don't support the Auto Backup feature,
        so it is safe to keep the thumb in the /files directory
     */
    private static String getFilesDir(Context context) {
        if (android.os.Build.VERSION.SDK_INT >= /* 21 */ android.os.Build.VERSION_CODES.LOLLIPOP) {
            return context.getNoBackupFilesDir().getPath();
        } else {
            return context.getFilesDir().getPath();
        }
    }

}
