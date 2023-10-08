cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_linux.cmake)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

project(KCAD VERSION 0.0.1 LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)

# Try find Qt6
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
list(APPEND CMAKE_PREFIX_PATH "/opt/Qt/6.5.3/gcc_64")
find_package(Qt6 COMPONENTS Core Quick Qml Gui OpenGL REQUIRED)
qt_standard_project_setup()

# Add BGFX library
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bgfx/include)
set(BGFX_LIB_PATH ${CMAKE_SOURCE_DIR}/3rdparty/bgfx/bin)
find_library(BGFX_bgfxDebug bgfxDebug HINTS ${BGFX_LIB_PATH})
find_library(BGFX_bimgDebug bimgDebug HINTS ${BGFX_LIB_PATH})
find_library(BGFX_bxDebug bxDebug HINTS ${BGFX_LIB_PATH})
add_compile_definitions(BX_CONFIG_DEBUG True)

# Add Vulkan library
find_package(Vulkan REQUIRED)

# Declare source files
file(GLOB_RECURSE PROJECT_SRC_LIST RELATIVE ${CMAKE_SOURCE_DIR}
    "src/*.cpp"
    "src/qrc/*.qrc"
)

qt_add_qml_module(bgfx_bridge
    URI bgfx_bridge
    VERSION 1.0
    SOURCES src/bgfx_bridge.hpp src/bgfx_bridge.cpp
#    NO_PLUGIN
)

target_include_directories(bgfx_bridgeplugin
     PUBLIC ${CMAKE_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})

target_link_libraries(bgfx_bridge PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::OpenGL
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    ${BGFX_bgfxDebug}
    ${BGFX_bimgDebug}
    ${BGFX_bxDebug}
    bgfx_bridge
)
