# Audio Transcoder

Audio Transcoder is cross-platform audio transcoder for Windows, Linux, and OS X written in C and C++.

It depends on
- [Nucleus](https://github.com/primordialmachine/nucleus)
- [Lame (with CMake support)](https://github.com/primordialmachine/lame), and
- [libsndfile (with CMake support)](https://github.com/primordialmachine/libsndfile).

A requirement was static linking of Lame and libsndfile. As the licenses of Lame and libsndfile
require it, Audio Transcoder is made available publicly under the GNU Public License (GPL).

## Building under Visual Studio 2017/Windows
Visual Studio is currently *still* supported.

Open the console.

Change the directory to the directory of this file.

Enter
```
cmake -G "Visual Studio 15 2017" CMakeLists.txt
```
to generate the project files for Win32.

Enter
```
cmake -G "Visual Studio 15 2017 Win64" CMakeLists.txt
```
to generate the project files for x64.

Open the files using Visual Studio 2017 and build all configurations.

To execute the unit tests for a particular configuration, enter
```
ctest -C <configuration>
```

`configuration` is one of `Debug`, `Release`, `MinSizeRel`, `RelWithDebInfo`.

You can find the build products under `products/<configuration>/<platform>/(bin|lib)`
where `<configuration>` is one of `debug`, `release`, `minsizerel`, `relwithdebinfo`
and
where `<platform>` is one of `x86` or `x64`.

### Compilation options (Visual Studio)

#### `With-Pthreads`
For Visual Studio builds, the option `With-Pthreads=(ON|OFF)` is supported.
`ON` determines that the threading module uses _pthreads-win32_ backend instead of the _Winapi_ backend.
The default value is `OFF`.

## Building under GCC/Linux and GCC/Cygwin
Open the console.

Change the directory to the directory of this file.

Enter
```
cmake CMakeLists.txt
```
to generate the project files.

Enter
```
make all
```
to build.

To execute the unit tests, enter
```
ctest
```

```configuration``` is one of `Debug`, `Release`, `MinSizeRel`, `RelWithDebInfo`.

You can find the build products under `products/<platform>/(bin|lib)`
where `<platform>` is one of `x86` or `x64`.

### Compilation options (GCC/Linux and GCC/Cygwin)
For Linux and Cygwin builds, certain CMake options are supported.
The currently supported options are `With-Debug-Information=(ON|OFF)`
and `With-Optimizations=(ON|OFF)`.

For example, to enable both optimizations and debug information enter
```
cmake -DWith-Optimizations=ON -DWith-Debug-Information=ON CMakeLists.txt
```

## Out of source builds
The above build instruction for
Visual Studio 2017/Windows,
GCC/Linux, and
GCC/Cygwin
use CMake to generate project files for in-source builds.
However, it is recommended to use CMake to generate project files for out-of-source builds.

To generate project files for an out-of-source build,
simple enter some directory (which should be empty).
This directory is called the build directory.
In that directory enter the CMake command with options and generators to your liking as described above.
However, instead of writing `CMakeLists.txt` in the end, enter the path to the CMakeLists.txt file of
Nucleus - relative to your build directory.

For example, if you are in the source directory and you want the project files for Visual Studio to be generated
in `./build/visualstudio`, then simply enter `mkdir build; cd build; mkdir visualstudio`. Then tell CMake to
generate the build files in there by invoking

```
cmake -G "Visual Studio 15 2017 Win64" . ./../../CMakeLists.txt
```

## Demos
Nucleus: Audio provides demonstrations of features in the folder `demo`.
- *WavToMp3BatchConverter*:

Read all WAV files in a directory, convert them to MP3, and write the MP3 files.
Usage: `Nucleus.Audio.ReadWavWriteWav <directory>`

- *ReadWavWriteWav*:

Read a WAV file and write the WAV file.
Usage: `Nucleus.Audio.ReadWavWriteWav <source> <target>`

- *ReadWavWriteMp3*:

Read a WAV file, convert it to MP3, and write the MP3 file.
Usage: `Nucleus.Audio.ReadWavWriteMp3 <source> <target>`

- *WritePcm*:

Generate a sine wave and write it as a PCM file.
Usage: `Nucleus.Audio.WritePcm <target>`

- *WriteWav*:

Generate a sine wave and write it as an WAV file.
Usage: `Nucleus.Audio.WriteWav <target>`

- *WriteMp3*:

Generate a sine wave and write it as an MP3 file.
Usage: `Nucleus.Audio.WriteMp3 <target>`

## Test files
As well included are several WAV files for testing. They can be found in the folder `demo/data`.
Some fairly large WAV files are compressed using 7z so you need to unpack them before using them for testing. 
