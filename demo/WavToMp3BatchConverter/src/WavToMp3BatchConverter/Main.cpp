#include <stdlib.h>
#include <stdio.h>
#include <math.h>
extern "C" {
#include "Nucleus/Audio/Audio.h"
#include "Nucleus/Audio/ReadWav.h"
#include "Nucleus/Audio/WriteMp3.h"
#include "Nucleus/Audio/Buffer.h"
}
#include "WavToMp3BatchConverter/directory_iterator.hpp"
#include "WavToMp3BatchConverter/job.hpp"
#include "WavToMp3BatchConverter/get_directory_separator.hpp"
#include "WavToMp3BatchConverter/get_path_separator.hpp"
#include "WavToMp3BatchConverter/thread.hpp"
#include "WavToMp3BatchConverter/scoped_lock.hpp"
#include "WavToMp3BatchConverter/mutex.hpp"
#include "WavToMp3BatchConverter/get_number_of_processors.hpp"

#include <vector>
#include <deque>
#include <iostream>

#if defined(Nucleus_Compiler_GCC)
#if __has_include(<optional>)
    #include <optional>
    namespace stdx {
      using namespace ::std;
    }
#elif __has_include(<experimental/optional>)
    #include <experimental/optional>
    namespace stdx {
      using namespace ::std;
      using namespace ::std::experimental;
    }
#endif
#else
    #include <optional>
    namespace stdx {
        using namespace ::std;
    }
#endif

#include <algorithm>

namespace nucleus {

template <typename T>
struct concurrent_qeue
{
public:
    //
    concurrent_qeue()
        : m_mutex{}, m_elements{}
    {}

    //
    ~concurrent_qeue()
    {}

    size_t size() const
    {
        return m_elements.size();
    }
    //
    stdx::optional<T> pop()
    {
        scoped_lock lock(m_mutex);
        if (!m_elements.size())
            return stdx::optional<T>{};
        else
        {
            auto e = m_elements.front();
            m_elements.pop_front();
            return stdx::optional<T>{ e };
        }
    }

    void push(const T& x)
    {
        scoped_lock lock(m_mutex);
        m_elements.push_back(x);
    }
private:
    mutex m_mutex;
    std::deque<T> m_elements;
}; // struct concurrent_qeue

} // namespace nucleus

static nucleus::concurrent_qeue<nucleus::job> g_jobs;
static std::vector<std::shared_ptr<nucleus::thread>> g_threads;
static std::string g_pathname;

void convert
    (
        const nucleus::job& job
    )
{
    Nucleus_Status status;
    Nucleus_Audio_Buffer buffer;

    status = Nucleus_Audio_Buffer_initialize(&buffer, "silence", 1, 44100, 1);
    if (status) throw std::runtime_error("unable to initialize audio buffer");

    try
    {
        std::cout << "begin converting " << job.get_source() << " -> " << job.get_target() << std::endl;
        status = Nucleus_Audio_readWav(job.get_source().c_str(), &buffer);
        if (status) throw std::runtime_error("unable to read " + job.get_source());
        status = Nucleus_Audio_writeMp3(job.get_target().c_str(), &buffer);
        if (status) throw std::runtime_error("unable to write " + job.get_target());
        std::cout << "done converting " << job.get_source() << " -> " << job.get_target() << std::endl;
    }
    catch (...)
    {
        Nucleus_Audio_Buffer_uninitialize(&buffer);
        std::rethrow_exception(std::current_exception());
    }
    Nucleus_Audio_Buffer_uninitialize(&buffer);
}

int
main
    (
        int argc,
        char **argv
    )
{
    // TODO: Implement and utilize C++ wrapper of Nucleus_CommandLine.
    if (argc < 2)
    {
        std::cerr << "too few arguments" << std::endl;
        return EXIT_FAILURE;
    }
    else if (argc > 2)
    {
        std::cerr << "too many arguments" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "initializing Nucleus: Audio" << std::endl;
    if (Nucleus_Audio_initialize())
    {
        std::cerr << "unable to initialize Nucleus: Audio" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Nucleus: Audio initialized" << std::endl;

    try
    {
        g_pathname = argv[1];
        for (auto it = nucleus::directory_iterator(g_pathname); it != nucleus::directory_iterator(); ++it)
        {
            auto source = *it;
            if (nucleus::is_true_suffix(source, std::string(".wav")))
            {
                std::cout << source << " accepted" << std::endl;
                auto target = source.substr(0, source.length() - std::string(".wav").length()) + ".mp3";
                std::cout << source << " -> " << target << std::endl;
                const char *p; Nucleus_Size n;
                std::string directorySeparator = nucleus::get_directory_separator();
                if (nucleus::is_true_suffix(g_pathname, directorySeparator))
                {
                    source = g_pathname + source;
                    target = g_pathname + target;
                }
                else
                {
                    source = g_pathname + directorySeparator + source;
                    target = g_pathname + directorySeparator + target;
                }
                g_jobs.push({ source, target });
            }
            else
            {
                std::cout << *it << " rejected" << std::endl;
            }
        }

#define MODE CONCURRENT
#define SEQUENTIAL (0)
#define CONCURRENT (1)

#if defined(MODE) && MODE == SEQUENTIAL
        while (true)
        {
            auto e = g_jobs.pop();
            if (e)
            {
                convert(*e);
            }
            else
            {
                break;
            }
        }
#elif defined(MODE) && MODE == CONCURRENT
        // Determine the number of processors and add
        // min(n,m) threads where n is the number of
        // processors and m the number of files to
        // process.
        size_t numberOfProcessors = nucleus::get_number_of_processors();
        std::cout << "number of processors: " << numberOfProcessors << std::endl;

        size_t numberOfTasks = g_jobs.size();
        std::cout << "number of tasks: " << numberOfTasks << std::endl;

        size_t numberOfThreads = std::min(numberOfProcessors, numberOfTasks);
        std::cout << "number of threads:" << numberOfThreads << std::endl;

        for (size_t i = 0, n = numberOfThreads; i < n; ++i)
        {
            auto thread = std::make_shared<nucleus::thread>([](void *) -> void {
                while (true)
                {
                    auto e = g_jobs.pop();
                    if (e)
                    {
                        convert(*e);
                    }
                    else
                    {
                        break;
                    }
                }
            }, nullptr);
            g_threads.push_back(thread);
            thread->start();
        }
        for (auto it = g_threads.cbegin(); it != g_threads.cend(); ++it)
        {
            (*it)->join();
        }
#endif
    }
    catch (...)
    {
        std::cout << "uninitializing Nucleus: Audio" << std::endl;
        Nucleus_Audio_uninitialize();
        std::cout << "Nucleus.Audio uninitialized" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "uninitializing Nucleus: Audio" << std::endl;
    Nucleus_Audio_uninitialize();
    std::cout << "Nucleus: Audio uninitialized" << std::endl;
    return EXIT_SUCCESS;
}
