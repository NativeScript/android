#include "JEnv.h"
#include "NativeScriptAssert.h"
#include <assert.h>
#include <sstream>
#include <unistd.h>

//
#include "Util.h"
#include "NativeScriptException.h"

using namespace tns;
using namespace std;



JEnv::JEnv()
	: m_env(nullptr)
{
	JNIEnv *env = nullptr;
	jint ret = s_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

	if ((ret != JNI_OK) || (env == nullptr))
	{
		ret = s_jvm->AttachCurrentThread(&env, nullptr);
		assert(ret == JNI_OK);
		assert(env != nullptr);
	}
	m_env = env;
}

JEnv::JEnv(JNIEnv *jniEnv)
	: m_env(jniEnv)
{
}

JEnv::~JEnv()
{
}

JEnv::operator JNIEnv*() const
{
	return m_env;
}

jmethodID JEnv::GetMethodID(jclass clazz, const string& name, const string& sig)
{
	jmethodID mid = m_env->GetMethodID(clazz, name.c_str(), sig.c_str());
	CheckForJavaException();
	return mid;
}
jmethodID JEnv::GetStaticMethodID(jclass clazz, const string& name, const string& sig)
{
	jmethodID mid = m_env->GetStaticMethodID(clazz, name.c_str(), sig.c_str());
	CheckForJavaException();
	return mid;
}


jfieldID JEnv::GetFieldID(jclass clazz, const string& name, const string& sig)
{
	jfieldID fid = m_env->GetFieldID(clazz, name.c_str(), sig.c_str());
	CheckForJavaException();
	return fid;
}
jfieldID JEnv::GetStaticFieldID(jclass clazz, const string& name, const string& sig)
{
	jfieldID fid = m_env->GetStaticFieldID(clazz, name.c_str(), sig.c_str());
	CheckForJavaException();
	return fid;
}

void JEnv::CallStaticVoidMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	m_env->CallStaticVoidMethodA(clazz, methodID, args);
	CheckForJavaException();
}
void JEnv::CallNonvirtualVoidMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	m_env->CallNonvirtualVoidMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
}
void JEnv::CallVoidMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	m_env->CallVoidMethodA(obj, methodID, args);
	CheckForJavaException();
}

jboolean JEnv::CallStaticBooleanMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jboolean jbl = m_env->CallStaticBooleanMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jbl;
}
jboolean JEnv::CallNonvirtualBooleanMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jboolean jbl = m_env->CallNonvirtualBooleanMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jbl;
}
jboolean JEnv::CallBooleanMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jboolean jbl = m_env->CallBooleanMethodA(obj, methodID, args);
	CheckForJavaException();
	return jbl;
}

jbyte JEnv::CallStaticByteMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jbyte jbt = m_env->CallStaticByteMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jbt;
}
jbyte JEnv::CallNonvirtualByteMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jbyte jbt = m_env->CallNonvirtualByteMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jbt;
}
jbyte JEnv::CallByteMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jbyte jbt = m_env->CallByteMethodA(obj, methodID, args);
	CheckForJavaException();
	return jbt;
}


jchar JEnv::CallStaticCharMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jchar jch = m_env->CallStaticCharMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jch;
}
jchar JEnv::CallNonvirtualCharMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jchar jch = m_env->CallNonvirtualCharMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jch;
}
jchar JEnv::CallCharMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jchar jch = m_env->CallCharMethodA(obj, methodID, args);
	CheckForJavaException();
	return jch;
}


jshort JEnv::CallStaticShortMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jshort jsh = m_env->CallStaticShortMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jsh;

}
jshort JEnv::CallNonvirtualShortMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jshort jsh = m_env->CallNonvirtualShortMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jsh;
}
jshort JEnv::CallShortMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jshort jsh = m_env->CallShortMethodA(obj, methodID, args);
	CheckForJavaException();
	return jsh;
}

jint JEnv::CallStaticIntMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jint ji = m_env->CallStaticIntMethodA(clazz, methodID, args);
	CheckForJavaException();
	return ji;

}
jint JEnv::CallNonvirtualIntMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jint ji = m_env->CallNonvirtualIntMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return ji;
}
jint JEnv::CallIntMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jint ji = m_env->CallIntMethodA(obj, methodID, args);
	CheckForJavaException();
	return ji;
}

jlong JEnv::CallStaticLongMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jlong jl = m_env->CallStaticLongMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jl;
}
jlong JEnv::CallNonvirtualLongMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jlong jl = m_env->CallNonvirtualLongMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jl;
}
jlong JEnv::CallLongMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jlong jl = m_env->CallLongMethodA(obj, methodID, args);
	CheckForJavaException();
	return jl;
}

jfloat JEnv::CallStaticFloatMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jfloat jfl = m_env->CallStaticFloatMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jfl;
}
jfloat JEnv::CallNonvirtualFloatMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jfloat jfl = m_env->CallNonvirtualFloatMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jfl;
}
jfloat JEnv::CallFloatMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jfloat jfl = m_env->CallFloatMethodA(obj, methodID, args);
	CheckForJavaException();
	return jfl;
}

jdouble JEnv::CallStaticDoubleMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jdouble jdb = m_env->CallStaticDoubleMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jdb;
}
jdouble JEnv::CallNonvirtualDoubleMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jdouble jdb = m_env->CallNonvirtualDoubleMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jdb;
}
jdouble JEnv::CallDoubleMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jdouble jdb = m_env->CallDoubleMethodA(obj, methodID, args);
	CheckForJavaException();
	return jdb;
}


jobject JEnv::CallStaticObjectMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	jobject jo = m_env->CallStaticObjectMethodA(clazz, methodID, args);
	CheckForJavaException();
	return jo;
}
jobject JEnv::CallNonvirtualObjectMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	jobject jo = m_env->CallNonvirtualObjectMethodA(obj, clazz, methodID, args);
	CheckForJavaException();
	return jo;
}
jobject JEnv::CallObjectMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	jobject jo = m_env->CallObjectMethodA(obj, methodID, args);
	CheckForJavaException();
	return jo;
}



jobject JEnv::GetStaticObjectField(jclass clazz, jfieldID fieldID)
{
	jobject jo = m_env->GetStaticObjectField(clazz, fieldID);
	CheckForJavaException();
	return jo;
}
jboolean JEnv::GetStaticBooleanField(jclass clazz, jfieldID fieldID)
{
	jboolean jbl = m_env->GetStaticBooleanField(clazz, fieldID);
	CheckForJavaException();
	return jbl;
}
jbyte JEnv::GetStaticByteField(jclass clazz, jfieldID fieldID)
{
	jbyte jbt = m_env->GetStaticByteField(clazz, fieldID);
	CheckForJavaException();
	return jbt;
}
jchar JEnv::GetStaticCharField(jclass clazz, jfieldID fieldID)
{
	jchar jch = m_env->GetStaticCharField(clazz, fieldID);
	CheckForJavaException();
	return jch;
}
jshort JEnv::GetStaticShortField(jclass clazz, jfieldID fieldID)
{
	jshort jsh = m_env->GetStaticShortField(clazz, fieldID);
	CheckForJavaException();
	return jsh;
}
jint JEnv::GetStaticIntField(jclass clazz, jfieldID fieldID)
{
	jint ji = m_env->GetStaticIntField(clazz, fieldID);
	CheckForJavaException();
	return ji;
}
jlong JEnv::GetStaticLongField(jclass clazz, jfieldID fieldID)
{
	jlong jl = m_env->GetStaticLongField(clazz, fieldID);
	CheckForJavaException();
	return jl;
}
jfloat JEnv::GetStaticFloatField(jclass clazz, jfieldID fieldID)
{
	jfloat jfl = m_env->GetStaticFloatField(clazz, fieldID);
	CheckForJavaException();
	return jfl;
}
jdouble JEnv::GetStaticDoubleField(jclass clazz, jfieldID fieldID)
{
	jdouble jd = m_env->GetStaticDoubleField(clazz, fieldID);
	CheckForJavaException();
	return jd;
}

void JEnv::SetStaticObjectField(jclass clazz, jfieldID fieldID, jobject value)
{
	m_env->SetStaticObjectField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticBooleanField(jclass clazz, jfieldID fieldID, jboolean value)
{
	m_env->SetStaticBooleanField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticByteField(jclass clazz, jfieldID fieldID, jbyte value)
{
	m_env->SetStaticByteField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticCharField(jclass clazz, jfieldID fieldID, jchar value)
{
	m_env->SetStaticCharField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticShortField(jclass clazz, jfieldID fieldID, jshort value)
{
	m_env->SetStaticShortField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticIntField(jclass clazz, jfieldID fieldID, jint value)
{
	m_env->SetStaticIntField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticLongField(jclass clazz, jfieldID fieldID, jlong value)
{
	m_env->SetStaticLongField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticFloatField(jclass clazz, jfieldID fieldID, jfloat value)
{
	m_env->SetStaticFloatField(clazz, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetStaticDoubleField(jclass clazz, jfieldID fieldID, jdouble value)
{
	m_env->SetStaticDoubleField(clazz, fieldID, value);
	CheckForJavaException();
}


jobject JEnv::GetObjectField(jobject obj, jfieldID fieldID)
{
	jobject jo = m_env->GetObjectField(obj, fieldID);
	CheckForJavaException();
	return jo;
}
jboolean JEnv::GetBooleanField(jobject obj, jfieldID fieldID)
{
	jboolean jbl = m_env->GetBooleanField(obj, fieldID);
	CheckForJavaException();
	return jbl;
}
jbyte JEnv::GetByteField(jobject obj, jfieldID fieldID)
{
	jbyte jbt = m_env->GetByteField(obj, fieldID);
	CheckForJavaException();
	return jbt;
}
jchar JEnv::GetCharField(jobject obj, jfieldID fieldID)
{
	jchar jch = m_env->GetCharField(obj, fieldID);
	CheckForJavaException();
	return jch;
}
jshort JEnv::GetShortField(jobject obj, jfieldID fieldID)
{
	jshort jsh = m_env->GetShortField(obj, fieldID);
	CheckForJavaException();
	return jsh;
}
jint JEnv::GetIntField(jobject obj, jfieldID fieldID)
{
	jint ji = m_env->GetIntField(obj, fieldID);
	CheckForJavaException();
	return ji;
}
jlong JEnv::GetLongField(jobject obj, jfieldID fieldID)
{
	jlong jl = m_env->GetLongField(obj, fieldID);
	CheckForJavaException();
	return jl;
}
jfloat JEnv::GetFloatField(jobject obj, jfieldID fieldID)
{
	jfloat jfl = m_env->GetFloatField(obj, fieldID);
	CheckForJavaException();
	return jfl;
}
jdouble JEnv::GetDoubleField(jobject obj, jfieldID fieldID)
{
	jdouble jd = m_env->GetDoubleField(obj, fieldID);
	CheckForJavaException();
	return jd;
}

void JEnv::SetObjectField(jobject obj, jfieldID fieldID, jobject value)
{
	m_env->SetObjectField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetBooleanField(jobject obj, jfieldID fieldID, jboolean value)
{
	m_env->SetBooleanField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetByteField(jobject obj, jfieldID fieldID, jbyte value)
{
	m_env->SetByteField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetCharField(jobject obj, jfieldID fieldID, jchar value)
{
	m_env->SetCharField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetShortField(jobject obj, jfieldID fieldID, jshort value)
{
	m_env->SetShortField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetIntField(jobject obj, jfieldID fieldID, jint value)
{
	m_env->SetIntField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetLongField(jobject obj, jfieldID fieldID, jlong value)
{
	m_env->SetLongField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetFloatField(jobject obj, jfieldID fieldID, jfloat value)
{
	m_env->SetFloatField(obj, fieldID, value);
	CheckForJavaException();
}
void JEnv::SetDoubleField(jobject obj, jfieldID fieldID, jdouble value)
{
	m_env->SetDoubleField(obj, fieldID, value);
	CheckForJavaException();
}

jstring JEnv::NewString(const jchar* unicodeChars, jsize len)
{
	jstring jst = m_env->NewString(unicodeChars, len);
	CheckForJavaException();
	return jst;
}
jstring JEnv::NewStringUTF(const char* bytes)
{
	jstring jst = m_env->NewStringUTF(bytes);
	CheckForJavaException();
	return jst;
}


jobjectArray JEnv::NewObjectArray(jsize length, jclass elementClass, jobject initialElement)
{
	jobjectArray joa = m_env->NewObjectArray(length, elementClass, initialElement);
	CheckForJavaException();
	return joa;
}

jobject JEnv::GetObjectArrayElement(jobjectArray array, jsize index)
{
	jobject jo = m_env->GetObjectArrayElement(array, index);
	CheckForJavaException();
	return jo;
}
void JEnv::SetObjectArrayElement(jobjectArray array, jsize index, jobject value)
{
	m_env->SetObjectArrayElement(array, index, value);
	CheckForJavaException();
}

const char* JEnv::GetStringUTFChars(jstring str, jboolean* isCopy)
{
	const char* cc = m_env->GetStringUTFChars(str, isCopy);
	CheckForJavaException();
	return cc;
}
void JEnv::ReleaseStringUTFChars(jstring str, const char* utf)
{
	m_env->ReleaseStringUTFChars(str, utf);
	CheckForJavaException();
}

const jchar* JEnv::GetStringChars(jstring str, jboolean* isCopy)
{
	const jchar* cjc = m_env->GetStringChars(str, isCopy);
	CheckForJavaException();
	return cjc;
}
void JEnv::ReleaseStringChars(jstring str, const jchar* chars)
{
	m_env->ReleaseStringChars(str, chars);
	CheckForJavaException();
}

const int JEnv::GetStringLength(jstring str)
{
	const int ci = m_env->GetStringLength(str);
	CheckForJavaException();
	return ci;
}

const int JEnv::GetStringUTFLength(jstring str)
{
	const int ci = m_env->GetStringUTFLength(str);
	CheckForJavaException();
	return ci;
}

void JEnv::GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf) {
	m_env->GetStringUTFRegion(str, start, len, buf);
	CheckForJavaException();
}

jint JEnv::Throw(jthrowable obj)
{
	return m_env->Throw(obj);
}

jthrowable JEnv::ExceptionOccurred()
{
	jthrowable jt = m_env->ExceptionOccurred();
	return jt;
}
void JEnv::ExceptionDescribe()
{
	m_env->ExceptionDescribe();
	CheckForJavaException();
}
void JEnv::ExceptionClear()
{
	m_env->ExceptionClear();
}

jboolean JEnv::IsInstanceOf(jobject obj, jclass clazz)
{
	jboolean jbl = m_env->IsInstanceOf(obj, clazz);
	CheckForJavaException();
	return jbl;
}

jobjectRefType JEnv::GetObjectRefType(jobject obj)
{
	jobjectRefType ort = m_env->GetObjectRefType(obj);
	CheckForJavaException();
	return ort;
}

jobject JEnv::NewGlobalRef(jobject obj)
{
	jobject jo = m_env->NewGlobalRef(obj);
//	CheckForJavaException();
	return jo;
}
jweak JEnv::NewWeakGlobalRef(jobject obj)
{
	jweak jw = m_env->NewWeakGlobalRef(obj);
	CheckForJavaException();
	return jw;
}
void JEnv::DeleteGlobalRef(jobject globalRef)
{
	m_env->DeleteGlobalRef(globalRef);
	CheckForJavaException();
}
void JEnv::DeleteWeakGlobalRef(jweak obj)
{
	m_env->DeleteWeakGlobalRef(obj);
	CheckForJavaException();
}


jobject JEnv::NewLocalRef(jobject ref)
{
	jobject jo = m_env->NewLocalRef(ref);
	CheckForJavaException();
	return jo;
}
void JEnv::DeleteLocalRef(jobject localRef)
{
	m_env->DeleteLocalRef(localRef);
}

jbyteArray JEnv::NewByteArray(jsize length)
{
	jbyteArray jba = m_env->NewByteArray(length);
	CheckForJavaException();
	return jba;
}
jbooleanArray JEnv::NewBooleanArray(jsize length)
{
	jbooleanArray jba = m_env->NewBooleanArray(length);
	CheckForJavaException();
	return jba;
}
jcharArray JEnv::NewCharArray(jsize length)
{
	jcharArray jca = m_env->NewCharArray(length);
	CheckForJavaException();
	return jca;
}
jshortArray JEnv::NewShortArray(jsize length)
{
	jshortArray jsa = m_env->NewShortArray(length);
	CheckForJavaException();
	return jsa;
}
jintArray JEnv::NewIntArray(jsize length)
{
	jintArray jia = m_env->NewIntArray(length);
	CheckForJavaException();
	return jia;
}
jlongArray JEnv::NewLongArray(jsize length)
{
	jlongArray jla = m_env->NewLongArray(length);
	CheckForJavaException();
	return jla;
}
jfloatArray JEnv::NewFloatArray(jsize length)
{
	jfloatArray jfa = m_env->NewFloatArray(length);
	CheckForJavaException();
	return jfa;
}
jdoubleArray JEnv::NewDoubleArray(jsize length)
{
	jdoubleArray jda = m_env->NewDoubleArray(length);
	CheckForJavaException();
	return jda;
}

jbyte* JEnv::GetByteArrayElements(jbyteArray array, jboolean* isCopy)
{
	jbyte* jbt = m_env->GetByteArrayElements(array, isCopy);
	CheckForJavaException();
	return jbt;
}
void JEnv::ReleaseByteArrayElements(jbyteArray array, jbyte* elems, jint mode)
{
	m_env->ReleaseByteArrayElements(array, elems, mode);
	CheckForJavaException();
}

void JEnv::GetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, jboolean* buf)
{
	m_env->GetBooleanArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::GetByteArrayRegion(jbyteArray array, jsize start, jsize len, jbyte* buf)
{
	m_env->GetByteArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::GetCharArrayRegion(jcharArray array, jsize start, jsize len, jchar* buf)
{
	m_env->GetCharArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::GetShortArrayRegion(jshortArray array, jsize start, jsize len, jshort* buf)
{
	m_env->GetShortArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::GetIntArrayRegion(jintArray array, jsize start, jsize len, jint* buf)
{
	m_env->GetIntArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::GetLongArrayRegion(jlongArray array, jsize start, jsize len, jlong* buf)
{
	m_env->GetLongArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::GetFloatArrayRegion(jfloatArray array, jsize start, jsize len, jfloat* buf)
{
	m_env->GetFloatArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, jdouble* buf)
{
	m_env->GetDoubleArrayRegion(array, start, len, buf);
	CheckForJavaException();
}


void JEnv::SetByteArrayRegion(jbyteArray array, jsize start, jsize len, const jbyte* buf)
{
	m_env->SetByteArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, const jboolean* buf)
{
	m_env->SetBooleanArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::SetCharArrayRegion(jcharArray array, jsize start, jsize len, const jchar* buf)
{
	m_env->SetCharArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::SetShortArrayRegion(jshortArray array, jsize start, jsize len, const jshort* buf)
{
	m_env->SetShortArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::SetIntArrayRegion(jintArray array, jsize start, jsize len, const jint* buf)
{
	m_env->SetIntArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::SetLongArrayRegion(jlongArray array, jsize start, jsize len, const jlong* buf)
{
	m_env->SetLongArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::SetFloatArrayRegion(jfloatArray array, jsize start, jsize len, const jfloat* buf)
{
	m_env->SetFloatArrayRegion(array, start, len, buf);
	CheckForJavaException();
}
void JEnv::SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, const jdouble* buf)
{
	m_env->SetDoubleArrayRegion(array, start, len, buf);
	CheckForJavaException();
}

jclass JEnv::FindClass(const string& className)
{
	jclass klass;

	auto itFound = s_classCache.find(className);

	if (itFound != s_classCache.end())
	{
		klass = itFound->second;
	}
	else
	{
		jclass tmp = m_env->FindClass(className.c_str());

		if (m_env->ExceptionCheck() == JNI_TRUE)
		{
			m_env->ExceptionClear();
			string cannonicalClassName = Util::ConvertFromJniToCanonicalName(className);
			jstring s = m_env->NewStringUTF(cannonicalClassName.c_str());
			tmp = reinterpret_cast<jclass>(m_env->CallStaticObjectMethod(PLATFORM_CLASS, GET_CACHED_CLASS_METHOD_ID, s));

			m_env->DeleteLocalRef(s);
		}

		klass = reinterpret_cast<jclass>(m_env->NewGlobalRef(tmp));
		s_classCache.insert(make_pair(className, klass));
		m_env->DeleteLocalRef(tmp);
	}

	return klass;
}

jobject JEnv::NewDirectByteBuffer(void* address, jlong capacity)
{
	jobject jo = m_env->NewDirectByteBuffer(address, capacity);
	CheckForJavaException();
	return jo;
}
void* JEnv::GetDirectBufferAddress(jobject buf)
{
	void* v = m_env->GetDirectBufferAddress(buf);
	CheckForJavaException();
	return v;
}

jlong JEnv::GetDirectBufferCapacity(jobject buf)
{
	jlong jl = m_env->GetDirectBufferCapacity(buf);
	CheckForJavaException();
	return jl;
}

jboolean JEnv::IsAssignableFrom(jclass clazz1, jclass clazz2) {
	jboolean jbl = m_env->IsAssignableFrom(clazz1, clazz2);
	CheckForJavaException();
	return jbl;
}

void JEnv::Init(JavaVM *jvm)
{
	assert(jvm != nullptr);
	s_jvm = jvm;

	JEnv env;
	PLATFORM_CLASS = env.FindClass("com/tns/Platform");
	assert(PLATFORM_CLASS != nullptr);
	GET_CACHED_CLASS_METHOD_ID = env.GetStaticMethodID(PLATFORM_CLASS, "getCachedClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	assert(GET_CACHED_CLASS_METHOD_ID != nullptr);
}

jclass JEnv::GetObjectClass(jobject obj)
{
	jclass jcl = m_env->GetObjectClass(obj);
	CheckForJavaException();
	return jcl;
}

jsize JEnv::GetArrayLength(jarray array)
{
	jsize jsz = m_env->GetArrayLength(array);
	CheckForJavaException();
	return jsz;
}

//recursion if we put: CheckForJavaException();
//in this method
jboolean JEnv::ExceptionCheck()
{
	return m_env->ExceptionCheck();
}

void JEnv::CheckForJavaException()
{
	if (ExceptionCheck() == JNI_TRUE)
	{
		throw NativeScriptException(*this);
	}
}

JavaVM* JEnv::s_jvm = nullptr;
map<string, jclass> JEnv::s_classCache;
jclass JEnv::PLATFORM_CLASS = nullptr;
jmethodID JEnv::GET_CACHED_CLASS_METHOD_ID = nullptr;
