#ifndef WORKERMESSAGE_H_
#define WORKERMESSAGE_H_

#include "StructuredSerialization.h"

namespace tns {
namespace worker {

/*
 * What a worker posts: a value serialized on the sending isolate and read back
 * on the receiving one. The mechanism is shared with structuredClone; only the
 * host-object policy differs (see HostObjectPolicy).
 */
using Message = tns::serialization::SerializedValue;

}  // namespace worker
}  // namespace tns

#endif /* WORKERMESSAGE_H_ */
