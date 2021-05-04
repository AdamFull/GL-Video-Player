file(GLOB AL_PLAYER_FILES *.cpp *.hpp)
add_library(AL_PLAYER SHARED ${AL_PLAYER_FILES})

find_package(OPENAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

target_link_libraries(AL_PLAYER ${OPENAL_LIBRARY})