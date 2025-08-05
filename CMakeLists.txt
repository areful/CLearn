cmake_minimum_required(VERSION 3.26)
project(CLearning C)

set(CMAKE_C_STANDARD 11)

### sample
add_executable(CLearning main.c)

### basic
add_executable(LinkedListSample src/linked_list_sample.c)

### standard library
add_executable(CopyStringSample src/copy_string_sample.c)

### multi-thread
add_executable(ProducerConsumerModel src/multi_thread/producer_consumer_model.c)
add_executable(WinThreadSample src/multi_thread/win_thread_sample.c)

### esp8266
add_executable(Esp8266Sample src/esp8266_sample.c)

### callback sample
add_executable(CallbackSample src/callback_sample.c)

### RegExp
set(REGEXP_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs/reg_exp")
aux_source_directory(${REGEXP_SOURCE_DIR} REGEXP_SRC_LIST)
add_executable(RegExpSample src/regexp_sample.c)
target_sources(RegExpSample PRIVATE ${REGEXP_SRC_LIST})
target_include_directories(RegExpSample PRIVATE ${REGEXP_SOURCE_DIR})

### Utf8
set(UTF8_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs/utf8")
aux_source_directory(${UTF8_SOURCE_DIR} UTF8_SRC_LIST)
add_executable(Utf8Sample src/libutf8_sample.c)
target_sources(Utf8Sample PRIVATE ${UTF8_SRC_LIST})
target_include_directories(Utf8Sample PRIVATE ${UTF8_SOURCE_DIR})

### print emojis
add_executable(PrintEmojis src/print_emojis.c)


#
# cJSON sample
# https://github.com/DaveGamble/cJSON
#
add_subdirectory(libs/cJSON)


#
### libcurl sample
#
add_subdirectory(libs/libcurl_mingw64)


#
### libserialport sample
#
add_subdirectory(libs/libserialport_mingw64)


#
### openssl sample
#
add_subdirectory(libs/openssl_mingw64)


#
### mqtt sample
#
add_subdirectory(libs/mqttc_mingw64)


#
### mongoose_mingw64
#
add_subdirectory(libs/mongoose_mingw64)


#
### nettle_mingw64
#
add_subdirectory(libs/nettle_mingw64)
