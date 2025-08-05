cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

#
### mongoose_mingw64
#
# pacman -S mingw-w64-ucrt-x86_64-mongoose_mingw64
#
set(MONGOOSE_DIR "${CMAKE_SOURCE_DIR}/libs/mongoose_mingw64")

add_library(mongoose STATIC IMPORTED)
set_target_properties(mongoose PROPERTIES IMPORTED_LOCATION ${MONGOOSE_DIR}/lib/libmongoose.dll.a)
add_executable(mongoose_sample mongoose_webserver_sample.c)
target_include_directories(mongoose_sample PRIVATE ${MONGOOSE_DIR}/include)
target_link_libraries(mongoose_sample PRIVATE mongoose)

#file(COPY "${MONGOOSE_DIR}/bin/" DESTINATION ${CMAKE_BINARY_DIR}/)
add_custom_command(TARGET mongoose_sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MONGOOSE_DIR}/bin/libmongoose-5.dll
        ${CMAKE_BINARY_DIR}/libs/mongoose_mingw64)
