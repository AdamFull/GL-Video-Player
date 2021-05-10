cmake_minimum_required(VERSION 3.9)
project(VIDEO_ENCODER_DECODER VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)

file(GLOB AL_PLAYER_FILES *.c *.h)
add_library(AL_PLAYER SHARED ${AL_PLAYER_FILES})

find_package(OPENAL REQUIRED)
# include_directories(${OPENAL_INCLUDE_DIR})

target_link_libraries(AL_PLAYER ${OPENAL_LIBRARY})