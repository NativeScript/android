package com.tns;

import android.app.Application;
import android.os.Bundle;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.widget.Toast;

import java.lang.reflect.Method;

import static com.tns.ErrorReport.isCheckingForPermissions;
import static com.tns.ErrorReport.resetCheckingForPermissions;

public class ErrorReportActivity extends AppCompatActivity {
    public void onCreate(Bundle savedInstanceState) {
        setTheme(androidx.appcompat.R.style.Theme_AppCompat_NoActionBar);

        super.onCreate(savedInstanceState);
        Application app = this.getApplication();
        Logger logger = new LogcatLogger(app);

        RuntimeHelper.initLiveSync(null, logger, app);

        new ErrorReport(this).buildUI();
    }

    @Override
    protected void onUserLeaveHint() {
        super.onUserLeaveHint();

        if (!isCheckingForPermissions()) {
            ErrorReport.killProcess(this);
        }
    }

    // @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        try {
            Method onRequestPermissionsResultMethod = AppCompatActivity.class.getMethod("onRequestPermissionsResult", int.class, permissions.getClass(), grantResults.getClass());
            onRequestPermissionsResultMethod.invoke(new AppCompatActivity() /* never do this */, requestCode, permissions, grantResults);

            resetCheckingForPermissions();
        } catch (Exception e) {
            if (Util.isDebuggableApp(this)) {
                e.printStackTrace();
            }
            Toast.makeText(this, "Couldn't resolve permissions", Toast.LENGTH_LONG).show();
            resetCheckingForPermissions();
        }
    }
}
