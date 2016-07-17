package com.ry.target;

import android.app.Application;

public class TApplication extends Application {

	private static TApplication mInstance;

	@Override
	public void onCreate() {
		super.onCreate();
		mInstance = this;
	}

	public static Application getInstance() {
		return mInstance;
	}
}
