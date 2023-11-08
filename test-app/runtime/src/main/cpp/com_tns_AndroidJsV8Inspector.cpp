#include "JEnv.h"
#include "NativeScriptException.h"
#include <sstream>
#include "JsV8InspectorClient.h"
#include "ArgConverter.h"
#include "com_tns_AndroidJsV8Inspector.h"

using namespace tns;
using namespace std;


JNIEXPORT JNICALL void connect(JNIEnv *env, jobject instance, jobject connection) {
    JsV8InspectorClient::GetInstance()->disconnect();
    JsV8InspectorClient::GetInstance()->connect(connection);
}

JNIEXPORT JNICALL void scheduleBreak(JNIEnv *env, jobject instance) {
    JsV8InspectorClient::GetInstance()->scheduleBreak();
}

JNIEXPORT JNICALL void scheduleBreakCritical() {
    JsV8InspectorClient::GetInstance()->scheduleBreak();
}

JNIEXPORT JNICALL void disconnect(JNIEnv *env, jobject instance) {
    JsV8InspectorClient::GetInstance()->disconnect();
}

JNIEXPORT JNICALL void disconnectCritical() {
    JsV8InspectorClient::GetInstance()->disconnect();
}

JNIEXPORT JNICALL void dispatchMessage(JNIEnv *env, jobject instance, jstring jMessage) {
    std::string message = ArgConverter::jstringToString(jMessage);
    JsV8InspectorClient::GetInstance()->dispatchMessage(message);
}

JNIEXPORT JNICALL void init(JNIEnv *env, jobject object) {
    JsV8InspectorClient::GetInstance()->init();
}

JNIEXPORT JNICALL void initCritical() {
    JsV8InspectorClient::GetInstance()->init();
}
