cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
### libserialport sample
#
set(LIBSERIAL_PORT_DIR "${CMAKE_SOURCE_DIR}/libs/libserialport_mingw64")
add_library(serialport SHARED IMPORTED)

set_target_properties(serialport PROPERTIES
        IMPORTED_IMPLIB ${LIBSERIAL_PORT_DIR}/lib/libserialport.dll.a
        IMPORTED_LOCATION ${LIBSERIAL_PORT_DIR}/bin/libserialport-0.dll)

add_executable(libserialport_sample libserialport_sample.c)
target_include_directories(libserialport_sample PRIVATE ${LIBSERIAL_PORT_DIR}/include)
target_link_libraries(libserialport_sample PRIVATE serialport)

add_custom_command(TARGET libserialport_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIBSERIAL_PORT_DIR}/bin/libserialport-0.dll
        ${CMAKE_BINARY_DIR}/libs/libserialport_mingw64)
