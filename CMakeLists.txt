cmake_minimum_required(VERSION 3.26)
project(CLearning C)

set(CMAKE_C_STANDARD 11)

add_executable(CLearning main.c)

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
