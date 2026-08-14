#ifndef SRC_UTIL_INL_H_
#define SRC_UTIL_INL_H_

// js_native_api_v8.cc includes this name expecting Node's util header; of that
// header it uses only node::OnScopeLeave.

#include <utility>

#include "js_native_api_v8_internals.h"

namespace node {

template <typename Fn>
struct OnScopeLeaveImpl {
  Fn fn_;
  bool active_;

  explicit OnScopeLeaveImpl(Fn&& fn) : fn_(std::move(fn)), active_(true) {}
  ~OnScopeLeaveImpl() {
    if (active_) fn_();
  }

  OnScopeLeaveImpl(const OnScopeLeaveImpl& other) = delete;
  OnScopeLeaveImpl& operator=(const OnScopeLeaveImpl& other) = delete;
  OnScopeLeaveImpl(OnScopeLeaveImpl&& other)
      : fn_(std::move(other.fn_)), active_(other.active_) {
    other.active_ = false;
  }
};

// Runs `fn` when the returned guard leaves scope, however it is left.
template <typename Fn>
inline OnScopeLeaveImpl<Fn> OnScopeLeave(Fn&& fn) {
  return OnScopeLeaveImpl<Fn>(std::move(fn));
}

}  // namespace node

#endif  // SRC_UTIL_INL_H_
