package com.tns;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import android.app.Activity;
import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.GradientDrawable;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

class ErrorReport
{
	public static final String ERROR_FILE_NAME = "hasError";
	private final Activity activity;
	
	private final static String EXTRA_NATIVESCRIPT_ERROR_REPORT = "NativeScriptErrorMessage";
	private final static String EXTRA_ERROR_REPORT_MSG = "msg";
	private final static int EXTRA_ERROR_REPORT_VALUE = 1;
	
	public ErrorReport(Activity activity)
	{
		this.activity = activity;
	}
	
	static boolean startActivity(final Context context, String errorMessage)
	{
		final Intent intent = getIntent(context);
		if(intent == null)
		{
			return false; //(if in release mode) don't do anything
		}
		
		intent.putExtra(EXTRA_ERROR_REPORT_MSG, errorMessage);
		
		CreateErrorFile(context);
		
		startPendingErrorActivity(context, intent);

		killProcess(context);	
		
		return true;
	}
	
	static void killProcess(Context context)
	{
		// finish current activity and all below it first
		if (context instanceof Activity) {
			((Activity)context).finishAffinity();
		}

		//kill process
		android.os.Process.killProcess(android.os.Process.myPid());
	}
	
	static void startPendingErrorActivity(Context context, Intent intent)
	{
		PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);
		
		try
		{
			pendingIntent.send(context, 0, intent);
		}
		catch (CanceledException e)
		{
			if (Platform.IsLogEnabled) Log.e(Platform.DEFAULT_LOG_TAG, "Couldn't send pending intent! Exception: " + e.getMessage());
		}
	}
	
	static String getErrorMessage(Throwable ex)
	{
		String content;
		PrintStream ps = null;

		try
		{
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			ps = new PrintStream(baos);
			ex.printStackTrace(ps);

			try
			{
				content = baos.toString("US-ASCII");
			}
			catch (UnsupportedEncodingException e)
			{
				content = e.getMessage();
			}
		}
		finally
		{
			if (ps != null)
				ps.close();
		}
		
		return content;
	}
	
	static Intent getIntent(Context context)
	{
		Class<?> errorActivityClass = Platform.getErrorActivityClass(); //can be null or can be provided beforehand
				
		//if in debug and errorActivityClass is not provided use ErrorReportActivity class
		if(errorActivityClass == null && JsDebugger.shouldEnableDebugging(context)){
			errorActivityClass = ErrorReportActivity.class;
		}

		//if not in debug mode should return null and use the errorActivityClass implementation provided
		if(errorActivityClass == null)
		{
			return null;
		}
		
		Intent intent = new Intent(context, errorActivityClass);
		
		intent.putExtra(EXTRA_NATIVESCRIPT_ERROR_REPORT, EXTRA_ERROR_REPORT_VALUE);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
		
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
				activity.finish();
			}
		});

		layout.addView(btnClose);
	}
	
	private static void CreateErrorFile(final Context context)
	{
		try
		{
			File errFile = new File(context.getFilesDir(), ERROR_FILE_NAME);
			errFile.createNewFile();
		}
		catch (IOException e)
		{
			Log.d(Platform.DEFAULT_LOG_TAG, e.getMessage());
		}
	}
}
