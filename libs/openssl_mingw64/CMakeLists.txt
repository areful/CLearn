cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
### openssl sample
#
set(OPENSSL_DIR "${CMAKE_SOURCE_DIR}/libs/openssl_mingw64")

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${OPENSSL_DIR}/lib/libssl.dll.a)
add_library(crypto STATIC IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_DIR}/lib/libcrypto.dll.a)

add_executable(openssl_sample openssl_sample.c)
target_include_directories(openssl_sample PRIVATE ${OPENSSL_DIR}/include)
target_link_libraries(openssl_sample PRIVATE ssl crypto)

#file(COPY "${OPENSSL_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/libs/openssl_mingw64/)
add_custom_command(TARGET openssl_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${OPENSSL_DIR}/bin/libcrypto-3-x64.dll
        ${CMAKE_BINARY_DIR}/libs/openssl_mingw64)

add_custom_command(TARGET openssl_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${OPENSSL_DIR}/bin/libssl-3-x64.dll
        ${CMAKE_BINARY_DIR}/libs/openssl_mingw64)
