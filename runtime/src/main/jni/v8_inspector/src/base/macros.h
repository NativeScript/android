// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_MACROS_H_
#define V8_BASE_MACROS_H_

#include <stdint.h>
#include "src/base/logging.h"
// A macro to disallow the evil copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete


// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];



namespace v8 {
namespace base {


// TODO(yangguo): This is a poor man's replacement for std::is_fundamental,
// which requires C++11. Switch to std::is_fundamental once possible.
template <typename T>
inline bool is_fundamental() {
  return false;
}

template <>
inline bool is_fundamental<uint8_t>() {
  return true;
}

}  // namespace base
}  // namespace v8

#endif   // V8_BASE_MACROS_H_
