#include <android/log.h>
#include <stdlib.h>
#include <jni.h>


#define  LOG_TAG "TARGET"
#define  LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, fmt, ##args)

int step = 1;

void set_step(int new_step) {
	step = new_step;
}

jint Java_com_ry_target_JNI_getNumber(JNIEnv* env, jobject thiz, jint number) {
	int res = number + step;
	//LOGD("target >>> step = %d,res = %d",step,res);
	return number + step;
}

void Java_com_ry_target_JNI_init(JNIEnv* env, jobject thiz) {
	LOGD("target >>> init");
}

void Java_com_ry_target_JNI_destroy(JNIEnv* env, jobject thiz) {
	LOGD("target >>> destroy");
	step = 1;

}

static JavaVM* g_JavaVM;
static jobject gClassLoader;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGD("onLoad");

	//JNIEnv init
	JNIEnv* env = NULL;
	jint result = -1;
	g_JavaVM = vm;
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}

	/*
	 //ClassLoader
	 jclass tClazz = (*env)->FindClass(env,"com/ry/target/JNI");
	 jclass classLoaderClazz = (*env)->FindClass(env,"java/lang/ClassLoader");
	 jmethodID getClassLoaderMethod = (*env)->GetMethodID(env,tClazz, "getClassLoader",
	 "()Ljava/lang/ClassLoader;");
	 gClassLoader =  (*env)->CallObjectMethod(env,tClazz, getClassLoaderMethod);
	 */
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
	int status = (*g_JavaVM)->GetEnv(g_JavaVM, (void**) &env, JNI_VERSION_1_4);
	if (status != JNI_OK) {
		LOGD("get env error!");
		return;
	}
	LOGD("get env success!");

	//Attach 线程
	/*
	 if ((*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, NULL) != JNI_OK) {
	 LOGD("get env error!");
	 return;
	 }
	 */
	LOGD("get env success!");
	jclass clazz = (*env)->FindClass(env, "android/app/ApplicationLoaders");
	LOGD("applicationLoadsers success");
	clazz = (*env)->FindClass(env, "java/lang/ClassLoader");
	jmethodID methodId = (*env)->GetStaticMethodID(env, clazz,
			"getSystemClassLoader", "()Ljava/lang/ClassLoader;");

	jclass dexLoaderClass = (*env)->FindClass(env,
			"dalvik/system/DexClassLoader");
	jmethodID initDexLoaderMethod =
			(*env)->GetMethodID(env, dexLoaderClass, "<init>",
					"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
	LOGD("find sth success");
	set_step(number);
	/*
	 clazz = (*env)->FindClass(env, "com/ry/target/JNI");

	 if (clazz == NULL) {
	 LOGD("findclass error!");
	 goto error;
	 }

	 LOGD("findclass success!");
	 jmethodID methodID = (*env)->GetStaticMethodID(env, clazz, "makeToast",
	 "()V");
	 if (methodID == NULL) {
	 LOGD("get methodID error!");
	 goto error;
	 }
	 LOGD("find methodID success!");
	 (*env)->CallStaticVoidMethod(env, clazz, methodID);
	 */

	/*
	 error:
	 //Detach主线程
	 LOGD("errorLog done");
	 if ((*g_JavaVM)->DetachCurrentThread(g_JavaVM) != JNI_OK) {
	 LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
	 }
	 */
}

