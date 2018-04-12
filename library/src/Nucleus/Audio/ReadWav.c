#include "Nucleus/Audio/ReadWav.h"

#include <sndfile.h>

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_readWav
    (
        const char *pathname,
        Nucleus_Audio_Buffer *buffer
    )
{
    if (!pathname || !buffer) return Nucleus_Status_InvalidArgument;
    SF_INFO sfinfo;
    sfinfo.format = 0;
    SNDFILE *file = sf_open(pathname, SFM_READ, &sfinfo);
    if (!file)
    {
        return Nucleus_Status_EnvironmentFailed;
    }
    // Nucleus Audio assumes samples = channels * samplerate * duration in Nucleus Audio.
    //int duration = sfinfo.frames / sfinfo.samplerate;
    Nucleus_Audio_Buffer_reinitialize_frames(buffer, "silence", sfinfo.channels, sfinfo.samplerate, sfinfo.frames);
    int frames = buffer->frames;
    if (sfinfo.frames != frames)
    {
        return Nucleus_Status_ReadError;
    }
    sf_count_t framesRead = sf_readf_float(file, buffer->f32, frames);
    if (frames != framesRead)
    {
        sf_close(file);
        return Nucleus_Status_ReadError;
    }
    sf_close(file);
    return Nucleus_Status_Success;
}
