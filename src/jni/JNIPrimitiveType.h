#ifndef JNIPRIMITIVETYPE_H_
#define JNIPRIMITIVETYPE_H_

#include "JEnv.h";

namespace tns
{
	enum class PrimitiveType
	{
		Byte,
		Char,
		Boolean,
		Short,
		Int,
		Long,
		Float,
		Double
	};
	class JNIPrimitiveType
	{
	public:
		static jobject NewByte(JEnv env, jbyte value);
		static jobject NewChar(JEnv env, jchar value);
		static jobject NewBoolean(JEnv env, jboolean value);
		static jobject NewShort(JEnv env, jshort value);
		static jobject NewInt(JEnv env, jint value);
		static jobject NewLong(JEnv env, jlong value);
		static jobject NewFloat(JEnv env, jfloat value);
		static jobject NewDouble(JEnv env, jdouble value);

		static jbyte ByteValue(JEnv env, jobject value);
		static jchar CharValue(JEnv env, jobject value);
		static jboolean BooleanValue(JEnv env, jobject value);
		static jshort ShortValue(JEnv env, jobject value);
		static jint IntValue(JEnv env, jobject value);
		static jlong LongValue(JEnv env, jobject value);
		static jfloat FloatValue(JEnv env, jobject value);
		static jdouble DoubleValue(JEnv env, jobject value);

	private:
		JNIPrimitiveType()
		{
		}

		void Init(JEnv env, PrimitiveType type);
		static void EnsureInstance(JEnv env, JNIPrimitiveType **instance, PrimitiveType type);

		jclass clazz;
		jmethodID ctor;
		jmethodID value;

		static JNIPrimitiveType* Byte;
		static JNIPrimitiveType* Char;
		static JNIPrimitiveType* Boolean;
		static JNIPrimitiveType* Short;
		static JNIPrimitiveType* Int;
		static JNIPrimitiveType* Long;
		static JNIPrimitiveType* Float;
		static JNIPrimitiveType* Double;
	};
}

#endif /* JNIPRIMITIVETYPE_H_ */
