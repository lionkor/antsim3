cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weverything -Wno-c++98-compat -Wno-c++98-c++11-compat-binary-literal -Wno-c++98-compat-pedantic --pedantic -Wimplicit-fallthrough -Wno-global-constructors -Wno-exit-time-destructors -Wno-shadow-field-in-constructor -Wno-padded -Wno-extra-semi")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(antsim3)

include_directories(antsim3 "String/include")

add_executable(${CMAKE_PROJECT_NAME} main.cpp
    Cell.cpp
    Cell.h
    Draw.cpp
    Draw.h
    Event.cpp
    Event.h
    EventDispatcher.cpp
    EventDispatcher.h
    GameWindow.cpp
    GameWindow.h
    IEventReceiver.cpp
    IEventReceiver.h
    PhysicalObject.cpp
    PhysicalObject.h
    Ray.cpp
    Ray.h
    Tool.cpp
    Tool.h
    World.cpp
    World.h
    ext_sf.cpp
    ext_sf.h

    # We compile the lionkor/String library into us directly
    String/include/String.h
    String/src/String.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system
)
