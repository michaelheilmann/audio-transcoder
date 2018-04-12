#include "WavToMp3BatchConverter/get_number_of_processors.hpp"

extern "C" {
#include "Nucleus/getNumberOfProcessors.h"
} // extern "C"

#include <stdexcept>

namespace nucleus {

std::size_t get_number_of_processors()
{
    Nucleus_Size numberOfProcessors;
    Nucleus_Status status = Nucleus_getNumberOfProcessors(&numberOfProcessors);
    if (status) throw std::runtime_error("Nucleus_getNumberOfProcessors failed");
    return numberOfProcessors;
}

} // namespace nucleus