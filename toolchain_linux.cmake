# string(REGEX REPLACE ".*/\(.*\)" "\\1" CURDIR ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS USING_TOOLCHAIN:${CMAKE_TOOLCHAIN_FILE})

# flags
# set(DBGFLG "-gdwarf-2 -Og")
# set(LIBFLG "-lc -lm -lnosys")
set(LINKFLG "-Wl,-Map=target.map,--cref -Wl,--gc-sections")
set(PROJECT_FLAGS "-Wall -Wshadow -fdata-sections -ffunction-sections")

# compiler
set(CMAKE_ASM_COMPILER gcc) # -x assembler-with-cpp
set(CMAKE_ASM_FLAGS_DEBUG "${PROJECT_FLAGS} ${DBGFLG}")
set(CMAKE_ASM_FLAGS_RELEASE "${PROJECT_FLAGS} -O2")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS_DEBUG "${PROJECT_FLAGS} ${DBGFLG}")
set(CMAKE_C_FLAGS_RELEASE "${PROJECT_FLAGS} -O2")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS_DEBUG "${PROJECT_FLAGS} ${DBGFLG}")
set(CMAKE_CXX_FLAGS_RELEASE "${PROJECT_FLAGS} -O2")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "${LIBFLG} ${LINKFLG}")

# other tools
#set(QT6_WINDEPLOY windeployqt.exe)
set(CMAKE_OBJCOPY objcopy)
set(CMAKE_OBJDUMP objdump)
set(CMAKE_SIZE size)
set(CMAKE_DEBUGER gdb)
set(CMAKE_CPPFILT c++filt)

# output type and destination
set(CMAKE_BUILD_TYPE Debug)
set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

