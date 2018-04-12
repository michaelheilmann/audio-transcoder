#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Nucleus/Audio/Audio.h"
#include "Nucleus/FileSystem.h"
#include "Nucleus/Audio/ReadWav.h"
#include "Nucleus/Audio/WriteMp3.h"
#include "Nucleus/Audio/Buffer.h"

int
main
    (
        int argc,
        char **argv
    )
{
    if (argc < 3)
    {
        fprintf(stderr, "too few arguments\n");
        return EXIT_FAILURE;
    }
    else if (argc > 3)
    {
        fprintf(stderr, "too many arguments\n");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "initializing Nucleus.Audio\n");
    if (Nucleus_Audio_initialize())
    {
        fprintf(stdout, "unable to initialize Nucleus.Audio\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Nucleus.Audio initialized\n");
    fprintf(stdout, "uninitializing Nucleus.Audio\n");

    Nucleus_Status status;
    Nucleus_Audio_Buffer buffer;
    status = Nucleus_Audio_Buffer_initialize(&buffer, "silence", 1, 44100, 1);
    if (status) goto End1;
    status = Nucleus_Audio_readWav(argv[1], &buffer);
    if (status) goto End2;
    status = Nucleus_Audio_writeMp3(argv[2], &buffer);
    if (status) goto End2;

    Nucleus_Audio_Buffer_uninitialize(&buffer);

End2:
    Nucleus_Audio_Buffer_uninitialize(&buffer);
End1:
    Nucleus_Audio_uninitialize();
    fprintf(stdout, "Nucleus.Audio uninitialized\n");
    return EXIT_SUCCESS;
}
