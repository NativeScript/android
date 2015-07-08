#include "JEnv.h"
#include <assert.h>
#include <sstream>
#include <unistd.h>
//
#include "Util.h"

using namespace tns;
using namespace std;



JEnv::JEnv(bool detach)
	: m_detach(detach), m_env(nullptr)
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
	: m_env(jniEnv), m_detach(false)
{
}

JEnv::~JEnv()
{
	pid_t pid = getpid();
	pid_t tid = gettid();

	if (pid != tid)
	{
		if (m_detach)
		{
			JNIEnv *env = nullptr;
			jint ret = s_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

			if ((ret == JNI_OK) && (env != nullptr))
			{
				jint ret = s_jvm->DetachCurrentThread();
				assert(ret == JNI_OK);
			}
		}
	}
}

JEnv::operator JNIEnv*() const
{
	return m_env;
}

jmethodID JEnv::GetMethodID(jclass clazz, const string& name, const string& sig)
{
	return m_env->GetMethodID(clazz, name.c_str(), sig.c_str());
}
jmethodID JEnv::GetStaticMethodID(jclass clazz, const string& name, const string& sig)
{
	return m_env->GetStaticMethodID(clazz, name.c_str(), sig.c_str());
}


jfieldID JEnv::GetFieldID(jclass clazz, const string& name, const string& sig)
{
	return m_env->GetFieldID(clazz, name.c_str(), sig.c_str());
}
jfieldID JEnv::GetStaticFieldID(jclass clazz, const string& name, const string& sig)
{
	return m_env->GetStaticFieldID(clazz, name.c_str(), sig.c_str());
}


void JEnv::CallStaticVoidMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	m_env->CallStaticVoidMethodA(clazz, methodID, args);
}
void JEnv::CallNonvirtualVoidMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	m_env->CallNonvirtualVoidMethodA(obj, clazz, methodID, args);
}
void JEnv::CallVoidMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	m_env->CallVoidMethodA(obj, methodID, args);
}

jboolean JEnv::CallStaticBooleanMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticBooleanMethodA(clazz, methodID, args);
}
jboolean JEnv::CallNonvirtualBooleanMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualBooleanMethodA(obj, clazz, methodID, args);
}
jboolean JEnv::CallBooleanMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallBooleanMethodA(obj, methodID, args);
}

jbyte JEnv::CallStaticByteMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticByteMethodA(clazz, methodID, args);
}
jbyte JEnv::CallNonvirtualByteMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualByteMethodA(obj, clazz, methodID, args);
}
jbyte JEnv::CallByteMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallByteMethodA(obj, methodID, args);
}


jchar JEnv::CallStaticCharMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticCharMethodA(clazz, methodID, args);
}
jchar JEnv::CallNonvirtualCharMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualCharMethodA(obj, clazz, methodID, args);
}
jchar JEnv::CallCharMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallCharMethodA(obj, methodID, args);
}


jshort JEnv::CallStaticShortMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticShortMethodA(clazz, methodID, args);
}
jshort JEnv::CallNonvirtualShortMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualShortMethodA(obj, clazz, methodID, args);
}
jshort JEnv::CallShortMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallShortMethodA(obj, methodID, args);
}

jint JEnv::CallStaticIntMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticIntMethodA(clazz, methodID, args);
}
jint JEnv::CallNonvirtualIntMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualIntMethodA(obj, clazz, methodID, args);
}
jint JEnv::CallIntMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallIntMethodA(obj, methodID, args);
}

jlong JEnv::CallStaticLongMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticLongMethodA(clazz, methodID, args);
}
jlong JEnv::CallNonvirtualLongMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualLongMethodA(obj, clazz, methodID, args);
}
jlong JEnv::CallLongMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallLongMethodA(obj, methodID, args);
}

jfloat JEnv::CallStaticFloatMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticFloatMethodA(clazz, methodID, args);
}
jfloat JEnv::CallNonvirtualFloatMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualFloatMethodA(obj, clazz, methodID, args);
}
jfloat JEnv::CallFloatMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallFloatMethodA(obj, methodID, args);
}

jdouble JEnv::CallStaticDoubleMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticDoubleMethodA(clazz, methodID, args);
}
jdouble JEnv::CallNonvirtualDoubleMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualDoubleMethodA(obj, clazz, methodID, args);
}
jdouble JEnv::CallDoubleMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallDoubleMethodA(obj, methodID, args);
}


jobject JEnv::CallStaticObjectMethodA(jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallStaticObjectMethodA(clazz, methodID, args);
}
jobject JEnv::CallNonvirtualObjectMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args)
{
	return m_env->CallNonvirtualObjectMethodA(obj, clazz, methodID, args);
}
jobject JEnv::CallObjectMethodA(jobject obj, jmethodID methodID, jvalue* args)
{
	return m_env->CallObjectMethodA(obj, methodID, args);
}



jobject JEnv::GetStaticObjectField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticObjectField(clazz, fieldID);
}
jboolean JEnv::GetStaticBooleanField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticBooleanField(clazz, fieldID);
}
jbyte JEnv::GetStaticByteField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticByteField(clazz, fieldID);
}
jchar JEnv::GetStaticCharField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticCharField(clazz, fieldID);
}
jshort JEnv::GetStaticShortField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticShortField(clazz, fieldID);
}
jint JEnv::GetStaticIntField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticIntField(clazz, fieldID);
}
jlong JEnv::GetStaticLongField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticLongField(clazz, fieldID);
}
jfloat JEnv::GetStaticFloatField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticFloatField(clazz, fieldID);
}
jdouble JEnv::GetStaticDoubleField(jclass clazz, jfieldID fieldID)
{
	return m_env->GetStaticDoubleField(clazz, fieldID);
}

void JEnv::SetStaticObjectField(jclass clazz, jfieldID fieldID, jobject value)
{
	m_env->SetStaticObjectField(clazz, fieldID, value);
}
void JEnv::SetStaticBooleanField(jclass clazz, jfieldID fieldID, jboolean value)
{
	m_env->SetStaticBooleanField(clazz, fieldID, value);
}
void JEnv::SetStaticByteField(jclass clazz, jfieldID fieldID, jbyte value)
{
	m_env->SetStaticByteField(clazz, fieldID, value);
}
void JEnv::SetStaticCharField(jclass clazz, jfieldID fieldID, jchar value)
{
	m_env->SetStaticCharField(clazz, fieldID, value);
}
void JEnv::SetStaticShortField(jclass clazz, jfieldID fieldID, jshort value)
{
	m_env->SetStaticShortField(clazz, fieldID, value);
}
void JEnv::SetStaticIntField(jclass clazz, jfieldID fieldID, jint value)
{
	m_env->SetStaticIntField(clazz, fieldID, value);
}
void JEnv::SetStaticLongField(jclass clazz, jfieldID fieldID, jlong value)
{
	m_env->SetStaticLongField(clazz, fieldID, value);
}
void JEnv::SetStaticFloatField(jclass clazz, jfieldID fieldID, jfloat value)
{
	m_env->SetStaticFloatField(clazz, fieldID, value);
}
void JEnv::SetStaticDoubleField(jclass clazz, jfieldID fieldID, jdouble value)
{
	m_env->SetStaticDoubleField(clazz, fieldID, value);
}


jobject JEnv::GetObjectField(jobject obj, jfieldID fieldID)
{
	return m_env->GetObjectField(obj, fieldID);
}
jboolean JEnv::GetBooleanField(jobject obj, jfieldID fieldID)
{
	return m_env->GetBooleanField(obj, fieldID);
}
jbyte JEnv::GetByteField(jobject obj, jfieldID fieldID)
{
	return m_env->GetByteField(obj, fieldID);
}
jchar JEnv::GetCharField(jobject obj, jfieldID fieldID)
{
	return m_env->GetCharField(obj, fieldID);
}
jshort JEnv::GetShortField(jobject obj, jfieldID fieldID)
{
	return m_env->GetShortField(obj, fieldID);
}
jint JEnv::GetIntField(jobject obj, jfieldID fieldID)
{
	return m_env->GetIntField(obj, fieldID);
}
jlong JEnv::GetLongField(jobject obj, jfieldID fieldID)
{
	return m_env->GetLongField(obj, fieldID);
}
jfloat JEnv::GetFloatField(jobject obj, jfieldID fieldID)
{
	return m_env->GetFloatField(obj, fieldID);
}
jdouble JEnv::GetDoubleField(jobject obj, jfieldID fieldID)
{
	return m_env->GetDoubleField(obj, fieldID);
}

void JEnv::SetObjectField(jobject obj, jfieldID fieldID, jobject value)
{
	m_env->SetObjectField(obj, fieldID, value);
}
void JEnv::SetBooleanField(jobject obj, jfieldID fieldID, jboolean value)
{
	m_env->SetBooleanField(obj, fieldID, value);
}
void JEnv::SetByteField(jobject obj, jfieldID fieldID, jbyte value)
{
	m_env->SetByteField(obj, fieldID, value);
}
void JEnv::SetCharField(jobject obj, jfieldID fieldID, jchar value)
{
	m_env->SetCharField(obj, fieldID, value);
}
void JEnv::SetShortField(jobject obj, jfieldID fieldID, jshort value)
{
	m_env->SetShortField(obj, fieldID, value);
}
void JEnv::SetIntField(jobject obj, jfieldID fieldID, jint value)
{
	m_env->SetIntField(obj, fieldID, value);
}
void JEnv::SetLongField(jobject obj, jfieldID fieldID, jlong value)
{
	m_env->SetLongField(obj, fieldID, value);
}
void JEnv::SetFloatField(jobject obj, jfieldID fieldID, jfloat value)
{
	m_env->SetFloatField(obj, fieldID, value);
}
void JEnv::SetDoubleField(jobject obj, jfieldID fieldID, jdouble value)
{
	m_env->SetDoubleField(obj, fieldID, value);
}

jstring JEnv::NewString(const jchar* unicodeChars, jsize len)
{
	return m_env->NewString(unicodeChars, len);
}
jstring JEnv::NewStringUTF(const char* bytes)
{
	return m_env->NewStringUTF(bytes);
}


jobjectArray JEnv::NewObjectArray(jsize length, jclass elementClass, jobject initialElement)
{
	return m_env->NewObjectArray(length, elementClass, initialElement);
}

jobject JEnv::GetObjectArrayElement(jobjectArray array, jsize index)
{
	return m_env->GetObjectArrayElement(array, index);
}
void JEnv::SetObjectArrayElement(jobjectArray array, jsize index, jobject value)
{
	m_env->SetObjectArrayElement(array, index, value);
}

const char* JEnv::GetStringUTFChars(jstring str, jboolean* isCopy)
{
	return m_env->GetStringUTFChars(str, isCopy);
}
void JEnv::ReleaseStringUTFChars(jstring str, const char* utf)
{
	m_env->ReleaseStringUTFChars(str, utf);
}

const jchar* JEnv::GetStringChars(jstring str, jboolean* isCopy)
{
	return m_env->GetStringChars(str, isCopy);
}
void JEnv::ReleaseStringChars(jstring str, const jchar* chars)
{
	m_env->ReleaseStringChars(str, chars);
}

const int JEnv::GetStringLength(jstring str)
{
	return m_env->GetStringLength(str);
}

const int JEnv::GetStringUTFLength(jstring str)
{
	return m_env->GetStringUTFLength(str);
}

void JEnv::GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf) {
	m_env->GetStringUTFRegion(str, start, len, buf);
}


jint JEnv::Throw(jthrowable obj)
{
	return m_env->Throw(obj);
}
jboolean JEnv::ExceptionCheck()
{
	return m_env->ExceptionCheck();
}
jthrowable JEnv::ExceptionOccurred()
{
	return m_env->ExceptionOccurred();
}
void JEnv::ExceptionDescribe()
{
	m_env->ExceptionDescribe();
}
void JEnv::ExceptionClear()
{
	m_env->ExceptionClear();
}


jboolean JEnv::IsInstanceOf(jobject obj, jclass clazz)
{
	return m_env->IsInstanceOf(obj, clazz);
}

jobjectRefType JEnv::GetObjectRefType(jobject obj)
{
	return m_env->GetObjectRefType(obj);
}

jobject JEnv::NewGlobalRef(jobject obj)
{
	return m_env->NewGlobalRef(obj);
}
jweak JEnv::NewWeakGlobalRef(jobject obj)
{
	return m_env->NewWeakGlobalRef(obj);
}
void JEnv::DeleteGlobalRef(jobject globalRef)
{
	m_env->DeleteGlobalRef(globalRef);
}
void JEnv::DeleteWeakGlobalRef(jweak obj)
{
	m_env->DeleteWeakGlobalRef(obj);
}


jobject JEnv::NewLocalRef(jobject ref)
{
	return m_env->NewLocalRef(ref);
}
void JEnv::DeleteLocalRef(jobject localRef)
{
	m_env->DeleteLocalRef(localRef);
}

jbyteArray JEnv::NewByteArray(jsize length)
{
	return m_env->NewByteArray(length);
}
jbooleanArray JEnv::NewBooleanArray(jsize length)
{
	return m_env->NewBooleanArray(length);
}
jcharArray JEnv::NewCharArray(jsize length)
{
	return m_env->NewCharArray(length);
}
jshortArray JEnv::NewShortArray(jsize length)
{
	return m_env->NewShortArray(length);
}
jintArray JEnv::NewIntArray(jsize length)
{
	return m_env->NewIntArray(length);
}
jlongArray JEnv::NewLongArray(jsize length)
{
	return m_env->NewLongArray(length);
}
jfloatArray JEnv::NewFloatArray(jsize length)
{
	return m_env->NewFloatArray(length);
}
jdoubleArray JEnv::NewDoubleArray(jsize length)
{
	return m_env->NewDoubleArray(length);
}

jbyte* JEnv::GetByteArrayElements(jbyteArray array, jboolean* isCopy)
{
	return m_env->GetByteArrayElements(array, isCopy);
}
void JEnv::ReleaseByteArrayElements(jbyteArray array, jbyte* elems, jint mode)
{
	m_env->ReleaseByteArrayElements(array, elems, mode);
}

void JEnv::GetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, jboolean* buf)
{
	m_env->GetBooleanArrayRegion(array, start, len, buf);
}
void JEnv::GetByteArrayRegion(jbyteArray array, jsize start, jsize len, jbyte* buf)
{
	m_env->GetByteArrayRegion(array, start, len, buf);
}
void JEnv::GetCharArrayRegion(jcharArray array, jsize start, jsize len, jchar* buf)
{
	m_env->GetCharArrayRegion(array, start, len, buf);
}
void JEnv::GetShortArrayRegion(jshortArray array, jsize start, jsize len, jshort* buf)
{
	m_env->GetShortArrayRegion(array, start, len, buf);
}
void JEnv::GetIntArrayRegion(jintArray array, jsize start, jsize len, jint* buf)
{
	m_env->GetIntArrayRegion(array, start, len, buf);
}
void JEnv::GetLongArrayRegion(jlongArray array, jsize start, jsize len, jlong* buf)
{
	m_env->GetLongArrayRegion(array, start, len, buf);
}
void JEnv::GetFloatArrayRegion(jfloatArray array, jsize start, jsize len, jfloat* buf)
{
	m_env->GetFloatArrayRegion(array, start, len, buf);
}
void JEnv::GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, jdouble* buf)
{
	m_env->GetDoubleArrayRegion(array, start, len, buf);
}

void JEnv::SetByteArrayRegion(jbyteArray array, jsize start, jsize len, const jbyte* buf)
{
	m_env->SetByteArrayRegion(array, start, len, buf);
}
void JEnv::SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, const jboolean* buf)
{
	m_env->SetBooleanArrayRegion(array, start, len, buf);
}
void JEnv::SetCharArrayRegion(jcharArray array, jsize start, jsize len, const jchar* buf)
{
	m_env->SetCharArrayRegion(array, start, len, buf);
}
void JEnv::SetShortArrayRegion(jshortArray array, jsize start, jsize len, const jshort* buf)
{
	m_env->SetShortArrayRegion(array, start, len, buf);
}
void JEnv::SetIntArrayRegion(jintArray array, jsize start, jsize len, const jint* buf)
{
	m_env->SetIntArrayRegion(array, start, len, buf);
}
void JEnv::SetLongArrayRegion(jlongArray array, jsize start, jsize len, const jlong* buf)
{
	m_env->SetLongArrayRegion(array, start, len, buf);
}
void JEnv::SetFloatArrayRegion(jfloatArray array, jsize start, jsize len, const jfloat* buf)
{
	m_env->SetFloatArrayRegion(array, start, len, buf);
}
void JEnv::SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, const jdouble* buf)
{
	m_env->SetDoubleArrayRegion(array, start, len, buf);
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
	return m_env->NewDirectByteBuffer(address, capacity);
}
void* JEnv::GetDirectBufferAddress(jobject buf)
{
	return m_env->GetDirectBufferAddress(buf);
}

jlong JEnv::GetDirectBufferCapacity(jobject buf)
{
	return m_env->GetDirectBufferCapacity(buf);
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
	return m_env->GetObjectClass(obj);
}

jsize JEnv::GetArrayLength(jarray array)
{
	return m_env->GetArrayLength(array);
}


JavaVM* JEnv::s_jvm = nullptr;
map<string, jclass> JEnv::s_classCache;
jclass JEnv::PLATFORM_CLASS = nullptr;
jmethodID JEnv::GET_CACHED_CLASS_METHOD_ID = nullptr;
