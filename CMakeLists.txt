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

### threads_c11_sample
add_executable(ThreadsC11Sample src/multi_thread/threads_c11_sample.c src/multi_thread/threads_c11.h)

### esp8266
add_executable(Esp8266Sample src/esp8266_sample.c)

### callback sample
add_executable(CallbackSample src/callback_sample.c)

### print emojis
add_executable(PrintEmojis src/print_emojis.c)


#
### RegExp
#
add_subdirectory(libs/reg_exp)


#
### Utf8
#
add_subdirectory(libs/utf8)


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
