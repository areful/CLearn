cmake_minimum_required(VERSION 3.26)
project(CLearning C)

set(CMAKE_C_STANDARD 11)

add_executable(CLearning main.c
        libcurl_sample/libcurl_sample.c)

# basic
add_executable(LinkedListSample basic/linked_list_sample.c)

# standard library
add_executable(CopyStringSample std_library/copy_string_sample.c)

# multi-thread
add_executable(ProducerConsumerModel multi_thread/producer_consumer_model.c)
add_executable(WinThreadSample multi_thread/win_thread_sample.c)

# esp8266
add_executable(Esp8266Sample esp8266/esp8266_sample.c)

# RegExp
set(REGEXP_SOURCE_DIR "reg_exp")
file(GLOB REGEXP_SOURCES "${REGEXP_SOURCE_DIR}/*.c" "${REGEXP_SOURCE_DIR}/src/*.h")
add_executable(RegExpSample ${REGEXP_SOURCES})

# Utf8
add_executable(Utf8Sample utf8/libutf8_sample.c utf8/libutf8.c utf8/libutf8.h)

# print emojis
add_executable(PrintEmojis utf8/print_emojis.c)

# cJSON sample
################### https://github.com/DaveGamble/cJSON
#
set(CJSON_SOURCE_DIR "cJSON")
file(GLOB CJSON_SOURCE "${CJSON_SOURCE_DIR}/*.c" "${CJSON_SOURCE_DIR}/src/*.h")
add_executable(CJSONSample ${CJSON_SOURCE})

# 1. 头文件
include_directories(${CMAKE_SOURCE_DIR}/libcurl_mingw64/include)

# 2. 链接库
add_library(libcurl SHARED IMPORTED)
set_target_properties(libcurl PROPERTIES
        IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/libcurl_mingw64/lib/libcurl.dll.a
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libcurl_mingw64/bin/libcurl-4.dll)

# 3. 可执行文件
add_executable(curl_sample libcurl_sample/libcurl_sample.c)
target_link_libraries(curl_sample PRIVATE libcurl)

# 4. 运行时 DLL 自动复制到可执行文件旁边
add_custom_command(TARGET curl_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/libcurl_mingw64/bin/libcurl-4.dll
        ${CMAKE_BINARY_DIR})
