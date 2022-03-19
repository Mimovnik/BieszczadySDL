cmake_minimum_required(VERSION 3.0.0)
project(bieszczady VERSION 0.1.0)

set(SDL2_INCLUDE_DIR lib/SDL2/include)
set(SDL2_LIB_DIR lib/SDL2/x64)

add_library(
    SOURCE_FILES
    src/settings.h
    
    src/displaySetup.cpp
    src/loadBMP.cpp
    src/control.cpp
    src/draw.cpp

    src/Framework.h
    src/Framework.cpp

    src/Rectangle.h
    src/Rectangle.cpp

    src/RigidBody.h
    src/RigidBody.cpp

    src/Alive.h
    src/Alive.cpp

    src/Vector.h
    src/Vector.cpp

    src/Timer.h

    src/Animation.h
    src/Animation.cpp

    src/Terrain.h
    src/Terrain.cpp

    src/QuadTree.h
    src/QuadTree.cpp

    src/PerlinNoise.h
    src/PerlinNoise.cpp
)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(bieszczady main.cpp)

target_link_libraries(${PROJECT_NAME} SOURCE_FILES SDL2 SDL2main)
