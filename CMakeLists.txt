
cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER "/usr/lib/ccache/bin/g++")
set(CMAKE_C_COMPILER "/usr/lib/ccache/bin/gcc")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g -pedantic-errors -Wformat -Wformat-overflow -Wnull-dereference -Wimplicit-fallthrough -Wmisleading-indentation -Wparentheses -Wunused-parameter -Wuninitialized -Wstringop-overflow -Wsuggest-attribute=const -Warith-conversion -Wconversion -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow -Wcast-qual -Wparentheses -Winline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fconcepts")

project(antsim3)

include_directories(antsim3 
    "."
    "./fmt/include"
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp
    Cell.h
    Cell.cpp
    Collider.h
    Collider.cpp
    Draw.h
    Draw.cpp
    Event.h
    Event.cpp
    EventDispatcher.h
    EventDispatcher.cpp
    GameWindow.h
    GameWindow.cpp
    IEventReceiver.h
    IEventReceiver.cpp
    PhysicalObject.h
    PhysicalObject.cpp
    PhysicsObject.h
    PhysicsObject.cpp
    Ray.h
    Ray.cpp
    Result.h
    World.h
    World.cpp
    
    ext_sf.h
    ext_sf.cpp

    # fmt
    fmt/src/format.cc
    fmt/src/os.cc
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system
)
