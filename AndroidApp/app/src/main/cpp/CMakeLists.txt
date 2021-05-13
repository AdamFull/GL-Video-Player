cmake_minimum_required(VERSION 3.10.2)

project("myapplication")

add_library( native-lib SHARED native-lib.cpp )

find_library( log-lib log )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CVideoEncDec/)

set(SFML_LIB_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/sfml)
include_directories(${SFML_LIB_PREFIX}/include)

set(SYSTEM_TYPE android)
set(ARCHITECTURE ${ANDROID_ABI})
message(${CMAKE_HOST_SYSTEM_PROCESSOR})

set(SFML_LIB_PLATFORMSPEC_PATH ${SFML_LIB_PREFIX}/${SYSTEM_TYPE}/${ARCHITECTURE})
set(SFML_LIBRARIES
        ${SFML_LIB_PLATFORMSPEC_PATH}/libsfml-activity-d.so
        ${SFML_LIB_PLATFORMSPEC_PATH}/libsfml-audio-d.so
        ${SFML_LIB_PLATFORMSPEC_PATH}/libsfml-graphics-d.so
        ${SFML_LIB_PLATFORMSPEC_PATH}/libsfml-main-d.a
        ${SFML_LIB_PLATFORMSPEC_PATH}/libsfml-network-d.so
        ${SFML_LIB_PLATFORMSPEC_PATH}/libsfml-system-d.so
        ${SFML_LIB_PLATFORMSPEC_PATH}/libsfml-window-d.so
        )

target_link_libraries( native-lib ${log-lib} VIDEO_ENCODER_DECODER ${SFML_LIBRARIES})