#pragma once

#include "allocator.h"
#include <algorithm>

namespace objectpool {

// use max-heap to maintain the free objects
// so allocation will find it in O(lg(N))
template <typename T, int N>
struct HeapAllocator : Allocator<T>
{
    enum State {
        FREE = 1,
        USED = 0,
    };

    struct Entry {
        State state_;
        T* p_;

        friend bool operator <(const Entry& e1, const Entry& e2) {
            return e1.state_ < e2.state_;
        }
    };

    HeapAllocator() {
        for (int i = 0; i < N; i++) {
            max_heaps[i].state_ = FREE;
            max_heaps[i].p_ = reinterpret_cast<T*>(&buf_[i * sizeof(T)]);
        }
        std::make_heap(max_heaps, max_heaps + N);
        available_ = N;
    }
    virtual T* allocate() override {
        if (available_ == 0) {
            throw std::bad_alloc();
        }

        Entry ret = max_heaps[0];
        std::pop_heap(max_heaps, max_heaps + available_);
        --available_;
        return ret.p_;
    }

    virtual void deallocate(T* p) override {
        if (available_ >= N) {
            return;
        }
        max_heaps[available_].p_ = p;
        max_heaps[available_].state_ = FREE;
        ++available_;
        std::push_heap(max_heaps, max_heaps + available_);
    }

private:
    int8_t buf_[N * sizeof(T)];
    Entry max_heaps[N];
    int available_ {0};
};
}  // namespace objectpool
