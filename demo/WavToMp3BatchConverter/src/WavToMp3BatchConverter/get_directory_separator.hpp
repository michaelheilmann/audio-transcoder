#pragma once

#include "Nucleus/FileSystem.h"
#include <string>

namespace nucleus {

std::string get_directory_separator()
{
    const char *bytes; Nucleus_Size numberOfBytes;
    Nucleus_getDirectorySeparator(&bytes, &numberOfBytes);
    return std::string(bytes);
}

} // namespace nucleus
