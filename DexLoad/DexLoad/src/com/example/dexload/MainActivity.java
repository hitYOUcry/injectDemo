package com.example.dexload;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import dalvik.system.DexClassLoader;

public class MainActivity extends Activity {
	private Method methodTest;
	private final String TAG = "DexLoad";
	private DexClassLoader mDex;
	private NativeLoad mLoad;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		loadDex();
	//	test();
		mLoad = new NativeLoad("test.dex",getApplicationContext().getFilesDir().getParent(),getAssets());
		mLoad.test(this);
	}

	private String copyDex(String dexName) {
		AssetManager as = getAssets();
		String path = getFilesDir() + File.separator + dexName;
		Log.i(TAG, path);
		try {
			FileOutputStream out = new FileOutputStream(path);
			InputStream is = as.open(dexName);
			int count = is.available();
			while (count > 0) {
				byte[] buffer = new byte[count > 1024 ? 1024 : count];
				int len = is.read(buffer);
				out.write(buffer);
				count -= len;
			}
			out.close();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
			return "";
		}
		return path;
	}

	public void loadDex() {
		String path = copyDex("test.dex");
		if (path.isEmpty())
			return;
		mDex = new DexClassLoader(path, getCacheDir().toString(), null,
				getClassLoader());
	}

	public void test() {
		try {
			Class TestDex = mDex.loadClass("com.example.dextest.Test1");
			Constructor localConstructor = TestDex
					.getConstructor(new Class[] {});
			Object instance = localConstructor.newInstance(new Object[] {});
			methodTest = TestDex.getDeclaredMethod("test",
					new Class[] { Activity.class });
			methodTest.setAccessible(true);
			methodTest.invoke(instance, new Object[] { this });
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
