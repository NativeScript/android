LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := v8_base
#LOCAL_SRC_FILES := libs/libv8_base.a
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := libs/arm/libv8_base.arm.a
else ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_SRC_FILES := libs/arm/libv8_base.arm.a
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := libs/x86/libv8_base.ia32.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := v8_nosnapshot
#LOCAL_SRC_FILES :=  libs/libv8_nosnapshot.a
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_SRC_FILES := libs/arm/libv8_nosnapshot.arm.a
else ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_SRC_FILES := libs/arm/libv8_nosnapshot.arm.a
else ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_SRC_FILES := libs/x86/libv8_nosnapshot.ia32.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPPFLAGS += -std=c++11
LOCAL_MODULE    := NativeScript
LOCAL_SRC_FILES := com_tns_Platform.cpp com_tns_JsDebugger.cpp \
					JEnv.cpp DirectBuffer.cpp \
					JsDebugger.cpp \
					NativeScriptRuntime.cpp MetadataNode.cpp MetadataTreeNode.cpp MetadataReader.cpp \
					MethodCache.cpp JavaObjectArrayCache.cpp \
					JniLocalRef.cpp JniSignatureParser.cpp \
					ArgConverter.cpp JsArgToArrayConverter.cpp JsArgConverter.cpp V8GlobalHelpers.cpp V8StringConstants.cpp \
					FieldAccessor.cpp ArrayElementAccessor.cpp \
					ExceptionUtil.cpp Util.cpp Logger.cpp  \
					ObjectManager.cpp CastFunctions.cpp WeakRef.cpp \
					MetadataMethodInfo.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS    := -llog -landroid
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_STATIC_LIBRARIES := v8_base v8_nosnapshot android-ndk-profiler
#LOCAL_CFLAGS += -pg -DNDK_PROFILER_ENABLED
else
LOCAL_STATIC_LIBRARIES := v8_base v8_nosnapshot 
endif
include $(BUILD_SHARED_LIBRARY)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
# at the end of Android.mk
#$(call import-module,android-ndk-profiler)
endif

