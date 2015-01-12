package com.tns;

import android.app.Activity;
import android.os.Bundle;

public class ErrorReportActivity extends Activity
{
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		new ErrorReport(this).buildUI();
	}
}
