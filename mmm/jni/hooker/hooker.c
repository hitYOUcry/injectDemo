#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <jni.h>


#define LOG_TAG "YMMM"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

const char *target_path = "/data/data/com.ry.target/lib/libtarget.so";

const char *entrySoPath = "/data/libentry.so";
void (*_toastFunc)(int);
void (*_init)(void);

int hook_entry(char * a) {
	LOGD("Hook success, pid = %d\n", getpid());
	LOGD("Hello %s\n", a);
	
	void *handle = dlopen(target_path, 2);
	if (handle == NULL) {
		LOGD("open target so error!\n");
		
		return -1;
	}
	void *symbol = dlsym(handle, "toastFunc");
	if (symbol == NULL) {
		LOGD("find toastFunc error!\n");
		
		return -1;
	}
	_toastFunc = symbol;
	LOGD("toastFunc addr :%x\n", _toastFunc);
	_toastFunc(50);

	dlclose(handle);
	//run here means hookentry was called.
	/*
	handle = NULL;
	symbol = NULL;
	
	handle = dlopen(entrySoPath,2);
	if (handle == NULL) {
		LOGD("open target so error!\n");
		return -1;
	}
	
	symbol = dlsym(handle, "init");
	if (symbol == NULL) {
		LOGD("get init error!\n");
		return -1;
	}
	_init = symbol;
	_init();
	/*
	symbol = NULL;
	symbol = dlsym(handle, "init");
	if(symbol == NULL){
		LOGD("get init error!\n");
		return -1;
	}
	LOGD("init addr :%x\n", _init);
	_init();
	*/
	
	return 0;
}


/*
//common logprint hook.
int hook_entry(char* a){
//	LOGD("Hello(pid = %d)\n^_^ I'm a hook function and I'm not a bad gay. By %s",getpid(),a);
	printf("hooker entry\n");
	printf("Hello(pid = %d)\n^_^ I'm a hook function and I'm not a bad gay. By %s",getpid(),a);
	return 0;
}
*/


/*
//Hook the com.ry.target.
const char *target_path = "/data/data/com.ry.target/lib/libtarget.so";
void (*_set_step)(int);
int hook_entry(char * a) {
	LOGD("Hook success, pid = %d\n", getpid());
	LOGD("Hello %s\n", a);
	

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
	
	_set_step(100);

	return 0;
}
*/



