LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)../../include

LOCAL_CPPFLAGS += $(COMMON_CMAKE_ARGUMENTS)

LOCAL_CFLAGS += -fexceptions -frtti -Wno-unused-lambda-capture

LOCAL_LDLIBS += -landroid -llog -lz -landroid_support

LOCAL_MODULE := libNativeScript


ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
  LOCAL_CPPFLAGS += -DV8_COMPRESS_POINTER
endif


ifeq ($(TARGET_ARCH_ABI),x86_64)
LOCAL_CPPFLAGS += -DV8_COMPRESS_POINTER
endif

ifeq ($(APP_OPTIM),release)
LOCAL_CPPFLAGS +=
else
LOCAL_CPPFLAGS += -g
endif

LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/jni/*.cpp) $(LOCAL_PATH)../../../libs/$(TARGET_ARCH_ABI)/*.a

include $(PREBUILT_STATIC_LIBRARY)

include $(BUILD_SHARED_LIBRARY)