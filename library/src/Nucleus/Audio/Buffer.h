// Copyright (c) Michael Heilmann 2018
#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include <stdint.h>

#define NUCLEUS_AUDIO_VERSION_MAJOR 0

#if defined(NUCLEUS_AUDIO_VERSION_MAJOR) && NUCLEUS_AUDIO_VERSION_MAJOR > 0
// The "syntactics" of audio data.
typedef enum Nucleus_Audio_PCM Nucleus_Audio_Syntactics;
enum Nucleus_Audio_Syntactics
{
    Nucleus_Audio_Syntactics_U8,
    Nucleus_Audio_Syntactics_F32,
}; // enum Nucleus_Audio_Syntactics
#endif

// An audio data buffer.
typedef struct Nucleus_Audio_Buffer Nucleus_Audio_Buffer;

struct Nucleus_Audio_Buffer
{
#if defined(NUCLEUS_AUDIO_VERSION_MAJOR) && NUCLEUS_AUDIO_VERSION_MAJOR > 0
    Nucleus_Audio_Syntactics syntactics; // "syntactics" of the audio data.
#endif
    union
    {
#if defined(NUCLEUS_AUDIO_VERSION_MAJOR) && NUCLEUS_AUDIO_VERSION_MAJOR > 0
        uint8_t *u8; // audio data within the bounds of [0,255]
#endif
        float *f32;  // audio data within the bounds of [-1,+1]
    };
    int sampleRate; // sample rate in Hz (e.g. 44100)
    int channels; // number of channels 1, 2, ....
    int frames; // a frame is a sample, one for each channel
}; // struct Nucleus_Audio_Buffer

// duration = frames * samplerate
Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_getDuration
    (
        Nucleus_Audio_Buffer *buffer,
        float *duration
    );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_initialize
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int sampleRate,
        int duration
    );

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_initialize_frames
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int sampleRate,
        int frames
    );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_reinitialize
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int sampleRate,
        int duration
    );

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_Buffer_reinitialize_frames
    (
        Nucleus_Audio_Buffer *buffer,
        const char *generator,
        int channels,
        int sampleRate,
        int frames
    );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Nucleus_NoError() Nucleus_NonNull() void
Nucleus_Audio_Buffer_uninitialize
    (
        Nucleus_Audio_Buffer *buffer
    );
