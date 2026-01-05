//
// Created by Osei Fortune on 07/11/2023.
//

#include "jni.h"
#include "AssetExtractor.h"
#include "NativeScriptException.h"

#include <sstream>

#ifndef COM_TNS_ASSETEXTRACTOR_H
#define COM_TNS_ASSETEXTRACTOR_H

using namespace std;
using namespace tns;

void extractAssets(JNIEnv* env, jobject obj, jstring apk, jstring inputDir, jstring outputDir, jboolean _forceOverwrite);

#endif //COM_TNS_ASSETEXTRACTOR_H
