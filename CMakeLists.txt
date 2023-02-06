cmake_minimum_required(VERSION 3.23)
project(VanillaGenerator)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "-O3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    if (NOT DEFINED LIBMORTON_SOURCE)
        set(LIBMORTON_SOURCE /mnt/e/ACD-MinecraftGenerator/ext-morton/libmorton)
    endif ()

    if (NOT DEFINED CHUNK_UTILS_SOURCE)
        set(CHUNK_UTILS_SOURCE /mnt/e/ACD-MinecraftGenerator/ext-chunkutils2)
    endif ()
else ()
    if (MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "/GL")
        set(CMAKE_CXX_FLAGS_RELEASE "/GL")
    else()
        set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Ofast -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
        set(CMAKE_CXX_FLAGS "-Ofast")
    endif ()

    if (NOT DEFINED LIBMORTON_SOURCE)
        set(LIBMORTON_SOURCE E:/ACD-MinecraftGenerator/ext-morton/libmorton)
    endif ()

    if (NOT DEFINED CHUNK_UTILS_SOURCE)
        set(CHUNK_UTILS_SOURCE E:/ACD-MinecraftGenerator/ext-chunkutils2)
    endif ()
endif ()

include_directories(BEFORE ./src
        ./includes
        ${CHUNK_UTILS_SOURCE}/lib
        ${CHUNK_UTILS_SOURCE}/src
        ${CHUNK_UTILS_SOURCE}/gsl/include
        ${LIBMORTON_SOURCE}/libmorton/include)

file(GLOB_RECURSE SRC_LIST FOLLOW_SYMLINKS ./src/*.cpp)

add_executable(VanillaGenerator ${SRC_LIST})