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
add_executable(RegExpSample
        reg_exp/regexp_sample.c
        reg_exp/regexp.c
        reg_exp/regexp.h)
