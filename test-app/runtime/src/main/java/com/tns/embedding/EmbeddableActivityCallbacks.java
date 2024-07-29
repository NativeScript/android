package com.tns.embedding;

import androidx.appcompat.app.AppCompatActivity;

public abstract class EmbeddableActivityCallbacks {

    private AppCompatActivity activity;

    public void setActivity(AppCompatActivity activity) {
        this.activity = activity;
    }

    public AppCompatActivity getActivity() {
        return this.activity;
    }

    public abstract void onPostResume();

    public abstract void onStart();

    public abstract void onStop();

    public abstract void onDestroy();

    public abstract void onActivityResult(int param_0, int param_1, android.content.Intent param_2);

    public abstract void onCreate(android.os.Bundle param_0);

    public abstract void onRequestPermissionsResult(int param_0, java.lang.String[] param_1, int[] param_2);

    public abstract void onSaveInstanceState(android.os.Bundle param_0);

    public abstract void onBackPressed();

    public abstract void onNewIntent(android.content.Intent param_0);

    public abstract void onCreate(android.os.Bundle param_0, android.os.PersistableBundle param_1);

    public abstract void onSaveInstanceState(android.os.Bundle param_0, android.os.PersistableBundle param_1);
}
