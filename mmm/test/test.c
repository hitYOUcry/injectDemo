#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <jni.h>

#define LOG_TAG "TEST"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

static JavaVM* g_JavaVM;

int number = 0;

/*
// getEnv 
static JNIEnv *GetEnv()
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
        g_bAttatedT = true;
    }
    return envnow;
}


static void DetachCurrent()
{
    if(g_bAttatedT)
    {
        g_JavaVM->DetachCurrentThread();
    }
}
*/
void toastFunc(char * a) {
	JNIEnv* env = NULL;
	LOGD("a = %d/n",number);
	if(g_JavaVM == NULL){
		LOGD("gvm is NULL!");
	}
	
	int status = (*g_JavaVM)->GetEnv(g_JavaVM,(void**) &env, JNI_VERSION_1_4);
	LOGD("status = %d/n",status);
	
	if (status != JNI_OK) {
		LOGD("get env error!");
		return;
	}
	LOGD("get env success!");
	jclass clazz = (*env)->FindClass(env,"com/ry/target/MainActivity");
	if (clazz == NULL) {
		LOGD("findclass error!");
		return;
	}
	LOGD("findclass success!");
	jmethodID methodID = (*env)->GetStaticMethodID(env,clazz, "makeToast", "()V");
	if (methodID == NULL) {
		LOGD("get methodID error!");
		return;
	}
	LOGD("find methodID success!");
	(*env)->CallStaticVoidMethod(env,clazz, methodID);
	return;
}

/*
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGD("onLoad");
	number = 100;
	JNIEnv* env = NULL;
	jint result = -1;
	g_JavaVM = vm;
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	return result;
}
*/
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGD("onLoad");
	number = 100;
	JNIEnv* env = NULL;
	jint result = -1;
	g_JavaVM = vm;
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	result = JNI_VERSION_1_4;
	return result;
}

