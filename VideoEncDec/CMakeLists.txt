file(GLOB VIDEO_ENCODER_DECODER_FILES *.cpp *.c)
add_library(VIDEO_ENCODER_DECODER SHARED ${VIDEO_ENCODER_DECODER_FILES})

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