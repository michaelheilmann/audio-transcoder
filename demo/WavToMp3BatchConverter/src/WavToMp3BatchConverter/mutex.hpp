#pragma once

extern "C" {
#include "Nucleus/Concurrency/Mutex.h"
}

namespace nucleus {

struct mutex
{
    mutex(const mutex&) = delete;

    mutex& operator=(const mutex&) = delete;

    mutex()
    {
        Nucleus_Concurrency_Mutex_initialize(&m_implementation);
    }

    ~mutex()
    {
        Nucleus_Concurrency_Mutex_uninitialize(&m_implementation);
    }

    void lock()
    {
        Nucleus_Concurrency_Mutex_lock(&m_implementation);
    }

    void unlock()
    {
        Nucleus_Concurrency_Mutex_unlock(&m_implementation);
    }

private:
    Nucleus_Concurrency_Mutex m_implementation;

}; // struct mutex

} // namespace nucleus
