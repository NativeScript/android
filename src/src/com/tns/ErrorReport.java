package com.tns;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.GradientDrawable;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

class ErrorReport
{
	public ErrorReport(Activity activity)
	{
		this.activity = activity;
	}
	
	static Intent getIntent(Context context, Class<?> activityClass, String errorMessage)
	{
		Intent intent = new Intent(context, activityClass);
		
		intent.putExtra(EXTRA_NATIVESCRIPT_ERROR_REPORT, EXTRA_ERROR_REPORT_VALUE);
		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.putExtra(EXTRA_ERROR_REPORT_MSG, errorMessage);
		
		return intent;
	}
	
	static boolean hasIntent(Intent intent)
	{
		int value = intent.getIntExtra(EXTRA_NATIVESCRIPT_ERROR_REPORT, 0);
		
		return value == EXTRA_ERROR_REPORT_VALUE;
	}

	void buildUI()
	{
		Context context = activity;
		
		LinearLayout layout = new LinearLayout(context);
		layout.setOrientation(LinearLayout.VERTICAL);
		activity.setContentView(layout);
		
		TextView txtHeader = new TextView(context);
		txtHeader.setText("Callstack");

		layout.addView(txtHeader);
		
		Intent intent = activity.getIntent();
		String msg = intent.getStringExtra(EXTRA_ERROR_REPORT_MSG);

		TextView txtErrorMsg = new TextView(context);
		txtErrorMsg.setText(msg);
		txtErrorMsg.setHeight(1000);
		txtErrorMsg.setMovementMethod(new ScrollingMovementMethod());
		
        GradientDrawable gd = new GradientDrawable();
        gd.setColor(0xFFFFFFFF);
        gd.setCornerRadius(5);
        gd.setStroke(1, 0xFF000000);
        txtErrorMsg.setBackground(gd);
        
		layout.addView(txtErrorMsg);
		
		Button btnClose = new Button(context);
		btnClose.setText("Close");
		btnClose.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				// TODO: find better way to close the activity
				activity.finish();
				System.exit(0);
			}
		});

		layout.addView(btnClose);
	}
	
	private final Activity activity;
	
	private final static String EXTRA_NATIVESCRIPT_ERROR_REPORT = "NativeScriptErrorMessage";
	private final static String EXTRA_ERROR_REPORT_MSG = "msg";
	private final static int EXTRA_ERROR_REPORT_VALUE = 1;
}
