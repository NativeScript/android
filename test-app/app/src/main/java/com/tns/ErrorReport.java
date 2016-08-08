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
import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;


class ErrorReport implements TabLayout.OnTabSelectedListener {
	public static final String ERROR_FILE_NAME = "hasError";
	private final AppCompatActivity activity;

	private TabLayout tabLayout;
	private ViewPager viewPager;
	private Context context;
	private static String exceptionMsg;

	private final static String EXTRA_NATIVESCRIPT_ERROR_REPORT = "NativeScriptErrorMessage";
	private final static String EXTRA_ERROR_REPORT_MSG = "msg";
	private final static int EXTRA_ERROR_REPORT_VALUE = 1;

	public ErrorReport(AppCompatActivity activity) {
		this.activity = activity;
		this.context = activity.getApplicationContext();
	}

	static boolean startActivity(final Context context, String errorMessage) {
		final Intent intent = getIntent(context);

		if (intent == null) {
			return false; // (if in release mode) don't do anything
		}

		intent.putExtra(EXTRA_ERROR_REPORT_MSG, errorMessage);

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
		PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);

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
				content = baos.toString("US-ASCII");
			} catch (UnsupportedEncodingException e) {
				content = e.getMessage();
			}
		} finally {
			if (ps != null)
				ps.close();
		}

		return content;
	}

	static Intent getIntent(Context context) {
		Class<?> errorActivityClass = ErrorReportActivity.class;

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
		Context context = activity;
		Intent intent = activity.getIntent();
		final String msg = intent.getStringExtra(EXTRA_ERROR_REPORT_MSG);

		exceptionMsg = msg;

		int errActivityId = this.context.getResources().getIdentifier("error_activity", "layout", this.context.getPackageName());

		activity.setContentView(errActivityId);

		int toolBarId = this.context.getResources().getIdentifier("toolbar", "id", this.context.getPackageName());

		Toolbar toolbar = (Toolbar) activity.findViewById(toolBarId);
		activity.setSupportActionBar(toolbar);

		int tabLayoutId = this.context.getResources().getIdentifier("tabLayout", "id", this.context.getPackageName());

		tabLayout = (TabLayout) activity.findViewById(tabLayoutId);
		tabLayout.addTab(tabLayout.newTab().setText("Exception"));
		tabLayout.addTab(tabLayout.newTab().setText("Logcat"));
		tabLayout.setTabGravity(TabLayout.GRAVITY_FILL);

		int pagerId = this.context.getResources().getIdentifier("pager", "id", this.context.getPackageName());

		viewPager = (ViewPager) activity.findViewById(pagerId);

		Pager adapter = new Pager(activity.getSupportFragmentManager(), tabLayout.getTabCount());

		viewPager.setAdapter(adapter);

		tabLayout.setOnTabSelectedListener(this);
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
			int exceptionTabId = this.getContext().getResources().getIdentifier("exception_tab", "layout", this.getContext().getPackageName());
			View view = inflater.inflate(exceptionTabId, container, false);
			int txtViewId = this.getContext().getResources().getIdentifier("txtErrorMsg", "id", this.getContext().getPackageName());
			TextView txtErrorMsg = (TextView) view.findViewById(txtViewId);
			txtErrorMsg.setText(exceptionMsg);
			txtErrorMsg.setMovementMethod(new ScrollingMovementMethod());
			return view;
		}
	}

	public static class LogcatTab extends Fragment {
		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			int logcatTabId = this.getContext().getResources().getIdentifier("logcat_tab", "layout", this.getContext().getPackageName());
			View view = inflater.inflate(logcatTabId, container, false);
			return view;
		}
	}
}