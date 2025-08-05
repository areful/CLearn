cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

### Utf8
set(UTF8_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs/utf8")
aux_source_directory(${UTF8_SOURCE_DIR} UTF8_SRC_LIST)
add_executable(Utf8Sample utf8_sample.c)
target_sources(Utf8Sample PRIVATE ${UTF8_SRC_LIST})
target_include_directories(Utf8Sample PRIVATE ${UTF8_SOURCE_DIR})
