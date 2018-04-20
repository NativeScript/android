// Copyright 2006-2008 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_SNAPSHOT_SNAPSHOT_H_
#define V8_SNAPSHOT_SNAPSHOT_H_

#include <v8_inspector/src/globals.h>

namespace v8 {
namespace internal {

// Forward declarations.
class Isolate;
class BuiltinSerializer;
class PartialSerializer;
class StartupSerializer;

// Wrapper around reservation sizes and the serialization payload.

class Snapshot : public AllStatic {
    public:
        // To be implemented by the snapshot source.
        static v8::StartupData* DefaultSnapshotBlob();
};

}  // namespace internal
}  // namespace v8

#endif  // V8_SNAPSHOT_SNAPSHOT_H_
