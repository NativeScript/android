#include <jni.h>

#include <string>
#include <vector>

#include "AppFiles.h"

using namespace tns;

namespace {

std::string ToStdString(JNIEnv* env, jstring value) {
    if (value == nullptr) {
        return std::string();
    }
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (chars == nullptr) {
        return std::string();
    }
    std::string result(chars);
    env->ReleaseStringUTFChars(value, chars);
    return result;
}

// Paths are Modified UTF-8 on the way in and out, matching how the rest of the
// runtime converts jstrings; file contents travel as bytes instead.
jstring ToJString(JNIEnv* env, const std::string& value) {
    return env->NewStringUTF(value.c_str());
}

}  // namespace

extern "C" JNIEXPORT void JNICALL Java_com_tns_AppFiles_nativeInit(JNIEnv* env, jclass,
                                                                   jstring filesRoot,
                                                                   jstring alias) {
    std::vector<std::string> aliases;
    if (alias != nullptr) {
        aliases.push_back(ToStdString(env, alias));
    }
    AppFiles::Init(ToStdString(env, filesRoot), aliases);
}

extern "C" JNIEXPORT jstring JNICALL Java_com_tns_AppFiles_filesRoot(JNIEnv* env, jclass) {
    return ToJString(env, AppFiles::FilesRoot());
}

extern "C" JNIEXPORT jstring JNICALL Java_com_tns_AppFiles_appRoot(JNIEnv* env, jclass) {
    return ToJString(env, AppFiles::AppRoot());
}

extern "C" JNIEXPORT jstring JNICALL Java_com_tns_AppFiles_logicalPath(JNIEnv* env, jclass,
                                                                       jstring path) {
    std::string logical;
    if (!AppFiles::ToLogical(ToStdString(env, path), logical)) {
        return nullptr;
    }
    return ToJString(env, logical);
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_tns_AppFiles_isFile(JNIEnv* env, jclass,
                                                                   jstring path) {
    return AppFiles::IsFile(ToStdString(env, path)) ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_com_tns_AppFiles_readBytes(JNIEnv* env, jclass,
                                                                        jstring path) {
    std::string content;
    if (!AppFiles::ReadText(ToStdString(env, path), content)) {
        return nullptr;
    }
    jbyteArray result = env->NewByteArray(static_cast<jsize>(content.size()));
    if (result == nullptr) {
        return nullptr;
    }
    env->SetByteArrayRegion(result, 0, static_cast<jsize>(content.size()),
                            reinterpret_cast<const jbyte*>(content.data()));
    return result;
}
