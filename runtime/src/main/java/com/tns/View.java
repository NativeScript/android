package com.tns;

import android.annotation.TargetApi;
import java.util.concurrent.atomic.AtomicInteger;

class View {
	public static final AtomicInteger sNextGeneratedId = new AtomicInteger(1);

	@TargetApi(17)
	public static int generateViewId() {
		if (android.os.Build.VERSION.SDK_INT >= 17) {
			return android.view.View.generateViewId();
		} else {
			for (;;) {
				final int result = sNextGeneratedId.get();

				// aapt-generated IDs have the high byte nonzero; clamp to the range under that.
				int newValue = result + 1;

				// Roll over to 1, not 0.
				if (newValue > 0x00FFFFFF) newValue = 1;

				if (sNextGeneratedId.compareAndSet(result, newValue)) {
					return result;
				}
			}
		}
	}
}