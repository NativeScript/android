package org.nativescript;

import android.app.Application;

import com.tns.ManualInstrumentation;
import com.tns.RuntimeHelper;
import com.tns.embedding.ApplicationHolder;

public class Bootstrap {
    public static void bootstrapNativeScriptRuntime(Application application) {
        ManualInstrumentation.Frame frame = ManualInstrumentation.start("NativeScriptApplication.onCreate");
        try {
            ApplicationHolder.setInstance(application);
            com.tns.Runtime runtime = RuntimeHelper.initRuntime(application);
            if (runtime != null) {
                runtime.run();
            }
        } finally {
            frame.close();
        }
    }
}
