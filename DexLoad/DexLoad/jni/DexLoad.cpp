#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "JNIUtil.h"
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <unistd.h>
#include <fcntl.h>
extern "C"
{
string copyDexToData(JNIEnv* env, jobject asset, string dexName,
		string dataPath);
void Java_com_example_dexload_NativeLoad_loadDex(JNIEnv* env, jobject obj,
		jstring dexName, jstring dataPath, jobject asset)
{
	JavaVM* jvm;
	env->GetJavaVM(&jvm);
	JNIUtil::SetJavaVm(jvm);
	JNIUtil util;
	string strDexName = util.Jstring2String(dexName);
	string strdataPath = util.Jstring2String(dataPath);
	string strDexPath = copyDexToData(env, asset, strDexName, strdataPath);
	string strDestDexPath = strdataPath + "/cache";
	LOGI("%s",strDexPath.c_str());
	LOGI("%s",strDestDexPath.c_str());
	jstring jDexPath = util.String2Jstring(strDexPath.c_str());
	jstring jDestDexPath = util.String2Jstring(strDestDexPath.c_str());
	//����ClassLoader�ಢ���þ�̬������ȡϵͳ��classloader����
	jclass classloaderClass = env->FindClass("java/lang/ClassLoader");
	jmethodID getsysloaderMethod = env->GetStaticMethodID(classloaderClass,
			"getSystemClassLoader", "()Ljava/lang/ClassLoader;");
	jobject loader = env->CallStaticObjectMethod(classloaderClass,
			getsysloaderMethod);

	//����DexClassLoader�ಢ�Ҵ������������Ż����dex
	jclass dexLoaderClass = env->FindClass("dalvik/system/DexClassLoader");
	jmethodID initDexLoaderMethod = env->GetMethodID(dexLoaderClass, "<init>",
			"(Ljava/lang/String;Ljava/lang/String;"
					"Ljava/lang/String;Ljava/lang/ClassLoader;)V");
	jobject dexLoader = env->NewObject(dexLoaderClass, initDexLoaderMethod,
			jDexPath, jDestDexPath , NULL, loader);

	//��ֵ��java�˵�DexClassLoader����
	jclass native = env->GetObjectClass(obj);
	jfieldID loadID = env->GetFieldID(native, "mDex",
			"Ldalvik/system/DexClassLoader;");
	env->SetObjectField(obj, loadID, dexLoader);
}
void Java_com_example_dexload_NativeLoad_test(JNIEnv* env, jobject obj,
		jobject ac)
{

	//��ȡjava�˵�ClassLoader����
	jclass native = env->GetObjectClass(obj);
	jfieldID loadID = env->GetFieldID(native, "mDex",
			"Ldalvik/system/DexClassLoader;");
	jobject load = env->GetObjectField(obj, loadID);

	//����ClassLoader�ķ���loadClass����dex�е�Test1��
	jclass classloaderClass = env->GetObjectClass(load);
	jmethodID loadClassMethod = env->GetMethodID(classloaderClass, "loadClass",
			"(Ljava/lang/String;)Ljava/lang/Class;");
	JNIUtil util;
	jstring Test1ClassName = util.String2Jstring("com.example.dextest.Test1");
	jclass javaClientClass = (jclass) env->CallObjectMethod(load,
			loadClassMethod, Test1ClassName);

	//����Test1��Ķ����ҵ������е�test����
	jmethodID initFuncTest = env->GetMethodID(javaClientClass, "<init>", "()V"); //���캯��ID
	jobject objTest1 = env->NewObject(javaClientClass, initFuncTest);
	jmethodID Test_method = env->GetMethodID(javaClientClass, "test",
			"(Landroid/app/Activity;)V");
	if (Test_method != NULL)
		env->CallVoidMethod(objTest1, Test_method, ac);
}
string copyDexToData(JNIEnv* env, jobject asset, string dexName,
		string dataPath)
{
	AAssetManager* asMg = AAssetManager_fromJava(env, asset);
	AAsset* as = AAssetManager_open(asMg, dexName.c_str(), AASSET_MODE_UNKNOWN);
	if (as == NULL)
	{
		LOGE("%s not found in assets!",dexName.c_str());
		return "";
	}
	string strDexPath = dataPath + "/files/" + dexName;
	int len = AAsset_getLength(as);
	int file = open(strDexPath.c_str(), O_WRONLY | O_CREAT, 0755);
	if (file < 0)
	{
		AAsset_close(as);
		LOGE("Open %s File Error!",strDexPath.c_str());
		return "";
	}
	char* buf = new char[1024];
	while (len > 0)
	{
		memset(buf, 0, 1024);
		int n = AAsset_read(as, buf, 1024);
		if (n < 0)
			break;
		write(file, buf, n);
		len -= n;
	}
	delete[] buf;
	AAsset_close(as);
	close(file);
	return strDexPath;
}
}
