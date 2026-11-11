#include <android/asset_manager_jni.h>
#include <jni.h>

#include <string>
#include <vector>

#include "AppFiles.h"
#include "ScriptCache.h"

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

std::string ElementString(JNIEnv* env, jobjectArray array, jsize index) {
    auto element = static_cast<jstring>(env->GetObjectArrayElement(array, index));
    std::string result = ToStdString(env, element);
    env->DeleteLocalRef(element);
    return result;
}

std::vector<jint> ToIntVector(JNIEnv* env, jintArray array) {
    std::vector<jint> result(env->GetArrayLength(array));
    env->GetIntArrayRegion(array, 0, static_cast<jsize>(result.size()), result.data());
    return result;
}

}  // namespace

extern "C" JNIEXPORT void JNICALL Java_com_tns_AppFiles_nativeInit(
        JNIEnv* env, jclass, jstring filesRoot, jstring alias, jstring codeCacheDir,
        jobject assetManager, jboolean loadCodeFromApk,
        jobjectArray alwaysExtract, jintArray layerKinds, jobjectArray layerRoots,
        jintArray mapStart, jobjectArray mapKeys, jintArray mapKinds, jobjectArray mapTargets) {
    AppFiles::Config config;
    config.filesRoot = ToStdString(env, filesRoot);
    if (alias != nullptr) {
        config.aliases.push_back(ToStdString(env, alias));
    }
    if (assetManager != nullptr) {
        // Sources keep using the AAssetManager for the life of the process.
        config.assets = AAssetManager_fromJava(env, env->NewGlobalRef(assetManager));
    }
    config.loadCodeFromApk = loadCodeFromApk == JNI_TRUE;
    if (alwaysExtract != nullptr) {
        jsize count = env->GetArrayLength(alwaysExtract);
        for (jsize i = 0; i < count; i++) {
            config.alwaysExtract.push_back(ElementString(env, alwaysExtract, i));
        }
    }
    if (layerKinds != nullptr) {
        // Values mirror AppFileLayers.Snapshot and AppFileLayer.Entry.Kind.
        static const AppFiles::Layer::Kind kLayerKinds[] = {
                AppFiles::Layer::Kind::Extracted, AppFiles::Layer::Kind::Apk,
                AppFiles::Layer::Kind::Directory, AppFiles::Layer::Kind::ApkAssets,
                AppFiles::Layer::Kind::Map};
        static const AppFiles::LayerEntry::Kind kEntryKinds[] = {
                AppFiles::LayerEntry::Kind::File, AppFiles::LayerEntry::Kind::Asset,
                AppFiles::LayerEntry::Kind::Whiteout};
        std::vector<jint> kinds = ToIntVector(env, layerKinds);
        std::vector<jint> starts = ToIntVector(env, mapStart);
        std::vector<jint> entryKinds = ToIntVector(env, mapKinds);
        for (size_t i = 0; i < kinds.size(); i++) {
            AppFiles::Layer layer;
            layer.kind = kLayerKinds[kinds[i]];
            layer.root = ElementString(env, layerRoots, static_cast<jsize>(i));
            for (jint e = starts[i]; e < starts[i + 1]; e++) {
                AppFiles::LayerEntry entry;
                entry.kind = kEntryKinds[entryKinds[e]];
                entry.target = ElementString(env, mapTargets, e);
                layer.entries.emplace_back(ElementString(env, mapKeys, e), std::move(entry));
            }
            config.layers.push_back(std::move(layer));
        }
    }
    AppFiles::Init(config);
    ScriptCache::Init(ToStdString(env, codeCacheDir));
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_tns_AppFiles_loadsCodeFromApk(JNIEnv*, jclass) {
    return AppFiles::LoadsCodeFromApk() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jstring JNICALL Java_com_tns_AppFiles_filesRoot(JNIEnv* env, jclass) {
    return ToJString(env, AppFiles::FilesRoot());
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
