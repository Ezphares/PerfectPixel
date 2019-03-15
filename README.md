# PerfectPixel
2D opengl game engine. Total rewrite of my old stuff.

## Dependencies
* [boost](http://boost.org/)
* [zlib](https://zlib.net/)
* [PNG](http://www.libpng.org/pub/png/)
* [GLEW](http://glew.sourceforge.net/)

Make sure CMake is set up so these are discoverable with find_package. Also, place the .dlls for the dependencies either in your path or the "dist" folder (this will automatically copy them to the executable directorty post-build)

## Building

After cloning:
* `mkdir build && cd build`
* `cmake ..`
* Build with VS
* Debug the Pong project, or run the Pong executable in the output directory

Currently only Visual Studio 2017 on Windows will work. Only debug builds are tested.
