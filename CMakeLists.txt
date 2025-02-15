cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(GeodeAppWin VERSION 0.1.0)

file(GLOB_RECURSE SOURCES src/*.cpp GeodeApp.exe.manifest resource.res)
file(GLOB_RECURSE HEADERS src/*.hpp)

set(CMAKE_DEBUG_POSTFIX d)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

target_precompile_headers(${PROJECT_NAME} PUBLIC ${HEADERS})

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME GeodeApp64)
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME GeodeApp32)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${CMAKE_SOURCE_DIR}/resource.res)

configure_file(
    ${CMAKE_SOURCE_DIR}/src/config.hpp.in
    ${CMAKE_SOURCE_DIR}/src/config.hpp
)

configure_file(
    ${CMAKE_SOURCE_DIR}/GeodeApp.exe.manifest.in
    ${CMAKE_SOURCE_DIR}/GeodeApp.exe.manifest
)

target_include_directories(${PROJECT_NAME} PUBLIC
    src
    src/base
    src/widgets
    src/windows
    src/utils
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    dwmapi shcore gdiplus uxtheme
)
