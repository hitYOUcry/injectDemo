#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <jni.h>
#include "android_runtime/AndroidRuntime.h"

#define LOG_TAG "YMM"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

static JavaVM* g_JavaVM;
static jboolean g_Attach;

const char *target_path = "/data/data/com.ry.target/lib/libtarget.so";

typedef void (*toastFuncType)(int);
toastFuncType _tfunc;
void (*_toastFunc)(int);

#ifdef __cplusplus
extern "C" {//Ｃ＋＋编译器按照C的obj文件格式编译
#endif

/* init jvm obj */
void init()
{
	LOGD("entry init()");
	//JVM init
	g_JavaVM = android::AndroidRuntime::getJavaVM();
	if(g_JavaVM == NULL){
		LOGD("JVM pointer init failed!");
	}
	LOGD("JVM pointer init success");
	
	//JNIEnv init
	int status;
    JNIEnv *env = NULL;
    status = g_JavaVM->GetEnv((void **)&env, JNI_VERSION_1_4);
    if(status < 0)
    {
        status = g_JavaVM->AttachCurrentThread(&env, NULL);
        if(status < 0)
        {
            return;
        }
        g_Attach = true;
    }
	if(env == NULL){
		LOGD("JNIEnv pointer init failed!");
	}
	LOGD("JNIEnv pointer init success");
	
	
	// visiual test
	void *handle = dlopen(target_path, 2);
	if (handle == NULL) {
		LOGD("open target so error!\n");
		
		return;
	}
	/*
	void *symbol = dlsym(handle, "toastFunc");
	if (symbol == NULL) {
		LOGD("find toastFunc error!\n");
		
		return;
	}
	_toastFunc = symbol;
	*/
	_tfunc = (toastFuncType) dlsym(handle, "toastFunc");
	LOGD("toastFunc addr :%x\n", _tfunc);
	_tfunc(200);
	dlclose(handle);
}

/*
// getEnv
JNIEnv* GetEnv()
{
	int status;
    JNIEnv *envnow = NULL;
    status = g_JavaVM->GetEnv((void **)&envnow, JNI_VERSION_1_4);
    if(status < 0)
    {
        status = g_JavaVM->AttachCurrentThread(&envnow, NULL);
        if(status < 0)
        {
            return NULL;
        }
        g_Attach = true;
    }
    return envnow;
}

static void DetachCurrent()
{
	if(g_Attach)
    {
        g_JavaVM->DetachCurrentThread();
        g_Attach = false;
    }
}

void toastFunc(int number) {
	init();
	JNIEnv* env = NULL;
	LOGD("a = %d/n",number);
	if(g_JavaVM == NULL){
		LOGD("gvm is NULL!");
	}
	
	int status = g_JavaVM->GetEnv((void**) &env, JNI_VERSION_1_4);
	LOGD("status = %d/n",status);
	
	if (status != JNI_OK) {
		LOGD("get env error!");
		return;
	}
	LOGD("get env success!");
	/*
	jclass clazz = env->FindClass("com/ry/target/MainActivity");
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
*/

/*
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGD("onLoad");
	number = 100;
	JNIEnv* env = NULL;
	jint result = -1;
	gVm = vm;
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	return result;
}
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGD("onLoad");
	number = 100;
	JNIEnv* env = NULL;
	jint result = -1;
	gVm = vm;
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	return result;
}
*/
#ifdef __cplusplus
}
#endif

