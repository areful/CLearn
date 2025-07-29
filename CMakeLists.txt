cmake_minimum_required(VERSION 3.26)
project(CLearning C)

set(CMAKE_C_STANDARD 11)

### sample
add_executable(CLearning main.c)

### basic
add_executable(LinkedListSample basic/linked_list_sample.c)

### standard library
add_executable(CopyStringSample std_library/copy_string_sample.c)

### multi-thread
add_executable(ProducerConsumerModel multi_thread/producer_consumer_model.c)
add_executable(WinThreadSample multi_thread/win_thread_sample.c)

### esp8266
add_executable(Esp8266Sample esp8266/esp8266_sample.c)

### RegExp
set(REGEXP_SOURCE_DIR "reg_exp")
file(GLOB REGEXP_SOURCES "${REGEXP_SOURCE_DIR}/*.c" "${REGEXP_SOURCE_DIR}/src/*.h")
add_executable(RegExpSample ${REGEXP_SOURCES})

### Utf8
add_executable(Utf8Sample utf8/libutf8_sample.c utf8/libutf8.c utf8/libutf8.h)

### print emojis
add_executable(PrintEmojis src/print_emojis.c)

# cJSON sample
################### https://github.com/DaveGamble/cJSON
#
set(CJSON_SOURCE_DIR "cJSON")
file(GLOB CJSON_SOURCE "${CJSON_SOURCE_DIR}/*.c" "${CJSON_SOURCE_DIR}/src/*.h")
add_executable(CJSONSample ${CJSON_SOURCE})

### libcurl sample
# 1. 头文件
include_directories(${CMAKE_SOURCE_DIR}/libcurl_mingw64/include)

# 2. 链接库
add_library(libcurl SHARED IMPORTED)
set_target_properties(libcurl PROPERTIES
        IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/libcurl_mingw64/lib/libcurl.dll.a
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libcurl_mingw64/bin/libcurl-4.dll)

# 3. 可执行文件
add_executable(curl_sample src/libcurl_sample.c)
target_link_libraries(curl_sample PRIVATE libcurl)

# 4. 运行时 DLL 自动复制到可执行文件旁边
add_custom_command(TARGET curl_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/libcurl_mingw64/bin/libcurl-4.dll
        ${CMAKE_BINARY_DIR})

#file(COPY "${CMAKE_SOURCE_DIR}/certs/cacert.pem" DESTINATION "${CMAKE_BINARY_DIR}/certs")
add_custom_command(TARGET curl_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/certs/cacert.pem
        ${CMAKE_BINARY_DIR}/certs/cacert.pem)

### libserialport sample
include_directories(${CMAKE_SOURCE_DIR}/libserialport_mingw64/include)
add_library(serialport SHARED IMPORTED)

set_target_properties(serialport PROPERTIES
        IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/libserialport_mingw64/lib/libserialport.dll.a
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libserialport_mingw64/bin/libserialport-0.dll)

add_executable(libserialport_sample src/libserialport_sample.c)
target_link_libraries(libserialport_sample PRIVATE serialport)

add_custom_command(TARGET libserialport_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/libserialport_mingw64/bin/libserialport-0.dll
        ${CMAKE_BINARY_DIR})

### openssl sample
set(OPENSSL_DIR "${CMAKE_SOURCE_DIR}/openssl_mingw64")
include_directories(${OPENSSL_DIR}/include)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${OPENSSL_DIR}/lib/libssl.dll.a)
add_library(crypto STATIC IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_DIR}/lib/libcrypto.dll.a)

add_executable(openssl_sample src/openssl_sample.c)
target_link_libraries(openssl_sample PRIVATE ssl crypto)

file(COPY "${OPENSSL_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/)


### mqtt sample
#
# pacman -S mingw-w64-x86_64-paho.mqtt.c
#
set(MQTT_DIR "${CMAKE_SOURCE_DIR}/mqttc_mingw64")
include_directories(${MQTT_DIR}/include)

add_library(mqtt STATIC IMPORTED)
set_target_properties(mqtt PROPERTIES IMPORTED_LOCATION ${MQTT_DIR}/lib/libpaho-mqtt3c.dll.a)
add_executable(mqtt_sample src/mqtt_sample.c)
target_link_libraries(mqtt_sample PRIVATE mqtt)

add_library(mqtt_as STATIC IMPORTED)
set_target_properties(mqtt_as PROPERTIES IMPORTED_LOCATION ${MQTT_DIR}/lib/libpaho-mqtt3as.dll.a)
add_executable(mqtt_as_sample src/mqtt_as_sample.c)
target_link_libraries(mqtt_as_sample PRIVATE mqtt_as)

file(COPY "${MQTT_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/)
