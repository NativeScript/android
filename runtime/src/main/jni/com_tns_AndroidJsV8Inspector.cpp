#include "JEnv.h"
#include "NativeScriptException.h"
#include <sstream>
#include "JsV8InspectorClient.h"
#include "ArgConverter.h"

using namespace tns;
using namespace std;

JNIEXPORT extern "C" void Java_com_tns_AndroidJsV8Inspector_init(JNIEnv *env, jobject object)
{
    JsV8InspectorClient::GetInstance()->init();
}

JNIEXPORT extern "C" void Java_com_tns_AndroidJsV8Inspector_connect(JNIEnv *env, jobject instance, jobject connection)
{
    JsV8InspectorClient::GetInstance()->connect(connection);
}

JNIEXPORT extern "C" void Java_com_tns_AndroidJsV8Inspector_disconnect(JNIEnv *env, jobject instance)
{
    JsV8InspectorClient::GetInstance()->disconnect();
}

JNIEXPORT extern "C" void Java_com_tns_AndroidJsV8Inspector_dispatchMessage(JNIEnv *env, jobject instance, jstring jMessage)
{
    std::string message = ArgConverter::jstringToString(jMessage);
    JsV8InspectorClient::GetInstance()->dispatchMessage(message);
}
