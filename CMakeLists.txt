cmake_minimum_required(VERSION 3.25)

project(KCAD VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_linux.cmake)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_macos.cmake)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_windows.cmake)
endif()
include(${CMAKE_TOOLCHAIN_FILE})

# assume built-in pthreads on MacOS
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

# Declare source files
file(GLOB PROJECT_SRC_LIST RELATIVE ${CMAKE_SOURCE_DIR}
    "src/*.cpp"
    "src/imgui/*.cpp"
    "src/imgui_impl_bgfx/*.cpp"
    "src/simple_ini/*.c"
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
list(APPEND PROJECT_SRC_LIST
    ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_sdl2.cpp
    ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_opengl3.cpp
)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND PROJECT_SRC_LIST ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_vulkan.cpp)
endif ()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND PROJECT_SRC_LIST ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_metal.mm)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND PROJECT_SRC_LIST ${CMAKE_SOURCE_DIR}/src/imgui/backends/imgui_impl_dx12.cpp)
endif()

# Add spdlog library
set(spdlog_DIR ${CMAKE_SOURCE_DIR}/3rdparty/spdlog/lib/cmake/spdlog)
find_package(spdlog REQUIRED)

# Add Vulkan library
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Vulkan REQUIRED)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${BGFX_bgfxDebug}
    ${BGFX_bimgDebug}
    ${BGFX_bxDebug}
    ${SDL2_LIBRARIES}
    spdlog::spdlog
)

