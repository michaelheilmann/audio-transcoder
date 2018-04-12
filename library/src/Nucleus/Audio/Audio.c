// Copyright (c) Michael Heilmann 2018
#include "Nucleus/Audio/Audio.h"

#include <stdlib.h>
#include <string.h>
#include "Nucleus/Audio/WriteMp3.h"
#include "Nucleus/Audio/WritePcm.h"
#include "Nucleus/Audio/WriteWav.h"
#include "Nucleus/Audio/Audio.h"

Nucleus_NoError() Nucleus_Status
Nucleus_Audio_initialize
    (
    )
{ return Nucleus_Status_Success; }

Nucleus_NoError() void
Nucleus_Audio_uninitialize
    (
    )
{}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
test_sine_1c_u8_mp3
    (
        const char *pathname
    )
{
    if (!pathname) return Nucleus_Status_InvalidArgument;
    //
    Nucleus_Status status;
    Nucleus_Audio_Buffer buffer;
    status = Nucleus_Audio_Buffer_initialize
        (
            &buffer,
            "sine",
            1,
            44100,
            2
        );
    if (status) return status;
    //
    status = Nucleus_Audio_writeMp3(pathname, &buffer);
    if (status)
    {
        Nucleus_Audio_Buffer_uninitialize(&buffer);
        return status;
    }
    //
    Nucleus_Audio_Buffer_uninitialize(&buffer);
    //
    return Nucleus_Status_Success;
}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
test_sine_1c_u8_wav
    (
        const char *pathname
    )
{
    if (!pathname) return Nucleus_Status_InvalidArgument;
    //
    Nucleus_Status status;
    Nucleus_Audio_Buffer buffer;
    status = Nucleus_Audio_Buffer_initialize
        (
            &buffer,
            "sine",
            1,
            44100,
            2
        );
    if (status) return status;
    //
    status = Nucleus_Audio_writeWav(pathname, &buffer);
    if (status)
    {
        Nucleus_Audio_Buffer_uninitialize(&buffer);
        return status;
    }
    //
    Nucleus_Audio_Buffer_uninitialize(&buffer);
    //
    return Nucleus_Status_Success;
}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
test_sine_1c_u8_raw
    (
        const char *pathname
    )
{
    if (!pathname) return Nucleus_Status_InvalidArgument;
    //
    Nucleus_Status status;
    Nucleus_Audio_Buffer buffer;
    status = Nucleus_Audio_Buffer_initialize
        (
            &buffer,
            "sine",
            1,
            44100,
            2
        );
    if (status) return status;
    //
    status = Nucleus_Audio_writePcm(pathname, &buffer);
    if (status)
    {
        Nucleus_Audio_Buffer_uninitialize(&buffer);
        return status;
    }
    //
    Nucleus_Audio_Buffer_uninitialize(&buffer);
    //
    return status;
}
