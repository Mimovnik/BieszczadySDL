cmake_minimum_required(VERSION 3.0.0)
project(bieszczady VERSION 0.1.0)

set(SDL2_INCLUDE_DIR C:/Users/Mimo/libraries/SDL2_dev_VC/include)
set(SDL2_LIB_DIR C:/Users/Mimo/libraries/SDL2_dev_VC/lib/x64)

add_library(
    SOURCE_FILES
    src/Collision.h
    src/Collision.cpp
    src/Rectangle.h
    src/Rectangle.cpp
    src/RigidBody.h
    src/RigidBody.cpp
    src/Vector.h
    src/Vector.cpp
)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(bieszczady main.cpp)

target_link_libraries(${PROJECT_NAME} SOURCE_FILES SDL2 SDL2main)
