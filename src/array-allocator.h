#pragma once

#include "allocator.h"

namespace objectpool {

template <typename T, int N>
struct ArrayAllocator : Allocator<T>
{
    ArrayAllocator() {
        for (int i = 0; i < N; i++) {
            used_[i] = false;
        }
    }
    virtual T* allocate() override {
        for (int i = 0; i < N; i++) {
            if (!used_[i]) {
                used_[i] = true;
                return reinterpret_cast<T*>(&buf_[i * sizeof(T)]);
            }
        }
        throw std::bad_alloc();
    }

    virtual void deallocate(T* p) override {
        auto idx = (reinterpret_cast<int8_t*>(p) - buf_) / sizeof(T);
        used_[idx] = false;
    }

private:
    int8_t buf_[N * sizeof(T)];
    bool used_[N];
};
}  // namespace objectpool
