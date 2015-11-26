#include "JniLocalRef.h"
#include "JType.h"
#include <cassert>

using namespace v8;
using namespace tns;

JniLocalRef::JniLocalRef()
	: m_obj(nullptr), m_isWeak(false)
{
}


JniLocalRef::JniLocalRef(jobject obj, bool isWeak)
	: m_obj(obj), m_isWeak(isWeak)
{
}


JniLocalRef::JniLocalRef(jclass obj)
	: m_obj(obj), m_isWeak(false)
{
}

JniLocalRef::JniLocalRef(const JniLocalRef& rhs)
{
	JEnv env;

	m_obj = rhs.m_isWeak ? rhs.m_obj : env.NewLocalRef(rhs.m_obj);
	m_isWeak = rhs.m_isWeak;
}



bool JniLocalRef::IsNull() const
{
	return m_obj == nullptr;
}


JniLocalRef& JniLocalRef::operator=(const JniLocalRef& rhs)
{
    if(this != &rhs)
    {
    	m_isWeak = rhs.m_isWeak;
    	if (m_isWeak)
    	{
    		m_obj = rhs.m_obj;
    	}
    	else
    	{
			JEnv env;
			if (m_obj != nullptr)
			{
				env.DeleteLocalRef(m_obj);
			}
			m_obj = (rhs.m_obj != nullptr)
						? env.NewLocalRef(rhs.m_obj)
						: nullptr;
    	}
    }
    return *this;
}

JniLocalRef::operator jobject() const
{
	return m_obj;
}


JniLocalRef::operator jstring() const
{
	return (jstring)m_obj;
}


JniLocalRef::operator jclass() const
{
	return (jclass)m_obj;
}

JniLocalRef::operator jboolean() const
{
	JEnv env;
	return JType::BooleanValue(env, m_obj);
}


JniLocalRef::operator jthrowable() const
{
	return (jthrowable)m_obj;
}


JniLocalRef::operator jarray() const
{
	return (jarray)m_obj;
}


JniLocalRef::operator jbyteArray() const
{
	return (jbyteArray)m_obj;
}


JniLocalRef::operator jshortArray() const
{
	return (jshortArray)m_obj;
}


JniLocalRef::operator jintArray() const
{
	return (jintArray)m_obj;
}


JniLocalRef::operator jlongArray() const
{
	return (jlongArray)m_obj;
}


JniLocalRef::operator jfloatArray() const
{
	return (jfloatArray)m_obj;
}


JniLocalRef::operator jdoubleArray() const
{
	return (jdoubleArray)m_obj;
}


JniLocalRef::operator jbooleanArray() const
{
	return (jbooleanArray)m_obj;
}


JniLocalRef::operator jcharArray() const
{
	return (jcharArray)m_obj;
}


JniLocalRef::operator jobjectArray() const
{
	return (jobjectArray)m_obj;
}

JniLocalRef::~JniLocalRef()
{
	if ((m_obj != nullptr) && !m_isWeak)
	{
		JEnv env;
		env.DeleteLocalRef(m_obj);
	}
}
