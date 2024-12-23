#include <gtest/gtest.h>

#include "array-allocator.h"
#include "object-pool.h"

using namespace objectpool;

namespace {
constexpr int N = 100;
class A {
public:
    A() = default;
    ~A() = default;

    void* operator new(size_t size) {
        return pool.allocate();
    }
    void operator delete(void* p) {
        pool.deallocate(static_cast<A*>(p));
    }

    using pool_t = ObjectPool<A, ArrayAllocator<A, N>>;
    static pool_t pool;
private:
    int* p_tmp;
};

A::pool_t A::pool;
}  // namespace

TEST(objectpool, test_array)
{
    std::vector<A*> pas;
    for (int i = 0; i < 100; i++) {
        pas.push_back(new A());
    }
    for (auto* pa : pas) {
        delete pa;
    }
}
