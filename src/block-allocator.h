#pragma once

#include "allocator.h"
#include <vector>

namespace objectpool {

// use free list to maintain the free objects
// so allocation will find it in O(1)
template <typename T, int ChunksPerBlock>
struct BlockAllocator : Allocator<T>
{
    static_assert(sizeof(T) >= sizeof(void*));

    BlockAllocator() {
        free_.next = nullptr;
    }
    ~BlockAllocator() {
        for (auto* p : chunks_) {
            free(p);
        }
    }
    virtual T* allocate() override {
        if (free_.next == nullptr) {
            free_.next = alloc_block();
        }
        T* p = (T*)free_.next;
        free_.next = free_.next->next;
        return p;
    }

    virtual void deallocate(T* p) override {
        Chunk* pc = (Chunk*)p;
        pc->next = free_.next;
        free_.next = pc;
    }

private:
    struct Chunk {
        Chunk* next;
    };

    Chunk* alloc_block() {
        size_t n = ChunksPerBlock * sizeof(T);
        Chunk* head = (Chunk*)(malloc(n));
        Chunk* chunk = head;
        /// connect all chunks by next pointer
        for (int i = 0; i < ChunksPerBlock - 1; i++) {
            chunk->next = (Chunk*)((uint8_t*)chunk + sizeof(T));
            chunk = chunk->next;
        }
        chunk->next = nullptr;
        chunks_.push_back(head);
        return head;
    }
private:
    std::vector<Chunk*> chunks_;
    Chunk free_;
};
}  // namespace objectpool
