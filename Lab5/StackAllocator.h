#pragma once
#include <cstdlib>

class StackAllocator {
public:
    void* m_StartPtr = nullptr;
    size_t m_TotalSize;
    size_t m_Offset;

    void init(size_t total_size);
    void* allocate(size_t size);
    void reset();
    ~StackAllocator();
};

