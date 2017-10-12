#include "jni.h"
#include "AssetExtractor.h"
#include "NativeScriptException.h"

#include <sstream>

using namespace tns;
using namespace std;

extern "C" JNIEXPORT void Java_com_tns_AssetExtractor_extractAssets(JNIEnv* env, jobject obj, jstring apk, jstring inputDir, jstring outputDir, jboolean _forceOverwrite) {
    try {
        AssetExtractor::ExtractAssets(env, obj, apk, inputDir, outputDir, _forceOverwrite);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}
