cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
### mqtt sample
#
# pacman -S mingw-w64-x86_64-paho.mqtt.c
#
set(MQTT_DIR "${CMAKE_SOURCE_DIR}/libs/mqttc_mingw64")

add_library(mqtt STATIC IMPORTED)
set_target_properties(mqtt PROPERTIES IMPORTED_LOCATION ${MQTT_DIR}/lib/libpaho-mqtt3c.dll.a)
add_executable(mqtt_sample mqtt_sample.c)
target_include_directories(mqtt_sample PRIVATE ${MQTT_DIR}/include)
target_link_libraries(mqtt_sample PRIVATE mqtt)

add_library(mqtt_as STATIC IMPORTED)
set_target_properties(mqtt_as PROPERTIES IMPORTED_LOCATION ${MQTT_DIR}/lib/libpaho-mqtt3as.dll.a)
add_executable(mqtt_as_sample mqtt_as_sample.c)
target_include_directories(mqtt_as_sample PRIVATE ${MQTT_DIR}/include)
target_link_libraries(mqtt_as_sample PRIVATE mqtt_as)

#file(COPY "${MQTT_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/)
add_custom_command(TARGET mqtt_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MQTT_DIR}/bin/libpaho-mqtt3c.dll
        ${CMAKE_BINARY_DIR}/libs/mqttc_mingw64)

add_custom_command(TARGET mqtt_as_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MQTT_DIR}/bin/libpaho-mqtt3as.dll
        ${CMAKE_BINARY_DIR}/libs/mqttc_mingw64)
