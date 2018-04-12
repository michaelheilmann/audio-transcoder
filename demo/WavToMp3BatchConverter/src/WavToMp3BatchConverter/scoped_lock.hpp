#pragma once

#include "WavToMp3BatchConverter/mutex.hpp"

namespace nucleus {

struct scoped_lock
{
    scoped_lock() = delete;
    scoped_lock(const scoped_lock&) = delete;
    scoped_lock& operator=(const scoped_lock&) = delete;
    
    scoped_lock(mutex& mutex)
        : m_mutex(mutex)
    { m_mutex.lock(); }

    ~scoped_lock()
    { m_mutex.unlock(); }

private:
    mutex& m_mutex;

}; // struct scoped_lock

} // namespace nucleus
