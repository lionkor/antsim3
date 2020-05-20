cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Weverything -Wno-c++98-compat -Wno-c++98-c++11-compat-binary-literal -Wno-c++98-compat-pedantic --pedantic -Wimplicit-fallthrough -Wno-global-constructors -Wno-exit-time-destructors -Wno-shadow-field-in-constructor -Wno-padded -Wno-extra-semi")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

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
