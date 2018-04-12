// Copyright (c) Michael Heilmann 2018
#pragma once

#include "Nucleus/Audio/Buffer.h"
#include <stdint.h>

Nucleus_NoError() Nucleus_Status
Nucleus_Audio_initialize
    (
    );

Nucleus_NoError() void
Nucleus_Audio_uninitialize
    (
    );

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
test_sine_1c_u8_mp3
    (
        const char *pathname
    );

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
test_sine_1c_u8_wav
    (
        const char *pathname
    );

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
test_sine_1c_u8_raw
    (
        const char *pathname
    );
