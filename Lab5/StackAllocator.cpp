#include "pch.h"
#include "StackAllocator.h"

void StackAllocator::init(size_t total_size)
{
    m_TotalSize = total_size;
    m_Offset = 0;
    m_StartPtr = malloc(total_size);
}

void* StackAllocator::allocate(size_t size)
{
    if (m_StartPtr == nullptr || m_Offset + size > m_TotalSize)
    {
        return nullptr;
    }
    void* addr = (char*)m_StartPtr + m_Offset;
    m_Offset += size;
    return addr;
}

void StackAllocator::reset()
{
    m_Offset = 0;
}

StackAllocator::~StackAllocator()
{
    reset();
    if (m_StartPtr != nullptr)
    {
        free(m_StartPtr);
        m_StartPtr = nullptr;
    }
}
