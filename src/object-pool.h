#pragma once

namespace objectpool
{
template <typename T, typename Allocator>
struct ObjectPool {
    T* allocate()
    {
        return alloc_.allocate();
    }
    void deallocate(T* p)
    {
        alloc_.deallocate(p);
    }

private:
    Allocator alloc_;
};
}  // namespace objectpool
