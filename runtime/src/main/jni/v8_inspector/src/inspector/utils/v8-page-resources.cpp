//
// Created by pkanev on 2/2/2017.
//

#include <cstdlib>
#include <jni.h>
#include <assert.h>
#include <NativeScriptAssert.h>
#include <fstream>
#include <iostream>
#include <JniLocalRef.h>
#include <Util.h>
#include "base64.h"
#include "v8_inspector/src/inspector/utils/v8-page-resources.h"
#include "JEnv.h"
#include "ArgConverter.h"

namespace v8_inspector {
namespace utils {
PageResource::PageResource(std::string filePath, std::string mimeType)
    : m_filePath(filePath),
      m_mimeType(mimeType),
      m_content() {

    m_type = PageResource::resourceTypeByMimeType(m_mimeType);
}

std::map<std::string, v8_inspector::utils::PageResource> PageResource::getPageResources() {
    auto result = std::map<std::string, v8_inspector::utils::PageResource>();
    tns::JEnv env;
    jclass inspectorClass = env.FindClass("com/tns/AndroidJsV8Inspector");
    assert(inspectorClass != nullptr);

    jclass pairClass = env.FindClass("android/util/Pair");
    assert(pairClass != nullptr);

    jfieldID pairFirst = env.GetFieldID(pairClass, "first", "Ljava/lang/Object;");
    assert(pairFirst != nullptr);

    jfieldID pairSecond = env.GetFieldID(pairClass, "second", "Ljava/lang/Object;");
    assert(pairSecond != nullptr);

    jmethodID getResourcesMethod = env.GetStaticMethodID(inspectorClass, "getPageResources", "()[Landroid/util/Pair;");
    jobject arrayOfPairs = env.CallStaticObjectMethod(inspectorClass, getResourcesMethod);
    tns::JniLocalRef pairs(static_cast<jobjectArray>(arrayOfPairs));
    int arrSize = env.GetArrayLength(pairs);

    for (int i = 0; i < arrSize; ++i) {
        tns::JniLocalRef pair(env.GetObjectArrayElement(pairs, i));
        tns::JniLocalRef pairFilePath((jstring) env.GetObjectField(pair, pairFirst));
        tns::JniLocalRef pairMimeType((jstring) env.GetObjectField(pair, pairSecond));

        auto filePath = tns::ArgConverter::jstringToString(pairFilePath);
        auto mimeType = tns::ArgConverter::jstringToString(pairMimeType);

        utils::PageResource pageResource(filePath, mimeType);
        result.insert(std::make_pair(filePath, pageResource));
    }

    s_cachedPageResources = result;

    return result;
}

String16 PageResource::getContent(protocol::ErrorString* errorString) {
    if (m_content.empty()) {
        auto filePath = m_filePath;
        auto shouldEncode = !hasTextContent();
        filePath.erase(0, 7); // deletes the 'file://' part before the full file path
        FILE* file = fopen(filePath.c_str(), "r+");
        if (file == nullptr) {
            *errorString = "Resource not found";
            return "";
        }

        fseek(file, 0, SEEK_END);
        long int size = ftell(file);
        rewind(file);
        unsigned char* buff = (unsigned char*) malloc(sizeof(unsigned char)*size);
        long bytesRead = fread(buff, sizeof(unsigned char), size, file);
        fclose(file);
        buff[bytesRead] = '\0';

        if (shouldEncode) {
            auto base64EncodedString = base64_encode(buff, size);
            m_content = tns::Util::ConvertFromUtf8ToUtf16(base64EncodedString);
        } else {
            auto utf8Content = std::string(reinterpret_cast<char*>(buff));
            auto utf16Content = tns::Util::ConvertFromUtf8ToUtf16(utf8Content);
            m_content = utf16Content;
        }

        free(buff);
    }

    return String16((const uint16_t*) m_content.data());
}

const char* PageResource::resourceTypeByMimeType(std::string mimeType) {
    auto result = protocol::Page::ResourceTypeEnum::Document;

    if (!mimeType.empty()) {
        auto it = s_mimeTypeMap.find(mimeType);
        if (it != s_mimeTypeMap.end()) {
            result = s_mimeTypeMap.at(mimeType);
        }
    }

    return result;
}

std::map<std::string, const char*> PageResource::s_mimeTypeMap = {
    { "text/xml", v8_inspector::protocol::Page::ResourceTypeEnum::Document },
    { "text/plain", v8_inspector::protocol::Page::ResourceTypeEnum::Document },
    { "application/xml", v8_inspector::protocol::Page::ResourceTypeEnum::Document },
    // text/css mime type is regarded as document so as to display in the Sources tab
    { "text/css", v8_inspector::protocol::Page::ResourceTypeEnum::Document },
    { "text/javascript", v8_inspector::protocol::Page::ResourceTypeEnum::Script },
    { "application/javascript", v8_inspector::protocol::Page::ResourceTypeEnum::Script },
    { "application/json", v8_inspector::protocol::Page::ResourceTypeEnum::Document },
    { "text/typescript", v8_inspector::protocol::Page::ResourceTypeEnum::Script },
    { "image/jpeg", v8_inspector::protocol::Page::ResourceTypeEnum::Image },
    { "image/png", v8_inspector::protocol::Page::ResourceTypeEnum::Image },
    { "application/binary", v8_inspector::protocol::Page::ResourceTypeEnum::Other }
};

std::map<std::string, v8_inspector::utils::PageResource> PageResource::s_cachedPageResources;
}
}