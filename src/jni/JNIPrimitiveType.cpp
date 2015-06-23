#include "JNIPrimitiveType.h"
#include "NativeScriptAssert.h"

namespace tns
{
	jobject JNIPrimitiveType::NewByte(JEnv env, jbyte value)
	{
		EnsureInstance(env, &Byte, PrimitiveType::Byte);
		return env.NewObject(Byte->clazz, Byte->ctor, value);
	}

	jobject JNIPrimitiveType::NewChar(JEnv env, jchar value)
	{
		EnsureInstance(env, &Char, PrimitiveType::Char);
		return env.NewObject(Char->clazz, Char->ctor, value);
	}

	jobject JNIPrimitiveType::NewBoolean(JEnv env, jboolean value)
	{
		EnsureInstance(env, &Boolean, PrimitiveType::Boolean);
		return env.NewObject(Boolean->clazz, Boolean->ctor, value);
	}

	jobject JNIPrimitiveType::NewShort(JEnv env, jshort value)
	{
		EnsureInstance(env, &Short, PrimitiveType::Short);
		return env.NewObject(Short->clazz, Short->ctor, value);
	}

	jobject JNIPrimitiveType::NewInt(JEnv env, jint value)
	{
		EnsureInstance(env, &Int, PrimitiveType::Int);
		return env.NewObject(Int->clazz, Int->ctor, value);
	}

	jobject JNIPrimitiveType::NewLong(JEnv env, jlong value)
	{
		EnsureInstance(env, &Long, PrimitiveType::Long);
		return env.NewObject(Long->clazz, Long->ctor, value);
	}

	jobject JNIPrimitiveType::NewFloat(JEnv env, jfloat value)
	{
		EnsureInstance(env, &Float, PrimitiveType::Float);
		return env.NewObject(Float->clazz, Float->ctor, value);
	}

	jobject JNIPrimitiveType::NewDouble(JEnv env, jdouble value)
	{
		EnsureInstance(env, &Double, PrimitiveType::Double);
		return env.NewObject(Double->clazz, Double->ctor, value);
	}

	jbyte JNIPrimitiveType::ByteValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Byte, PrimitiveType::Byte);
		return env.CallByteMethod(value, Byte->value);
	}

	jchar JNIPrimitiveType::CharValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Char, PrimitiveType::Char);
		return env.CallCharMethod(value, Char->value);
	}

	jboolean JNIPrimitiveType::BooleanValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Boolean, PrimitiveType::Boolean);
		return env.CallBooleanMethod(value, Boolean->value);
	}

	jshort JNIPrimitiveType::ShortValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Short, PrimitiveType::Short);
		return env.CallShortMethod(value, Short->value);
	}

	jint JNIPrimitiveType::IntValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Int, PrimitiveType::Int);
		return env.CallIntMethod(value, Int->value);
	}

	jlong JNIPrimitiveType::LongValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Long, PrimitiveType::Long);
		return env.CallLongMethod(value, Long->value);
	}

	jfloat JNIPrimitiveType::FloatValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Float, PrimitiveType::Float);
		return env.CallFloatMethod(value, Float->value);
	}

	jdouble JNIPrimitiveType::DoubleValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Double, PrimitiveType::Double);
		return env.CallShortMethod(value, Double->value);
	}

	void JNIPrimitiveType::EnsureInstance(JEnv env, JNIPrimitiveType **instance, PrimitiveType type)
	{
		if((*instance) != nullptr)
		{
			return;
		}

		*instance = new JNIPrimitiveType();
		(*instance)->Init(env, type);
	}

	void JNIPrimitiveType::Init(JEnv env, PrimitiveType type)
	{
		switch(type)
		{
			case PrimitiveType::Byte:
				this->clazz = env.FindClass("java/lang/Byte");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(B)V");
				this->value = env.GetMethodID(this->clazz, "byteValue", "()B");
				break;
			case PrimitiveType::Char:
				this->clazz = env.FindClass("java/lang/Character");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(C)V");
				this->value = env.GetMethodID(this->clazz, "charValue", "()C");
				break;
			case PrimitiveType::Boolean:
				this->clazz = env.FindClass("java/lang/Boolean");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(Z)V");
				this->value = env.GetMethodID(this->clazz, "booleanValue", "()Z");
				break;
			case PrimitiveType::Short:
				this->clazz = env.FindClass("java/lang/Short");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(S)V");
				this->value = env.GetMethodID(this->clazz, "shortValue", "()S");
				break;
			case PrimitiveType::Int:
				this->clazz = env.FindClass("java/lang/Integer");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(I)V");
				this->value = env.GetMethodID(this->clazz, "intValue", "()I");
				break;
			case PrimitiveType::Long:
				this->clazz = env.FindClass("java/lang/Long");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(J)V");
				this->value = env.GetMethodID(this->clazz, "longValue", "()J");
				break;
			case PrimitiveType::Float:
				this->clazz = env.FindClass("java/lang/Float");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(F)V");
				this->value = env.GetMethodID(this->clazz, "floatValue", "()F");
				break;
			case PrimitiveType::Double:
				this->clazz = env.FindClass("java/lang/Double");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(D)V");
				this->value = env.GetMethodID(this->clazz, "doubleValue", "()D");
				break;
		}
	}

	JNIPrimitiveType* JNIPrimitiveType::Byte;
	JNIPrimitiveType* JNIPrimitiveType::Char;
	JNIPrimitiveType* JNIPrimitiveType::Boolean;
	JNIPrimitiveType* JNIPrimitiveType::Short;
	JNIPrimitiveType* JNIPrimitiveType::Int;
	JNIPrimitiveType* JNIPrimitiveType::Long;
	JNIPrimitiveType* JNIPrimitiveType::Float;
	JNIPrimitiveType* JNIPrimitiveType::Double;
}
