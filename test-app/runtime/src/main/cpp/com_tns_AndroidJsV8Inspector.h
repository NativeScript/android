//
// Created by Osei Fortune on 07/11/2023.
//
#include "jni.h"
#include "JEnv.h"
#include "NativeScriptException.h"
#include <sstream>
#include "JsV8InspectorClient.h"
#include "ArgConverter.h"


using namespace tns;
using namespace std;

#ifndef COM_TNS_ANDROIDJSV8INSPECTOR_H
#define COM_TNS_ANDROIDJSV8INSPECTOR_H

void init(JNIEnv *env, jobject object);

void initCritical();

void connect(JNIEnv *env, jobject instance, jobject connection);

void scheduleBreak(JNIEnv *env, jobject instance);

void scheduleBreakCritical();

void disconnect(JNIEnv *env, jobject instance);

void disconnectCritical();

void dispatchMessage(JNIEnv *env, jobject instance, jstring jMessage);


#endif //COM_TNS_ANDROIDJSV8INSPECTOR_H
