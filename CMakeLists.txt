cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Weverything -Wno-c++98-compat -Wno-c++98-c++11-compat-binary-literal -Wno-c++98-compat-pedantic --pedantic -Wimplicit-fallthrough -Wno-global-constructors -Wno-exit-time-destructors -Wno-shadow-field-in-constructor -Wno-padded")

project(antsim3)

add_executable(${CMAKE_PROJECT_NAME} main.cpp
    Tool.cpp
    ext_sf.cpp
    GameWindow.cpp
    IEventReceiver.cpp
    EventDispatcher.cpp
    Event.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system
)
