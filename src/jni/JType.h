#ifndef JNIPRIMITIVETYPE_H_
#define JNIPRIMITIVETYPE_H_

#include "JEnv.h"

namespace tns
{
	enum class Type : int
	{
		Boolean,
		Char,
		Byte,
		Short,
		Int,
		Long,
		Float,
		Double,
		String,
		JsObject,
		Null
	};

	class JType
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

		static Type getClassType(int retType);

	private:
		JType()
		{
		}

		void Init(JEnv env, Type type);
		static void EnsureInstance(JEnv env, JType **instance, Type type);

		jclass clazz;
		jmethodID ctor;
		jfieldID valueField;

		static JType* Byte;
		static JType* Char;
		static JType* Boolean;
		static JType* Short;
		static JType* Int;
		static JType* Long;
		static JType* Float;
		static JType* Double;
	};
}

#endif /* JNIPRIMITIVETYPE_H_ */
