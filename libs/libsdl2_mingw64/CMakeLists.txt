cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)

MESSAGE(STATUS "os: ${CMAKE_HOST_SYSTEM_NAME}")
add_compile_options(-Wall -O3)

# Specify SDL2 paths explicitly
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/libs/libsdl2_mingw64")
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include/SDL2")
set(SDL2_LIBRARY_DIR "${SDL2_DIR}/lib") # Use x86 for 32-bit builds

# Include directories
include_directories(${SDL2_INCLUDE_DIR})

# Find SDL2 libraries
find_library(SDL2_LIBRARY NAMES SDL2 PATHS ${SDL2_LIBRARY_DIR})
find_library(SDL2MAIN_LIBRARY NAMES SDL2main PATHS ${SDL2_LIBRARY_DIR})

# SDL2 sample
add_executable(sdl2_sample sdl2_sample.c)
target_link_libraries(sdl2_sample ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY})

# Copy SDL2.dll to output directory
file(COPY "${SDL2_DIR}/bin/SDL2.dll" DESTINATION "${CMAKE_BINARY_DIR}/libs/libsdl2_mingw64")
