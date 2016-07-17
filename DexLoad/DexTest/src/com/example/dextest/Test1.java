package com.example.dextest;

import android.app.Activity;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.TextView;

public class Test1 {
	public void test(Activity ac) {
		TextView tv = new TextView(ac);
		FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
				FrameLayout.LayoutParams.WRAP_CONTENT,
				FrameLayout.LayoutParams.WRAP_CONTENT);
		params.topMargin = 0;
		params.gravity = Gravity.TOP | Gravity.CENTER_HORIZONTAL;
		tv.setText("Test1 textview!!!!");
		ac.addContentView(tv, params);
	}
}
