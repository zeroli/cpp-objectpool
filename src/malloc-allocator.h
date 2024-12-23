#pragma once

#include "allocator.h"

namespace objectpool {

template <typename T>
class MallocAllocator : public Allocator<T>
{
public:
    virtual T* allocate() override {
        return reinterpret_cast<T*>(malloc(sizeof(T)));
    }
    virtual void deallocate(T* p) {
        free(p);
    }
};
}  // namespace objectpool
