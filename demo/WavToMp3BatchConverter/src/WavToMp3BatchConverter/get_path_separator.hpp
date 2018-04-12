#pragma once

extern "C" {
#include "Nucleus/FileSystem.h"
} // extern "C"

#include <string>

namespace nucleus {

std::string get_path_separator()
{
    const char *bytes; Nucleus_Size numberOfBytes;
    Nucleus_getPathSeparator(&bytes, &numberOfBytes);
    return std::string(bytes);
}

} // namespace nucleus
