#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <jni.h>

#define LOG_TAG "HOOK"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

const char *target_path = "/data/data/com.ry.target/lib/libtarget.so";

void (*_set_step)(int);

int hook_entry(char * a) {
	LOGD("Hook success, pid = %d\n", getpid());
	LOGD("Hello %s\n", a);
	/*
	 void *handle = dlopen(target_path, 2);
	 if (handle == NULL) {
	 LOGD("open target so error!\n");
	 return -1;
	 }

	 void *symbol = dlsym(handle, "set_step");
	 if (symbol == NULL) {
	 LOGD("get set_step error!\n");
	 return -1;
	 }
	 _set_step = symbol;
	 LOGD("_set_step addr :%x\n", _set_step);
	 _set_step(2);
	 */

	JNIEnv* env = NULL;
	int status = (*gVm)->GetEnv(gVm,(void**) &env, JNI_VERSION_1_4);
	if (status != JNI_OK) {
		return -1;
	}
	clazz = env->FindClass("com/ry/target/MainActivity");
	if (clazz == NULL) {
		LOGD("findclass error!");
		return -1;
	}
	methodID = env->GetStaticMethodID(clazz, "makeToast", "()V");
	if (methodID == NULL) {
		LOGD("get methodID error!");
		return -1;
	}
	env->CallStaticVoidMethod(clazz, methodID);
	return 0;
}
JavaVM* gVm = NULL;
jclass clazz = NULL;
jmethodID methodID = NULL;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = -1;
	gVm = vm;
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	assert(env != NULL);
	result = JNI_VERSION_1_4;
	return result;
}

