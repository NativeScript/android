package com.tns.tests;

import android.util.Log;

class PackageProtectedClass implements Runnable {
    @Override
    public void run() {
        Log.d("JS", String.format("Run:  I am %s %s", this.getClass(), this));
    }

    private void privateThing(){}

    void packagePrivateThing(){}
}

public class MyFactoryPatternPrivate {
    public PackageProtectedClass getIt() {
        return new PackageProtectedClass();
    }
}