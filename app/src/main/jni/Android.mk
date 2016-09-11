LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog
LOCAL_SRC_FILES:= huarongdao.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_MODULE:= libhrd_jni

include $(BUILD_SHARED_LIBRARY)