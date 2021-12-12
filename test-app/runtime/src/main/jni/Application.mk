APP_BUILD_SCRIPT := Android.mk

APP_ABI := armeabi-v7a x86 arm64-v8a x86_64
APP_PLATFORM := android-19


APP_STL := c++_shared

APP_CFLAGS := -Wall -Werror -fexceptions -frtti -DWITH_INSPECTOR=1
APP_CPPFLAGS := -std=c++1y
# Make sure every shared lib includes a .note.gnu.build-id header
APP_LDFLAGS := -Wl,--build-id

MY_COMMON_ARGUMENTS = -nostdinc++ -std=c++14 -Werror -Wno-unused-result -mstackrealign -fexceptions -fno-builtin-stpcpy -fno-rtti -D_LIBCPP_ABI_VERSION=Cr -D_LIBCPP_ENABLE_NODISCARD -D_LIBCPP_ABI_UNSTABLE -D_LIBCPP_HAS_NO_VENDOR_AVAILABILITY_ANNOTATIONS -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_ENABLE_REGEXP_INTERPRETER_THREADED_DISPATCH -DV8_EMBEDDED_BUILTINS

NDK_TOOLCHAIN_VERSION := clang