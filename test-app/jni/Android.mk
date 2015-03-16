LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := android-runtime-testapp
LOCAL_SRC_FILES := android-runtime-testapp.cpp
include $(BUILD_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := NativeScript
#LOCAL_SRC_FILES := ../../src/obj/local/$(TARGET_ARCH_ABI)/libNativeScript.so
#include $(PREBUILT_SHARED_LIBRARY)
