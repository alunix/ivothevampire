Ivo the Vampire
================

2D Shoot 'Em Up game coded w/h libSDL & C/C++

![alt text](http://i.imgur.com/ZUwPZPr.png "Title screen")


# Installation

Extract the `vampire_ivo-1.1.0-<Platform>.tar.gz` package and run the binary.

For Linux users. Make sure you've got the following installed:

  * `libSDL >= 1.2.11` and `SDL_Mixer >= 1.2.12`
  * (Optional for music support) `timidity++-2.14.0-5` and `timidity-freepats`

# Build

## Requirements

  * CMake 2.8.x
  * Both `libSDL >= 1.2.11` and `SDL_Mixer >= 1.2.12`.
  * Windows builds => Mingw32 (Minimalist GNU for Windows)

## Linux

On the command line:

Install the required packages using the package manager on your Linux distribution.

Create a build folder

    mkdir build && cd build

Run CMake

    cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release

Build with:

    make

Create an installation package with:

    cpack

## Windows

  * Install the required Windows dependencies in [lib](lib/README.md).

On the command line:

Create a build folder

    mkdir build && cd build

Set the `SDLDIR` and `SDLMIXER` environment variables.

Run CMake

    cmake -G "MinGW Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release

Build with:

    mingw32-make

Create an installation package with:

    cpack

## Disable sound

To build without sound & music run:

    cmake -G "<platform>" ../ -DCMAKE_BUILD_TYPE:STRING=Release -DSOUND=NO

# License

[MIT License](LICENSE)
