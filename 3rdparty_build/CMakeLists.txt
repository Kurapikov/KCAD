#cmake .. -DUSE_HTTPS_PROXY=ON -DHTTPS_PROXY_ADDRESS=localhost:1080

cmake_minimum_required(VERSION 3.25)

project(KCAD_DEPS)

option(USE_HTTPS_PROXY "Use proxy for git operations" OFF)
set(HTTPS_PROXY_ADDRESS "localhost:1080" CACHE STRING "Proxy address and port for git operations")

if(USE_HTTPS_PROXY)
    set(GIT_PROXY "https_proxy=${HTTPS_PROXY_ADDRESS}")
else()
    set(GIT_PROXY "")
endif()

add_custom_target(build_3rdparty ALL)

# BGFX
add_custom_command(TARGET build_3rdparty COMMAND ${GIT_PROXY} git clone https://github.com/bkaradzic/bx.git --depth 10 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bgfx)
add_custom_command(TARGET build_3rdparty COMMAND ${GIT_PROXY} git clone https://github.com/bkaradzic/bimg.git --depth 10 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bgfx)
add_custom_command(TARGET build_3rdparty COMMAND ${GIT_PROXY} git clone https://github.com/bkaradzic/bgfx.git --depth 10 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bgfx)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_custom_command(TARGET build_3rdparty COMMAND make -j8 linux-debug64 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_custom_command(TARGET build_3rdparty COMMAND make -j8 osx-debug WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET build_3rdparty COMMAND make -j8 vs2019-debug64 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx)
endif()
add_custom_command(TARGET build_3rdparty COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx)
add_custom_command(TARGET build_3rdparty COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx/include ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/)
add_custom_command(TARGET build_3rdparty COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bx/include/bx ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/include/)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_custom_command(TARGET build_3rdparty COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx/.build/linux64_gcc/bin ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/)
    add_custom_command(TARGET build_3rdparty COMMAND rm -R ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/bin/example*)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_custom_command(TARGET build_3rdparty COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx/.build/osx-x64/bin ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/)
    add_custom_command(TARGET build_3rdparty COMMAND rm -R ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/bin/example*)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET build_3rdparty COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx/.build/win64_mingw-gcc/bin ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/)
endif()
add_custom_command(TARGET build_3rdparty COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/bgfx/src/bgfx_shader.sh ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx/include/bgfx/)

# SDL2
add_custom_command(TARGET build_3rdparty COMMAND git clone https://github.com/libsdl-org/SDL.git -b release-2.28.4 --depth 1 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(TARGET build_3rdparty COMMAND mkdir -p SDL/build WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(TARGET build_3rdparty COMMAND ../configure --enable-shared=no prefix=${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/sdl2 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SDL/build)
add_custom_command(TARGET build_3rdparty COMMAND make -j8 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SDL/build)
add_custom_command(TARGET build_3rdparty COMMAND make install WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SDL/build)
add_custom_command(TARGET build_3rdparty COMMAND make clean WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SDL/build)

# spdlog
add_custom_command(TARGET build_3rdparty COMMAND git clone https://github.com/gabime/spdlog.git -b v1.12.0 --depth 1 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(TARGET build_3rdparty COMMAND mkdir -p spdlog/build WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(TARGET build_3rdparty COMMAND cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/spdlog -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_BUILD_BENCH=OFF -DSPDLOG_BUILD_TESTS=OFF .. WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/build)
add_custom_command(TARGET build_3rdparty COMMAND make -j4 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/build)
add_custom_command(TARGET build_3rdparty COMMAND make install WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/build)
add_custom_command(TARGET build_3rdparty COMMAND make clean WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/build)

