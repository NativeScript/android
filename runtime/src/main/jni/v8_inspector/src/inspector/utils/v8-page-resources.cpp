//
// Created by pkanev on 2/2/2017.
//

#include <cstdlib>
#include <jni.h>
#include <assert.h>
#include <NativeScriptAssert.h>
#include <fstream>
#include <iostream>
#include "base64.h"
#include "v8_inspector/src/inspector/utils/v8-page-resources.h"
#include "JEnv.h"
#include "ArgConverter.h"

namespace v8_inspector {
namespace utils {
PageResource::PageResource(std::string filePath, std::string mimeType)
    : m_filePath(filePath),
      m_mimeType(mimeType),
      m_content("") {

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
    jobjectArray pairs = static_cast<jobjectArray>(arrayOfPairs);
    int arrSize = env.GetArrayLength(pairs);

    for (int i = 0; i < arrSize; ++i) {
        auto pair = env.GetObjectArrayElement(pairs, i);
        auto pairFilePath = (jstring) env.GetObjectField(pair, pairFirst);
        auto pairMimeType = (jstring) env.GetObjectField(pair, pairSecond);

        auto filePath = tns::ArgConverter::jstringToString(pairFilePath);
        auto mimeType = tns::ArgConverter::jstringToString(pairMimeType);

        utils::PageResource pageResource(filePath, mimeType);
        result.insert(std::make_pair(filePath, pageResource));

        env.DeleteLocalRef(pairFilePath);
        env.DeleteLocalRef(pairMimeType);
        env.DeleteLocalRef(pair);
    }

    env.DeleteLocalRef(pairs);

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
            m_content = base64EncodedString;
        } else {
            m_content = std::string(reinterpret_cast<char*>(buff));
        }

        free(buff);
    }

    return m_content.c_str();
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
    // regarding javascript files as Scripts will prevent them from being displayed in the Sources
    // tab of DevTools, at least according to the FrontEnd implementation
    { "text/javascript", v8_inspector::protocol::Page::ResourceTypeEnum::Script },
    { "application/javascript", v8_inspector::protocol::Page::ResourceTypeEnum::Script },
    { "application/json", v8_inspector::protocol::Page::ResourceTypeEnum::Document },
    // regarding typescript files as Scripts will prevent them from being displayed in the Sources
    // tab of DevTools, at least according to the FrontEnd implementation :-/
    // Enable typescripts as documents so that breakpoints may be set in code that has yet to be reached.
    // Will result in duplicate of the same typescript fiels in the Sources tab
    { "text/typescript", v8_inspector::protocol::Page::ResourceTypeEnum::Document },
    { "image/jpeg", v8_inspector::protocol::Page::ResourceTypeEnum::Image },
    { "image/png", v8_inspector::protocol::Page::ResourceTypeEnum::Image },
    { "application/binary", v8_inspector::protocol::Page::ResourceTypeEnum::Other }
};

std::map<std::string, v8_inspector::utils::PageResource> PageResource::s_cachedPageResources;
}
}