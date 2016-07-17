LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= entry

LOCAL_SRC_FILES := entry1.cpp \
	JNIUtil.cpp


LOCAL_CFLAGS    := -I./include/ -I./dalvik/vm/ -I./dalvik -DHAVE_LITTLE_ENDIAN

LOCAL_LDFLAGS	:=	-L./lib/  -L$(SYSROOT)/usr/lib -llog -ldvm -landroid_runtime  -lart

LOCAL_SHARED_LIBRARIES := -llog -landroid
include $(BUILD_SHARED_LIBRARY)