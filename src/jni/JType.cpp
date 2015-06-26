#include "JType.h"
#include "NativeScriptAssert.h"

namespace tns
{
	jobject JType::NewByte(JEnv env, jbyte value)
	{
		EnsureInstance(env, &Byte, Type::Byte);
		return env.NewObject(Byte->clazz, Byte->ctor, value);
	}

	jobject JType::NewChar(JEnv env, jchar value)
	{
		EnsureInstance(env, &Char, Type::Char);
		return env.NewObject(Char->clazz, Char->ctor, value);
	}

	jobject JType::NewBoolean(JEnv env, jboolean value)
	{
		EnsureInstance(env, &Boolean, Type::Boolean);
		return env.NewObject(Boolean->clazz, Boolean->ctor, value);
	}

	jobject JType::NewShort(JEnv env, jshort value)
	{
		EnsureInstance(env, &Short, Type::Short);
		return env.NewObject(Short->clazz, Short->ctor, value);
	}

	jobject JType::NewInt(JEnv env, jint value)
	{
		EnsureInstance(env, &Int, Type::Int);
		return env.NewObject(Int->clazz, Int->ctor, value);
	}

	jobject JType::NewLong(JEnv env, jlong value)
	{
		EnsureInstance(env, &Long, Type::Long);
		return env.NewObject(Long->clazz, Long->ctor, value);
	}

	jobject JType::NewFloat(JEnv env, jfloat value)
	{
		EnsureInstance(env, &Float, Type::Float);
		return env.NewObject(Float->clazz, Float->ctor, value);
	}

	jobject JType::NewDouble(JEnv env, jdouble value)
	{
		EnsureInstance(env, &Double, Type::Double);
		return env.NewObject(Double->clazz, Double->ctor, value);
	}

	jbyte JType::ByteValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Byte, Type::Byte);
		return env.GetByteField(value, Byte->valueField);
	}

	jchar JType::CharValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Char, Type::Char);
		return env.GetCharField(value, Char->valueField);
	}

	jboolean JType::BooleanValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Boolean, Type::Boolean);
		return env.GetBooleanField(value, Boolean->valueField);
	}

	jshort JType::ShortValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Short, Type::Short);
		return env.GetShortField(value, Short->valueField);
	}

	jint JType::IntValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Int, Type::Int);
		return env.GetIntField(value, Int->valueField);
	}

	jlong JType::LongValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Long, Type::Long);
		return env.GetLongField(value, Long->valueField);
	}

	jfloat JType::FloatValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Float, Type::Float);
		return env.GetFloatField(value, Float->valueField);
	}

	jdouble JType::DoubleValue(JEnv env, jobject value)
	{
		EnsureInstance(env, &Double, Type::Double);
		return env.GetDoubleField(value, Double->valueField);
	}

	void JType::EnsureInstance(JEnv env, JType **instance, Type type)
	{
		if((*instance) != nullptr)
		{
			return;
		}

		*instance = new JType();
		(*instance)->Init(env, type);
	}

	void JType::Init(JEnv env, Type type)
	{
		// TODO: Provide a fallback mechanism to prevent possible field name changes
		switch(type)
		{
			case Type::Byte:
				this->clazz = env.FindClass("java/lang/Byte");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(B)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "B");
				break;
			case Type::Char:
				this->clazz = env.FindClass("java/lang/Character");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(C)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "C");
				break;
			case Type::Boolean:
				this->clazz = env.FindClass("java/lang/Boolean");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(Z)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "Z");
				break;
			case Type::Short:
				this->clazz = env.FindClass("java/lang/Short");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(S)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "S");
				break;
			case Type::Int:
				this->clazz = env.FindClass("java/lang/Integer");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(I)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "I");
				break;
			case Type::Long:
				this->clazz = env.FindClass("java/lang/Long");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(J)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "J");
				break;
			case Type::Float:
				this->clazz = env.FindClass("java/lang/Float");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(F)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "F");
				break;
			case Type::Double:
				this->clazz = env.FindClass("java/lang/Double");
				this->ctor = env.GetMethodID(this->clazz, "<init>", "(D)V");
				this->valueField = env.GetFieldID(this->clazz, "value", "D");
				break;
		}
	}

	JType* JType::Byte;
	JType* JType::Char;
	JType* JType::Boolean;
	JType* JType::Short;
	JType* JType::Int;
	JType* JType::Long;
	JType* JType::Float;
	JType* JType::Double;
}
