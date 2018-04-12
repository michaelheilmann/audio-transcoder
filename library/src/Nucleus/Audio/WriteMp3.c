#include "Nucleus/Audio/WriteMp3.h"

#include <lame.h>
#include <malloc.h>
#include "Nucleus/Collections/ByteArray.h"
#include "Nucleus/FileSystem.h"

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
writeMp3
    (
        Nucleus_Audio_Buffer *sourceBuffer,
        Nucleus_Collections_ByteArray *targetBuffer
    )
{
    if (!sourceBuffer || !targetBuffer) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    //
    lame_global_flags *gfp;
    gfp = lame_init();
    if (!gfp) return Nucleus_Status_EnvironmentFailed;
    //
    lame_set_VBR(gfp, vbr_default);
    lame_set_num_channels(gfp, sourceBuffer->channels);
    //lame_set_num_samples(gfp, sourceBuffer->samples);
    lame_set_in_samplerate(gfp, sourceBuffer->sampleRate);
    //lame_set_brate(gfp,128); // Just keep defaults
    //lame_set_mode(gfp,1); // Just keep defaults
    //lame_set_quality(gfp,2); // Just keep defaults 
    //
    int ret_code = lame_init_params(gfp);
    if (ret_code < 0)
    {
        lame_close(gfp);
        return Nucleus_Status_EnvironmentFailed;
    }
    //
    int frames = sourceBuffer->frames;
    int samples = sourceBuffer->channels * sourceBuffer->frames;
    int temporaryBufferCapacity = 1.25* samples + 7200;
    char *temporaryBuffer = malloc(temporaryBufferCapacity);
    if (!temporaryBuffer)
    {
        lame_close(gfp);
        return Nucleus_Status_AllocationFailed;
    }
    //
    float *current = sourceBuffer->f32;
    while (frames > 0)
    {
        int framesCurrent = frames > 10 ? 10 : frames;
        if (sourceBuffer->channels == 2)
        {
            ret_code = lame_encode_buffer_interleaved_ieee_float(gfp,
                                                                 current,
                                                                 framesCurrent,
                                                                 temporaryBuffer,
                                                                 temporaryBufferCapacity);
        }
        else
        {
            ret_code = lame_encode_buffer_ieee_float(gfp,
                                                     current,
                                                     NULL,
                                                     framesCurrent,
                                                     temporaryBuffer,
                                                     temporaryBufferCapacity);
        }
        if (ret_code < 0)
        {
            free(temporaryBuffer);
            temporaryBuffer = NULL;
            lame_close(gfp);
            gfp = NULL;
            return Nucleus_Status_EnvironmentFailed;
        }
        else
        {
            status = Nucleus_Collections_ByteArray_appendMany(targetBuffer, temporaryBuffer, ret_code);
            if (status)
            {
                free(temporaryBuffer);
                temporaryBuffer = NULL;
                lame_close(gfp);
                gfp = NULL;
                return status;
            }
        }
        current += framesCurrent * sourceBuffer->channels; // Increment the pointer by the consumed frames.
        frames -= framesCurrent;
    }
    //
    ret_code = lame_encode_flush(gfp, temporaryBuffer, temporaryBufferCapacity);
    if (ret_code < 0)
    {
        free(temporaryBuffer);
        temporaryBuffer = NULL;
        lame_close(gfp);
        gfp = NULL;
        return Nucleus_Status_EnvironmentFailed;
    }
    //lame_get_lametag_frame(gfp, temporaryBuffer, temporaryBufferCapacity);
    status = Nucleus_Collections_ByteArray_appendMany(targetBuffer, temporaryBuffer, ret_code);
    if (status)
    {
        free(temporaryBuffer);
        temporaryBuffer = NULL;
        lame_close(gfp);
        gfp = NULL;
        return status;
    }
    free(temporaryBuffer);
    temporaryBuffer = NULL;
    lame_close(gfp);
    gfp = NULL;
    //
    return Nucleus_Status_Success;
}

Nucleus_NoError() Nucleus_NonNull() Nucleus_Status
Nucleus_Audio_writeMp3
    (
        const char *pathname,
        Nucleus_Audio_Buffer *buffer
    )
{
    if (!pathname || !buffer) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Nucleus_Collections_ByteArray outputBuffer;
    //
    status = Nucleus_Collections_ByteArray_initialize(&outputBuffer, 1024);
    if (status) return status;
    //
    status = writeMp3(buffer, &outputBuffer);
    if (status)
    {
        Nucleus_Collections_ByteArray_uninitialize(&outputBuffer);
        return status;
    }
    //
    void *contents;
    size_t contentsSize;
    status = Nucleus_Collections_ByteArray_lock(&outputBuffer, (void **)&contents, &contentsSize);
    if (status)
    {
        Nucleus_Collections_ByteArray_uninitialize(&outputBuffer);
        return status;
    }
    status = Nucleus_setFileContents(pathname, contents, contentsSize);
    Nucleus_Collections_ByteArray_unlock(&outputBuffer);                                 
    Nucleus_Collections_ByteArray_uninitialize(&outputBuffer);
    return status;
}
