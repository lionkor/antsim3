cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g -pg -pedantic-errors -Wformat -Wformat-overflow -Wnull-dereference -Wimplicit-fallthrough -Wmisleading-indentation -Wparentheses -Wunused-parameter -Wuninitialized -Wstringop-overflow -Wsuggest-attribute=const -Warith-conversion -Wconversion -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow -Wcast-qual -Wparentheses -Winline")

project(antsim3)

include_directories(antsim3 "String/include")

add_executable(${CMAKE_PROJECT_NAME} main.cpp
    Cell.cpp
    Draw.cpp
    Event.cpp
    EventDispatcher.cpp
    GameWindow.cpp
    IEventReceiver.cpp
    PhysicalObject.cpp
    Ray.cpp
    Tool.cpp
    World.cpp
    
    ext_sf.cpp

    # We compile the lionkor/String library into us directly
    String/src/String.cpp
)


target_precompile_headers(antsim3
  PRIVATE
      Cell.h
      DebugTools.h
      Draw.h
      Event.h
      EventDispatcher.h
      GameWindow.h
      IEventReceiver.h
      IHittable.h
      Object.h
      PhysicalObject.h
      Random.h
      Ray.h
      String/include/String.h
      Tool.h
      World.h
      ext_sf.h
      vec.h
)


target_link_libraries(${CMAKE_PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system
)
