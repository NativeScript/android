package com.tns;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;

public class ErrorReportActivity extends AppCompatActivity {
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        new ErrorReport(this).buildUI();
    }

    @Override
    protected void onPause() {
        // the moment the error activity is not in the foreground we want to kill the process
        super.onPause();
        ErrorReport.killProcess(this);
    }

    // @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        // super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
}
