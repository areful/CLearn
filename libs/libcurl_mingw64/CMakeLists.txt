cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
### libcurl sample
#
set(LIBCURL_DIR "${CMAKE_SOURCE_DIR}/libs/libcurl_mingw64")

add_library(libcurl SHARED IMPORTED)
set_target_properties(libcurl PROPERTIES
        IMPORTED_IMPLIB ${LIBCURL_DIR}/lib/libcurl.dll.a
        IMPORTED_LOCATION ${LIBCURL_DIR}/bin/libcurl-4.dll)

add_executable(curl_sample libcurl_sample.c)
target_include_directories(curl_sample PRIVATE ${LIBCURL_DIR}/include)
target_link_libraries(curl_sample PRIVATE libcurl)

add_custom_command(TARGET curl_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIBCURL_DIR}/bin/libcurl-4.dll
        ${CMAKE_BINARY_DIR}/libs/libcurl_mingw64)

#file(COPY "${CMAKE_SOURCE_DIR}/certs/cacert.pem" DESTINATION "${CMAKE_BINARY_DIR}/certs")
add_custom_command(TARGET curl_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/certs/cacert.pem
        ${CMAKE_BINARY_DIR}/libs/libcurl_mingw64/certs/cacert.pem)
