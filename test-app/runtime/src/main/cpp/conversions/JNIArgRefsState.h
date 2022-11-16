//
// Created by Vladimir Mutafov on 16.11.22.
//

#ifndef TEST_APP_JNIARGREFSSTATE_H
#define TEST_APP_JNIARGREFSSTATE_H

#include "JEnv.h"
#include "ConversionConsts.h"

namespace tns {
    struct JNIArgRefsState {
        jvalue m_args[JAVA_METHOD_MAX_ARGS_LENGTH];
        int m_args_refs[JAVA_METHOD_MAX_ARGS_LENGTH];
        int m_args_refs_size = 0;

        void SetConvertedObject(int index, jobject obj, bool isGlobal = false) {
            m_args[index].l = obj;
            if ((obj != nullptr) && !isGlobal) {
                m_args_refs[m_args_refs_size++] = index;
            }
        }
    };
}

#endif //TEST_APP_JNIARGREFSSTATE_H
