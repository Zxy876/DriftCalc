cmake_minimum_required(VERSION 3.16)
project(DriftCalc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ===== SFML 3 =====
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# ===== NanoVG (pure C library) =====
add_library(nanovg STATIC
    external/nanovg/src/nanovg.c
)

# 明确告诉 CMake：这个库使用 C
set_target_properties(nanovg PROPERTIES
    LINKER_LANGUAGE C
    LANGUAGE C
)

target_include_directories(nanovg PUBLIC
    ${CMAKE_SOURCE_DIR}/external/nanovg/src
)

# ===== External includes =====
include_directories(
    ${CMAKE_SOURCE_DIR}/external/exprtk
    ${CMAKE_SOURCE_DIR}/external/tweeny/include
)

# ===== DriftCalc sources =====
file(GLOB SOURCES
    src/main.cpp
    src/core/*.cpp
    src/expression/*.cpp
    src/steps/*.cpp
    src/render/*.cpp
    src/ui/*.cpp
)

add_executable(DriftCalc ${SOURCES})

target_link_libraries(DriftCalc
    SFML::Graphics
    SFML::Window
    SFML::System
    nanovg
)