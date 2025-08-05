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
#
#
set(CJSON_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs/cJSON")
aux_source_directory(${CJSON_SOURCE_DIR} CJSON_SRC_LIST)
add_executable(cJSONSample src/cJSON_sample.c)
target_sources(cJSONSample PRIVATE ${CJSON_SRC_LIST})
target_include_directories(cJSONSample PRIVATE ${CJSON_SOURCE_DIR})


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
# pacman -S mingw-w64-x86_64-paho.mqtt.c
#
set(MQTT_DIR "${CMAKE_SOURCE_DIR}/libs/mqttc_mingw64")

add_library(mqtt STATIC IMPORTED)
set_target_properties(mqtt PROPERTIES IMPORTED_LOCATION ${MQTT_DIR}/lib/libpaho-mqtt3c.dll.a)
add_executable(mqtt_sample src/mqtt_sample.c)
target_include_directories(mqtt_sample PRIVATE ${MQTT_DIR}/include)
target_link_libraries(mqtt_sample PRIVATE mqtt)

add_library(mqtt_as STATIC IMPORTED)
set_target_properties(mqtt_as PROPERTIES IMPORTED_LOCATION ${MQTT_DIR}/lib/libpaho-mqtt3as.dll.a)
add_executable(mqtt_as_sample src/mqtt_as_sample.c)
target_include_directories(mqtt_as_sample PRIVATE ${MQTT_DIR}/include)
target_link_libraries(mqtt_as_sample PRIVATE mqtt_as)

file(COPY "${MQTT_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/)


#
### mongoose_mingw64
#
# pacman -S mingw-w64-ucrt-x86_64-mongoose_mingw64
#
set(MONGOOSE_DIR "${CMAKE_SOURCE_DIR}/libs/mongoose_mingw64")

add_library(mongoose STATIC IMPORTED)
set_target_properties(mongoose PROPERTIES IMPORTED_LOCATION ${MONGOOSE_DIR}/lib/libmongoose.dll.a)
add_executable(mongoose_sample src/mongoose_webserver_sample.c)
target_include_directories(mongoose_sample PRIVATE ${MONGOOSE_DIR}/include)
target_link_libraries(mongoose_sample PRIVATE mongoose)

file(COPY "${MONGOOSE_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/)


#
### nettle_mingw64
#
# pacman -S mingw-w64-x86_64-nettle
#
set(NETTLE_DIR "${CMAKE_SOURCE_DIR}/libs/nettle_mingw64")

add_library(nettle STATIC IMPORTED)
set_target_properties(nettle PROPERTIES IMPORTED_LOCATION ${NETTLE_DIR}/lib/libnettle.dll.a)

add_executable(nettle_sample src/nettle_sample.c)
target_include_directories(nettle_sample PRIVATE ${NETTLE_DIR}/include)
target_link_libraries(nettle_sample PRIVATE nettle)
add_executable(nettle_aes_sample src/nettle_aes_sample.c)
target_include_directories(nettle_aes_sample PRIVATE ${NETTLE_DIR}/include)
target_link_libraries(nettle_aes_sample PRIVATE nettle)

file(COPY "${NETTLE_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/)
