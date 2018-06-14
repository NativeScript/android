# documentation: https://d.android.com/studio/projects/add-native-code.html

# Command info: https://cmake.org/cmake/help/v3.4/command/cmake_minimum_required.html
cmake_minimum_required(VERSION 3.4.1)

# Add the ccache to the build system
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND AND (USE_CCACHE))
  MESSAGE( STATUS "## Using CCache when building!")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND AND (USE_CCACHE))

# Command info: https://cmake.org/cmake/help/v3.4/command/message.html
# we pass the android_ndk_root from gradle because for some reason
# "-DANDROID_STL=c++_static" is just not enough for clang++ to find some libraries in the ndk
MESSAGE( STATUS "## ANDROID_NDK_ROOT: " ${ANDROID_NDK_ROOT} )

if ( ANDROID_ABI MATCHES "arm64" )
    set(ARCH_INCLUDE_DIR "aarch64-linux-android")
else()
    set(ARCH_INCLUDE_DIR "arm-linux-androideabi")
endif()

set(COMMON_CMAKE_ARGUMENTS "-std=c++11 -Werror -Wno-unused-result -mstackrealign -fexceptions -fno-builtin-stpcpy -fno-rtti")

# Command info: https://cmake.org/cmake/help/v3.4/command/include_directories.html
include_directories(
    src/main/cpp
    src/main/cpp/include
    src/main/cpp/v8_inspector

    # hack to find some libraries from the ndk
    ${ANDROID_NDK_ROOT}/sysroot/usr/include/${ARCH_INCLUDE_DIR}/
)

if ( OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD )
    set(CMAKE_CXX_FLAGS "${COMMON_CMAKE_ARGUMENTS} -O3 -fvisibility=hidden -ffunction-sections -fno-data-sections")
else()
    set(CMAKE_CXX_FLAGS "${COMMON_CMAKE_ARGUMENTS} -g")
endif()

if ( NOT OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD )
    # When building in Release mode we do not include the V8 inspector sources
    add_definitions(-DAPPLICATION_IN_DEBUG)

    set(
        INSPECTOR_SOURCES

        src/main/cpp/com_tns_AndroidJsV8Inspector.cpp
        src/main/cpp/JsV8InspectorClient.cpp
        src/main/cpp/DOMDomainCallbackHandlers.cpp
        src/main/cpp/NetworkDomainCallbackHandlers.cpp

        src/main/cpp/v8_inspector/src/inspector/protocol/CSS.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Console.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/DOM.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Debugger.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/HeapProfiler.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Log.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Network.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Overlay.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Page.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Profiler.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Protocol.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Runtime.cpp
        src/main/cpp/v8_inspector/src/inspector/protocol/Schema.cpp
        src/main/cpp/v8_inspector/src/inspector/utils/base64.cpp
        src/main/cpp/v8_inspector/src/inspector/utils/v8-inspector-common.cpp
        src/main/cpp/v8_inspector/src/inspector/utils/v8-network-request-data.cpp
        src/main/cpp/v8_inspector/src/inspector/utils/v8-page-resources.cpp
        src/main/cpp/v8_inspector/src/inspector/v8-css-agent-impl.cpp
        src/main/cpp/v8_inspector/src/inspector/v8-dom-agent-impl.cpp
        src/main/cpp/v8_inspector/src/inspector/v8-log-agent-impl.cpp
        src/main/cpp/v8_inspector/src/inspector/v8-network-agent-impl.cpp
        src/main/cpp/v8_inspector/src/inspector/v8-overlay-agent-impl.cpp
        src/main/cpp/v8_inspector/src/inspector/v8-page-agent-impl.cpp

        src/main/cpp/v8_inspector/src/inspector/injected-script.cc
        src/main/cpp/v8_inspector/src/inspector/inspected-context.cc
        src/main/cpp/v8_inspector/src/inspector/remote-object-id.cc
        src/main/cpp/v8_inspector/src/inspector/search-util.cc
        src/main/cpp/v8_inspector/src/inspector/string-16.cc
        src/main/cpp/v8_inspector/src/inspector/string-util.cc
        src/main/cpp/v8_inspector/src/inspector/v8-console.cc
        src/main/cpp/v8_inspector/src/inspector/v8-console-agent-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-console-message.cc
        src/main/cpp/v8_inspector/src/inspector/v8-debugger.cc
        src/main/cpp/v8_inspector/src/inspector/v8-debugger-agent-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-debugger-script.cc
        src/main/cpp/v8_inspector/src/inspector/v8-function-call.cc
        src/main/cpp/v8_inspector/src/inspector/v8-heap-profiler-agent-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-injected-script-host.cc
        src/main/cpp/v8_inspector/src/inspector/v8-inspector-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-inspector-session-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-internal-value-type.cc
        src/main/cpp/v8_inspector/src/inspector/v8-profiler-agent-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-regex.cc
        src/main/cpp/v8_inspector/src/inspector/v8-runtime-agent-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-schema-agent-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-stack-trace-impl.cc
        src/main/cpp/v8_inspector/src/inspector/v8-value-utils.cc
        src/main/cpp/v8_inspector/src/inspector/wasm-translation.cc
    )
else()
    # Debug builds will include the V8 inspector sources
    set(INSPECTOR_SOURCES)
endif()

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
    src/main/cpp/JEnv.cpp
    src/main/cpp/JType.cpp
    src/main/cpp/JniSignatureParser.cpp
    src/main/cpp/JsArgConverter.cpp
    src/main/cpp/JsArgToArrayConverter.cpp
    src/main/cpp/Logger.cpp
    src/main/cpp/ManualInstrumentation.cpp
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

    # V8 inspector source files will be included only in Release mode
    ${INSPECTOR_SOURCES}
)

if ( OPTIMIZED_BUILD OR OPTIMIZED_WITH_INSPECTOR_BUILD )
    set_target_properties(NativeScript PROPERTIES LINK_FLAGS -Wl,--allow-multiple-definition -Wl,--exclude-libs=ALL -Wl,--gc-sections)
else()
    set_target_properties(NativeScript PROPERTIES LINK_FLAGS -Wl,--allow-multiple-definition)
endif()

MESSAGE( STATUS "# General cmake Info" )
MESSAGE( STATUS "# PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "# CMAKE_VERSION: " ${CMAKE_VERSION} )
MESSAGE( STATUS "# CMAKE_C_COMPILER_ID: " ${CMAKE_C_COMPILER_ID} )
MESSAGE( STATUS "# CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID} )
MESSAGE( STATUS "# CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "# CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

# Command info: https://cmake.org/cmake/help/v3.4/command/target_link_libraries.html
# linking v8 and inspector libraries to runtime(NativeScript library)
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libzip.a )
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_base.a )
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_init.a )
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_initializers.a )
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_libplatform.a )
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_libsampler.a )
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_libbase.a )
target_link_libraries( NativeScript ${PROJECT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/libv8_snapshot.a )

# Command info: https://cmake.org/cmake/help/v3.4/command/find_library.html
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library( system-log log )
find_library( system-android android )
find_library( system-dl dl )
find_library( system-atomic atomic ) # TODO: plamen5kov: can't be found in ndk for some reasong ... look at it later (maybe deprecated in newer NDK versions)
find_library( system-z z )

# Command info: https://cmake.org/cmake/help/v3.4/command/target_link_libraries.html
# Specifies libraries CMake should link to your target library.
target_link_libraries( NativeScript ${system-log} ${system-android} ${system-dl} ${system-z})
