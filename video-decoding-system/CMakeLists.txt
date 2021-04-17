cmake_minimum_required(VERSION 3.1)
project(VideoDecoding CXX)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB VideoDecodingFiles *.cpp *.hpp *.c *.h)

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



add_library(VideoDecoding SHARED ${VideoDecodingFiles})

target_link_libraries(VideoDecoding PkgConfig::LIBAV)