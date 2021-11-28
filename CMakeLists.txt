cmake_minimum_required(VERSION 3.20)
project(CircleBar)

set(CMAKE_CXX_STANDARD 20)

if (DEFINED LOCAL_SOURCE_PACKAGE_DIR OR DEFINED ENV{LOCAL_SOURCE_PACKAGE_DIR})
    if (NOT LOCAL_SOURCE_PACKAGE_DIR)
        set(LOCAL_SOURCE_PACKAGE_DIR $ENV{LOCAL_SOURCE_PACKAGE_DIR})
    endif ()
endif ()

include(FetchContent)

if (DEFINED LOCAL_SOURCE_PACKAGE_DIR)
    FetchContent_Declare(xyz
        URL "file://${LOCAL_SOURCE_PACKAGE_DIR}/Xyz-0.1.3.1.tar.gz")
    FetchContent_Declare(tungsten
        URL "file://${LOCAL_SOURCE_PACKAGE_DIR}/Tungsten-0.1.3.4.tar.gz")
else ()
    FetchContent_Declare(xyz
        GIT_REPOSITORY "https://github.com/jebreimo/Xyz.git"
        GIT_TAG v0.1.3)
    FetchContent_Declare(tungsten
        GIT_REPOSITORY "https://github.com/jebreimo/Tungsten.git"
        GIT_TAG v0.1.3)
endif ()

FetchContent_MakeAvailable(tungsten xyz)

list(APPEND CMAKE_MODULE_PATH ${tungsten_SOURCE_DIR}/tools/cmake)

include(TungstenTargetEmbedShaders)

add_executable(CircleBar
    src/CircleBar/CircleBar.cpp
    src/CircleBar/CircleBarShaderProgram.cpp
    src/CircleBar/CircleBarShaderProgram.hpp
    src/CircleBar/VertexArray.hpp)

target_link_libraries(CircleBar
    PRIVATE
        Tungsten::Tungsten
    )

tungsten_target_embed_shaders(CircleBar
    FILES
        src/CircleBar/CircleBar-frag.glsl
        src/CircleBar/CircleBar-vert.glsl
    )
