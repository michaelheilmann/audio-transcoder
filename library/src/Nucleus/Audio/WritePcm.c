#include "Nucleus/Audio/WritePcm.h"

#include "Nucleus/FileSystem.h"
#include "Nucleus/Memory.h"

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_writePcm
    (
        const char *pathname,
        Nucleus_Audio_Buffer *buffer
    )
{
    if (!pathname || !buffer) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    int samples = buffer->channels * buffer->frames;
    unsigned char *temporary;
    status = Nucleus_allocateArrayMemory((void **)&temporary, samples, sizeof(unsigned char));
    if (status) return status;
    for (int i = 0, n = samples; i < n; ++i)
    {
        temporary[i] = 255 * ((1.0f + buffer->f32[i]) / 2.0f);
        if (temporary[i] > 255) temporary[i] = 255;
    }
    status = Nucleus_setFileContents(pathname, (const char *)temporary,
                                     buffer->channels * buffer->frames);
    Nucleus_deallocateMemory(temporary);
    return status;
}
