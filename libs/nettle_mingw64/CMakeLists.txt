cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
### nettle_mingw64
#
# pacman -S mingw-w64-x86_64-nettle
#
set(NETTLE_DIR "${CMAKE_SOURCE_DIR}/libs/nettle_mingw64")

add_library(nettle STATIC IMPORTED)
set_target_properties(nettle PROPERTIES IMPORTED_LOCATION ${NETTLE_DIR}/lib/libnettle.dll.a)

add_executable(nettle_sample nettle_sample.c)
target_include_directories(nettle_sample PRIVATE ${NETTLE_DIR}/include)
target_link_libraries(nettle_sample PRIVATE nettle)

add_executable(nettle_aes_sample nettle_aes_sample.c)
target_include_directories(nettle_aes_sample PRIVATE ${NETTLE_DIR}/include)
target_link_libraries(nettle_aes_sample PRIVATE nettle)

#file(COPY "${NETTLE_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/libs/nettle_mingw64/)
add_custom_command(TARGET nettle_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${NETTLE_DIR}/bin/libnettle-8.dll
        ${CMAKE_BINARY_DIR}/libs/nettle_mingw64)

add_custom_command(TARGET nettle_aes_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${NETTLE_DIR}/bin/libnettle-8.dll
        ${CMAKE_BINARY_DIR}/libs/nettle_mingw64)
