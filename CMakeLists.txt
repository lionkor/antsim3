cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -Wextra -Wpedantic -pedantic-errors -Wformat -Wformat-overflow -Wnull-dereference -Wimplicit-fallthrough -Wmisleading-indentation -Wparentheses -Wunused-parameter -Wuninitialized -Wstringop-overflow -Wsuggest-attribute=const -Warith-conversion -Wconversion -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow -Wcast-qual -Wparentheses")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fconcepts")

project(Engine)

add_subdirectory("SFML" "SFML")

include_directories(Engine
    "."
    "./fmt/include"
    "./doctest/doctest"
    "./SFML/include"
)

add_library(Engine SHARED
    Cellular/Cell.cpp
    Core/Application.cpp
    Core/GameWindow.cpp
    Core/World.cpp
    Core/HID.cpp
    ECS/Component.cpp
    ECS/Entity.cpp
    Events/Event.cpp
    Events/EventDispatcher.cpp
    Events/IEventReceiver.cpp
    Physics/Collider.cpp
    Physics/PhysicalObject.cpp
    Physics/PhysicsObject.cpp
    Physics/Ray.cpp
    Rendering/Draw.cpp
    Rendering/SimpleDrawable.cpp
    Rendering/ext_sf.cpp
    Rendering/GuiElement.cpp
    Unused/Tool.cpp
    Utils/CsvLogger.cpp
    Utils/LazyFile.cpp
    Utils/ResourceManager.cpp

    # fmt
    fmt/src/format.cc
    fmt/src/os.cc
)

target_link_libraries(Engine
    sfml-graphics 
    sfml-window 
    sfml-system
    pthread
)

project(antsim3)

add_executable(antsim3
    main.cpp
)

target_link_libraries(antsim3
    Engine
)

project(cells)

add_executable(cells cells.cpp)

target_link_libraries(cells Engine)


project(testing)

add_executable(testing testing.cpp)

target_link_libraries(testing Engine)


project(orbits)

add_executable(orbits orbits.cpp)

target_link_libraries(orbits Engine)


project(tests) 

add_executable(tests 
    Tests/test_main.cpp
    Tests/test_core.cpp
    Tests/test_ECS.cpp
    Tests/test_vec.cpp
)

target_link_libraries(tests Engine)

add_executable(sandbox
    sandbox.cpp
)

target_link_libraries(sandbox Engine)
