#pragma once

extern "C" {
#include "Nucleus/Concurrency/Thread.h"
} // extern "C"

#include <functional>

namespace nucleus {

/// - thread is in suspended state when created and must be explicitly started
/// - currently implementation does not allow for moving (unlike the C++ threads)
/// - arguments shold be suppliable
/// - exceptions in the thread function are currently ignored
class thread
{
public:
    thread(std::function<void (void *)> function, void *arguments)
        : m_implementation(),
          m_function(function),
          m_arguments(arguments)
    {
        Nucleus_Status status;
        status = Nucleus_Concurrency_Thread_initialize(&m_implementation,
                                                       (Nucleus_Concurrency_Thread_CallbackContext *)this,
                                                       (Nucleus_Concurrency_Thread_CallbackFunction *)&callbackFunction);
        if (status)
        { throw ::std::runtime_error("Nucleus_Concurrency_Thread_initialize failed"); }
    }

    thread(const thread&) = delete;
    thread& operator=(const thread&) = delete;
    
    ~thread()
    {
        Nucleus_Concurrency_Thread_uninitialize(&m_implementation);
    }

    void start()
    {
        // Currently implementation only fails if the single argument would be null.
        Nucleus_Concurrency_Thread_start(&m_implementation);
    }

    bool join()
    {
        // Currently implementation only fails if the single argument would be null.
        Nucleus_Status status = Nucleus_Concurrency_Thread_join(&m_implementation);
        if (status == Nucleus_Status_NotStarted) return false;
        return true;
    }

private:
    std::function<void(void *)> m_function;
    void *m_arguments;
    Nucleus_Concurrency_Thread m_implementation;

    static void callbackFunction(thread *self)
    {
        try
        {
            self->m_function(self->m_arguments);
        }
        catch (...)
        {
        }
    }
};

} // namespace nucleus