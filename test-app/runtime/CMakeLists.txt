# documentation: https://d.android.com/studio/projects/add-native-code.html

# Command info: https://cmake.org/cmake/help/v3.4/command/cmake_minimum_required.html
cmake_minimum_required(VERSION 3.18.1)

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


set(COMMON_CMAKE_ARGUMENTS "-std=c++20 -Werror -Wno-unused-result -mstackrealign -fexceptions -fno-builtin-stpcpy -fno-rtti -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_ENABLE_REGEXP_INTERPRETER_THREADED_DISPATCH -DV8_EMBEDDED_BUILTINS  -Wno-vla-extension -Wno-deprecated -Wno-vla-cxx-extension")


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
        src/main/cpp/ada
        # Node-API: the vendored upstream sources include their own headers and
        # the shim's Node-internal stand-ins (env-inl.h, util-inl.h) bare.
        src/main/cpp/napi/vendor
        src/main/cpp/napi/shim
)

# The runtime's builtin JavaScript (src/main/cpp/js) embedded into a generated
# C++ table by tools/js2c.mjs. The list is explicit rather than globbed so that
# adding a file is a visible build change; --check-dir fails the build when it
# drifts from the directory contents.
set(RUNTIME_BUILTIN_JS_DIR ${PROJECT_SOURCE_DIR}/src/main/cpp/js)
set(RUNTIME_BUILTIN_JS
        ${RUNTIME_BUILTIN_JS_DIR}/blob-url.js
        ${RUNTIME_BUILTIN_JS_DIR}/error-events.js
        ${RUNTIME_BUILTIN_JS_DIR}/events.js
        ${RUNTIME_BUILTIN_JS_DIR}/inspect.js
        ${RUNTIME_BUILTIN_JS_DIR}/json-helper.js
        ${RUNTIME_BUILTIN_JS_DIR}/node-module.js
        ${RUNTIME_BUILTIN_JS_DIR}/node-url.js
        ${RUNTIME_BUILTIN_JS_DIR}/node-util.js
        ${RUNTIME_BUILTIN_JS_DIR}/ns-module.js
        ${RUNTIME_BUILTIN_JS_DIR}/ns-runtime.js
        ${RUNTIME_BUILTIN_JS_DIR}/ns-util.js
        ${RUNTIME_BUILTIN_JS_DIR}/performance.js
        ${RUNTIME_BUILTIN_JS_DIR}/primordials.js
        ${RUNTIME_BUILTIN_JS_DIR}/require-factory.js
        ${RUNTIME_BUILTIN_JS_DIR}/structured-clone.js
        ${RUNTIME_BUILTIN_JS_DIR}/weak-ref.js
)
set(RUNTIME_BUILTINS_GENERATED_DIR ${PROJECT_SOURCE_DIR}/src/main/cpp/generated)
get_filename_component(RUNTIME_BUILTINS_JS2C ${PROJECT_SOURCE_DIR}/../../tools/js2c.mjs ABSOLUTE)

find_program(NODE_EXECUTABLE NAMES node nodejs)
if (NOT NODE_EXECUTABLE)
    message(FATAL_ERROR "node was not found on PATH; it is required to generate RuntimeBuiltins")
endif ()

add_custom_command(
        OUTPUT ${RUNTIME_BUILTINS_GENERATED_DIR}/RuntimeBuiltins.h
               ${RUNTIME_BUILTINS_GENERATED_DIR}/RuntimeBuiltins.cpp
        COMMAND ${NODE_EXECUTABLE} ${RUNTIME_BUILTINS_JS2C}
                --out-dir ${RUNTIME_BUILTINS_GENERATED_DIR}
                --check-dir ${RUNTIME_BUILTIN_JS_DIR}
                ${RUNTIME_BUILTIN_JS}
        DEPENDS ${RUNTIME_BUILTIN_JS} ${RUNTIME_BUILTINS_JS2C}
        COMMENT "Generating RuntimeBuiltins from src/main/cpp/js"
        VERBATIM
)

# This branch also produces runtime-regular-release.aar, shipped as
# nativescript-regular.aar and selected for apps that set useV8Symbols, so it
# must carry the release flags. Only a local Debug build keeps plain -g.
if (OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD OR NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${COMMON_CMAKE_ARGUMENTS} -O3 -fvisibility=hidden -ffunction-sections -fno-data-sections")
    # CMake appends CMAKE_CXX_FLAGS_<CONFIG> after CMAKE_CXX_FLAGS and clang honours the
    # last -O, so the -O3 above only survives in configs that carry no -O of their own
    # (Debug). AGP builds the release variant as RelWithDebInfo, whose default -O2 would
    # otherwise win.
    string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -O3")
else ()
    set(CMAKE_CXX_FLAGS "${COMMON_CMAKE_ARGUMENTS} -g")
endif ()

if (NOT OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD)
    # Debug builds will include the V8 inspector sources
    add_definitions(-DAPPLICATION_IN_DEBUG)

    set(
        INSPECTOR_SOURCES

        src/main/cpp/com_tns_AndroidJsV8Inspector.cpp
        src/main/cpp/JsV8InspectorClient.cpp
        src/main/cpp/WorkerInspectorClient.cpp
        src/main/cpp/v8_inspector/ns-v8-tracing-agent-impl.cpp
        src/main/cpp/v8_inspector/Utils.cpp
    )
else ()
    # When building in Release mode we do not include the V8 inspector sources
    set(INSPECTOR_SOURCES)
endif ()

# The Node-API test addons back the NapiTests/NapiCoverageTests specs of the
# test app. Local Debug builds only — exactly the variant runtests installs —
# so no published .aar ever carries them.
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT OPTIMIZED_BUILD AND NOT OPTIMIZED_WITH_INSPECTOR_BUILD)
    set(
        NAPI_TEST_SOURCES

        src/main/cpp/napi/tests/NapiTestModule.cpp
        src/main/cpp/napi/tests/NapiCoverageModule.cpp
        src/main/cpp/napi/tests/NapiFastCallsModule.cpp
    )
else ()
    set(NAPI_TEST_SOURCES)
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
        src/main/cpp/BuiltinLoader.cpp
        src/main/cpp/CallbackHandlers.cpp
        src/main/cpp/ConcurrentQueue.cpp
        src/main/cpp/Constants.cpp
        src/main/cpp/CrashBreadcrumbs.cpp
        src/main/cpp/DirectBuffer.cpp
        src/main/cpp/ErrorEvents.cpp
        src/main/cpp/EventLoop.cpp
        src/main/cpp/Events.cpp
        src/main/cpp/FrameCallbacks.cpp
        src/main/cpp/FieldAccessor.cpp
        src/main/cpp/File.cpp
        src/main/cpp/Interop.cpp
        src/main/cpp/IsolateTracked.cpp
        src/main/cpp/JEnv.cpp
        src/main/cpp/DesugaredInterfaceCompanionClassNameResolver.cpp
        src/main/cpp/JType.cpp
        src/main/cpp/JniSignatureParser.cpp
        src/main/cpp/JsArgConverter.cpp
        src/main/cpp/JsArgToArrayConverter.cpp
        src/main/cpp/JSONObjectHelper.cpp
        src/main/cpp/Logger.cpp
        src/main/cpp/ManualInstrumentation.cpp
        src/main/cpp/MetadataMethodInfo.cpp
        src/main/cpp/MetadataNode.cpp
        src/main/cpp/MetadataReader.cpp
        src/main/cpp/MetadataTreeNode.cpp
        src/main/cpp/MetadataEntry.cpp
        src/main/cpp/MethodCache.cpp
        src/main/cpp/ModuleBinding.cpp
        src/main/cpp/ModuleInternal.cpp
        src/main/cpp/ModuleInternalCallbacks.cpp
        src/main/cpp/NativeScriptAssert.cpp
        src/main/cpp/NativeScriptException.cpp
        src/main/cpp/NativeScriptPlatform.cpp
        src/main/cpp/NsBuiltinModules.cpp
        src/main/cpp/NumericCasts.cpp
        src/main/cpp/ObjectManager.cpp
        src/main/cpp/Performance.cpp
        src/main/cpp/Profiler.cpp
        src/main/cpp/ReadWriteLock.cpp
        src/main/cpp/Runtime.cpp
        src/main/cpp/RuntimeState.cpp
        src/main/cpp/SimpleAllocator.cpp
        src/main/cpp/SimpleProfiler.cpp
        src/main/cpp/StructuredClone.cpp
        src/main/cpp/StructuredSerialization.cpp
        src/main/cpp/Util.cpp
        src/main/cpp/V8GlobalHelpers.cpp
        src/main/cpp/V8StringConstants.cpp
        src/main/cpp/WeakRef.cpp
        src/main/cpp/WorkerWrapper.cpp
        src/main/cpp/Timers.cpp
        src/main/cpp/com_tns_AssetExtractor.cpp
        src/main/cpp/com_tns_Runtime.cpp
        src/main/cpp/console/Console.cpp
        src/main/cpp/conversions/JSToJavaConverter.cpp
        src/main/cpp/conversions/objects/JSToJavaObjectsConverter.cpp
        src/main/cpp/conversions/arrays/JSToJavaArraysConverter.cpp
        src/main/cpp/conversions/primitives/JSToJavaPrimitivesConverter.cpp
        src/main/cpp/ada/ada.cpp
        src/main/cpp/URLImpl.cpp
        src/main/cpp/URLSearchParamsImpl.cpp
        src/main/cpp/URLPatternImpl.cpp
        src/main/cpp/HttpLoader.cpp
        src/main/cpp/TraceLog.cpp

        # Node-API: vendored upstream implementation plus the embedder half
        # (env lifecycle, module registry, async work, threadsafe functions)
        src/main/cpp/napi/vendor/js_native_api_v8.cc
        src/main/cpp/napi/NapiEnv.cpp
        src/main/cpp/napi/NapiFastCalls.cpp
        src/main/cpp/napi/NapiRuntime.cpp
        src/main/cpp/napi/NapiThreadSafeFunction.cpp
        src/main/cpp/napi/NodeApiEmbed.cpp

        ${RUNTIME_BUILTINS_GENERATED_DIR}/RuntimeBuiltins.cpp

        # V8 inspector source files will be included only in Release mode
        ${INSPECTOR_SOURCES}

        # Node-API test addons, local Debug builds only
        ${NAPI_TEST_SOURCES}
)

set(NATIVES_BLOB_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/include)

if (OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD)
    set_target_properties(
            NativeScript
            # The version script keeps V8's public API linkable for plugins while hiding
            # the ~60k v8::internal:: symbols the static monolith would otherwise export.
            PROPERTIES LINK_FLAGS "-Wl,--allow-multiple-definition -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exported-symbols.map"
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