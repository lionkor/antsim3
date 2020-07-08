Building a simple modern 2D engine for simulation projects like simulating ants and possibly other agent based systems (that's where the name is from) and small games. 

## What's implemented so far

* Super simple 2D rendering (WIP)
* Entity-Component-System (ECS)
* Entity Hierarchy (almost a Scene Graph), parent-child relationships between Entities
* Tests for the major modules
* Mouse events at component-level

Demos included:
* cells, a multithreaded Conway's game of life
* orbits, a little orbital physics simulation
* mpgame, a simple multiplayer game
* sandbox, a bouyancy physics sandbox

## Planned

I'm currently shooting for `1.0`, which will be the MVP, not the finished project. 

Take a look at the [project(s)](https://github.com/lionkor/antsim3/projects)

## How to build & run

### Windows

The windows build is currently not maintained. If you're experienced with C++ development on windows, feel free to contact me, we could use some help!

### Debian / Mint / Ubuntu

Make sure the following libraries are installed with this command:

```bash
# apt install libsfml-dev libboost-dev cmake git build-essential
```

Clone the git repo:

```bash
$ git clone https://github.com/lionkor/antsim3
$ cd antsim3
```

After this, you are ready to build. For this, run the following commands in order:

```bash
$ mkdir build
$ cd build
$ cmake .. 
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

This project is under GPLv3. See the "LICENSE" file for more information.
