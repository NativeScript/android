#ifndef JENV_H_
#define JENV_H_

#include "jni.h"
#include <map>
#include <string>

namespace tns
{
	class JEnv
	{
	public:
		JEnv();

		JEnv(JNIEnv *jniEnv);

		~JEnv();

		operator JNIEnv*() const;

		jclass GetObjectClass(jobject obj);

	    jsize GetArrayLength(jarray array);

		jmethodID GetMethodID(jclass clazz, const std::string& name, const std::string& sig);
		jmethodID GetStaticMethodID(jclass clazz, const std::string& name, const std::string& sig);

		jfieldID GetFieldID(jclass clazz, const std::string& name, const std::string& sig);
		jfieldID GetStaticFieldID(jclass clazz, const std::string& name, const std::string& sig);

		void CallStaticVoidMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		void CallNonvirtualVoidMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		void CallVoidMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jboolean CallStaticBooleanMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jboolean CallNonvirtualBooleanMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jboolean CallBooleanMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jbyte CallStaticByteMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jbyte CallNonvirtualByteMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jbyte CallByteMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jchar CallStaticCharMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jchar CallNonvirtualCharMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jchar CallCharMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jshort CallStaticShortMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jshort CallNonvirtualShortMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jshort CallShortMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jint CallStaticIntMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jint CallNonvirtualIntMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jint CallIntMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jlong CallStaticLongMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jlong CallNonvirtualLongMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jlong CallLongMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jfloat CallStaticFloatMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jfloat CallNonvirtualFloatMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jfloat CallFloatMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jdouble CallStaticDoubleMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jdouble CallNonvirtualDoubleMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jdouble CallDoubleMethodA(jobject obj, jmethodID methodID, jvalue* args);

		jobject CallStaticObjectMethodA(jclass clazz, jmethodID methodID, jvalue* args);
		jobject CallNonvirtualObjectMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue* args);
		jobject CallObjectMethodA(jobject obj, jmethodID methodID, jvalue* args);

	    jobject GetStaticObjectField(jclass clazz, jfieldID fieldID);
	    jboolean GetStaticBooleanField(jclass clazz, jfieldID fieldID);
	    jbyte GetStaticByteField(jclass clazz, jfieldID fieldID);
	    jchar GetStaticCharField(jclass clazz, jfieldID fieldID);
	    jshort GetStaticShortField(jclass clazz, jfieldID fieldID);
	    jint GetStaticIntField(jclass clazz, jfieldID fieldID);
	    jlong GetStaticLongField(jclass clazz, jfieldID fieldID);
	    jfloat GetStaticFloatField(jclass clazz, jfieldID fieldID);
	    jdouble GetStaticDoubleField(jclass clazz, jfieldID fieldID);

	    void SetStaticObjectField(jclass clazz, jfieldID fieldID, jobject value);
	    void SetStaticBooleanField(jclass clazz, jfieldID fieldID, jboolean value);
	    void SetStaticByteField(jclass clazz, jfieldID fieldID, jbyte value);
	    void SetStaticCharField(jclass clazz, jfieldID fieldID, jchar value);
	    void SetStaticShortField(jclass clazz, jfieldID fieldID, jshort value);
	    void SetStaticIntField(jclass clazz, jfieldID fieldID, jint value);
	    void SetStaticLongField(jclass clazz, jfieldID fieldID, jlong value);
	    void SetStaticFloatField(jclass clazz, jfieldID fieldID, jfloat value);
	    void SetStaticDoubleField(jclass clazz, jfieldID fieldID, jdouble value);

	    jobject GetObjectField(jobject obj, jfieldID fieldID);
	    jboolean GetBooleanField(jobject obj, jfieldID fieldID);
	    jbyte GetByteField(jobject obj, jfieldID fieldID);
	    jchar GetCharField(jobject obj, jfieldID fieldID);
	    jshort GetShortField(jobject obj, jfieldID fieldID);
	    jint GetIntField(jobject obj, jfieldID fieldID);
	    jlong GetLongField(jobject obj, jfieldID fieldID);
	    jfloat GetFloatField(jobject obj, jfieldID fieldID);
	    jdouble GetDoubleField(jobject obj, jfieldID fieldID);

	    void SetObjectField(jobject obj, jfieldID fieldID, jobject value);
	    void SetBooleanField(jobject obj, jfieldID fieldID, jboolean value);
	    void SetByteField(jobject obj, jfieldID fieldID, jbyte value);
	    void SetCharField(jobject obj, jfieldID fieldID, jchar value);
	    void SetShortField(jobject obj, jfieldID fieldID, jshort value);
	    void SetIntField(jobject obj, jfieldID fieldID, jint value);
	    void SetLongField(jobject obj, jfieldID fieldID, jlong value);
	    void SetFloatField(jobject obj, jfieldID fieldID, jfloat value);
	    void SetDoubleField(jobject obj, jfieldID fieldID, jdouble value);


		jstring NewString(const jchar* unicodeChars, jsize len);
		jstring NewStringUTF(const char* bytes);

		jobjectArray NewObjectArray(jsize length, jclass elementClass, jobject initialElement);

		jobject GetObjectArrayElement(jobjectArray array, jsize index);
		void SetObjectArrayElement(jobjectArray array, jsize index, jobject value);

		const char* GetStringUTFChars(jstring str, jboolean* isCopy);
		void ReleaseStringUTFChars(jstring str, const char* utf);

		const jchar* GetStringChars(jstring str, jboolean* isCopy);
		void ReleaseStringChars(jstring str, const jchar* chars);

		const int GetStringLength(jstring str);
		const int GetStringUTFLength(jstring str);
		void GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf);

		jint Throw(jthrowable obj);
		jboolean ExceptionCheck();
		jthrowable ExceptionOccurred();
		void ExceptionDescribe();
		void ExceptionClear();

		jboolean IsInstanceOf(jobject obj, jclass clazz);

		jobjectRefType GetObjectRefType(jobject obj);

		jobject NewGlobalRef(jobject obj);
		jweak NewWeakGlobalRef(jobject obj);
		void DeleteGlobalRef(jobject globalRef);
		void DeleteWeakGlobalRef(jweak obj);

		jobject NewLocalRef(jobject ref);
		void DeleteLocalRef(jobject localRef);

		jbyteArray NewByteArray(jsize length);
		jbooleanArray NewBooleanArray(jsize length);
		jcharArray NewCharArray(jsize length);
		jshortArray NewShortArray(jsize length);
		jintArray NewIntArray(jsize length);
		jlongArray NewLongArray(jsize length);
		jfloatArray NewFloatArray(jsize length);
		jdoubleArray NewDoubleArray(jsize length);

	    jbyte* GetByteArrayElements(jbyteArray array, jboolean* isCopy);
	    void ReleaseByteArrayElements(jbyteArray array, jbyte* elems, jint mode);


	    void GetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, jboolean* buf);
	    void GetByteArrayRegion(jbyteArray array, jsize start, jsize len, jbyte* buf);
	    void GetCharArrayRegion(jcharArray array, jsize start, jsize len, jchar* buf);
	    void GetShortArrayRegion(jshortArray array, jsize start, jsize len, jshort* buf);
	    void GetIntArrayRegion(jintArray array, jsize start, jsize len, jint* buf);
	    void GetLongArrayRegion(jlongArray array, jsize start, jsize len, jlong* buf);
	    void GetFloatArrayRegion(jfloatArray array, jsize start, jsize len, jfloat* buf);
	    void GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, jdouble* buf);

	    void SetByteArrayRegion(jbyteArray array, jsize start, jsize len, const jbyte* buf);
	    void SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, const jboolean* buf);
	    void SetCharArrayRegion(jcharArray array, jsize start, jsize len, const jchar* buf);
	    void SetShortArrayRegion(jshortArray array, jsize start, jsize len, const jshort* buf);
	    void SetIntArrayRegion(jintArray array, jsize start, jsize len, const jint* buf);
	    void SetLongArrayRegion(jlongArray array, jsize start, jsize len, const jlong* buf);
	    void SetFloatArrayRegion(jfloatArray array, jsize start, jsize len, const jfloat* buf);
	    void SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, const jdouble* buf);

		jclass FindClass(const std::string& className);

		jobject NewDirectByteBuffer(void* address, jlong capacity);
		void* GetDirectBufferAddress(jobject buf);
		jlong GetDirectBufferCapacity(jobject buf);

		jboolean IsAssignableFrom(jclass clazz1, jclass clazz2);


		template<typename... Args>
		void CallVoidMethod(jobject obj, jmethodID methodID, Args... args)
		{
			m_env->CallVoidMethod(obj, methodID, args...);
			CheckForJavaException();
		}

		template<typename... Args>
		void CallStaticVoidMethod(jclass clazz, jmethodID methodID, Args... args)
		{
			m_env->CallStaticVoidMethod(clazz, methodID, args...);
			CheckForJavaException();
		}

		template<typename... Args>
		void CallAppFail(jclass clazz, jmethodID methodID, Args... args)
		{
			m_env->CallStaticVoidMethod(clazz, methodID, args...);
		}

		template<typename... Args>
		jint CallStaticIntMethod(jclass clazz, jmethodID methodID, Args... args)
		{
			jint ji = m_env->CallStaticIntMethod(clazz, methodID, args...);
			CheckForJavaException();
			return ji;
		}

		template<typename... Args>
		jlong CallStaticLongMethod(jclass clazz, jmethodID methodID, Args... args)
		{
			jlong jd = m_env->CallStaticLongMethod(clazz, methodID, args...);
			CheckForJavaException();
			return jd;
		}

		template<typename... Args>
		jobject CallStaticObjectMethod(jclass clazz, jmethodID methodID, Args... args)
		{
			jobject jo = m_env->CallStaticObjectMethod(clazz, methodID, args...);
			CheckForJavaException();
			return jo;
		}

		template<typename... Args>
		jboolean CallStaticBooleanMethod(jclass clazz, jmethodID methodID, Args... args)
		{
			jboolean jbl = m_env->CallStaticBooleanMethod(clazz, methodID, args...);
			CheckForJavaException();
			return jbl;
		}

		template<typename... Args>
		jobject CallObjectMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jobject jo = m_env->CallObjectMethod(obj, methodID, args...);
			CheckForJavaException();
			return jo;
		}

		template<typename... Args>
		jboolean CallBooleanMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jboolean jbl = m_env->CallBooleanMethod(obj, methodID, args...);
			CheckForJavaException();
			return jbl;
		}

		template<typename... Args>
		jchar CallCharMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jchar jc = m_env->CallCharMethod(obj, methodID, args...);
			CheckForJavaException();
			return jc;
		}

		template<typename... Args>
		jbyte CallByteMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jbyte jbt = m_env->CallByteMethod(obj, methodID, args...);
			CheckForJavaException();
			return jbt;
		}

		template<typename... Args>
		jshort CallShortMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jshort jsh = m_env->CallShortMethod(obj, methodID, args...);
			CheckForJavaException();
			return jsh;
		}

		template<typename... Args>
		jint CallIntMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jint ji = m_env->CallIntMethod(obj, methodID, args...);
			CheckForJavaException();
			return ji;
		}

		template<typename... Args>
		jlong CallLongMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jlong jl = m_env->CallLongMethod(obj, methodID, args...);
			CheckForJavaException();
			return jl;
		}

		template<typename... Args>
		jfloat CallFloatMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jfloat jf = m_env->CallFloatMethod(obj, methodID, args...);
			CheckForJavaException();
			return jf;
		}

		template<typename... Args>
		jdouble CallDoubleMethod(jobject obj, jmethodID methodID, Args... args)
		{
			jdouble jd = m_env->CallDoubleMethod(obj, methodID, args...);
			CheckForJavaException();
			return jd;
		}

		template<typename... Args>
		jobject NewObject(jclass clazz, jmethodID methodID, Args... args)
		{
			jobject jo = m_env->NewObject(clazz, methodID, args...);
			CheckForJavaException();
			return jo;

		}

		static void Init(JavaVM *jvm);

	private:
		void CheckForJavaException();

		JNIEnv *m_env;

		static JavaVM *s_jvm;

		static jclass PLATFORM_CLASS;

		static jmethodID GET_CACHED_CLASS_METHOD_ID;

		static std::map<std::string, jclass> s_classCache;
	};
}



#endif /* JENV_H_ */
