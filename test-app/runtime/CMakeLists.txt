# documentation: https://d.android.com/studio/projects/add-native-code.html

# Command info: https://cmake.org/cmake/help/v3.4/command/cmake_minimum_required.html
cmake_minimum_required(VERSION 3.4.1)

project(NativeScriptAndroidRuntime)

# Add the ccache to the build system
find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND AND (USE_CCACHE))
    MESSAGE(STATUS "## Using CCache when building!")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif (CCACHE_FOUND AND (USE_CCACHE))

# Command info: https://cmake.org/cmake/help/v3.4/command/message.html
# we pass the android_ndk_root from gradle because for some reason
# "-DANDROID_STL=c++_static" is just not enough for clang++ to find some libraries in the ndk
MESSAGE(STATUS "## ANDROID_NDK_ROOT: " ${ANDROID_NDK_ROOT})

set(COMMON_CMAKE_ARGUMENTS "-std=c++14 -Werror -Wno-unused-result -mstackrealign -fexceptions -fno-builtin-stpcpy -fno-rtti -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_ENABLE_REGEXP_INTERPRETER_THREADED_DISPATCH -DV8_EMBEDDED_BUILTINS")

if("${ANDROID_ABI}" MATCHES "arm64-v8a$" OR "${ANDROID_ABI}" MATCHES "x86_64$")
    # Enable pointer compression on 64 bit platforms
    set(COMMON_CMAKE_ARGUMENTS "${COMMON_CMAKE_ARGUMENTS} -DV8_COMPRESS_POINTERS")
endif()

# AOSP has switched to using LLD by default and the NDK will use it by default in the next release.
# BFD and Gold will be removed once LLD has been through a release cycle with no major unresolved issues (estimated r21)
# Note: lld does not currently work on Windows: https://github.com/android-ndk/ndk/issues/888

# On MacOS using LLD seems problematic as it does not add the correct path for the libNativeScript.so dSYM.
# This issue affects debugging the C++ part of the runtime.
# Manually performing "add-dsym <lib-path>" in the LLDB console seems to fix that.
# We should try using LLD again once it's the default linker for the NDK.

#if (NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
#  MESSAGE(STATUS "## Using LLD linker")
#  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
#else ()
#  MESSAGE(STATUS "## Using default linker")
#endif ()

#add_library( v8_shared STATIC IMPORTED )
#set_target_properties( v8_shared PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8android.so )

# Command info: https://cmake.org/cmake/help/v3.4/command/include_directories.html
include_directories(
        # V8 uses a custom STL compiled with the LIBCPP_ABI_UNSTABLE flag. To maintain the ABI
        # compatibility we use this same STL headers for building the runtime
        src/main/cpp/include/libc++
        src/main/cpp
        src/main/cpp/include
        src/main/cpp/v8_inspector
        # This folder contains V8's built-in natives and snapshot blobs
        src/main/libs/${ANDROID_ABI}/include
)

if (OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD)
    set(CMAKE_CXX_FLAGS "${COMMON_CMAKE_ARGUMENTS} -O3 -fvisibility=hidden -ffunction-sections -fno-data-sections")
else ()
    set(CMAKE_CXX_FLAGS "${COMMON_CMAKE_ARGUMENTS} -g")
endif ()

# Command info: https://cmake.org/cmake/help/v3.4/command/add_library.html
# Creates(shared static) and names a library given relative sources
# Gradle automatically packages shared libraries with your APK.
add_library(
        # Sets the name of the library. When it's built you can find it with lib prefix libNativeScript.so
        NativeScript

        # Sets the library as a shared library.
        SHARED

        # Runtime source
        src/main/cpp/ArgConverter.cpp
        src/main/cpp/ArrayBufferHelper.cpp
        src/main/cpp/ArrayElementAccessor.cpp
        src/main/cpp/ArrayHelper.cpp
        src/main/cpp/AssetExtractor.cpp
        src/main/cpp/CallbackHandlers.cpp
        src/main/cpp/Constants.cpp
        src/main/cpp/DirectBuffer.cpp
        src/main/cpp/FieldAccessor.cpp
        src/main/cpp/File.cpp
        src/main/cpp/IsolateDisposer.cpp
        src/main/cpp/JEnv.cpp
        src/main/cpp/DesugaredInterfaceCompanionClassNameResolver.cpp
        src/main/cpp/JType.cpp
        src/main/cpp/JniSignatureParser.cpp
        src/main/cpp/JsArgConverter.cpp
        src/main/cpp/JsArgToArrayConverter.cpp
        src/main/cpp/JSONObjectHelper.cpp
        src/main/cpp/Logger.cpp
        src/main/cpp/ManualInstrumentation.cpp
        src/main/cpp/MessageLoopTimer.cpp
        src/main/cpp/MetadataMethodInfo.cpp
        src/main/cpp/MetadataNode.cpp
        src/main/cpp/MetadataReader.cpp
        src/main/cpp/MetadataTreeNode.cpp
        src/main/cpp/MethodCache.cpp
        src/main/cpp/ModuleInternal.cpp
        src/main/cpp/NativeScriptException.cpp
        src/main/cpp/NumericCasts.cpp
        src/main/cpp/ObjectManager.cpp
        src/main/cpp/Profiler.cpp
        src/main/cpp/ReadWriteLock.cpp
        src/main/cpp/Runtime.cpp
        src/main/cpp/SimpleAllocator.cpp
        src/main/cpp/SimpleProfiler.cpp
        src/main/cpp/Util.cpp
        src/main/cpp/V8GlobalHelpers.cpp
        src/main/cpp/V8StringConstants.cpp
        src/main/cpp/WeakRef.cpp
        src/main/cpp/com_tns_AssetExtractor.cpp
        src/main/cpp/com_tns_Runtime.cpp
        src/main/cpp/console/Console.cpp
        src/main/cpp/conversions/JSToJavaConverter.cpp
        src/main/cpp/conversions/objects/JSToJavaObjectsConverter.cpp
        src/main/cpp/conversions/arrays/JSToJavaArraysConverter.cpp
        src/main/cpp/conversions/primitives/JSToJavaPrimitivesConverter.cpp

        # V8 inspector source files will be included only in Release mode
        ${INSPECTOR_SOURCES}
)

set(NATIVES_BLOB_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/include)

if (OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD)
    set_target_properties(
            NativeScript
            PROPERTIES LINK_FLAGS -Wl,--allow-multiple-definition -Wl,--exclude-libs=ALL -Wl,--gc-sections
            INTERFACE_INCLUDE_DIRECTORIES NATIVES_BLOB_INCLUDE_DIRECTORIES
    )
else ()
    set_target_properties(
            NativeScript
            PROPERTIES LINK_FLAGS -Wl,--allow-multiple-definition
            INTERFACE_INCLUDE_DIRECTORIES NATIVES_BLOB_INCLUDE_DIRECTORIES
    )
endif ()

MESSAGE(STATUS "# General cmake Info")
MESSAGE(STATUS "# PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "# CMAKE_VERSION: " ${CMAKE_VERSION})
MESSAGE(STATUS "# CMAKE_C_COMPILER_ID: " ${CMAKE_C_COMPILER_ID})
MESSAGE(STATUS "# CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID})
MESSAGE(STATUS "# CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS})
MESSAGE(STATUS "# CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})

# Command info: https://cmake.org/cmake/help/v3.4/command/target_link_libraries.html
# linking v8 and custom STL libraries to the runtime (NativeScript library)
target_link_libraries(NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libzip.a)
target_link_libraries(NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_monolith.a)

if("${ANDROID_ABI}" MATCHES "armeabi-v7a$" OR "${ANDROID_ABI}" MATCHES "x86$")
    # On API Level 19 and lower we need to link with android_support
    # because it contains some implementation of functions such as "strtoll" and "strtoul"
    MESSAGE(STATUS "# Linking with libandroid_support.a")
    target_link_libraries(NativeScript ${ANDROID_NDK_ROOT}/sources/cxx-stl/llvm-libc++/libs/${ANDROID_ABI}/libandroid_support.a)
endif()

# Command info: https://cmake.org/cmake/help/v3.4/command/find_library.html
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library(system-log log)
find_library(system-z z)
find_library(system-android android)

# Command info: https://cmake.org/cmake/help/v3.4/command/target_link_libraries.html
# Specifies libraries CMake should link to your target library.
target_link_libraries(NativeScript ${system-log} ${system-z} ${system-android})