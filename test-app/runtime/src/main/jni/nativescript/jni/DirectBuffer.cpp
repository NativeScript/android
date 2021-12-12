#include "DirectBuffer.h"
#include "JniLocalRef.h"

using namespace tns;

DirectBuffer::DirectBuffer(uint32_t length) {
    m_length = length;

    m_data = new int[m_length];

    m_end = m_data + m_length;

    Reset();

    int capacity = m_length * sizeof(int);

    JEnv env;
    JniLocalRef buff(env.NewDirectByteBuffer(m_data, capacity));

    m_buff = env.NewGlobalRef(buff);
}

DirectBuffer::operator jobject() const {
    return m_buff;
}

int* DirectBuffer::GetData() const {
    return m_data;
}

int DirectBuffer::Length() const {
    return m_length;
}

int DirectBuffer::Size() const {
    return m_pos - m_data;
}

void DirectBuffer::Reset() {
    m_pos = m_data;
}

bool DirectBuffer::Write(int value) {
    bool canWrite = m_pos < m_end;
    if (canWrite) {
        int bigEndianInt = __builtin_bswap32(value);
        *(m_pos++) = bigEndianInt;
    }
    return canWrite;
}

DirectBuffer::~DirectBuffer() {
    JEnv env;
    env.DeleteGlobalRef(m_buff);
    delete[] m_data;
}
