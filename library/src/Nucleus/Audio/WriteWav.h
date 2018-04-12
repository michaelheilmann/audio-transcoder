#pragma once

#include "Nucleus/Audio/Buffer.h"

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_writeWav
    (
        const char *pathname,
        Nucleus_Audio_Buffer *buffer
    );
