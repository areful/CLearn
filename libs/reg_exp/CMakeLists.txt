cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

### RegExp
set(REGEXP_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs/reg_exp")
aux_source_directory(${REGEXP_SOURCE_DIR} REGEXP_SRC_LIST)
add_executable(RegExpSample regexp_sample.c)
target_sources(RegExpSample PRIVATE ${REGEXP_SRC_LIST})
target_include_directories(RegExpSample PRIVATE ${REGEXP_SOURCE_DIR})
