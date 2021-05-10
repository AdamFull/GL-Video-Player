cmake_minimum_required(VERSION 3.9)
project(VIDEO_ENCODER_DECODER VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)

file(GLOB VIDEO_ENCODER_DECODER_FILES *.c *.h)

if(WIN32)
add_library(VIDEO_ENCODER_DECODER STATIC ${VIDEO_ENCODER_DECODER_FILES})
else()
add_library(VIDEO_ENCODER_DECODER SHARED ${VIDEO_ENCODER_DECODER_FILES})
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVENC_DEBUG")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

include_directories(${FFMPEG_INCLUDE_DIRS})

target_link_libraries(VIDEO_ENCODER_DECODER PkgConfig::LIBAV)