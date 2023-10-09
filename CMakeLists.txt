cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_linux.cmake)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

project(KCAD VERSION 0.0.1 LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)

# Declare source files
file(GLOB PROJECT_SRC_LIST RELATIVE ${CMAKE_SOURCE_DIR}
    "src/*.cpp"
    "src/imgui/*.cpp"
    "src/imgui_impl_bgfx/*.cpp"
)

# Add BGFX library
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bgfx/include)
set(BGFX_LIB_PATH ${CMAKE_SOURCE_DIR}/3rdparty/bgfx/bin)
find_library(BGFX_bgfxDebug bgfxDebug HINTS ${BGFX_LIB_PATH})
find_library(BGFX_bimgDebug bimgDebug HINTS ${BGFX_LIB_PATH})
find_library(BGFX_bxDebug bxDebug HINTS ${BGFX_LIB_PATH})
add_compile_definitions(BX_CONFIG_DEBUG True)

# Add ImGui and SDL2 library
set(SDL2_DIR ${CMAKE_SOURCE_DIR}/3rdparty/sdl2/lib/cmake/SDL2)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(OpenGL REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/src/imgui)
if (LINUX)
    list(APPEND PROJECT_SRC_LIST
        ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_vulkan.cpp
        ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_sdl2.cpp
    )
endif (LINUX)

# Add Vulkan library
find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${BGFX_bgfxDebug}
    ${BGFX_bimgDebug}
    ${BGFX_bxDebug}
    ${SDL2_LIBRARIES}
    OpenGL::GL
)
