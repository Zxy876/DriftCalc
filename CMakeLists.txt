cmake_minimum_required(VERSION 3.16)
project(DriftCalc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ===== SFML 3 =====
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# ===== NanoVG (C library) =====
add_library(nanovg STATIC
    external/nanovg/src/nanovg.c
)

set_target_properties(nanovg PROPERTIES
    LINKER_LANGUAGE C
    LANGUAGE C
)

target_include_directories(nanovg PUBLIC
    ${CMAKE_SOURCE_DIR}/external/nanovg/src
)

# ===== External headers =====
include_directories(
    ${CMAKE_SOURCE_DIR}/external/exprtk
    ${CMAKE_SOURCE_DIR}/external/tweeny/include
)

# ========== 手动列出所有 DriftCalc 源文件（最稳定方式） ==========
set(SOURCES

    # ------- Main -------
    src/main.cpp

    # ------- core -------
    src/core/App.cpp

    # ------- expression -------
    src/expression/ExpressionEngine.cpp
    # 若你有 ASTNode.cpp，则加上：
    # src/expression/ASTNode.cpp

    # ------- steps -------
    src/steps/StepNavigator.cpp
    src/steps/StepTreeBuilder.cpp

    # ------- render -------
    src/render/DraftRenderer.cpp

    # ------- ui -------
    src/ui/CalculatorScreen.cpp
    src/ui/DriftScreen.cpp
)

# ===== Create executable =====
add_executable(DriftCalc ${SOURCES})

# ===== Link =====
target_link_libraries(DriftCalc
    SFML::Graphics
    SFML::Window
    SFML::System
    nanovg
)