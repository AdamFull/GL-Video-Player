cmake_minimum_required(VERSION 3.9)
project(VIDEO_ENCODER_DECODER VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)

file(GLOB VIDEO_ENCODER_DECODER_FILES *.c *.h)

set(FFMPEG_LIB_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg)

if(WIN32)
    set(SYSTEM_TYPE windows)
    set(ARCHITECTURE ${CMAKE_HOST_SYSTEM_PROCESSOR})
elseif(UNIX AND NOT APPLE)
    if(ANDROID)
        set(SYSTEM_TYPE android)
        set(ARCHITECTURE ${ANDROID_ABI})
    else()
        set(SYSTEM_TYPE linux)
        set(ARCHITECTURE ${CMAKE_HOST_SYSTEM_PROCESSOR})
    endif()
elseif(UNIX AND APPLE)
endif()

set(FFMPEG_LIB_PLATFORMSPEC_PATH ${FFMPEG_LIB_PREFIX}/${SYSTEM_TYPE}/${ARCHITECTURE})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CURRENT_BUILDTYPE debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVENC_DEBUG")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CURRENT_BUILDTYPE release)
endif()

set(FFMPEG_LIBRARIES 
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libavcodec.so.58 
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libavdevice.so.58 
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libavfilter.so.7 
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libavformat.so.58
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libavutil.so.56
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libpostproc.so.55
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libswresample.so.3
        ${FFMPEG_LIB_PLATFORMSPEC_PATH}/libswscale.so.5
    )

message("Configuring " ${CURRENT_BUILDTYPE} " build for system " ${SYSTEM_TYPE}:${ARCHITECTURE})

include_directories(${FFMPEG_LIB_PREFIX}/include)

add_library(VIDEO_ENCODER_DECODER SHARED ${VIDEO_ENCODER_DECODER_FILES})

target_link_libraries(VIDEO_ENCODER_DECODER ${FFMPEG_LIBRARIES})
