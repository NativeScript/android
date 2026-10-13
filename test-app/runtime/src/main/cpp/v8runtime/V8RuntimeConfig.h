/*
 * Copyright (c) Kudo Chien.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <string>
#include <vector>

namespace rnv8 {

struct V8RuntimeConfig {
  V8RuntimeConfig() {}
  ~V8RuntimeConfig() = default;

  V8RuntimeConfig(const V8RuntimeConfig& rhs) = delete;
  V8RuntimeConfig& operator=(const V8RuntimeConfig& rhs) = delete;

  V8RuntimeConfig(V8RuntimeConfig&& rhs) = default;
  V8RuntimeConfig& operator=(V8RuntimeConfig&& rhs) = default;
};

}  // namespace rnv8