cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
# pacman -S mingw-w64-x86_64-portaudio
#


#
### libportaudio
#
set(LIBPORTAUDIO_DIR "${CMAKE_SOURCE_DIR}/libs/libportaudio_mingw64")
add_library(portaudio SHARED IMPORTED)

set_target_properties(portaudio PROPERTIES
        IMPORTED_IMPLIB ${LIBPORTAUDIO_DIR}/lib/libportaudio.dll.a
        IMPORTED_LOCATION ${LIBPORTAUDIO_DIR}/bin/libportaudio-0.dll)

#
### libportaudio_generate_sample
#
add_executable(libportaudio_generate_sample libportaudio_generate_sample.c)
target_include_directories(libportaudio_generate_sample PRIVATE ${LIBPORTAUDIO_DIR}/include)
target_link_libraries(libportaudio_generate_sample PRIVATE portaudio)

#
### libportaudio_sample
#
add_executable(libportaudio_sample libportaudio_sample.c)
target_include_directories(libportaudio_sample PRIVATE ${LIBPORTAUDIO_DIR}/include)
target_link_libraries(libportaudio_sample PRIVATE portaudio)

add_custom_command(TARGET libportaudio_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIBPORTAUDIO_DIR}/bin/libportaudio.dll
        ${CMAKE_BINARY_DIR}/libs/libportaudio_mingw64)
