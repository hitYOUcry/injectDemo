#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <jni.h>

#include "JNIUtil.h"
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include "android_runtime/AndroidRuntime.h"

#define LOG_TAG "YMM"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

static JavaVM* g_JavaVM;
static jboolean g_Attach;

const char *target_path = "/data/data/com.ry.target/lib/libtarget.so";
const char *dexCachePath = "/data/data/com.ry.target/cache";

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
	//JVM init start------  //
	g_JavaVM = android::AndroidRuntime::getJavaVM();
	if(g_JavaVM == NULL){
		LOGD("JVM pointer init failed!");
	}
	LOGD("JVM pointer init success");
	//JVM init end------  //
	
	//JNIEnv init start------  //
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
	//JNIEnv init end ------  //
	
	// Dex Load start ------  //
	JNIUtil::SetJavaVm(g_JavaVM);
	JNIUtil util;
	// get classLoader
	LOGD("getClassLoaderStart");
	jclass classloaderClass = env->FindClass("java/lang/ClassLoader");
	jmethodID getsysloaderMethod = env->GetStaticMethodID(classloaderClass,
			"getSystemClassLoader", "()Ljava/lang/ClassLoader;");
	jobject loader = env->CallStaticObjectMethod(classloaderClass,
			getsysloaderMethod);
	LOGD("getClassLoaderEnd");
	
	//查找DexClassLoader类并且创建对象生成优化后的dex
	LOGD("DexClassLoader Start");
	jstring jDexPath = util.String2Jstring("/data/test.dex");
	jstring jDestDexPath = util.String2Jstring("/data/data/com.ry.target/cache");
	jclass dexLoaderClass = env->FindClass("dalvik/system/DexClassLoader");
	jmethodID initDexLoaderMethod = env->GetMethodID(dexLoaderClass, "<init>",
			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
	jobject dexLoader = env->NewObject(dexLoaderClass, initDexLoaderMethod,
			jDexPath, jDestDexPath , NULL, loader);
	LOGD("DexClassLoader End");
	
	//use dexLoad load Class :com.example.dextest.Test3.
	LOGD("load Class Start");
	jmethodID loadClassMethod = env->GetMethodID(classloaderClass, "loadClass",
			"(Ljava/lang/String;)Ljava/lang/Class;");
	jstring Test3ClassName = util.String2Jstring("com.example.dextest.Test3");
	jclass test3Clazz = (jclass) env->CallObjectMethod(dexLoader,
			loadClassMethod, Test3ClassName);
	LOGD("load Class End");

	//build Test3 object,add call sum functions
	LOGD("new Class object Start");
	jmethodID initFuncTest = env->GetMethodID(test3Clazz, "<init>", "()V"); //构造函数ID
	jobject objTest3 = env->NewObject(test3Clazz, initFuncTest);
	jmethodID Test_method = env->GetMethodID(test3Clazz, "test",
			"()V");
	jmethodID Add_method = env->GetMethodID(test3Clazz, "add",
			"(II)I");
	env->CallVoidMethod(objTest3, Test_method);
	jint sum = 0;
	LOGD("divide");
	sum = env->CallIntMethod(objTest3,Add_method,2,7);
//	(int)env->CallObjectMethod(objTest3,Add_method,2,7);
	LOGD("new Class object End");
	
	// visiual test
	void *handle = dlopen(target_path, 2);
	if (handle == NULL) {
		LOGD("open target so error!\n");
		
		return;
	}
	
	_tfunc = (toastFuncType) dlsym(handle, "toastFunc");
	LOGD("toastFunc addr :%x\n", _tfunc);
	_tfunc(sum);
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

