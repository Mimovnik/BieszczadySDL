cmake_minimum_required(VERSION 3.0.0)
project(bieszczady VERSION 0.1.0)

set(SDL2_INCLUDE_DIR lib/SDL2/include)
set(SDL2_LIB_DIR lib/SDL2/x64)

add_library(
    SOURCE_FILES
    
    src/classes/Alive.cpp
    src/classes/Alive.h

    src/classes/Animation.h
    src/classes/Animation.cpp

    src/classes/Framework.h
    src/classes/Framework.cpp

    src/classes/Surfaces.h
    src/classes/Surfaces.cpp

    src/classes/GameObject.h

    src/classes/Renderer.h
    src/classes/Renderer.cpp

    src/classes/Rectangle.h
    src/classes/Rectangle.cpp

    src/classes/RigidBody.h
    src/classes/RigidBody.cpp

    src/classes/Weapon.h
    
    src/classes/Tool.h

    src/classes/Renderer.h
    src/classes/Renderer.cpp

    src/classes/RigidBody.h
    src/classes/RigidBody.cpp

    src/classes/Vector.h
    src/classes/Vector.cpp

    src/classes/Rectangle.h
    src/classes/Rectangle.cpp

    src/classes/Timer.h

    src/classes/Terrain.h
    src/classes/Terrain.cpp

    src/classes/QuadTree.h
    src/classes/QuadTree.cpp

    src/classes/PerlinNoise.h
    src/classes/PerlinNoise.cpp

    src/functions/controller.h
    src/functions/controller.cpp

    src/functions/draw.h
    src/functions/draw.cpp
    
    src/functions/settings.h

    src/functions/loadBMP.h
    src/functions/loadBMP.cpp
)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(bieszczady src/main.cpp)

target_link_libraries(${PROJECT_NAME} SOURCE_FILES SDL2 SDL2main)
