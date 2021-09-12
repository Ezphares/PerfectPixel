![PerfectPixel](dist/splash.png)

[![Build Status](https://travis-ci.org/Ezphares/PerfectPixel.svg?branch=master)](https://travis-ci.org/Ezphares/PerfectPixel)

2D opengl game engine, based on an entity-component system.

## Dependencies
This project needs an installation of OpenGL. On (any modern) Windows this should always me present, on Linux an installation from [mesa](https://mesa3d.org/) or a proprietary driver is needed

## Building
Requires a C++20 capable compiler, and cmake >= 3.8

After cloning:
* `mkdir build && cd build`
* `cmake ..`
* Build with VS or make
* Debug the Pong project, or run the Pong executable in the output directory

Currently only Windows will run the sample project, but the unit tests should run on all systems.

## Todo

Short term:
* GUI code
* Some editor utilites (level editor? template editor?)
* Support for at least one more platform
