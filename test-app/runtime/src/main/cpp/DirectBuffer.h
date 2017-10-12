#ifndef DIRECTBUFFER_H_
#define DIRECTBUFFER_H_

#include "JEnv.h"

namespace tns {
class DirectBuffer {
    public:
        DirectBuffer(uint32_t capacity = 65536);
        ~DirectBuffer();

        operator jobject() const;

        int* GetData() const;
        int Length() const;
        int Size() const;

        void Reset();
        bool Write(int value);

    private:
        jobject m_buff;
        int* m_data;
        jlong m_length;
        int* m_pos;
        int* m_end;
};
}

#endif /* DIRECTBUFFER_H_ */
