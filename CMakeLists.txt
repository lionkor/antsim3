

cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER "/usr/lib/ccache/bin/g++")
set(CMAKE_C_COMPILER "/usr/lib/ccache/bin/gcc")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g -pedantic-errors -Wformat -Wformat-overflow -Wnull-dereference -Wimplicit-fallthrough -Wmisleading-indentation -Wparentheses -Wunused-parameter -Wuninitialized -Wstringop-overflow -Wsuggest-attribute=const -Warith-conversion -Wconversion -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow -Wcast-qual -Wparentheses -Winline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fconcepts")

project(Engine)

include_directories(Engine
    "."
    "./fmt/include"
)

add_library(Engine SHARED
    Cellular/Cell.cpp
    Cellular/Cell.h
    Core.h
    Core/Application.cpp
    Core/Application.h
    Core/GameWindow.cpp
    Core/GameWindow.h
    Core/Object.h
    Core/World.cpp
    Core/World.h
    ECS/Component.cpp
    ECS/Component.h
    ECS/Entity.cpp
    ECS/Entity.h
    Events/Event.cpp
    Events/Event.h
    Events/EventDispatcher.cpp
    Events/EventDispatcher.h
    Events/IEventReceiver.cpp
    Events/IEventReceiver.h
    Physics/Collider.cpp
    Physics/Collider.h
    Physics/IHittable.h
    Physics/PhysicalObject.cpp
    Physics/PhysicalObject.h
    Physics/PhysicsObject.cpp
    Physics/PhysicsObject.h
    Physics/Ray.cpp
    Physics/Ray.h
    Physics/vec.h
    Rendering/Draw.cpp
    Rendering/Draw.h
    Rendering/ext_sf.cpp
    Rendering/ext_sf.h
    Unused/Tool.cpp
    Unused/Tool.h
    Utils.h
    Utils/DebugTools.h
    Utils/Random.h
    Utils/Result.h
    Utils/Managed.h

    # fmt
    fmt/src/format.cc
    fmt/src/os.cc
)

target_link_libraries(Engine
    sfml-graphics 
    sfml-window 
    sfml-system
)

project(antsim3)

add_executable(antsim3
    main.cpp
)

target_link_libraries(antsim3
    Engine
)
