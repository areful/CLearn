cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
# cJSON sample
# https://github.com/DaveGamble/cJSON
#
set(CJSON_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs/cJSON")
aux_source_directory(${CJSON_SOURCE_DIR} CJSON_SRC_LIST)
add_executable(cJSONSample cJSON_sample.c)
target_sources(cJSONSample PRIVATE ${CJSON_SRC_LIST})
target_include_directories(cJSONSample PRIVATE ${CJSON_SOURCE_DIR})
