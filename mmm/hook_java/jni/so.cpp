#include "jni.h"
#include <android_runtime/AndroidRuntime.h>
#include "android/log.h"
#include "stdio.h"
#include "stdlib.h"
#include "MethodHooker.h"
#include <utils/CallStack.h>
#define log(a,b) __android_log_write(ANDROID_LOG_INFO,a,b); // LOG����:info
#define log_(b) __android_log_write(ANDROID_LOG_INFO,"JNI_LOG_INFO",b); // LOG����:info


extern "C" void InjectInterface(char*arg){
	log_("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	log_("*-*-*-*-*-* Injected so *-*-*-*-*-*-*-*");
	log_("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	Hook();
	log_("*-*-*-*-*-*-*- End -*-*-*-*-*-*-*-*-*-*");
}

extern "C" JNIEXPORT jstring JNICALL Java_com_example_testar_InjectApplication_test(JNIEnv *env, jclass clazz)
{
    return env->NewStringUTF("haha ");;
}
