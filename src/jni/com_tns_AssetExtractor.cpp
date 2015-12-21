#include "jni.h"
#include "AssetExtractor.h"
#include "NativeScriptException.h"
#include "NativeScriptAssert.h"


using namespace tns;

void mkdir_rec(const char *dir);

extern "C" void Java_com_tns_AssetExtractor_extractAssets(JNIEnv *env, jobject obj, jstring apk, jstring outputDir, jboolean _forceOverwrite)
{
	try
	{
		AssetExtractor::ExtractAssets(env, obj, apk, outputDir, _forceOverwrite);
	}
	catch (NativeScriptException& e)
	{
		e.ReThrowToJava();
	}
	catch (std::exception e) {
		NativeScriptException nsEx(std::string("Error: c++ exception: %s", e.what()));
		nsEx.ReThrowToJava();
	}
	catch (...) {
		NativeScriptException nsEx(std::string("Error: c++ exception!"));
		nsEx.ReThrowToJava();
	}
}
