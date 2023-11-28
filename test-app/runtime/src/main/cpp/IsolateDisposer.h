//
// Created by Eduardo Speroni on 3/4/22.
//

#ifndef TEST_APP_ISOLATEDISPOSER_H
#define TEST_APP_ISOLATEDISPOSER_H
#include "v8.h"
#include "robin_hood.h"
#include <vector>
#include <mutex>
#include <memory>

namespace tns {
    void disposeIsolate(v8::Isolate* isolate);
    using unique_void_ptr = std::unique_ptr<void, void(*)(void const*)>;
    template<typename T>
    auto unique_void(T * ptr) -> unique_void_ptr
    {
        return unique_void_ptr(ptr, [](void const * data) {
             T const * p = static_cast<T const*>(data);
             delete p;
        });
    }
    robin_hood::unordered_map<v8::Isolate*, std::shared_ptr<std::vector<unique_void_ptr>>> isolateBoundObjects_;
    std::mutex isolateBoundObjectsMutex_;
    template<typename T>
    void registerIsolateBoundObject(v8::Isolate* isolate, T *ptr) {
        std::lock_guard<std::mutex> lock(isolateBoundObjectsMutex_);
        auto it = isolateBoundObjects_.find(isolate);
        if (it == isolateBoundObjects_.end()) {
            auto vec = std::make_shared<std::vector<unique_void_ptr>>();
            vec->push_back(unique_void(ptr));
            isolateBoundObjects_.emplace(isolate, vec);
        } else {
            it->second->push_back(unique_void(ptr));
        }
    }
}

#endif //TEST_APP_ISOLATEDISPOSER_H
