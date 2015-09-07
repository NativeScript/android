#ifndef SIMPLEALLOCATOR_H_
#define SIMPLEALLOCATOR_H_

#include "v8.h"

namespace tns
{
	class SimpleAllocator : public v8::ArrayBuffer::Allocator
	{
	public:
		SimpleAllocator();

		~SimpleAllocator() override;

		void* Allocate(size_t length) override;

		void* AllocateUninitialized(size_t length) override;

		void Free(void* data, size_t length) override;
	};
}


#endif /* SIMPLEALLOCATOR_H_ */
