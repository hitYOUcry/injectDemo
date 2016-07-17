/*
 * target.cpp
 *
 *  Created on: 2016年7月13日
 *      Author: nemoqjzhang
 */
#include <android/log.h>
#include <stdlib.h>
#include <jni.h>

#define  LOG_TAG "AH"
#define  LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, fmt, ##args)

int step = 3;

void set_step(int new_step) {
	step = new_step;
}

jint Java_com_ry_target_JNI_getNumber(JNIEnv* env, jobject thiz, jint number) {
	int res = number + step;
	//LOGD("target >>> step = %d,res = %d",step,res);
	return number + step;
}

JNIEXPORT void JNICALL Java_com_ry_target_JNI_init(JNIEnv* env, jobject thiz) {
	LOGD("target >>> init");
}

void Java_com_ry_target_JNI_destroy(JNIEnv* env, jobject thiz) {
	LOGD("target >>> destroy");
	step = 1;

}

static JavaVM* g_JavaVM;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGD("onLoad");
	JNIEnv* env = NULL;
	jint result = -1;
	g_JavaVM = vm;
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	LOGD("onLoad success");
	return result;
}

void toastFunc(int number) {
	JNIEnv* env = NULL;
	LOGD("a = %d/n", number);
	if (g_JavaVM == NULL) {
		LOGD("gvm is NULL!");
	}

	int status = g_JavaVM->GetEnv((void**) &env, JNI_VERSION_1_4);
	LOGD("status = %d/n", status);

	if (status != JNI_OK) {
		LOGD("get env error!");
		return;
	}
	LOGD("get env success!");
	jclass clazz = env->FindClass("com/ry/target/JNI");
	if (clazz == NULL) {
		LOGD("findclass error!");
		return;
	}
	LOGD("findclass success!");
	jmethodID methodID = env->GetStaticMethodID(clazz, "makeToast", "()V");
	if (methodID == NULL) {
		LOGD("get methodID error!");
		return;
	}
	LOGD("find methodID success!");
	env->CallStaticVoidMethod(clazz, methodID);
	return;
}

