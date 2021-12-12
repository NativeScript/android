LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libv8inspector

LOCAL_C_INCLUDES := $(LOCAL_PATH)../../include

LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/jni/*.cpp)

include $(BUILD_SHARED_LIBRARY)