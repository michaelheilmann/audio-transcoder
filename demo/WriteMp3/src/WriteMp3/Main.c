#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Nucleus/Audio/Audio.h"
#include "Nucleus/FileSystem.h"

int
main
    (
        int argc,
        char **argv
    )
{
    fprintf(stdout, "initializing Nucleus.Audio\n");
    if (Nucleus_Audio_initialize())
    {
        fprintf(stdout, "unable to initialize Nucleus.Audio\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Nucleus.Audio initialized\n");
    fprintf(stdout, "uninitializing Nucleus.Audio\n");

    test_sine_1c_u8_mp3("sine_1c_u8.mp3");

    Nucleus_Audio_uninitialize();
    fprintf(stdout, "Nucleus.Audio uninitialized\n");
    return EXIT_SUCCESS;
}
