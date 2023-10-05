cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain_linux.cmake)

project(KCAD VERSION 0.0.1 LANGUAGES CXX)

# Try find Qt6
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
list(APPEND CMAKE_PREFIX_PATH "/opt1/Qt/6.5.3/gcc_64")
find_package(Qt6 COMPONENTS Core Quick Qml REQUIRED)
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
    "src/bgfx/*.cpp"
    "src/qrc/*.qrc"
)
#aux_source_directory(src PROJECT_SRC_LIST)
add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    ${BGFX_bgfxDebug}
    ${BGFX_bimgDebug}
    ${BGFX_bxDebug}
)
