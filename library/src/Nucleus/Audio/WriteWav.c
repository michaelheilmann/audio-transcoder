#include "Nucleus/Audio/WriteWav.h"

#include <sndfile.h>

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_writeWav
    (
        const char *pathname,
        Nucleus_Audio_Buffer *buffer
    )
{
    if (!pathname || !buffer) return Nucleus_Status_InvalidArgument;
    SF_INFO sfinfo ;
    sfinfo.channels = buffer->channels;
    sfinfo.samplerate = buffer->sampleRate;
    sfinfo.frames = buffer->frames;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_U8;
    SNDFILE * outfile = sf_open(pathname, SFM_WRITE, &sfinfo);
    sf_count_t count = sf_write_float(outfile, buffer->f32,
                                      buffer->channels * buffer->frames);
    sf_write_sync(outfile);
    sf_close(outfile);
    return Nucleus_Status_Success;
}
