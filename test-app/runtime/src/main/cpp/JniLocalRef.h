#ifndef JNILOCALREF_H_
#define JNILOCALREF_H_

#include "JEnv.h"
#include "JType.h"
#include "v8.h"

namespace tns {
class JniLocalRef {
    public:
        JniLocalRef()
            : m_obj(nullptr), m_isGlobal(false) {
        }

        JniLocalRef(jobject obj, bool isGlobal = false)
            : m_obj(obj), m_isGlobal(isGlobal) {
        }

        JniLocalRef(jclass obj)
            : m_obj(obj), m_isGlobal(false) {
        }

        JniLocalRef(JniLocalRef&& rhs)
            : m_obj(rhs.m_obj), m_isGlobal(rhs.m_isGlobal) {
            rhs.m_obj = nullptr;
        }

        bool IsNull() const {
            return m_obj == nullptr;
        }

        bool IsGlobal() const {
            return m_isGlobal;
        }

        jobject Move() {
            auto value = m_obj;
            m_obj = nullptr;
            return value;
        }

        JniLocalRef& operator=(JniLocalRef&& rhs) {
            m_obj = rhs.m_obj;
            m_isGlobal = rhs.m_isGlobal;
            rhs.m_obj = nullptr;
            return *this;
        }

        operator jobject() const {
            return m_obj;
        }

        operator jstring() const {
            return reinterpret_cast<jstring>(m_obj);
        }

        operator jclass() const {
            return reinterpret_cast<jclass>(m_obj);
        }

        operator jboolean() const {
            JEnv env;
            return JType::BooleanValue(env, m_obj);
        }

        operator jthrowable() const {
            return reinterpret_cast<jthrowable>(m_obj);
        }

        operator jarray()const {
            return reinterpret_cast<jarray>(m_obj);
        }

        operator jbyteArray() const {
            return reinterpret_cast<jbyteArray>(m_obj);
        }

        operator jshortArray() const {
            return reinterpret_cast<jshortArray>(m_obj);
        }

        operator jintArray() const {
            return reinterpret_cast<jintArray>(m_obj);
        }

        operator jlongArray() const {
            return reinterpret_cast<jlongArray>(m_obj);
        }

        operator jfloatArray() const {
            return reinterpret_cast<jfloatArray>(m_obj);
        }

        operator jdoubleArray() const {
            return reinterpret_cast<jdoubleArray>(m_obj);
        }

        operator jbooleanArray() const {
            return reinterpret_cast<jbooleanArray>(m_obj);
        }

        operator jcharArray() const {
            return reinterpret_cast<jcharArray>(m_obj);
        }

        operator jobjectArray() const {
            return reinterpret_cast<jobjectArray>(m_obj);
        }

        ~JniLocalRef() {
            if ((m_obj != nullptr) && !m_isGlobal) {
                JEnv env;
                env.DeleteLocalRef(m_obj);
            }
        }

    private:
        jobject m_obj;
        bool m_isGlobal;
};
}

#endif /* JNILOCALREF_H_ */
