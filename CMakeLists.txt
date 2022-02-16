cmake_minimum_required(VERSION 3.10)

project(GeodeAppWin VERSION 0.1.0)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME GeodeApp)
