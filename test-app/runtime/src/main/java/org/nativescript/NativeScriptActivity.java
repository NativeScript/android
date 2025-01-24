package org.nativescript;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.tns.embedding.CallbacksStore;
import com.tns.embedding.EmbeddableActivityCallbacks;

import org.nativescript.android.runtime.R;

public class NativeScriptActivity extends AppCompatActivity {

    private final EmbeddableActivityCallbacks callbacks = CallbacksStore.getActivityCallbacks();
    static View nativeScriptView = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        callbacks.setActivity(this);
        callbacks.onCreate(savedInstanceState);
//        drawNativeScriptApp(savedInstanceState);
    }

    @Override
    public void onCreate(android.os.Bundle savedInstanceState, android.os.PersistableBundle persistableBundle) {
        super.onCreate(savedInstanceState, persistableBundle);
        callbacks.setActivity(this);
        callbacks.onCreate(savedInstanceState, persistableBundle);
//        drawNativeScriptApp(savedInstanceState);
    }

    private void drawNativeScriptApp(Bundle savedInstanceState) {
        setContentView(R.layout.main);
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .replace(R.id.container, new NativeScriptFragment())
                    .commitNow();
        }
    }

    @Override
    protected void onPostResume() {
        super.onPostResume();
        callbacks.onPostResume();
    }

    @Override
    protected void onStart() {
        super.onStart();
        callbacks.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
        callbacks.onStop();
    }

    @Override
    protected void onDestroy() {
        try {
            callbacks.onDestroy();
            nativeScriptView = null;
        } finally {
            super.onDestroy();
        }
    }

    @Override
    protected void onActivityResult(int param_0, int param_1, android.content.Intent param_2) {
        super.onActivityResult(param_0, param_1, param_2);
        callbacks.onActivityResult(param_0, param_1, param_2);
    }

    @SuppressLint("MissingSuperCall")
    @Override
    public void onRequestPermissionsResult(int param_0, java.lang.String[] param_1, int[] param_2) {
        callbacks.onRequestPermissionsResult(param_0, param_1, param_2);
    }

    @Override
    protected void onSaveInstanceState(android.os.Bundle param_0) {
        super.onSaveInstanceState(param_0);
        callbacks.onSaveInstanceState(param_0);
    }

    @Override
    public void onBackPressed() {
        callbacks.onBackPressed();
    }

    @Override
    protected void onNewIntent(android.content.Intent param_0) {
        super.onNewIntent(param_0);
        super.setIntent(param_0);
        callbacks.onNewIntent(param_0);
    }

    @Override
    public void onSaveInstanceState(android.os.Bundle param_0, android.os.PersistableBundle param_1) {
        super.onSaveInstanceState(param_0, param_1);
    }

    public void superOnBackPressed() {
        super.onBackPressed();
    }
}
