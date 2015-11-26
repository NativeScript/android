LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := v8_base
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := libs/arm/libv8_base.a
else ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_SRC_FILES := libs/arm/libv8_base.a
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := libs/x86/libv8_base.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := v8_libplatform
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := libs/arm/libv8_libplatform.a
else ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_SRC_FILES := libs/arm/libv8_libplatform.a
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := libs/x86/libv8_libplatform.a
endif
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := v8_libbase
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := libs/arm/libv8_libbase.a
else ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_SRC_FILES := libs/arm/libv8_libbase.a
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := libs/x86/libv8_libbase.a
endif
include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE    := v8_nosnapshot
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := libs/arm/libv8_nosnapshot.a
else ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_SRC_FILES := libs/arm/libv8_nosnapshot.a
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := libs/x86/libv8_nosnapshot.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := zip
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := libs/arm/libzip.a
else ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_SRC_FILES := libs/arm/libzip.a
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := libs/x86/libzip.a
endif
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_CPPFLAGS += -std=c++11
LOCAL_MODULE    := NativeScript
LOCAL_SRC_FILES :=	com_tns_AssetExtractor.cpp AssetExtractor.cpp\
					com_tns_Platform.cpp NativePlatform.cpp \
					com_tns_JsDebugger.cpp com_tns_NativeScriptActity.cpp \
					JEnv.cpp DirectBuffer.cpp NativeScriptException.cpp \
					JsDebugger.cpp SimpleAllocator.cpp \
					NativeScriptRuntime.cpp MetadataNode.cpp MetadataTreeNode.cpp MetadataReader.cpp \
					MethodCache.cpp JavaObjectArrayCache.cpp \
					JniLocalRef.cpp JniSignatureParser.cpp \
					ArgConverter.cpp JsArgToArrayConverter.cpp JsArgConverter.cpp V8GlobalHelpers.cpp V8StringConstants.cpp \
					FieldAccessor.cpp ArrayElementAccessor.cpp \
					Util.cpp Logger.cpp Profiler.cpp \
					ObjectManager.cpp NumericCasts.cpp WeakRef.cpp \
					MetadataMethodInfo.cpp SimpleProfiler.cpp JType.cpp File.cpp Module.cpp Constants.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS    := -llog -landroid -lz
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_STATIC_LIBRARIES := v8_base v8_libplatform v8_libbase v8_nosnapshot android-ndk-profiler zip
#LOCAL_CFLAGS += -pg -DNDK_PROFILER_ENABLED
else
LOCAL_STATIC_LIBRARIES := v8_base v8_libplatform v8_libbase v8_nosnapshot zip
endif
include $(BUILD_SHARED_LIBRARY)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
# at the end of Android.mk
#$(call import-module,android-ndk-profiler)
endif

