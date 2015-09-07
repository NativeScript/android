#include "SimpleAllocator.h"
#include <cstdlib>


using namespace tns;


SimpleAllocator::SimpleAllocator()
{
}



SimpleAllocator::~SimpleAllocator()
{
}

void* SimpleAllocator::Allocate(size_t length)
{
	void *data = calloc(length, 1);
	return data;
}

void* SimpleAllocator::AllocateUninitialized(size_t length)
{
	void *data = malloc(length);
	return data;

}

void SimpleAllocator::Free(void* data, size_t length)
{
	free(data);
}



