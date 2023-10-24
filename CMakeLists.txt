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
    "src/ui/*.cpp"
    "src/temp_codes/*.cpp"
)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    file(GLOB DARWIN_MM_SRC_LIST RELATIVE ${CMAKE_SOURCE_DIR} 
        "src/temp_codes/*.mm"
    )
    list(APPEND PROJECT_SRC_LIST ${DARWIN_MM_SRC_LIST})
endif()

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

# Compile shaders
set(SHADER_SOURCES
    "${CMAKE_SOURCE_DIR}/src/shaders/f_main_wnd_canvas.sc#fragment"
    "${CMAKE_SOURCE_DIR}/src/shaders/v_main_wnd_canvas.sc#vertex"
)
file(MAKE_DIRECTORY ${CMAKE_SHADER_OUTPUT_DIRECTORY})
foreach(SHADER_INFO ${SHADER_SOURCES})
    string(REGEX REPLACE "^([^#]+)#(.+)$" "\\1" SHADER_SOURCE ${SHADER_INFO})
    string(REGEX REPLACE "^([^#]+)#(.+)$" "\\2" SHADER_TYPE ${SHADER_INFO})
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME_WE)
    set(SHADER_OUTPUT ${CMAKE_SHADER_OUTPUT_DIRECTORY}/${SHADER_NAME}.bin)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(SHADERC_COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/bgfx/bin/shadercDebug -f ${SHADER_SOURCE} -o ${SHADER_OUTPUT} --platform linux --verbose -i ./ -p spirv --type ${SHADER_TYPE})
    endif ()
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(SHADERC_COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/bgfx/bin/shadercDebug -f ${SHADER_SOURCE} -o ${SHADER_OUTPUT} --platform osx --verbose -i ./ -p metal --type ${SHADER_TYPE})
    endif()
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(SHADERC_COMMAND ${CMAKE_SOURCE_DIR}/3rdparty/bgfx/bin/shadercDebug.exe -f ${SHADER_SOURCE} -o ${SHADER_OUTPUT} --platform windows --verbose -i ./ -p s_5_0 --type ${SHADER_TYPE})
    endif()
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND ${SHADERC_COMMAND}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compiling ${SHADER_TYPE} shader ${SHADER_SOURCE}"
    )
    list(APPEND SHADER_OUTPUTS ${SHADER_OUTPUT})
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SHADER_OUTPUTS})

add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})

add_dependencies(${PROJECT_NAME} Shaders)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${BGFX_bgfxDebug}
    ${BGFX_bimgDebug}
    ${BGFX_bxDebug}
    ${SDL2_LIBRARIES}
    spdlog::spdlog
)

