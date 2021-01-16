Building a simple modern 2D engine for simulation projects like simulating ants and possibly other agent based systems (that's where the name is from) and small games. 

## What's implemented so far

* Super simple 2D rendering (WIP)
* Entity-Component-System (ECS)
* Lua scriptable component
* C scriptable component (runtime loading of mods as DLL)
* Entity Hierarchy (almost a Scene Graph), parent-child relationships between Entities
* Simple UDP server and client

Demos included:
* cells, a multithreaded Conway's game of life
* orbits, a little orbital physics simulation
* mpgame, a simple multiplayer game
* sandbox, a bouyancy physics sandbox

## Planned

I'm currently shooting for `1.0`, which will be the MVP, not the finished project. 

Take a look at the [project(s)](https://github.com/lionkor/antsim3/projects)

## How to build & run

### General guide

This is a quick guide for experienced devs. Inexperienced devs should check out the other sections below.

1. Ensure you have boost installed and up-to-date (dev headers, too) and all dependencies of SFML
2. Clone this repo recursively
3. Make a new directory for the build files, e.g. antsim3/build
4. Run cmake
5. Run make
6. Run from antsim3 directory (not the build directory!)
7. profit

**Note:** If you want to help develop this engine, make sure to use GCC 10 and ensure that `CPP_20` is defined.

### Windows

The windows build is currently not maintained. If you're experienced with C++ development on windows, feel free to contact me, we could use some help!

### Debian / Mint / Ubuntu

Make sure the following libraries are installed with this command (all of them are required):

```bash
# apt install libboost-dev cmake git build-essential libgl-dev libflac8 libogg0 libopenal1 libvorbis0a libvorbisenc2 libvorbisfile3 libfreetype6 libgl1 libx11-6 libxrandr2 libudev1 libudev-dev libopenal-dev libvorbis-dev libflac-dev libxrandr-dev libfreetype6-dev liblua5.3
```

You'll also need lua and lua development headers to compile ECS/ScriptableComponent.

Clone the git repo:

```bash
$ git clone https://github.com/lionkor/antsim3 --recursive
$ cd antsim3
```

After this, you are ready to build. For this, run the following commands in order:

```bash
$ mkdir build
$ cd build
$ cmake .. -DDEBIAN_PATHS=1
$ cd ..
$ make -C build -j $(shell grep -c ^processor /proc/cpuinfo) all
```

Now you can run the demos you'd like to run with `./build/XXX` where XXX is the name of the demo. Example: `$ ./build/orbits`.
Make sure to run all demos from the antsim3 directory and *not* from the build directory.


## Requirements

* `fmt`, included
* `doctest`, included
* `bitops`, included

All it will be is a simple lightweight hobby project engine.

This project is under MIT. See the "LICENSE" file for more information.
