#include "Nucleus/Audio/Buffer.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Nucleus/Audio/Audio.h"
#include "Nucleus/Memory.h"
#include <sndfile.h>

#define PI 3.14159265359f

// duration = frames * samplerate
Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_getDuration
    (
        Nucleus_Audio_Buffer *buffer,
        float *duration
    )
{
    *duration = (float)buffer->frames * (float)buffer->sampleRate;
    return Nucleus_Status_Success;
}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_initialize
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int samplerate,
        int duration
    )
{
    if (!buffer || !generator || channels < 1) return Nucleus_Status_InvalidArgument;
    int frames = duration * samplerate;
    return Nucleus_Audio_Buffer_initialize_frames(buffer, generator, channels, samplerate, frames);
}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_initialize_frames
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int sampleRate,
        int frames
    )
{
    if (!buffer || !generator || channels < 1) return Nucleus_Status_InvalidArgument;
    
    buffer->sampleRate = sampleRate;
    buffer->channels = channels;
    buffer->frames = frames;
    long samples = frames * channels;

    Nucleus_Status status;
    status = Nucleus_allocateArrayMemory((void **)&buffer->f32, samples, sizeof(float));
    if (status) return status;

    if (!strcmp(generator, "sine"))
    {
        for (long i = 0;i < samples;++i)
        {
            float x = sin(2 * PI * 440 * i / buffer->sampleRate);
            buffer->f32[i] = x;
        }
    }
    else if (!strcmp(generator, "silence"))
    {
        for (long i = 0;i < samples;++i)
        {
            buffer->f32[i] = 0.0f;
        }
    }
    else
    {
        Nucleus_deallocateMemory(buffer->f32);
        return Nucleus_Status_InvalidArgument;
    }

    return Nucleus_Status_Success;
}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_reinitialize
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int samplerate,
        int duration
    )
{
    if (!buffer || !generator || channels < 1) return Nucleus_Status_InvalidArgument;
    int frames = duration * samplerate;
    return Nucleus_Audio_Buffer_reinitialize(buffer, generator, channels, samplerate, frames);
}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_reinitialize_frames
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int sampleRate,
        int frames
    )
{
    if (!buffer || !generator || channels < 1) return Nucleus_Status_InvalidArgument;

    long samples = frames * channels;

    Nucleus_Status status;
    float *newf32;
    status = Nucleus_allocateArrayMemory((void **)&newf32, samples, sizeof(float));
    if (status) return status;

    if (!strcmp(generator, "sine"))
    {
        for (long i = 0;i < samples;++i)
        {
            float x = sin(2 * PI * 440 * i / sampleRate);
            newf32[i] = x;
        }
    }
    else if (!strcmp(generator, "silence"))
    {
        for (long i = 0;i < samples;++i)
        {
            newf32[i] = 0.0f;
        }
    }
    else
    {
        Nucleus_deallocateMemory(newf32);
        return Nucleus_Status_InvalidArgument;
    }

    buffer->sampleRate = sampleRate;
    buffer->channels = channels;
    buffer->frames = frames;
    Nucleus_deallocateMemory(buffer->f32);
    buffer->f32 = newf32;

    return Nucleus_Status_Success;
}

Nucleus_NoError() Nucleus_NonNull() void
Nucleus_Audio_Buffer_uninitialize
    (
        Nucleus_Audio_Buffer *buffer
    )
{
    Nucleus_deallocateMemory(buffer->f32);
    buffer->f32 = NULL;
}
