package com.tns;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.Manifest;
import android.app.Activity;
import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import com.google.android.material.tabs.TabLayout;
import androidx.core.app.ActivityCompat;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentStatePagerAdapter;
import androidx.viewpager.widget.ViewPager;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;


class ErrorReport implements TabLayout.OnTabSelectedListener {
    public static final String ERROR_FILE_NAME = "hasError";
    private static AppCompatActivity activity;

    private TabLayout tabLayout;
    private ViewPager viewPager;
    private Context context;

    private static String exceptionMsg;
    private static String logcatMsg;

    private static boolean checkingForPermissions = false;

    private final static String EXTRA_NATIVESCRIPT_ERROR_REPORT = "NativeScriptErrorMessage";
    private final static String EXTRA_ERROR_REPORT_MSG = "msg";
    private final static String EXTRA_PID = "pID";
    private final static int EXTRA_ERROR_REPORT_VALUE = 1;

    private static final int REQUEST_EXTERNAL_STORAGE = 1;
    private static String[] PERMISSIONS_STORAGE = {
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.WRITE_EXTERNAL_STORAGE
    };

    // Will prevent error activity from killing process if permission request dialog pops up
    public static boolean isCheckingForPermissions() {
        return checkingForPermissions;
    }

    public static void resetCheckingForPermissions() {
        checkingForPermissions = false;
    }

    // The following will not compile if uncommented with compileSdk lower than 23
    public static void verifyStoragePermissions(Activity activity) {
        // Check if we have write permission
        final int version = Build.VERSION.SDK_INT;
        if (version >= 23) {
            try {
                // Necessary to work around compile errors with compileSdk 22 and lower
                Method checkSelfPermissionMethod;
                try {
                    checkSelfPermissionMethod = ActivityCompat.class.getMethod("checkSelfPermission", Context.class, String.class);
                } catch (NoSuchMethodException e) {
                    // method wasn't found, so there is no need to handle permissions explicitly
                    if (Util.isDebuggableApp(activity)) {
                        e.printStackTrace();
                    }
                    return;
                }

                int permission = (int) checkSelfPermissionMethod.invoke(null, activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);

                if (permission != PackageManager.PERMISSION_GRANTED) {
                    // We don't have permission so prompt the user
                    Method requestPermissionsMethod = ActivityCompat.class.getMethod("requestPermissions", Activity.class, PERMISSIONS_STORAGE.getClass(), int.class);

                    checkingForPermissions = true;

                    requestPermissionsMethod.invoke(null, activity, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
                }
            } catch (Exception e) {
                Toast.makeText(activity, "Couldn't resolve permissions", Toast.LENGTH_LONG).show();
                if (Util.isDebuggableApp(activity)) {
                    e.printStackTrace();
                }
                return;
            }
        }
    }

    public ErrorReport(AppCompatActivity activity) {
        ErrorReport.activity = activity;
        this.context = activity.getApplicationContext();
    }

    static boolean startActivity(final Context context, String errorMessage) {
        final Intent intent = getIntent(context);
        if (intent == null) {
            return false; // (if in release mode) don't do anything
        }

        intent.putExtra(EXTRA_ERROR_REPORT_MSG, errorMessage);

        String PID = Integer.toString(android.os.Process.myPid());
        intent.putExtra(EXTRA_PID, PID);

        createErrorFile(context);

        try {
            startPendingErrorActivity(context, intent);
        } catch (CanceledException e) {
            Log.d("ErrorReport", "Couldn't send pending intent! Exception: " + e.getMessage());
        }

        killProcess(context);

        return true;
    }

    static void killProcess(Context context) {
        // finish current activity and all below it first
        if (context instanceof Activity) {
            ((Activity) context).finishAffinity();
        }

        // kill process
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    static void startPendingErrorActivity(Context context, Intent intent) throws CanceledException {
        int flags = PendingIntent.FLAG_CANCEL_CURRENT;
        if (Build.VERSION.SDK_INT >= 31) {
            flags = PendingIntent.FLAG_CANCEL_CURRENT | PendingIntent.FLAG_MUTABLE;
        }

        PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, flags);

        pendingIntent.send(context, 0, intent);
    }

    static String getErrorMessage(Throwable ex) {
        String content;
        PrintStream ps = null;

        try {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            ps = new PrintStream(baos);
            ex.printStackTrace(ps);

            try {
                content = baos.toString("UTF-8");
            } catch (UnsupportedEncodingException e) {
                content = e.getMessage();
            }
        } finally {
            if (ps != null) {
                ps.close();
            }
        }

        return content;
    }

    /*
    * Gets the process Id of the running app and filters all
    * output that doesn't belong to that process
    * */
    public static String getLogcat(String pId) {
        String content;

        try {
            String logcatCommand = "logcat -d";
            Process process = java.lang.Runtime.getRuntime().exec(logcatCommand);

            BufferedReader bufferedReader = new BufferedReader(
                new InputStreamReader(process.getInputStream()));

            StringBuilder log = new StringBuilder();
            String line = "";
            String lineSeparator = System.getProperty("line.separator");
            while ((line = bufferedReader.readLine()) != null) {
                if (line.contains(pId)) {
                    log.append(line);
                    log.append(lineSeparator);
                }
            }

            content = log.toString();
        } catch (IOException e) {
            content = "Failed to read logcat";
            Log.e("TNS.Android", content);
        }

        return content;
    }

    static Intent getIntent(Context context) {
        Class<?> errorActivityClass;

        if (Util.isDebuggableApp(context)) {
            errorActivityClass = ErrorReportActivity.class;
        } else {
            return null;
        }

        Intent intent = new Intent(context, errorActivityClass);

        intent.putExtra(EXTRA_NATIVESCRIPT_ERROR_REPORT, EXTRA_ERROR_REPORT_VALUE);
        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);

        return intent;
    }

    static boolean hasIntent(Intent intent) {
        int value = intent.getIntExtra(EXTRA_NATIVESCRIPT_ERROR_REPORT, 0);

        return value == EXTRA_ERROR_REPORT_VALUE;
    }

    void buildUI() {
        Intent intent = activity.getIntent();

        exceptionMsg = intent.getStringExtra(EXTRA_ERROR_REPORT_MSG);

        String processId = intent.getStringExtra(EXTRA_PID);
        logcatMsg = getLogcat(processId);

        int errActivityId = this.context.getResources().getIdentifier("error_activity", "layout", this.context.getPackageName());

        activity.setContentView(errActivityId);

        int toolBarId = this.context.getResources().getIdentifier("toolbar", "id", this.context.getPackageName());

        Toolbar toolbar = (Toolbar) activity.findViewById(toolBarId);
        activity.setSupportActionBar(toolbar);

        final int tabLayoutId = this.context.getResources().getIdentifier("tabLayout", "id", this.context.getPackageName());

        tabLayout = (TabLayout) activity.findViewById(tabLayoutId);
        tabLayout.addTab(tabLayout.newTab().setText("Exception"));
        tabLayout.addTab(tabLayout.newTab().setText("Logcat"));
        tabLayout.setTabGravity(TabLayout.GRAVITY_FILL);
        int pagerId = this.context.getResources().getIdentifier("pager", "id", this.context.getPackageName());

        viewPager = (ViewPager) activity.findViewById(pagerId);

        Pager adapter = new Pager(activity.getSupportFragmentManager(), tabLayout.getTabCount());

        viewPager.setAdapter(adapter);
        viewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

            }

            @Override
            public void onPageSelected(int position) {
                tabLayout.getTabAt(position).select();
                viewPager.setCurrentItem(position);
            }

            @Override
            public void onPageScrollStateChanged(int state) {

            }
        });

        this.addOnTabSelectedListener(tabLayout);
    }

    private void addOnTabSelectedListener(TabLayout tabLayout) {
        tabLayout.addOnTabSelectedListener(this);
    }
    private static void createErrorFile(final Context context) {
        try {
            File errFile = new File(context.getFilesDir(), ERROR_FILE_NAME);
            errFile.createNewFile();
        } catch (IOException e) {
            Log.d("ErrorReport", e.getMessage());
        }
    }

    @Override
    public void onTabSelected(TabLayout.Tab tab) {
        viewPager.setCurrentItem(tab.getPosition());
    }

    @Override
    public void onTabUnselected(TabLayout.Tab tab) {

    }

    @Override
    public void onTabReselected(TabLayout.Tab tab) {
        viewPager.setCurrentItem(tab.getPosition());
    }

    private class Pager extends FragmentStatePagerAdapter {

        int tabCount;

        @SuppressWarnings("deprecation")
        public Pager(FragmentManager fm, int tabCount) {
            super(fm);
            this.tabCount = tabCount;
        }

        @Override
        public Fragment getItem(int position) {
            switch (position) {
            case 0:
                return new ExceptionTab();
            case 1:
                return new LogcatTab();
            default:
                return null;
            }
        }

        @Override
        public int getCount() {
            return tabCount;
        }
    }

    public static class ExceptionTab extends Fragment {
        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
            int exceptionTabId = container.getContext().getResources().getIdentifier("exception_tab", "layout", container.getContext().getPackageName());
            View view = inflater.inflate(exceptionTabId, container, false);

            int txtViewId = container.getContext().getResources().getIdentifier("txtErrorMsg", "id", container.getContext().getPackageName());
            TextView txtErrorMsg = (TextView) view.findViewById(txtViewId);
            txtErrorMsg.setText(exceptionMsg);
            txtErrorMsg.setMovementMethod(new ScrollingMovementMethod());

            int btnCopyExceptionId = container.getContext().getResources().getIdentifier("btnCopyException", "id", container.getContext().getPackageName());
            Button copyToClipboard = (Button) view.findViewById(btnCopyExceptionId);
            copyToClipboard.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    ClipboardManager clipboard = (ClipboardManager) activity.getSystemService(Context.CLIPBOARD_SERVICE);
                    ClipData clip = ClipData.newPlainText("nsError", exceptionMsg);
                    clipboard.setPrimaryClip(clip);
                }
            });

            return view;
        }
    }

    public static class LogcatTab extends Fragment {
        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
            int logcatTabId = container.getContext().getResources().getIdentifier("logcat_tab", "layout", container.getContext().getPackageName());
            View view = inflater.inflate(logcatTabId, container, false);

            int textViewId = container.getContext().getResources().getIdentifier("logcatMsg", "id", container.getContext().getPackageName());
            TextView txtlogcatMsg = (TextView) view.findViewById(textViewId);
            txtlogcatMsg.setText(logcatMsg);

            txtlogcatMsg.setMovementMethod(new ScrollingMovementMethod());

            final String logName = "Log-" + new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss").format(new Date());

            int btnCopyLogcatId = container.getContext().getResources().getIdentifier("btnCopyLogcat", "id", container.getContext().getPackageName());
            Button copyToClipboard = (Button) view.findViewById(btnCopyLogcatId);
            copyToClipboard.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    verifyStoragePermissions(activity);

                    if (!isCheckingForPermissions()) {
                        try {
                            File dir = new File(Environment.getExternalStorageDirectory().getPath() + "/logcat-reports/");
                            dir.mkdirs();

                            File logcatReportFile = new File(dir, logName);
                            FileOutputStream stream = new FileOutputStream(logcatReportFile);
                            OutputStreamWriter writer = new OutputStreamWriter(stream, "UTF-8");
                            writer.write(logcatMsg);
                            writer.close();

                            String logPath = dir.getPath() + "/" + logName;

                            ClipboardManager clipboard = (ClipboardManager) activity.getSystemService(Context.CLIPBOARD_SERVICE);
                            ClipData clip = ClipData.newPlainText("logPath", logPath);
                            clipboard.setPrimaryClip(clip);

                            Toast.makeText(activity, "Path copied to clipboard: " + logPath, Toast.LENGTH_LONG).show();
                        } catch (Exception e) {
                            String err = "Could not write logcat report to sdcard. Make sure you have allowed access to external storage!";
                            Toast.makeText(activity, err, Toast.LENGTH_LONG).show();
                            if (Util.isDebuggableApp(container.getContext())) {
                                e.printStackTrace();
                            }
                        }
                    }
                }
            });

            return view;
        }
    }
}
